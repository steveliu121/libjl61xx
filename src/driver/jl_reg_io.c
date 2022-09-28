#include <stdint.h>

#include "jl_kit.h"
#include "driver/hal_smi.h"
#include "driver/jl_reg_io.h"


jl_ret_t __smi_ops_regist(jl_io_desc_t *io_desc)
{
	switch (io_desc->type) {
#ifdef CONFIG_JL_SMI_OVER_MDIO
	case JL_IO_SMI:
		io_desc->smi.ops.init = jl_smi_mdio_init;
		io_desc->smi.ops.deinit = jl_smi_mdio_deinit;
		io_desc->smi.ops.read = jl_smi_mdio_read;
		io_desc->smi.ops.write = jl_smi_mdio_write;

		break;
#endif
#ifdef CONFIG_JL_SMI_OVER_GPIO
	case JL_IO_SMI_GPIO:
		io_desc->smi.ops.init = jl_smi_gpio_init;
		io_desc->smi.ops.deinit = jl_smi_gpio_deinit;
		io_desc->smi.ops.read = jl_smi_gpio_read;
		io_desc->smi.ops.write = jl_smi_gpio_write;

		break;
#endif
#if ((defined CONFIG_JL_SMI_OVER_SPI) || (defined CONFIG_JL_SMI_OVER_FTDI))
	case JL_IO_SMI_SPI:
	case JL_IO_SMI_SPI_DEMO:
		io_desc->smi.ops.init = jl_smi_spi_init;
		io_desc->smi.ops.deinit = jl_smi_spi_deinit;
		io_desc->smi.ops.read = jl_smi_spi_read;
		io_desc->smi.ops.write = jl_smi_spi_write;

		break;
#endif
#ifdef CONFIG_JL_SMI_OVER_HID
	case JL_IO_SMI_HID_DEMO:
		io_desc->smi.ops.init = jl_smi_hid_init;
		io_desc->smi.ops.deinit = jl_smi_hid_deinit;
		io_desc->smi.ops.read = jl_smi_hid_read;
		io_desc->smi.ops.write = jl_smi_hid_write;

		break;
#endif
	default:
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown SMI device type!!!\n");
		return JL_ERR_NOXIO;
	}

	return JL_ERR_OK;
}

jl_ret_t __io_device_init_ex(jl_io_desc_t *io_desc, jl_uint8 init)
{
	jl_ret_t ret = JL_ERR_OK;

	switch (io_desc->type) {
#ifdef CONFIG_JL_IO_CPU
	case JL_IO_CPU:
		/*TODO*/
		break;
#endif
#ifdef CONFIG_JL_IO_SMI
	case JL_IO_SMI:
	case JL_IO_SMI_GPIO:
	case JL_IO_SMI_SPI:
	case JL_IO_SMI_SPI_DEMO:
	case JL_IO_SMI_HID_DEMO:
		if (!init) {
			ret = io_desc->smi.ops.deinit(io_desc);
			break;
		}
		/* TODO */
#ifdef CONFIG_JL_CHIP_JL51XX
		if (io_desc->chip == JL_CHIP_51XX) {
			io_desc->ops.read = jl51xx_smi_read;
			io_desc->ops.write = jl51xx_smi_write;
		}
#endif
#ifdef CONFIG_JL_CHIP_JL61XX
		if (io_desc->chip == JL_CHIP_61XX) {
			io_desc->ops.read = jl61xx_smi_read;
			io_desc->ops.write = jl61xx_smi_write;
		}
#endif

		ret = __smi_ops_regist(io_desc);
		if (ret)
			return ret;

		ret = io_desc->smi.ops.init(io_desc);

		break;
#endif
#ifdef CONFIG_JL_IO_I2C
	case JL_IO_I2C:
		/*TODO*/
		break;
#endif
#ifdef CONFIG_JL_IO_SPI
	case JL_IO_SPI:
		/*TODO*/
		break;
#endif
	default:
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown IO device type!!!\n");
		return JL_ERR_NOXIO;
	}

	return ret;
}

jl_ret_t __io_device_init(jl_io_desc_t *io_desc)
{
	return __io_device_init_ex(io_desc, 1);
}

jl_ret_t __io_device_deinit(jl_io_desc_t *io_desc)
{
	return __io_device_init_ex(io_desc, 0);
}

void jl_read_bits(jl_uint8 *bit_buf, jl_uint64 *bits, jl_uint8 start, jl_uint8 count)
{
	jl_uint64 i_bits = 0;
	jl_uint8 i_count = 0;
	jl_uint32 i_index = (start) / 8;
	jl_uint8 i_mask = 1 << ((start) % 8);

	while (i_count < count) {
		if (bit_buf[i_index] & i_mask)
			i_bits |= (1ULL << i_count);
		if (i_mask == 0x80) {
			i_index++;
			i_mask = 0x01;
		} else
			i_mask <<= 1;
		i_count++;
	}
	*bits = i_bits;
};

void jl_write_bits(jl_uint8 *bit_buf, jl_uint64 bits, jl_uint8 start, jl_uint8 count)
{
	jl_uint64 i_bits = bits;
	jl_uint8 i_count = 0;
	jl_uint32 i_index = (start) / 8;
	jl_uint8 i_mask = 1 << ((start) % 8);

	while (i_count < count) {
		if ((i_bits >> i_count) & 0x01)
			bit_buf[i_index] |= i_mask;
		else
			bit_buf[i_index] &= ~i_mask;
		if (i_mask == 0x80) {
			i_index++;
			i_mask = 0x01;
		} else
			i_mask <<= 1;
		i_count++;
	}
};

jl_uint16 jl_phy_reg_read_ext(jl_io_desc_t *io_desc, jl_uint32 phy, jl_uint32 page, jl_uint32 reg)
{
	if (io_desc->chip != JL_CHIP_51XX) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Only JL51XX support this phy io api !!!\n");
		return 0;
	}

	io_desc->smi.ops.write(io_desc, phy, 0x1f, page);

	return io_desc->smi.ops.read(io_desc, phy, reg);
}

void jl_phy_reg_write_ext(jl_io_desc_t *io_desc, jl_uint32 phy, jl_uint32 page, jl_uint32 reg, jl_uint16 val)
{
	if (io_desc->chip != JL_CHIP_51XX) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Only JL51XX support this phy io api !!!\n");
		return;
	}

	io_desc->smi.ops.write(io_desc, phy, 0x1f, page);
	io_desc->smi.ops.write(io_desc, phy, reg, val);

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_ret_t ret;

	ret = jl_dump_direct_write(phy, page, reg, val);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Dump smi write fail!!!\n");
#endif
}

jl_api_ret_t jl_reg_read(jl_io_desc_t *io_desc, jl_uint32 addr, jl_uint32 *val)
{
	return io_desc->ops.read(io_desc, addr, val, 1);
}

jl_api_ret_t jl_reg_burst_read(jl_io_desc_t *io_desc, jl_uint32 addr, jl_uint32 *buf, jl_uint32 size)
{
	return io_desc->ops.read(io_desc, addr, buf, size);
}

jl_api_ret_t jl_reg_write(jl_io_desc_t *io_desc, jl_uint32 addr, jl_uint32 val)
{
	return io_desc->ops.write(io_desc, addr, &val, 1);
}

jl_api_ret_t jl_reg_burst_write(jl_io_desc_t *io_desc, jl_uint32 addr, jl_uint32 *buf, jl_uint32 size)
{
	return io_desc->ops.write(io_desc, addr, buf, size);
}

jl_api_ret_t jl_reg_io_init(jl_io_desc_t *io_desc)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(io_desc);

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_dump_file_open();
#endif
	/* TODO init mutex */
	/* __io_mutex_init*/

	ret = __io_device_init(io_desc);

	return ret;
}

jl_api_ret_t jl_reg_io_deinit(jl_io_desc_t *io_desc)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(io_desc);

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_dump_file_close();
#endif
	/* TODO deinit mutex */
	/* __io_mutex_deinit*/

	ret = __io_device_deinit(io_desc);

	return ret;
}
