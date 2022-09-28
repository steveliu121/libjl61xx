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

#ifndef __JL51XX_DRV_TEST_H__
#define __JL51XX_DRV_TEST_H__

#include "jl_base.h"
#include "test.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl61xx_test(jl_device_t *device, jl_test_t *ptest);
jl_ret_t jl61xx_test_io_write(jl_device_t *device, jl_uint32 addr, jl_uint32 val);
jl_ret_t jl61xx_test_io_read(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval);

#ifdef __cplusplus
}
#endif

#endif /* __JL51XX_DRV_TEST_H__ */
