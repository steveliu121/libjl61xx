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
#include "driver/jl_reg_io.h"
/*TODO replace this by using func register template*/

extern struct jl_module_s g_module;

static jl_ret_t __driver_probe(jl_device_t *device)
{
	jl_ret_t ret;

	JL_ASSERT(device != NULL);
	JL_ASSERT(device->driver != NULL);
	JL_ASSERT(device->driver->probe != NULL);

	ret = device->driver->probe(device);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"[%s]-[%s]: Driver probe failed!!!\n",
				device->driver->name, device->name);
		return ret;
	}

	return JL_ERR_OK;

}

static jl_ret_t __device_reorder(jl_device_t **devices, jl_uint32 num)
{
	jl_device_t *_devices[JL_MAX_CHIP_NUM] = {NULL};
	jl_uint32 id;
	jl_uint32 not_inorder = 0;
	jl_uint32 i;

	JL_ASSERT(devices != NULL);

	for (i = 0; i < num; i++) {
		if (devices[i] == NULL)
			continue;

		id = devices[i]->id;
		if (id >= JL_MAX_CHIP_NUM) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
					"Error: chip id out of range!!!\n");
			return JL_ERR_BADID;
		}
		if (_devices[id] != NULL) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
					"Error: duplicated chip id!!!\n");
			return JL_ERR_BADID;
		}

		_devices[id] = devices[i];

		if (id != i)
			not_inorder = 1;
	}

	if (!not_inorder)
		return JL_ERR_OK;

	for (i = 0; i < num; i++)
		devices[i] = _devices[i];

	return JL_ERR_OK;
}

static jl_ret_t __driver_match_device(jl_module_t *module)
{
	jl_ret_t ret;
	jl_uint32 driver_num;
	jl_uint32 device_num;
	jl_uint32 i, j;

	JL_ASSERT(module != NULL);

	driver_num = module->driver_num;
	device_num = module->device_num;

	ret = __device_reorder(module->devices, module->device_num);
	if (ret)
		return ret;

	for (i = 0; i < driver_num; i++) {
		for (j = 0; j < device_num; j++) {
			if (module->devices[j]->compat_id == module->drivers[i]->compat_id) {
				module->devices[j]->driver = module->drivers[i];

				ret = __driver_probe(module->devices[j]);
				if (ret)
					return ret;
			}
		}
	}

	return JL_ERR_OK;
}

static jl_ret_t __module_init(jl_module_t *module)
{
	jl_ret_t ret;

	ret = __driver_match_device(module);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"Driver match failed!!!\n");
		return ret;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
			"Module initialize done!!!\n");

	return JL_ERR_OK;
}

static jl_ret_t __module_exit(jl_module_t *module)
{
	jl_uint32 i;

	JL_ASSERT(module != NULL);

	for (i = 0; i < module->device_num ; i++)
		module->devices[i]->driver->remove(module->devices[i]);

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
			"JLSemi switch module exit!!!\n");

	return JL_ERR_OK;
}

JL_API jl_api_ret_t jl_switch_init(void)
{
	jl_api_ret_t ret = JL_ERR_OK;

	if (JL_STAT_IS_READY())
		return JL_ERR_OK;

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_dump_enable();
	jl_dump_add_comment("======jl switch init======");
#endif

	ret = __module_init(&g_module);
	if (ret)
		return ret;

	JL_API_MUTEX_INIT();
	JL_STAT_INIT();

	return JL_ERR_OK;
}

JL_API jl_api_ret_t jl_switch_deinit(void)
{
	jl_api_ret_t ret = JL_ERR_OK;

	if (JL_STAT_IS_UNINIT())
		return JL_ERR_OK;

	JL_API_MUTEX_LOCK();

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_dump_add_comment("======jl switch deinit======");
#endif

	ret = __module_exit(&g_module);
	if (ret)
		return ret;

	JL_API_MUTEX_UNLOCK();

	JL_API_MUTEX_DEINIT();
	JL_STAT_DEINIT();

	return ret;
}

JL_API jl_api_ret_t jl_switch_info(jl_uint32 chip_id, jl_switch_info_t *pinfo)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, switch_info, chip_id, pinfo);

	return ret;
}

JL_API jl_ret_t jl_switch_init_state_set(jl_stat_t state)
{
	if (state >= STAT_END)
		return JL_ERR_PARAM;

	g_module.stat = state;

	return JL_ERR_OK;
}

JL_API jl_ret_t jl_switch_init_state_get(jl_stat_t *pstate)
{
	JL_CHECK_POINTER(pstate);

	*pstate = g_module.stat;

	return JL_ERR_OK;
}
