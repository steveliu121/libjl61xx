#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "jl_types.h"
#include "jl_error.h"
#include "jl_portable_gpio.h"

//"A GPIO (over Raspberrypi 3B+) porting example. Do not release this part, just for demo&debug using"
#include <gpiod.h>

struct gpiod_chip *g_gpio_chip;
static jl_int32 g_gpio_ref;
struct gpiod_line *line;


jl_ret_t port_gpio_request(jl_gpio_desc_t *gpio)
{
	if (!g_gpio_chip) {
		g_gpio_chip = gpiod_chip_open("/dev/gpiochip0");
		if (!g_gpio_chip) {
			JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Raspberrypi gpio chip open fail\n");
			return JL_ERR_FAIL;
		}
	}

	gpio->priv = gpiod_chip_get_line(g_gpio_chip, gpio->pin);
	if (!gpio->priv) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Raspberrypi gpio line get fail\n");
		return JL_ERR_FAIL;
	}

	g_gpio_ref++;

	return JL_ERR_OK;
}

jl_ret_t port_gpio_free(jl_gpio_desc_t *gpio)
{
	(void) gpio;

	g_gpio_ref--;
	if (!g_gpio_ref && g_gpio_chip) {
		gpiod_chip_close(g_gpio_chip);
		g_gpio_chip = NULL;
	}

	return JL_ERR_OK;
}

/*dir: 0: input, 1: output*/
void port_gpio_set_dir(jl_gpio_desc_t *gpio, jl_int32 dir)
{
	jl_int32 ret = 0;

	if (dir) {
		gpiod_line_release(gpio->priv);
		ret = gpiod_line_request_output(gpio->priv, gpio->name, 1);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "GPIO set direction out fail, %s\n", strerror(errno));
	} else {
		gpiod_line_release(gpio->priv);
		ret = gpiod_line_request_input_flags(gpio->priv, gpio->name, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "GPIO set direction input fail, %s\n", strerror(errno));
	}
}

void port_gpio_set_value(jl_gpio_desc_t *gpio, jl_int32 val)
{
	jl_ret_t ret;

	ret = gpiod_line_set_value(gpio->priv, val);
	if (ret == -1)
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "GPIO set value fail, %s\n", strerror(errno));
}

jl_int32 port_gpio_get_value(jl_gpio_desc_t *gpio)
{
	jl_int32 ret;

	ret = gpiod_line_get_value(gpio->priv);
	if (ret == -1) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "GPIO get value fail, %s\n", strerror(errno));
		return JL_ERR_FAIL;
	}

	return ret;
}
