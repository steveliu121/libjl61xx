#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "test.h"
#include "driver/jl_reg_io.h"

static int g_exit;

static void sig_handle(int sig)
{
	(void) sig;
	g_exit = 1;
}

static void clear_window(void)
{
	fprintf(stdout, "\033c");
}

void print_usage(void)
{
	printf("======SDK TEST======\n");
	printf("Test commad: <index,value>\n");
	printf("1. lport\n");
	printf("2. utp_port\n");
	printf("3. ext_port\n");
	printf("4. portmask\n");
	printf("5. utp_portmask\n");
	printf("6. ext_portmask\n");
	printf("7. lcport\n");
	printf("8. clport\n");
	printf("9. lcportmask\n");
	printf("10. clportmask\n");
	printf("11. for_each\n");
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	char console_input[128] = {0};
	jl_uint32 cmd;
	jl_uint32 value;
	char value_str[128] = {0};
	jl_uint32 chip_id = 0;
	struct jl_test_s test;
	jl_uint32 val;
	jl_api_ret_t ret;

	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);

	ret = jl_switch_init();
	if (ret) {
		printf("smi init fail\n");
		return -1;
	}

	while (!g_exit) {
		print_usage();

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		memset(&test, 0xff, sizeof(test));

		sscanf(console_input, "%d,%s", &cmd, value_str);
		if (!strncmp(value_str, "0x", 2))
			value = strtoul(value_str, NULL, 16);
		else
			value = strtoul(value_str, NULL, 10);

		switch (cmd){
			case 1:
				test.lport = value;
				break;
			case 2:
				test.utp_port = value;
				break;
			case 3:
				test.ext_port = value;
				break;
			case 4:
				test.portmask = value;
				break;
			case 5:
				test.utp_portmask = value;
				break;
			case 6:
				test.ext_portmask = value;
				break;
			case 7:
				test.lcport = value;
				break;
			case 8:
				test.clport = value;
				break;
			case 9:
				test.lcportmask = value;
				break;
			case 10:
				test.clportmask = value;
				break;
			case 11:
				test.for_each = value;
				break;
		}

		ret = jl_test(chip_id, &test);
		if (ret)
			printf("jl_test error[%d]\n", ret);
	}

exit:
	jl_test_io_read(chip_id, 0x1202d8, &val);
	printf("jl_test_io_read:0x%04x\n", val);
	jl_test_io_write(chip_id, 0x1202d8, 0xff);
	printf("jl_test_io_write:0xff\n");
	jl_test_io_read(chip_id, 0x1202d8, &val);
	printf("jl_test_io_read:0x%04x\n", val);

	jl_switch_deinit();

	return 0;
}
