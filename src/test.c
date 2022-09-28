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
#include "test.h"
#include "jl_base.h"


JL_API jl_api_ret_t
jl_test(jl_uint32 chip_id, jl_test_t *ptest)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, test, chip_id, ptest);

	return ret;
}

JL_API jl_api_ret_t
jl_test_io_write(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 val)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, test_io_write, chip_id, addr, val);

	return ret;
}

JL_API jl_api_ret_t
jl_test_io_read(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 *pval)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, test_io_read, chip_id, addr, pval);

	return ret;
}
