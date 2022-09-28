/**
 * @file jl_switch.h
 * @brief Initialization
 */

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

#ifndef __JL_SWITCH_H__
#define __JL_SWITCH_H__

#include "jl_types.h"
#include "jl_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup initialization_group Initialization
 * @{
 */


/**
 * @struct jl_switch_info_s
 * @brief Initialization switch info.
 */
struct jl_switch_info_s {
	jl_uint32 core_ver;/**< Indicates switch pa core version */
	jl_uint32 pkg;/**< Indicates pkg chip id */
	jl_uint32 eco_ver;/**< Indicates switch pa eco version */
	jl_uint32 sdk_ver;/**< Indicates SDK version */
};

/**
 * @typedef jl_switch_info_t
 */
typedef struct jl_switch_info_s jl_switch_info_t;

/**
 * @enum stat_e
 * @brief Initialization stat_e.
 */
enum stat_e {
	STAT_UNINIT = 0,/**< Uninitialized State */
	STAT_READY,/**< Ready State */
	STAT_END/**< Invalid State */
};

/**
 * @public typedef jl_stat_t
 */
typedef enum stat_e jl_stat_t;


/**
 * @brief Initialize the switch system
 * @details This function initialize the switch system and sets chip registers to default configuration.
 * @param None
 * @return the switch system is initialized or not
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_FAIL		Fail
 */
JL_API jl_api_ret_t jl_switch_init(void);

/**
 * @brief Deinitialize the switch system
 * @details This function de-initialize the switch system.
 * @param None
 * @return the switch system is deinitialized or not
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_FAIL		Fail
 */
JL_API jl_api_ret_t jl_switch_deinit(void);

/**
 * @brief Get chip's version info
 * @details This function is used to get the version information
 * @param chip_id user defined chip index number
 * @param pinfo chip information
 * @return version info is sucessfully get or not
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_FAIL		Fail
 */
JL_API jl_api_ret_t jl_switch_info(jl_uint32 chip_id, jl_switch_info_t *pinfo);

/**
 * @brief set state to the switch
 * @details This function indicates set uninit or ready state to the switch .
 * @param state	the state of switch
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_PARAM		Fail
 */
JL_API jl_ret_t jl_switch_init_state_set(jl_stat_t state);

/**
 * @brief get state from the switch
 * @details This function indicates get uninit or ready state from the switch.
 * @param pstate	the pstate of switch
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_PARAM		Fail
 */
JL_API jl_ret_t jl_switch_init_state_get(jl_stat_t *pstate);


#ifdef __cplusplus
}
#endif
#endif /* __JL_SWITCH_H__ */
/** @} Initialization APIs*/
