/*
 * Copyright (c) 2014-2022 JLSemi Limited
 * All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of JLSemi Limited
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * No part of this code may be reproduced, stored in a retrieval system,
 * or transmitted, in any form or by any means, electronic, mechanical,
 * photocopying, recording, or otherwise, without the prior written
 * permission of JLSemi Limited
 */

#include "jl_base.h"
#include "version.h"
#include "jl61xx/jl61xx_chip.h"
#include "jl61xx/jl61xx_drv_switch.h"
#include "driver/jl_reg_io.h"
#ifdef CONFIG_JL61XX_DRV_TEST
#include "jl61xx/jl61xx_drv_test.h"
#endif

/*TODO led*/
/*TODO load patch*/
/*TODO reset*/
/*TODO uplink set link*/

struct jl_driver_s g_drv_61xx = {
	.compat_id = JL_CHIP_61XX,
	.name = "jl61xx-driver",
	.probe = jl61xx_chip_probe,
	.remove = jl61xx_chip_remove,
	/* TODO add your ops here */
	.ops = {
	/* 	.xxx = jl61xx_xxx, */
	/* 	.yyy = jl61xx_yyy, */
	/* 	.zzz = jl61xx_zzz, */
		.switch_info = jl61xx_switch_info,
#ifdef CONFIG_JL61XX_DRV_TEST
		.test = jl61xx_test,
		.test_io_write = jl61xx_test_io_write,
		.test_io_read = jl61xx_test_io_read
#endif
	},
	.drv_data = NULL
};

jl_ret_t jl61xx_switch_info(jl_device_t *device, jl_switch_info_t *pinfo)
{
	/* TODO */
	(void) device;
	pinfo->pkg = gp_switch_dev_61xx->pkg;
	pinfo->core_ver = gp_switch_dev_61xx->core_ver;
	pinfo->eco_ver = gp_switch_dev_61xx->eco_ver;
	pinfo->sdk_ver = (VERSION_JL_SDK_MAJOR << 28)
		| (VERSION_JL_SDK_MINOR << 16)
		| (VERSION_JL_SDK_MAINTENANCE);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_sw_core_init(void)
{
	/* jl_ret_t ret; */

	/*TODO bypass vlan by default*/

	return JL_ERR_OK;
}

jl_ret_t jl61xx_sw_core_deinit(void)
{
	/* jl_ret_t ret; */

	/*TODO reset fw pa ....*/

	return JL_ERR_OK;
}

jl_ret_t jl61xx_chip_probe(struct jl_device_s *device)
{
	jl_ret_t ret;
	/*TODO*/

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
			"Module probe: driver[%s]  device[%s]\n", device->driver->name, device->name);

	ret = jl_reg_io_init(&device->io_desc);
	if (ret)
		return ret;

	/*TODO check fw*/
	/*TODO check hw*/

	gp_switch_dev_61xx = &g_switch_dev_6110;

	ret = jl61xx_sw_core_init();

	return ret;
}

jl_ret_t jl61xx_chip_remove(struct jl_device_s *device)
{
	jl_ret_t ret;
	/*TODO*/

	jl_reg_io_deinit(&device->io_desc);

	ret = jl61xx_sw_core_deinit();

	return ret;
}
