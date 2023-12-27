#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include "jl_base.h"
#include "jl_switch.h"
#include "jl_acl.h"

static int g_exit;

static void sig_handle(int sig)
{
	(void) sig;
	g_exit = 1;
}

static void clear_window(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\033c");
}

static void print_ACL_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======ACL TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. ACL Tcam table test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. ACL Hash table test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. MAC Filter test\n");
}

static void acl_tcam_table_test(jl_uint32 chip_id)
{
	jl_uint32 field_idx = 0;
	jl_uint32 rule_idx = 1;
	jl_uint8 template_idx = 1;
	jl_uint8 port_idx = 1;
	jl_uint8 counter = 0;
	jl_uint32 i;

	jl_api_ret_t ret;
	jl_acl_template_t acl_templ;
	jl_acl_rule_t acl_rule;
	jl_acl_action_t acl_action;
	jl_acl_field_data_t acl_field;
	jl_acl_port_t port_state;
	jl_acl_field_data_t *pfield;

	/* set acl template */
	acl_templ.field[0].field_type = ACL_FIELD_IP6_SA;
	acl_templ.field[1].field_type = ACL_FIELD_RULE_POINTER;
	acl_templ.field[2].field_type = ACL_FIELD_L4_SOURCE_PORT;
	acl_templ.field[3].field_type = ACL_FIELD_NO_FIELD;
	acl_templ.field[4].field_type = ACL_FIELD_NO_FIELD;
	acl_templ.field[5].field_type = ACL_FIELD_NO_FIELD;

	ret = jl_acl_template_set(chip_id, template_idx, &acl_templ);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL set template %d error!\n",template_idx);
		return;
	}

	memset(&acl_templ, 0, sizeof(jl_acl_template_t));

	ret = jl_acl_template_get(chip_id, template_idx, &acl_templ);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get template %d error!\n",template_idx);
		return;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL TEMPLATE %d :",template_idx);

	for (field_idx = 0; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, " %d  ",acl_templ.field[field_idx].field_type);
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n set acl template done!\n");

	/* add rule */
	memset(&acl_rule, 0, sizeof(jl_acl_rule_t));
	memset(&acl_action, 0, sizeof(jl_acl_action_t));

	acl_rule.templ_idx = template_idx;

	memset(&acl_field, 0, sizeof(jl_acl_field_data_t));

	acl_field.field_type = ACL_FIELD_IP6_SA;
	acl_field.field.s_ip6.value.addr[0] = 0x1122;
	acl_field.field.s_ip6.value.addr[1] = 0x3344;
	acl_field.field.s_ip6.value.addr[6] = 0xaabb;
	acl_field.field.s_ip6.value.addr[7] = 0xccdd;

	for (i = 0; i < 8; i++)
	{
		acl_field.field.s_ip6.mask.addr[i] = 0xffff;
	}

	ret = jl_acl_field_insert(chip_id, &acl_field, &acl_rule);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL insert field error!\n");
		return;
	}


	ret = jl_acl_stat_counter_get(chip_id, &counter);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get counter error!\n");
		return;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "get counter index: %d \n", counter);

	acl_action.update_counter_enable = TRUE;
	acl_action.counter = counter;

	acl_action.force_queue_enable = TRUE;
	acl_action.e_queue = 1;

	ret = jl_acl_rule_set(chip_id, rule_idx, &acl_rule, &acl_action);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL set rule %d error!\n", rule_idx);
		return;
	}

	ret = jl_acl_field_clear(chip_id, &acl_rule); //free memery malloced
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL clear fields error!\n");
		return;
	}

	memset(&acl_rule, 0, sizeof(jl_acl_rule_t));
	memset(&acl_action, 0, sizeof(jl_acl_action_t));

	ret = jl_acl_rule_get(chip_id, rule_idx, &acl_rule, &acl_action);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get rule %d error!\n", rule_idx);
		return;
	}

	pfield = acl_rule.head;
	while (pfield)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "field type: %d ", pfield->field_type);

		if (pfield->field_type == ACL_FIELD_RULE_POINTER)
		{
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "val: %x \n", pfield->field.rule_pointer.value);
		}
		else if (pfield->field_type == ACL_FIELD_IP6_SA)
		{
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "val:");
			for (i = 0; i < 8; i++)
			{
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "%x : ", pfield->field.s_ip6.value.addr[7 - i]);
			}
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");
		}
		pfield = pfield->next;
	}
	ret = jl_acl_field_clear(chip_id, &acl_rule); //free memery malloced
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL clear fields error!\n");
		return;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n set acl rule done!\n");

	ret= jl_acl_stat_disable(chip_id, rule_idx);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Disable ACL statistics error!\n");

	ret = jl_acl_stat_counter_get(chip_id, &counter);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get counter error!\n");
		return;
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "get counter index: %d \n", counter);

	ret= jl_acl_stat_enable(chip_id, rule_idx, counter);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Enable ACL statistics error!\n");

	memset(&port_state, 0, sizeof(jl_acl_port_t));

	ret = jl_acl_port_get(chip_id, port_idx, &port_state);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get port %d state error!\n", port_idx);
		return;
	}

	port_state.valid = TRUE;
	port_state.enable = ENABLED;
	port_state.acl_tpl_type = template_idx;

	ret = jl_acl_port_set(chip_id, port_idx, &port_state);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL enable port %d acl error!\n", port_idx);
		return;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n enable port acl done!\n");

	ret = jl_acl_rule_del(chip_id, rule_idx);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL delete rule %d error!\n", rule_idx);
		return;
	}

	port_state.valid = TRUE;
	port_state.enable = DISABLED;

	ret = jl_acl_port_set(chip_id, port_idx, &port_state);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL disable port %d acl error!\n", port_idx);
		return;
	}

	return;
}

static void acl_hash_table_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret;
	jl_uint32 i, j;
	jl_uint32 field_idx = 0;
	jl_uint32 rule_idx = 0;
	jl_uint8 template_idx = 0;
	jl_uint8 port_idx = 0;
	jl_uint8 counter = 0;
	jl_uint8 buckets_num = 4;
	jl_acl_template_t acl_templ;
	jl_acl_rule_t acl_rule;
	jl_acl_action_t acl_action;
	jl_acl_field_data_t acl_field;
	jl_acl_port_t port_state;
	jl_acl_field_data_t *pfield;

	/* set acl template */
	acl_templ.field[0].field_type = ACL_FIELD_MAC_DA;
	acl_templ.field[1].field_type = ACL_FIELD_IP_DA;
	acl_templ.field[2].field_type = ACL_FIELD_OUTER_VID;
	acl_templ.field[3].field_type = ACL_FIELD_OUTER_PCP;
	acl_templ.field[4].field_type = ACL_FIELD_NO_FIELD;
	acl_templ.field[5].field_type = ACL_FIELD_NO_FIELD;

	ret = jl_acl_template_set(chip_id, template_idx, &acl_templ);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL set template %d error!\n",template_idx);
		return;
	}

	memset(&acl_templ, 0, sizeof(jl_acl_template_t));

	ret = jl_acl_template_get(chip_id, template_idx, &acl_templ);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get template %d error!\n",template_idx);
		return;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL TEMPLATE %d :",template_idx);

	for (field_idx = 0; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, " %d  ",acl_templ.field[field_idx].field_type);
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n set acl template done!\n");

	/* add rule */
	memset(&acl_rule, 0, sizeof(jl_acl_rule_t));
	memset(&acl_action, 0, sizeof(jl_acl_action_t));

	acl_rule.templ_idx = template_idx;

	memset(&acl_field, 0, sizeof(jl_acl_field_data_t));
	acl_field.field_type = ACL_FIELD_MAC_DA;
	acl_field.field.d_mac.value.val = 0xaabbccddeeff;  //ff:ee:dd:cc:bb:aa
	acl_field.field.d_mac.mask.val = 0xffffffffffff;
	ret = jl_acl_field_insert(chip_id, &acl_field, &acl_rule);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL add field error!\n");
		return;
	}

	memset(&acl_field, 0, sizeof(jl_acl_field_data_t));
	acl_field.field_type = ACL_FIELD_IP_DA;
	acl_field.field.d_ip.value.addr[0] = 0xc0;  //192.168.1.2
	acl_field.field.d_ip.value.addr[1] = 0xa8;
	acl_field.field.d_ip.value.addr[2] = 0x1;
	acl_field.field.d_ip.value.addr[3] = 0x2;

	acl_field.field.d_ip.mask.addr[0] = 0xff;
	acl_field.field.d_ip.mask.addr[1] = 0xff;
	acl_field.field.d_ip.mask.addr[2] = 0xff;
	acl_field.field.d_ip.mask.addr[3] = 0xff;

	ret = jl_acl_field_insert(chip_id, &acl_field, &acl_rule);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL add field error!\n");
		return;
	}

	memset(&acl_field, 0, sizeof(jl_acl_field_data_t));
	acl_field.field_type = ACL_FIELD_OUTER_VID;
	acl_field.field.outer_vid.value = 200;
	acl_field.field.outer_vid.mask = 0xfff;
	ret = jl_acl_field_insert(chip_id, &acl_field, &acl_rule);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL add field error!\n");
		return;
	}

	memset(&acl_field, 0, sizeof(jl_acl_field_data_t));
	acl_field.field_type = ACL_FIELD_OUTER_PCP;
	acl_field.field.outer_pcp.value = 7;
	acl_field.field.outer_pcp.mask = 0x7;
	ret = jl_acl_field_insert(chip_id, &acl_field, &acl_rule);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL add field error!\n");
		return;
	}

	buckets_num += 1;

	acl_action.force_queue_enable = TRUE;
	acl_action.e_queue = 3;
	acl_action.update_counter_enable = TRUE;

	for (i = 0; i < buckets_num; i++)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "add rule %d begin\n\n", i);

		ret = jl_acl_stat_counter_get(chip_id, &counter);
		if (ret)
		{
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get counter error!\n");
			return;
		}

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "get counter index: %d \n", counter);

		acl_action.counter = counter;

		ret = jl_acl_rule_set(chip_id, rule_idx, &acl_rule, &acl_action);
		if (ret)
		{
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL add rule %d error!\n", rule_idx);
			return;
		}
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "hash table : %d , entry idx : %d\n", acl_rule.table_type, acl_rule.entry_idx);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "add rule %d done\n\n", rule_idx);
		rule_idx++;
	}

	ret = jl_acl_field_clear(chip_id, &acl_rule); //free memery malloced
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL clear fields error!\n");
		return;
	}

	for (i = 0; i < rule_idx; i++)
	{
		memset(&acl_rule, 0, sizeof(jl_acl_rule_t));
		memset(&acl_action, 0, sizeof(jl_acl_action_t));

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "read rule %d begin\n\n", i);

		ret = jl_acl_rule_get(chip_id, i, &acl_rule, &acl_action);
		if (ret)
		{
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get rule %d error!\n", i);
			return;
		}

		pfield = acl_rule.head;
		while (pfield)
		{
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, " field type: %d \n val: ",pfield->field_type);

			if (pfield->field_type == ACL_FIELD_MAC_DA)
			{

				for (j = 0; j < 6; j++)
				{
					JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "%x : ",pfield->field.d_mac.value.addr[5 - j]);
				}
			}
			else if (pfield->field_type == ACL_FIELD_MAC_SA)
			{
				for (j = 0; j < 6; j++)
				{
					JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "%x : ",pfield->field.s_mac.value.addr[5 - j]);
				}
			}
			else if (pfield->field_type == ACL_FIELD_OUTER_VID)
			{
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "%x",pfield->field.outer_vid.value);
			}
			else if (pfield->field_type == ACL_FIELD_OUTER_PCP)
			{
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "%x",pfield->field.outer_pcp.value);
			}
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");
			pfield = pfield->next;
		}

		ret = jl_acl_field_clear(chip_id, &acl_rule); //free memery malloced
		if (ret)
		{
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL clear fields error!\n");
			return;
		}

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "read rule %d done\n\n", i);
	}

	memset(&port_state, 0, sizeof(jl_acl_port_t));

	ret = jl_acl_port_get(chip_id, port_idx, &port_state);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get port %d state error!\n", port_idx);
		return;
	}

	port_state.valid = TRUE;
	port_state.enable = ENABLED;
	port_state.acl_tpl_type = template_idx;

	ret = jl_acl_port_set(chip_id, port_idx, &port_state);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL enable port %d acl error!\n", port_idx);
		return;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n enable port acl done!\n");

	for (i = 0; i < rule_idx; i++)
	{
		ret = jl_acl_rule_del(chip_id, i);
		if (ret)
		{
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL delete rule %d error!\n", i);
			return;
		}
	}

	port_state.valid = TRUE;
	port_state.enable = DISABLED;

	ret = jl_acl_port_set(chip_id, port_idx, &port_state);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL disable port %d acl error!\n", port_idx);
		return;
	}

}

static void acl_mac_filter_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret;
	jl_uint32 rule_idx = 0;
	jl_acl_mac_range_t mac_range;
	jl_acl_mac_action_t action;

	memset(&action, 0, sizeof(jl_acl_mac_action_t));
	memset(&mac_range, 0, sizeof(jl_acl_mac_range_t));

	mac_range.mac_type = ACL_MAC_SOURCE;
	mac_range.mac_s.val = 0xaa2233445566;
	mac_range.mac_e.val = 0xaabbccddeeff;

	action.force_queue_enable = TRUE;
	action.e_queue = 6;
	SET_BITS(action.port_mask, UTP_PORT0, UTP_PORT7);

	ret = jl_acl_macrange_action_set(chip_id, rule_idx, &mac_range, &action);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL add SMAC filter rule %d error!\n", rule_idx);
		return;
	}

	memset(&action, 0, sizeof(jl_acl_mac_action_t));
	memset(&mac_range, 0, sizeof(jl_acl_mac_range_t));

	ret = jl_acl_macrange_action_get(chip_id, rule_idx, ACL_MAC_SOURCE, &mac_range, &action);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get SMAC filter rule %d error!\n", rule_idx);
		return;
	}

	ret = jl_acl_macrange_action_del(chip_id, rule_idx, ACL_MAC_SOURCE);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL del SMAC filter rule %d error!\n", rule_idx);
		return;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "rsvd l2 source mac rule done!\n");

	rule_idx = 3;
	mac_range.mac_type = ACL_MAC_DEST;

	ret = jl_acl_macrange_action_set(chip_id, rule_idx, &mac_range, &action);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL add DMAC filter rule %d error!\n", rule_idx);
		return;
	}

	memset(&action, 0, sizeof(jl_acl_mac_action_t));
	memset(&mac_range, 0, sizeof(jl_acl_mac_range_t));

	ret = jl_acl_macrange_action_get(chip_id, rule_idx, ACL_MAC_DEST, &mac_range, &action);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL get DMAC filter rule %d error!\n", rule_idx);
		return;
	}

	ret = jl_acl_macrange_action_del(chip_id, rule_idx, ACL_MAC_DEST);
	if (ret)
	{
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ACL del DMAC filter rule %d error!\n", rule_idx);
		return;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "rsvd l2 dest mac rule done!\n");

	return;
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	jl_api_ret_t ret = 0;
	jl_uint32 chip_id = 0;
	char console_input[128] = {0};
	jl_uint32 cmd = 0;

	jl_dev_t dev_6108 = {
		.compat_id = JL_CHIP_6108,
		.name = "device-jl6108",
		.id = chip_id, /* must be less than JL_MAX_CHIP_NUM */
		.io = {
			.type = JL_IO_SMI,
			.smi.mdio.bus_id = 0
		}
	};

	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);

	/* init sdk */
	ret = jl_switch_init();
	if (ret)
		return ret;

	ret = jl_switch_device_create(&dev_6108);
	if (ret)
		return ret;

	while (!g_exit) {
		print_ACL_usage();

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		sscanf(console_input, "%d", &cmd);

		switch (cmd) {
		case 1:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n acl_tcam_table_test start!\n");
			acl_tcam_table_test(chip_id);
			break;
		case 2:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n acl_hash_table_test start!\n");
			acl_hash_table_test(chip_id);
			break;
		case 3:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n acl_mac_filter_test start!\n");
			acl_mac_filter_test(chip_id);
			break;
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n Test commad: error!\n");
			break;
		}
	}

exit:
	ret = jl_switch_device_delete(chip_id);
	if (ret)
		return ret;

	/* deinit sdk */
	ret = jl_switch_deinit();
	if (ret)
		return ret;

	return JL_ERR_OK;
}

