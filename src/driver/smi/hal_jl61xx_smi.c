#include <stdint.h>

#include "jl_kit.h"
#include "driver/hal_smi.h"
#include "driver/jl_reg_io.h"

#define IDA_REG_CMD		0
#define IDA_REG_INFOR		1
#define IDA_REG_ADDR_L		2
#define IDA_REG_ADDR_H		3
#define IDA_REG_WD_L		4
#define IDA_REG_WD_H_SI0	13
#define IDA_REG_RD_L_SI0	14
#define IDA_REG_RD_H		7
#define IDA_PHY_ID		0
#define IDA_PAGE_ID		60
#define IDA_BURST_SIZE_MAX	16

jl_ret_t jl61xx_smi_read(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	jl_uint16 burst_size = 0;
	jl_uint16 burst_ctrl = 0;
	jl_uint16 try = 1000;
	jl_uint32 tmp = 0;
	jl_int32 i;
	jl_api_ret_t ret = 0;

	if ((size == 0) || (size > IDA_BURST_SIZE_MAX)) {
		JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR,
				"Wrong burst size, brust size range:[1, %d]\n",
				IDA_BURST_SIZE_MAX);

		return JL_ERR_PARAM;
	}

#ifdef CONFIG_JL_APB_MUTEX
	port_mutex_lock(g_smi_dev.apb_mutex);
#endif

	/* set IDA page id */
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, 31, IDA_PAGE_ID);

	/* 1. set burst control: read_indication & burst_size */
	burst_size = size - 1;
	burst_ctrl = burst_size << 1;
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_INFOR, burst_ctrl);

	/* 2. set register address */
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_ADDR_L, (addr & 0xffff));
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_ADDR_H, ((addr >> 16) & 0xff));

	/* 3. set ida read request */
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_CMD, 1);

	/* 4. check operation done & status */
	while (--try) {
		tmp = io_desc->smi.ops.read(io_desc, IDA_PHY_ID, IDA_REG_CMD);
		if (!(tmp & 1))
			break;
	}

	if (!try)
		goto timeout;

	/* 5. read data */
	for (i = 0; i < size; i++) {
		buf[i] = io_desc->smi.ops.read(io_desc, IDA_PHY_ID, IDA_REG_RD_L_SI0);
		tmp = io_desc->smi.ops.read(io_desc, IDA_PHY_ID, IDA_REG_RD_H);
		buf[i] |= tmp << 16;
	}

#ifdef CONFIG_JL_APB_MUTEX
	port_mutex_unlock(g_smi_dev.apb_mutex);
#endif

	return JL_ERR_OK;

timeout:
	JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR, "IDA read register timeout\n");
	ret = JL_ERR_TIMEOUT;

#ifdef CONFIG_JL_APB_MUTEX
	port_mutex_unlock(g_smi_dev.apb_mutex);
#endif

	return ret;
}

jl_ret_t jl61xx_smi_write(jl_io_desc_t *io_desc,
				jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	jl_uint16 burst_size = 0;
	jl_uint16 burst_ctrl = 0;
	jl_uint16 try = 1000;
	jl_uint16 tmp = 0;
	jl_int32 i;
	jl_ret_t ret = 0;

	if ((size == 0) || (size > IDA_BURST_SIZE_MAX)) {
		JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR,
				"Wrong burst size, brust size range:[1, %d]\n",
				IDA_BURST_SIZE_MAX);

		return JL_ERR_PARAM;
	}

#ifdef CONFIG_JL_DUMP_REG_WRITE
	if (size == 1) {
		ret = jl_dump_indirect_write(addr, *buf);
		if (ret)
			JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR, "Dump smi write fail!!!\n");
	} else {
		ret = jl_dump_indirect_burst_write(addr, size, buf);
		if (ret)
			JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR, "Dump smi write fail!!!\n");
	}
#endif

#ifdef CONFIG_JL_APB_MUTEX
	port_mutex_lock(g_smi_dev.apb_mutex);
#endif

	/* set IDA page id */
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, 31, IDA_PAGE_ID);

	/* 1. write data */
	for (i = 0; i < size; i++) {
		io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_WD_L, (buf[i] & 0x0000ffff));
		io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_WD_H_SI0,
					((buf[i] >> 16) & 0x0000ffff));
	}

	/* 2. set burst control: write_indication & burst_size */
	burst_size = size - 1;
	burst_ctrl = burst_size << 1 | 1;
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_INFOR, burst_ctrl);

	/* 3. set register address */
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_ADDR_L, (addr & 0xffff));
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_ADDR_H, ((addr >> 16) & 0xff));

	/* 3. issue write operation */
	io_desc->smi.ops.write(io_desc, IDA_PHY_ID, IDA_REG_CMD, 1);

	/* 4. check operation done & status */
	while (--try) {
		tmp = io_desc->smi.ops.read(io_desc, IDA_PHY_ID, IDA_REG_CMD);
		if (!(tmp & 1))
			break;
	}

	if (!try)
		goto timeout;

#ifdef CONFIG_JL_APB_MUTEX
	port_mutex_unlock(g_smi_dev.apb_mutex);
#endif

	return JL_ERR_OK;

timeout:
	JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR, "IDA write register timeout\n");
	ret = JL_ERR_TIMEOUT;

#ifdef CONFIG_JL_APB_MUTEX
	port_mutex_unlock(g_smi_dev.apb_mutex);
#endif

	return ret;
}
