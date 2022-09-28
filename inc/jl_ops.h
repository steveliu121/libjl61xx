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

#ifndef __JL_OPS_H__
#define __JL_OPS_H__

#include "jl_types.h"
#include "jl_device.h"
#include "jl_switch.h"
#ifdef CONFIG_JL61XX_DRV_TEST
#include "test.h"
#endif

struct jl_device_s;
typedef struct jl_device_s jl_device_t;

/**
 * @struct jl_ops_e
 * @brief jlsemi switch module operation template functions
 * @typedef jl_ops_t
 */
typedef struct jl_ops_s {
	/* TODO add your APIs template here */
	/* TODO for example */
	/* port */
	/* jl_ret_t port_phy_autoneg_ability_set(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability); */
	/* switch */
	jl_ret_t (*switch_info)(jl_device_t *device, jl_switch_info_t *pinfo);
	/* test */
#ifdef CONFIG_JL61XX_DRV_TEST
	jl_ret_t (*test)(jl_device_t *device, jl_test_t *ptest);
	jl_ret_t (*test_io_write)(jl_device_t *device, jl_uint32 addr, jl_uint32 val);
	jl_ret_t (*test_io_read)(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval);
#endif
} jl_ops_t;

#endif /* __JL_OPS_H__ */
