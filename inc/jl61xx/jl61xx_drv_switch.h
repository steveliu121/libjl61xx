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

#ifndef __JL61XX_DRV_SWITCH_H__
#define __JL61XX_DRV_SWITCH_H__

#include "jl_types.h"
#include "jl_device.h"
#include "jl_switch.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl61xx_switch_info(jl_device_t *device, jl_switch_info_t *pinfo);
jl_ret_t jl61xx_chip_probe(jl_device_t *device);
jl_ret_t jl61xx_chip_remove(jl_device_t *device);

#ifdef __cplusplus
}
#endif

#endif /* __JL61XX_DRV_SWITCH_H__ */
