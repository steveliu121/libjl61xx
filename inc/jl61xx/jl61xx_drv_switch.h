/*
 * Copyright (c) 2014-2023 JLSemi Limited
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

#ifndef __JL61XX_DRV_SWITCH_H__
#define __JL61XX_DRV_SWITCH_H__

#include "jl_types.h"
#include "jl_device.h"
#include "jl_switch.h"
#include "jl61xx/jl61xx_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

/*macro define*/
/*sys time base for xiaoyao: 25Mhz*/
#define JL_SYSTIME_BASE                             (25000000)
#define JL_SYSTIME_MAX_STEP                         (5)

#define JL_SYSTIME_DEFAULT_DIV                      (0x19)
#define JL_SYSTIME_DEFAULT_STEP                     (0xa)

#define JL_SYSTEM_JUMBO_LENGTH                      (16369)

#define JL_MMP_POINTER_NUM                          (16)

enum jl_mmp_pointer_module_e {
    JL_MMP_MODULE_INITIAL_ADM = 1,
    JL_MMP_MODULE_SOURCE_MAC,
    JL_MMP_MODULE_DEST_MAC,
    JL_MMP_MODULE_ACL,
    JL_MMP_MODULE_VID
};

typedef struct jl_mmp_used_info_s {
	jl_uint8 is_used;
	jl_uint8 adm_used_num;
	jl_uint8 acl_used_num;
	jl_uint8 smac_used_num;
	jl_uint8 dmac_used_num;
	jl_uint8 vlan_used_num;
} jl_mmp_used_info_t;

typedef struct jl_mmp_pointer_s {
	jl_mmp_used_info_t used_info[JL_MMP_POINTER_NUM];
} jl_mmp_pointer_t;

/*unit: us*/
extern jl_uint32 g_tick_freq_list[];

static inline jl_uint32 __power(jl_uint8 base, jl_uint8 powerRaised)
{
	if (powerRaised != 0)
		return (base * __power(base, powerRaised - 1));
	else
		return 1;
}

static inline jl_ret_t __get_sys_tick_cfg(jl_device_t *pDevice, jl_uint32 *pClkDivider, jl_uint32 *pStepDivider)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_TICK_CONFIGURATION_t tickCfg;

	/*read from driver */
	REG_BURST_READ(pDevice, SWCORE, TICK_CONFIGURATION, INDEX_ZERO, INDEX_ZERO, tickCfg.val);

	*pClkDivider = tickCfg.BF.clk_divider;
	*pStepDivider = tickCfg.BF.step_divider;

	return ret;
}
static inline jl_ret_t __calc_tick_freq(jl_device_t *pDevice)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 i = 0;
	jl_uint32 result = 0;
	jl_uint32 div, step;

    ret = __get_sys_tick_cfg(pDevice, &div, &step);

	if (ret != JL_ERR_OK)
		return ret;

	//default configure
	if (div == JL_SYSTIME_DEFAULT_DIV && step == JL_SYSTIME_DEFAULT_STEP && 1000000 == g_tick_freq_list[0])
		return JL_ERR_EXISTS;

	for (i = 0; i < JL_SYSTIME_MAX_STEP; i++) {
		result = __power((jl_uint8) step, i);
		if ((result) && (div))
			g_tick_freq_list[i] = JL_SYSTIME_BASE / ((div) * result);
	}

	return JL_ERR_OK;
}

jl_ret_t jl61xx_get_unused_mmp_pointer(jl_device_t *device, jl_uint8 *pptr);
jl_ret_t jl61xx_set_mmp_pointer_info(jl_device_t *device, jl_uint8 module, jl_uint8 mmp_ptr);
jl_ret_t jl61xx_get_mmp_pointer_info(jl_device_t *device, jl_uint8 mmp_ptr, jl_mmp_used_info_t *pptr_info);
jl_ret_t jl61xx_clear_mmp_pointer_info(jl_device_t *device, jl_uint8 module, jl_uint8 mmp_ptr);

jl_ret_t jl61xx_switch_info(jl_device_t *device, jl_switch_info_t *pinfo);
jl_ret_t jl61xx_chip_probe(jl_device_t *device);
jl_ret_t jl61xx_chip_remove(jl_device_t *device);

jl_ret_t jl61xx_load_patch(jl_device_t *device);
jl_ret_t jl61xx_show_switchport_info(jl_device_t *pdevice);

#ifdef __cplusplus
}
#endif
#endif				/* __JL61XX_DRV_SWITCH_H__ */
