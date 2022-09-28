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

#ifndef __JL61XX_CHIP_H__
#define __JL61XX_CHIP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "jl_error.h"
#include "jl_types.h"
#include "jl_debug.h"


#define JL_SWITCH_PORT_NUM		16


/* check logical port */
#define JL_CHECK_PORT(x)					\
	do {							\
		if (jl_lport_check(x) != JL_ERR_OK)	\
			return JL_ERR_PORT;			\
	} while (0)

/* check logical utp port */
#define JL_CHECK_UTP_PORT(x)					\
	do {							\
		if (jl_utpport_check(x) != JL_ERR_OK)	\
			return JL_ERR_PORT;			\
	} while (0)

/* check logical ext port */
#define JL_CHECK_EXT_PORT(x)                    \
	do {                            \
		if (jl_extport_check(x) != JL_ERR_OK)    \
		return JL_ERR_PORT;         \
	} while (0)

/* check logical portmask */
#define JL_CHECK_PORTMASK(x)					\
	do {							\
		if ((x) != (gp_switch_dev_61xx->lportmask & (x)))	\
			return JL_ERR_PORT;			\
	} while (0)

/* check logical utp portmask */
#define JL_CHECK_UTP_PORTMASK(x)					\
	do {							\
		if ((x) != (gp_switch_dev_61xx->utp_portmask & (x)))	\
			return JL_ERR_PORT;			\
	} while (0)

/* check logical ext portmask */
#define JL_CHECK_EXT_PORTMASK(x)					\
	do {							\
		if ((x) != (gp_switch_dev_61xx->ext_portmask & (x)))	\
			return JL_ERR_PORT;			\
	} while (0)

/*TODO assert gp_switch_dev_61xx*/
#define JL_FOR_EACH_PORT(x)						\
	for (x = 0; (gp_switch_dev_61xx != NULL) &&			\
		(x <= gp_switch_dev_61xx->max_lport); x++)		\
		if (jl_lport_check(x) == JL_ERR_OK)

#define JL_FOR_EACH_CPORT(x)						\
	for (x = 0; (gp_switch_dev_61xx != NULL) &&			\
		(x <= gp_switch_dev_61xx->max_cport); x++)		\
		if (jl_cport_check(x) == JL_ERR_OK)

#define JL_FOR_EACH_UTP_PORT(x)						\
	for (x = 0; (gp_switch_dev_61xx != NULL) &&			\
		(x <= gp_switch_dev_61xx->max_lport); x++)		\
		if (jl_utpport_check(x) == JL_ERR_OK)

#define JL_FOR_EACH_EXT_PORT(x)						\
	for (x = 0; (gp_switch_dev_61xx != NULL) &&			\
		(x <= gp_switch_dev_61xx->max_lport); x++)		\
		if (jl_extport_check(x) == JL_ERR_OK)


enum chip_id_e {
	CHIP_ID_JL6105 = 0,
	CHIP_ID_JL6108,
	CHIP_ID_JL6110,
	CHIP_ID_END
};

struct jl_switch_dev_s {
	jl_uint32 pkg;
	jl_uint32 core_ver;
	jl_uint32 eco_ver;
	/* logical port to switch core port */
	jl_uint32 l2c_port[JL_SWITCH_PORT_NUM];
	/* logical port to phy port */
	jl_uint32 l2p_port[JL_SWITCH_PORT_NUM];
	/* switch core port to logical port */
	jl_uint32 c2l_port[JL_SWITCH_PORT_NUM];
	/* logical port type */
	jl_port_type_t lport_type[JL_SWITCH_PORT_NUM];
	/* switch core port type */
	jl_port_type_t cport_type[JL_SWITCH_PORT_NUM];
	/* max logical port */
	jl_uint32 max_lport;
	/* max switch core port */
	jl_uint32 max_cport;
	/* logical portmask */
	jl_portmask_t lportmask;
	/* logical utp portmask */
	jl_portmask_t utp_portmask;
	/* logical ext portmask */
	jl_portmask_t ext_portmask;
};


/*TODO*/
extern struct jl_switch_dev_s *gp_switch_dev_61xx;
extern struct jl_switch_dev_s g_switch_dev_6110;

jl_ret_t jl_lport_check(jl_port_t lport);
jl_ret_t jl_cport_check(jl_port_t cport);
jl_ret_t jl_utpport_check(jl_port_t lport);
jl_ret_t jl_extport_check(jl_port_t lport);
jl_port_t jl_port_l2c(jl_port_t lport);
jl_port_t jl_port_l2p(jl_port_t lport);
jl_port_t jl_port_c2l(jl_port_t cport);
jl_ret_t jl_portmask_l2c(jl_portmask_t lportmask, jl_portmask_t *cportmask);
jl_ret_t jl_portmask_c2l(jl_portmask_t cportmask, jl_portmask_t *lportmask);

#ifdef __cplusplus
}
#endif

#endif /* __JL61XX_CHIP_H__ */
