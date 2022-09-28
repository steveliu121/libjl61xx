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

#ifndef __JL_TYPES_H__
#define __JL_TYPES_H__


#if defined(WIN32)
#if defined(CONFIG_JL_DLL_EXPORT_SYMBOL)
#define JL_API          __declspec(dllexport)
#elif defined(CONFIG_JL_STATIC_LINK)
#define JL_API
#else
#define JL_API          __declspec(dllimport)
#endif
#else/* WIN32*/
#define JL_API
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned long long		jl_uint64;
typedef long long			jl_int64;
typedef unsigned int			jl_uint32;
typedef int				jl_int32;
typedef unsigned short			jl_uint16;
typedef short				jl_int16;
typedef unsigned char			jl_uint8;
typedef char				jl_int8;
typedef unsigned int			jl_bool;
typedef unsigned int			jl_uintptr;
typedef jl_int32			jl_api_ret_t;
typedef jl_int32			jl_ret_t;
typedef jl_uint64			jl_u_long_t;

#define CONST_T     const


typedef enum jl_enable_e {
	DISABLED = 0,
	ENABLED,
	JL_ENABLE_END
} jl_enable_t;

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN      6
#endif
/**
 * @union mac_addr
 * @brief Initialization mac_addr.
 * @typedef jl_mac_addr_t
 */
typedef union mac_addr {
	jl_uint8 addr[ETHER_ADDR_LEN];/**< Indicates MAC Address */
	jl_uint64 val:48;/**< Indicates 48 bit MAC Address */
} jl_mac_addr_t;

/**
 * @public typedef jl_portmask_t
 */
typedef jl_uint32	jl_portmask_t;

/**
 * @enum jl_chip_e
 * @brief switch chip identifier.
 * @typedef jl_chip_t
 */

typedef enum jl_chip_e {
	JL_CHIP_51XX = 0,
	JL_CHIP_61XX
} jl_chip_t;


/**
 * @enum jl_port_e
 * @brief Initialization jl_port_e.
 * @typedef jl_port_t
 */
typedef enum jl_port_e {
	UTP_PORT0 = 0,
	UTP_PORT1,
	UTP_PORT2,
	UTP_PORT3,
	UTP_PORT4,
	UTP_PORT5,
	UTP_PORT6,
	UTP_PORT7,

	EXT_PORT0 = 8,
	EXT_PORT1,

	JL_PORT_MAX = 10,
	UNDEF_PORT = 0xffffffff
} jl_port_t;


/**
 * @enum jl_port_type_e
 * @brief Initialization port_type_e.
 * @typedef jl_port_type_t
 */
typedef enum jl_port_type_e {
	PORT_TYPE_UTP = 0,/**< UTP Port */
	PORT_TYPE_EXT,/**< External Port */
	PORT_TYPE_IN,/**< Inner Port */
	PORT_TYPE_UNDEF,/**< Undefined Port */
	PORT_TYPE_END/**< Invalid Type */
} jl_port_type_t;

/**
 * @enum jl_port_dir_e
 * @brief port direction
 * @typedef jl_port_dir_t
 */
typedef enum jl_port_dir_e {
	PORT_DIR_INGRESS = 0,/**< ingress direction */
	PORT_DIR_EGRESS = 1/**< egress direction */
} jl_port_dir_t;


#endif /* __JL_TYPES_H__ */
