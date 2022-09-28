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
#include "test.h"
#include "jl61xx/jl61xx_chip.h"
#include "driver/jl_reg_io.h"

jl_ret_t jl61xx_test(jl_device_t *device, struct jl_test_s *ptest)
{
	(void) device;
	jl_uint32 port;
	jl_ret_t ret = JL_ERR_OK;
	jl_portmask_t portmask = 0;

	if (ptest->lport != 0xffffffff) {
		printf("\n\n======JL_CHECK_PORT TEST======\n\n");
		JL_CHECK_PORT(ptest->lport);
	}

	if (ptest->utp_port != 0xffffffff) {
		printf("\n\n======JL_CHECK_UTP_PORT TEST======\n\n");
		JL_CHECK_UTP_PORT(ptest->utp_port);
	}

	if (ptest->ext_port != 0xffffffff) {
		printf("\n\n======JL_CHECK_EXT_PORT TEST======\n\n");
		JL_CHECK_EXT_PORT(ptest->ext_port);
	}

	if (ptest->portmask != 0xffffffff) {
		printf("\n\n======JL_CHECK_PORTMASK TEST======\n\n");
		JL_CHECK_PORTMASK(ptest->portmask);
	}

	if (ptest->utp_portmask != 0xffffffff) {
		printf("\n\n======JL_CHECK_UTP_PORTMASK TEST======\n\n");
		JL_CHECK_UTP_PORTMASK(ptest->utp_portmask);
	}

	if (ptest->ext_portmask != 0xffffffff) {
		printf("\n\n======JL_CHECK_EXT_PORTMASK TEST======\n\n");
		JL_CHECK_EXT_PORTMASK(ptest->ext_portmask);
	}

	if (ptest->lcport != 0xffffffff) {
		printf("\n\n======JL_PORT_L2C TEST======\n\n");
		printf("lport:%d, cport:%d\n",
			ptest->lcport, jl_port_l2c(ptest->lcport));
	}

	if (ptest->clport != 0xffffffff) {
		printf("\n\n======JL_PORT_C2L TEST======\n\n");
		printf("cport:%d, lport:%d\n",
			ptest->clport, jl_port_c2l(ptest->clport));
	}

	if (ptest->lcportmask != 0xffffffff) {
		printf("\n\n======JL_PORTMASK_L2C TEST======\n\n");
		ret = jl_portmask_l2c(ptest->lcportmask, &portmask);
		if (ret != JL_ERR_OK)
			return ret;
		printf("lportmask:0x%04x, cportmask:0x%04x\n",
			ptest->lcportmask, portmask);
	}

	if (ptest->clportmask != 0xffffffff) {
		printf("\n\n======JL_PORTMASK_C2L TEST======\n\n");
		ret = jl_portmask_c2l(ptest->clportmask, &portmask);
		if (ret != JL_ERR_OK)
			return ret;
		printf("cportmask:0x%04x, lportmask:0x%04x\n",
			ptest->clportmask, portmask);
	}

	if (ptest->for_each != 0xffffffff) {
		printf("\n\n======JL_FOR_EACH... TEST======\n");

		printf("JL_FOR_EACH_PORT:\n");
		JL_FOR_EACH_PORT(port)
			printf("%d\t", port);
		printf("\n\n");

		printf("JL_FOR_EACH_CPORT:\n");
		JL_FOR_EACH_CPORT(port)
			printf("%d\t", port);
		printf("\n\n");

		printf("JL_FOR_EACH_UTP_PORT:\n");
		JL_FOR_EACH_UTP_PORT(port)
			printf("%d\t", port);
		printf("\n\n");

		printf("JL_FOR_EACH_EXT_PORT:\n");
		JL_FOR_EACH_EXT_PORT(port)
			printf("%d\t", port);
		printf("\n\n");
	}

	return ret;
}

jl_ret_t jl61xx_test_io_write(jl_device_t *device, jl_uint32 addr, jl_uint32 val)
{
	jl_ret_t ret;

	ret = jl_reg_write(&device->io_desc, addr, val);

	return ret;
}

jl_ret_t jl61xx_test_io_read(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval)
{
	jl_ret_t ret;

	ret = jl_reg_read(&device->io_desc, addr, pval);

	return ret;
}
