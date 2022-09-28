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

#ifndef __JL_KIT_H__
#define __JL_KIT_H__

#include "jl_error.h"
#include "jl_types.h"
#include "jl_debug.h"
#include "jl_device.h"

extern struct jl_module_s g_module;

#define JL_ARRAY_SIZE(x)		(sizeof(x)/sizeof((x)[0]))

#define JL_CHECK_POINTER(x)			\
	do {					\
		if (x == NULL)			\
			return JL_ERR_PARAM;	\
	} while (0)

#define JL_ASSERT(expr) \
	do { \
		if (!(expr)) \
			return JL_ERR_ASSERT;	\
	} while (0)

#ifdef CONFIG_JL_API_MUTEX
#include "portable/jl_portable.h"
#define JL_API_MUTEX_INIT()						\
	do {								\
		g_module.api_mutex = port_mutex_init();		\
		if (g_module.api_mutex == NULL)			\
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "api mutex init fail\n"); \
	} while (0)

#define JL_API_MUTEX_DEINIT()						\
	do {								\
		if (g_module.api_mutex != NULL) {			\
			port_mutex_deinit(g_module.api_mutex);	\
			g_module.api_mutex = NULL;			\
		}							\
	} while (0)

#define JL_API_MUTEX_LOCK()			\
	do {								\
		if (g_module.api_mutex != NULL)			\
			port_mutex_lock(g_module.api_mutex);	\
	} while (0)

#define JL_API_MUTEX_UNLOCK()						\
	do {								\
		if (g_module.api_mutex != NULL)			\
			port_mutex_unlock(g_module.api_mutex);	\
	} while (0)
#else
#define JL_API_MUTEX_INIT()
#define JL_API_MUTEX_DEINIT()
#define JL_API_MUTEX_LOCK()
#define JL_API_MUTEX_UNLOCK()
#endif /* JL_API_MUTEX */

#define JL_STAT_INIT()		(g_module.stat = STAT_READY)
#define JL_STAT_DEINIT()	(g_module.stat = STAT_UNINIT)
#define JL_STAT_IS_READY()	(g_module.stat == STAT_READY)
#define JL_STAT_IS_UNINIT()	(g_module.stat == STAT_UNINIT)
#define JL_CHECK_STAT()					\
	do {						\
		if (g_module.stat != STAT_READY)	\
			return JL_ERR_INIT;		\
	} while (0)

#define JL_DRV_SAFE_CALL(ret, func, chip_id, ...) \
	do {\
		JL_CHECK_STAT();\
		JL_API_MUTEX_LOCK();\
		JL_ASSERT(chip_id < JL_MAX_CHIP_NUM);\
		JL_ASSERT(g_module.devices[chip_id] != NULL);\
		JL_ASSERT(g_module.devices[chip_id]->driver->ops.func != NULL);\
		ret = g_module.devices[chip_id]->driver->ops.func(g_module.devices[chip_id], __VA_ARGS__);\
		JL_API_MUTEX_UNLOCK();\
	} while (0)


/* Macros for memory access operations */
#define _REG8P(p, i)                        ((jl_uint8 *) ((jl_uintptr)((p) + (i))))
#define _REG16P(p, i)                       ((jl_uint16 *) ((jl_uintptr)((p) + (i))))
#define _REG32P(p, i)                       ((jl_uint32 *) ((jl_uintptr)((p) + (i))))
#define _REG8(p, i)                         (*(_REG8P(p, i)))
#define _REG16(p, i)                        (*(_REG16P(p, i)))
#define _REG32(p, i)                        (*(_REG32P(p, i)))
#define REG8(addr)                          _REG8((addr), 0)
#define REG16(addr)                         _REG16((addr), 0)
#define REG32(addr)                         _REG32((addr), 0)

/* Macros for address type convert and access operations */
#define ADDR16(addr)                        ((jl_uint32)(jl_uintptr)(addr))
#define ADDR32(addr)                        ((jl_uint32)(jl_uintptr)(addr))
#define ADDR8P(addr)                        ((jl_uint8 *)(jl_uintptr)(addr))
#define ADDR16P(addr)                       ((jl_uint16 *)(jl_uintptr)(addr))
#define ADDR32P(addr)                       ((jl_uint32 *)(jl_uintptr)(addr))

/* Macros for Bit Operations */
#define BITMASK_MAX                         0xFFFFFFFFUL
#define BITOFS_MAX                          31

// For 32 bit OS, Bit Operation support mask 32 bit wide
#define BIT(bitofs)                         (0x1UL << (bitofs))
#define BITS(start, end)                    ((BITMASK_MAX) << (start) & (BITMASK_MAX) >> (BITOFS_MAX - (end)))
#define GET_BIT(regval, bitofs)             (((regval) >> (bitofs)) & 0x1)
#define SET_BIT(regval, bitofs)             ((regval) |= BIT(bitofs))
#define CLR_BIT(regval, bitofs)             ((regval) &= (~BIT(bitofs)))
#define FLIP_BIT(regval, bitofs)            ((regval) ^= BIT(bitofs))
#define WRITE_BIT(regval, bitofs, val)      do { CLR_BIT(regval, bitofs); ((regval) |= ((val) << bitofs) & BIT(bitofs)); } while (0)
#define CHECK_BIT(regval, bitofs)           (!!((regval) & (0x1UL<<(bitofs))))
#define GET_BITS(regval, start, end)        (((regval) & BITS((start), (end))) >> (start))
#define SET_BITS(regval, start, end)        ((regval) |= BITS((start), (end)))
#define CLR_BITS(regval, start, end)        ((regval) &= (~BITS((start), (end))))
#define FLIP_BITS(regval, start, end)       ((regval) ^= BITS((start), (end)))
#define WRITE_BITS(regval, start, end, val) do { CLR_BITS(regval, start, end); ((regval) |= ((val) << start) & BITS((start), (end))); } while (0)
#define CHECK_BITS_ALL(regval, start, end)  (!((~(regval)) & BITS((start), (end))))
#define CHECK_BITS_ANY(regval, start, end)  ((regval) & BITS((start), (end)))

#define BITMASK_SET(regval, mask)           ((regval) |= (mask))
#define BITMASK_CLR(regval, mask)           ((regval) &= (~(mask)))
#define BITMASK_FLIP(regval, mask)          ((regval) ^= (mask))
#define BITMASK_CHECK_ALL(regval, mask)     (!((~(regval)) & (mask)))
#define BITMASK_CHECK_ANY(regval, mask)     ((regval) & (mask))

#endif /* __JL_KIT_H__ */
