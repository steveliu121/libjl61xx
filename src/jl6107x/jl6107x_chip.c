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

#include "jl_base.h"
#include "jl6107x/jl6107x_chip.h"


#if (defined CONFIG_JL_CHIP_ID_6105)
const struct jl_switch_dev_s g_switch_dev_6105 = {
	/* switch chip id */
	CHIP_ID_JL6105,

	/*core version */
	0x1,

	/* port transform: logic to switch core */
	{2, 3, 4, 5, 6, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, 7},

	/* port transform: logic to phy/mac(lmac/smac/cmac) */
	{0, 1, 2, 3, 4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, 0},

	/* port transform: switch core to logic */
	{UNDEF_PORT, UNDEF_PORT, UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3,
		UTP_PORT4, CPU_PORT0, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* logic port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_CPU},

	/* switch core port type */
	{PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_CPU,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF},

	/* max logic port */
	CPU_PORT0,

	/* max switch core port */
	7,

	/* logic port mask */
	((0x1UL << UTP_PORT0) | (0x1UL << UTP_PORT1) | (0x1UL << UTP_PORT2) | (0x1UL << UTP_PORT3) | (0x1UL << UTP_PORT4) | (0x1UL << CPU_PORT0)),

	/* utp port mask */
	((0x1UL << UTP_PORT0) | (0x1UL << UTP_PORT1) | (0x1UL << UTP_PORT2) | (0x1UL << UTP_PORT3) | (0x1UL << UTP_PORT4)),

	/* ext port mask */
	0,

	/* cpu port mask */
	(0x1UL << CPU_PORT0),

	/* switch core port mask */
	0xFC
};
#endif

#if (defined CONFIG_JL_CHIP_ID_6107)
const struct jl_switch_dev_s g_switch_dev_6107 = {
	/* switch chip id */
	CHIP_ID_JL6107,

	/*core version */
	0x1,

	/* port transform: logic to switch core */
	{2, 3, 4, 5, 6, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		0, 1, 7},


	/* port transform: logic to phy/mac(lmac/smac/cmac) */
	{0, 1, 2, 3, 4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		0, 1, 0},

	/* port transform: switch core to logic */
	{EXT_PORT0, EXT_PORT1, UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3,
		UTP_PORT4, CPU_PORT0, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* logic port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_EXT, PORT_TYPE_EXT,
		PORT_TYPE_CPU},

	/* switch core port type */
	{PORT_TYPE_EXT, PORT_TYPE_EXT, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_CPU,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF},

	/* max logic port */
	CPU_PORT0,

	/* max switch core port */
	7,

	/* logic port mask */
	((0x1UL << UTP_PORT0) | (0x1UL << UTP_PORT1) | (0x1UL << UTP_PORT2) | (0x1UL << UTP_PORT3) | (0x1UL << UTP_PORT4) | (0x1UL << EXT_PORT0) | (0x1UL << EXT_PORT1) | (0x1UL << CPU_PORT0)),

	/* utp port mask */
	((0x1UL << UTP_PORT0) | (0x1UL << UTP_PORT1) | (0x1UL << UTP_PORT2) | (0x1UL << UTP_PORT3) | (0x1UL << UTP_PORT4)),

	/* ext port mask */
	((0x1UL << EXT_PORT0) | (0x1UL << EXT_PORT1)),

	/* cpu port mask */
	(0x1UL << CPU_PORT0),

	/* switch core port mask */
	0xFF
};
#endif

#if (defined CONFIG_JL_CHIP_ID_6107S)
const struct jl_switch_dev_s g_switch_dev_6107s = {
	/* switch chip id */
	CHIP_ID_JL6107S,

	/*core version */
	0x1,

	/* port transform: logic to switch core */
	{2, 3, 4, 5, 6, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		0, 1, 7},

	/* port transform: logic to phy/mac(lmac/smac/cmac) */
	{0, 1, 2, 3, 4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		0, 1, 0},

	/* port transform: switch core to logic */
	{EXT_PORT0, EXT_PORT1, UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3,
		UTP_PORT4, CPU_PORT0, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* logic port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_EXT, PORT_TYPE_EXT,
		PORT_TYPE_CPU},

	/* switch core port type */
	{PORT_TYPE_EXT, PORT_TYPE_EXT, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_CPU,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF},

	/* max logic port */
	CPU_PORT0,

	/* max switch core port */
	7,

	/* logic port mask */
	((0x1UL << UTP_PORT0) | (0x1UL << UTP_PORT1) | (0x1UL << UTP_PORT2) | (0x1UL << UTP_PORT3) | (0x1UL << UTP_PORT4) | (0x1UL << EXT_PORT0) | (0x1UL << EXT_PORT1) | (0x1UL << CPU_PORT0)),

	/* utp port mask */
	((0x1UL << UTP_PORT0) | (0x1UL << UTP_PORT1) | (0x1UL << UTP_PORT2) | (0x1UL << UTP_PORT3) | (0x1UL << UTP_PORT4)),

	/* ext port mask */
	(0x1UL << EXT_PORT0),

	/* cpu port mask */
	(0x1UL << CPU_PORT0),

	/* switch core port mask */
	0xFF
};
#endif

#if (defined CONFIG_JL_CHIP_ID_6107SC)
const struct jl_switch_dev_s g_switch_dev_6107sc = {
	/* switch chip id */
	CHIP_ID_JL6107SC,

	/*core version */
	0x1,

	/* port transform: logic to switch core */
	{2, 3, 4, 5, 6, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		0, 1, 7},

	/* port transform: logic to phy/mac(lmac/smac/cmac) */
	{0, 1, 2, 3, 4, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		0, 1, 0},

	/* port transform: switch core to logic */
	{EXT_PORT0, EXT_PORT1, UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3,
		UTP_PORT4, CPU_PORT0, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT, UNDEF_PORT,
		UNDEF_PORT, UNDEF_PORT, UNDEF_PORT},

	/* logic port type */
	{PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_EXT, PORT_TYPE_EXT,
		PORT_TYPE_CPU},

	/* switch core port type */
	{PORT_TYPE_EXT, PORT_TYPE_EXT, PORT_TYPE_UTP, PORT_TYPE_UTP,
		PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_UTP, PORT_TYPE_CPU,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF, PORT_TYPE_UNDEF,
		PORT_TYPE_UNDEF},

	/* max logic port */
	CPU_PORT0,

	/* max switch core port */
	7,

	/* logic port mask */
	((0x1UL << UTP_PORT0) | (0x1UL << UTP_PORT1) | (0x1UL << UTP_PORT2) | (0x1UL << UTP_PORT3) | (0x1UL << UTP_PORT4) | (0x1UL << EXT_PORT0) | (0x1UL << EXT_PORT1) | (0x1UL << CPU_PORT0)),

	/* utp port mask */
	((0x1UL << UTP_PORT0) | (0x1UL << UTP_PORT1) | (0x1UL << UTP_PORT2) | (0x1UL << UTP_PORT3) | (0x1UL << UTP_PORT4)),

	/* ext port mask */
	((0x1UL << EXT_PORT0) | (0x1UL << EXT_PORT1)),

	/* cpu port mask */
	(0x1UL << CPU_PORT0),

	/* switch core port mask */
	0xFF
};
#endif
