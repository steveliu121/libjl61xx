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

#include "jl_kit.h"
#include "jl_device.h"


extern jl_driver_t g_drv_61xx;

jl_device_t g_dev_61xx_0 = {
	.compat_id = JL_CHIP_61XX,
	.name = "jl61xx-device-0",
	.id = 0, /* must be less than JL_MAX_CHIP_NUM */
	.driver = NULL,
	.io_desc = {
		.chip = JL_CHIP_61XX,
		.type = JL_IO_SMI_SPI_DEMO,
		.smi.spi.bus_id = 0
	},
	.switch_dev = NULL,
	.priv = NULL
};

/* jl_device_t g_dev_61xx_1 = { */
/* 	.compat_id = JL_CHIP_61XX, */
/* 	.name = "jl61xx-device-1", */
/* 	.id = 1, /1* must be less than JL_MAX_CHIP_NUM *1/ */
/* 	.driver = NULL, */
/* 	.io_desc = { */
/* 		.chip = JL_CHIP_61XX, */
/* 		.type = JL_IO_SMI_SPI_DEMO, */
/* 		.smi.spi.bus_id = 1 */
/* 	}, */
/* 	.switch_dev = NULL, */
/* 	.priv = NULL */
/* }; */

jl_device_t *gpp_devices[] = {
	&g_dev_61xx_0
	/* &g_dev_61xx_1 */
};

jl_driver_t *gpp_drivers[] = {
	&g_drv_61xx
};

jl_module_t g_module = {
	.drivers = gpp_drivers,
	.devices = gpp_devices,
	.driver_num = JL_ARRAY_SIZE(gpp_drivers),
	.device_num = JL_ARRAY_SIZE(gpp_devices),
	.api_mutex = NULL,
	.stat = STAT_UNINIT
};
