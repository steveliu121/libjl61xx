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
#include "jl_device.h"
#include "jl61xx/jl61xx_chip.h"
#include "jl61xx/jl61xx_drv_switch.h"

struct jl_switch_dev_s *gp_switch_dev_61xx = NULL;

/*TODO*/
#if ((defined CONFIG_JL_CHIP_ID_6110) || (defined CONFIG_JL_CHIP_ID_61XX))
struct jl_switch_dev_s g_switch_dev_6110 = {
	/* switch chip id */
	CHIP_ID_JL6110,

	/* eco version */
	0,

	/*TODO core version */
	0,

	/* port transform: logic to switch core */
	{2, 3, 4, 5, 6, 7, 8, 9, 0, 1, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* port transform: logic to phy */
	{0, 1, 2, 3, 4, 5, 6, 7, 0, 1, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* port transform: switch core to logic */
	{EXT_PORT0, EXT_PORT1, UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3,
		UTP_PORT4, UTP_PORT5, UTP_PORT6, UTP_PORT7, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* logic port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_EXT, PORT_TYPE_EXT, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* switch core port type */
	{PORT_TYPE_EXT, PORT_TYPE_EXT, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_IN, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF},

	/* max logic port */
	EXT_PORT1,

	/* max switch core port */
	10,

	/* logic port mask */
	((0x1 << UTP_PORT0) | (0x1 << UTP_PORT1) | (0x1 << UTP_PORT2) | (0x1 << UTP_PORT3) | (0x1 << UTP_PORT4) | (0x1 << UTP_PORT5) | (0x1 << UTP_PORT6) | (0x1 << UTP_PORT7) | (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1)),

	/* utp port mask */
	((0x1 << UTP_PORT0) | (0x1 << UTP_PORT1) | (0x1 << UTP_PORT2) | (0x1 << UTP_PORT3) | (0x1 << UTP_PORT4) | (0x1 << UTP_PORT5) | (0x1 << UTP_PORT6) | (0x1 << UTP_PORT7)),

	/* ext port mask */
	((0x1 << EXT_PORT0) | (0x1 << EXT_PORT1))
};
#endif

jl_ret_t jl_lport_check(jl_port_t lport)
{
	JL_ASSERT(gp_switch_dev_61xx);

	if (lport > gp_switch_dev_61xx->max_lport)
		return JL_ERR_PORT;

	if (gp_switch_dev_61xx->lport_type[lport] == PORT_TYPE_UNDEF)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jl_cport_check(jl_port_t cport)
{
	JL_ASSERT(gp_switch_dev_61xx);

	if (cport > gp_switch_dev_61xx->max_cport)
		return JL_ERR_PORT;

	if (gp_switch_dev_61xx->cport_type[cport] == PORT_TYPE_UNDEF)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jl_utpport_check(jl_port_t lport)
{
	JL_ASSERT(gp_switch_dev_61xx);

	if (lport > gp_switch_dev_61xx->max_lport)
		return JL_ERR_PORT;

	if (gp_switch_dev_61xx->lport_type[lport] != PORT_TYPE_UTP)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jl_extport_check(jl_port_t lport)
{
	JL_ASSERT(gp_switch_dev_61xx);

	if (lport > gp_switch_dev_61xx->max_lport)
		return JL_ERR_PORT;

	if (gp_switch_dev_61xx->lport_type[lport] != PORT_TYPE_EXT)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_port_t jl_port_l2c(jl_port_t lport)
{
	if (!(gp_switch_dev_61xx))
		return UNDEF_PORT;
	if (lport > gp_switch_dev_61xx->max_lport)
		return UNDEF_PORT;
	return gp_switch_dev_61xx->l2c_port[lport];
}

jl_port_t jl_port_l2p(jl_port_t lport)
{
	if (!(gp_switch_dev_61xx))
		return UNDEF_PORT;
	if (lport > gp_switch_dev_61xx->max_lport)
		return UNDEF_PORT;
	return gp_switch_dev_61xx->l2p_port[lport];
}

jl_port_t jl_port_c2l(jl_port_t cport)
{
	if (!(gp_switch_dev_61xx))
		return UNDEF_PORT;
	if (cport > gp_switch_dev_61xx->max_cport)
		return UNDEF_PORT;
	return gp_switch_dev_61xx->c2l_port[cport];
}

jl_ret_t jl_portmask_l2c(jl_portmask_t lportmask, jl_portmask_t *cportmask)
{
	jl_port_t lport;
	jl_port_t cport;

	JL_ASSERT(gp_switch_dev_61xx);

	JL_FOR_EACH_PORT(lport) {
		if (lportmask & (1 << lport)) {
			cport = jl_port_l2c(lport);
			*cportmask |= 1 << cport;
		}
	}

	return JL_ERR_OK;
}

jl_ret_t jl_portmask_c2l(jl_portmask_t cportmask, jl_portmask_t *lportmask)
{
	jl_port_t lport;
	jl_port_t cport;

	JL_ASSERT(gp_switch_dev_61xx);

	JL_FOR_EACH_CPORT(cport) {
		if (gp_switch_dev_61xx->cport_type[cport] == PORT_TYPE_IN)
			continue;
		if (cportmask & (1 << cport)) {
			lport = jl_port_c2l(cport);
			*lportmask |= 1 << lport;
		}
	}

	return JL_ERR_OK;
}
