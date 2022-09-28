#include <stdint.h>
#include <stdio.h>

#include "jl_error.h"
#include "jl_types.h"
#include "jl_device.h"
#include "portable/jl_portable.h"
#include "driver/hal_smi.h"


jl_uint16 jl_smi_mdio_read(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg)
{
	jl_uint16 val;

#ifdef CONFIG_JL_PHY_MUTEX
	port_mutex_lock(g_smi_dev.phy_mutex);
#endif

	val = port_mdio_read(io_desc, phy, reg);

#ifdef CONFIG_JL_PHY_MUTEX
	port_mutex_unlock(g_smi_dev.phy_mutex);
#endif

	return val;
}

void jl_smi_mdio_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
#ifdef CONFIG_JL_PHY_MUTEX
	port_mutex_lock(g_smi_dev.phy_mutex);
#endif

	port_mdio_write(io_desc, phy, reg, val);

#ifdef CONFIG_JL_PHY_MUTEX
	port_mutex_unlock(g_smi_dev.phy_mutex);
#endif
}

jl_api_ret_t jl_smi_mdio_init(jl_io_desc_t *io_desc)
{
#ifdef CONFIG_JL_PHY_MUTEX
	g_smi_dev.phy_mutex = port_mutex_init();
	if (!g_smi_dev.phy_mutex)
		return JL_ERR_INIT;
#endif

#ifdef CONFIG_JL_APB_MUTEX
	g_smi_dev.apb_mutex = port_mutex_init();
	if (!g_smi_dev.apb_mutex)
		return JL_ERR_INIT;
#endif

	return port_mdio_init(io_desc);
}

jl_api_ret_t jl_smi_mdio_deinit(jl_io_desc_t *io_desc)
{
#ifdef CONFIG_JL_PHY_MUTEX
	if (g_smi_dev.phy_mutex) {
		port_mutex_deinit(g_smi_dev.phy_mutex);
		g_smi_dev.phy_mutex = NULL;
	}
#endif

#ifdef CONFIG_JL_APB_MUTEX
	if (g_smi_dev.apb_mutex) {
		port_mutex_deinit(g_smi_dev.apb_mutex);
		g_smi_dev.apb_mutex = NULL;
	}
#endif

	return port_mdio_deinit(io_desc);
}
