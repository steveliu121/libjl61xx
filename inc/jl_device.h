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

#ifndef __JL_DEVICE_H__
#define __JL_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "jl_types.h"
#include "jl_ops.h"
#include "jl_switch.h"

struct jl_ops_s;
struct jl_io_desc_s;
struct jl_driver_s;
struct jl_device_s;
typedef struct jl_ops_s jl_ops_t;
typedef struct jl_io_desc_s jl_io_desc_t;
typedef struct jl_driver_s jl_driver_t;
typedef struct jl_device_s jl_device_t;

#define JL_MAX_CHIP_NUM			8

typedef struct jl_gpio_desc_s {
	jl_int32 pin;
	char name[16];
	void *priv;
} jl_gpio_desc_t;

typedef enum jl_io_type_e {
	JL_IO_CPU = 0,
	JL_IO_SMI,
	JL_IO_SMI_GPIO,
	JL_IO_SMI_SPI,
	JL_IO_SMI_SPI_DEMO,
	JL_IO_SMI_HID_DEMO,
	JL_IO_I2C,
	JL_IO_SPI
} jl_io_type_t;

typedef struct jl_io_desc_s {
	jl_chip_t chip;
	jl_io_type_t type;
	struct {
		jl_ret_t (*read)(jl_io_desc_t *io_desc,
				jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);
		jl_ret_t (*write)(jl_io_desc_t *io_desc,
				jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);
	} ops;
	union {
		struct {
			union {
				struct {
					jl_uint32 bus_id;
					void *priv;
				} mdio;
				struct {
					jl_gpio_desc_t mdc;
					jl_gpio_desc_t mdio;
					void *priv;
				} gpio;
				struct {
					jl_uint32 bus_id;
					void *priv;
				} spi;
				struct {
					jl_uint32 bus_id;
					void *priv;
				} hid;
			};
			struct {
				jl_api_ret_t (*init)(jl_io_desc_t *io_desc);
				jl_api_ret_t (*deinit)(jl_io_desc_t *io_desc);
				jl_uint16 (*read)(jl_io_desc_t *io_desc,
						jl_uint8 phy, jl_uint8 reg);
				void (*write)(jl_io_desc_t *io_desc,
						jl_uint8 phy, jl_uint8 reg,
						jl_uint16 val);
			} ops;
		} smi;
		struct {
			jl_uint32 bus_id;
			jl_uint32 addr;
			void *priv;
		} i2c;
		struct {
			jl_uint32 bus_id;
			void *priv;
		} spi;
	};
} jl_io_desc_t;

typedef struct jl_driver_s {
	jl_uint32 compat_id;
	char name[128];
	jl_ret_t (*probe)(jl_device_t *device);
	jl_ret_t (*remove)(jl_device_t *device);
	jl_ops_t ops;
	void *drv_data;
} jl_driver_t;

typedef struct jl_device_s {
	jl_uint32 compat_id;
	char name[128];
	jl_uint32 id;
	jl_driver_t *driver;
	jl_io_desc_t io_desc;
	void *switch_dev;
	void *priv;
} jl_device_t;

typedef struct jl_module_s {
	jl_driver_t **drivers;
	jl_device_t **devices;
	jl_uint32 driver_num;
	jl_uint32 device_num;
	void *api_mutex;
	jl_stat_t stat;
} jl_module_t;

/* extern struct jl_module_s g_module; */
/* extern struct jl_device_s *g_devices; */
/* extern struct jl_driver_s *g_drivers; */

#ifdef __cplusplus
}
#endif

#endif /* __JL_DEVICE_H__ */
