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

#ifndef __JL61XX_DRV_LED_H__
#define __JL61XX_DRV_LED_H__

#include "jl_base.h"
#include "jl_led.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl61xx_led_enable_set(jl_device_t *device, jl_led_group_t group, jl_portmask_t portmask);
jl_ret_t jl61xx_led_enable_get(jl_device_t *device, jl_led_group_t group, jl_portmask_t *pportmask);

jl_ret_t jl61xx_led_group_config_set(jl_device_t *device, jl_led_group_t group, jl_led_config_t config);
jl_ret_t jl61xx_led_group_config_get(jl_device_t *device, jl_led_group_t group, jl_led_config_t *ppconfig);

jl_ret_t jl61xx_led_blink_rate_set(jl_device_t *device, jl_led_blink_rate_t blink_rate);
jl_ret_t jl61xx_led_blink_rate_get(jl_device_t *device, jl_led_blink_rate_t *pblink_rate);

jl_ret_t jl61xx_led_group_active_high_set(jl_device_t *device, jl_led_group_t group, jl_portmask_t portmask);
jl_ret_t jl61xx_led_group_active_high_get(jl_device_t *device, jl_led_group_t group, jl_portmask_t *pportmask);
#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_LED_H__ */
