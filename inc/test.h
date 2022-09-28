
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

#ifndef __JL_TEST_H__
#define __JL_TEST_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct jl_test_s {
	jl_port_t lport;
	jl_port_t utp_port;
	jl_port_t ext_port;
	jl_portmask_t portmask;
	jl_portmask_t utp_portmask;
	jl_portmask_t ext_portmask;
	jl_port_t lcport;
	jl_port_t clport;
	jl_portmask_t lcportmask;
	jl_portmask_t clportmask;
	jl_uint32 for_each;
} jl_test_t;

JL_API jl_api_ret_t
jl_test(jl_uint32 chip_id, jl_test_t *ptest);
JL_API jl_api_ret_t
jl_test_io_write(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 val);
JL_API jl_api_ret_t
jl_test_io_read(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 *pval);

#ifdef __cplusplus
}
#endif

#endif /* __JL_TEST_H__ */
