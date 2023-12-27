#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_rwreg.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	jl_api_ret_t ret;
	/* jl_dev_t dev_6107s = { */
	/* 	.compat_id = JL_CHIP_6107S, */
	/* 	.name = "device-jl6107s", */
	/* 	.id = 0, /1* must be less than JL_MAX_CHIP_NUM *1/ */
	/* 	.io = { */
	/* 		.type = JL_IO_SMI_SPI_DEMO, */
	/* 		.smi.mdio.bus_id = 0 */
	/* 	} */
	/* }; */

	jl_dev_t dev_6110 = {
		.compat_id = JL_CHIP_6110,
		.name = "device-jl6110",
		.id = 1, /* must be less than JL_MAX_CHIP_NUM */
		.io = {
			.type = JL_IO_SMI_SPI_DEMO,
			.smi.mdio.bus_id = 0
		}
	};


	ret = jl_switch_init();
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "smi init fail\n");
		return -1;
	}

	/* /1* 1. create multiple devices 0&1 *1/ */
	/* ret = jl_switch_device_create(&dev_6107s); */
	/* if (ret) { */
	/* 	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create fail\n"); */
	/* 	return -1; */
	/* } */


	/* JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "000000000000000000000000000\n"); */
	jl_uint32 val[8] = {0};
	/* ret = jl_reg_io_burst_read(0, 0x115a9c, &val[0], 8); */
	/* if (ret) */
	/* 	return -1; */
	/* JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "###1, read 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", */
	/* 		val[0], val[1], val[2], val[3], */
	/* 		val[4], val[5], val[6], val[7]); */

	/* val[0] = 0x1234; */
	/* val[4] = 0x5678; */
	/* val[7] = 0x9abc; */
	/* ret = jl_reg_io_burst_write(0, 0x115a9c, &val[0], 8); */
	/* if (ret) */
	/* 	return -1; */
	/* JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "###2\n"); */

	/* ret = jl_reg_io_burst_read(0, 0x115a9c, &val[0], 8); */
	/* if (ret) */
	/* 	return -1; */

	/* JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "###3, read 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", */
	/* 		val[0], val[1], val[2], val[3], */
	/* 		val[4], val[5], val[6], val[7]); */


	ret = jl_switch_device_create(&dev_6110);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create fail\n");
		return -1;
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "111111111111111111111111111\n");
	ret = jl_reg_io_burst_read(1, 0x1202d8, &val[0], 8);
	if (ret)
		return -1;
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "###1, read 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
			val[0], val[1], val[2], val[3],
			val[4], val[5], val[6], val[7]);

	val[0] = 0x4321;
	val[4] = 0x8765;
	val[7] = 0xcba9;
	ret = jl_reg_io_burst_write(1, 0x1202d8, &val[0], 8);
	if (ret)
		return -1;
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "###2\n");

	ret = jl_reg_io_burst_read(1, 0x1202d8, &val[0], 8);
	if (ret)
		return -1;

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "###3, read 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
			val[0], val[1], val[2], val[3],
			val[4], val[5], val[6], val[7]);


	/* 2. SDK is ready
	 * TODO call sdk APIs here
	 * to control the corresponding switch device
	 * jl_xx_api(chip_id, ...)
	 */

	/* 3. close device io temporarily(free device io) */
	/* ret = jl_switch_device_close(0); */
	/* if (ret) { */
	/* 	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device close fail\n"); */
	/* 	return -1; */
	/* } */

	/* 4. re-open device io(request device io again)
	 * control the corresponding switch device again
	 */
	/* ret = jl_switch_device_open(0); */
	/* if (ret) { */
	/* 	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device open fail\n"); */
	/* 	return -1; */
	/* } */

	/* 5. delete multiple devices 0&1 */
	/* ret = jl_switch_device_delete(0); */
	/* if (ret) { */
	/* 	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device delete fail\n"); */
	/* 	return -1; */
	/* } */

	ret = jl_switch_device_delete(1);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device delete fail\n");
		return -1;
	}

	jl_switch_deinit();

	return 0;
}
