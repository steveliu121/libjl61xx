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

#include "jl6107x/jl6107x_drv_acl.h"
#include "jl6107x/jl6107x_drv_switch.h"
#include "jl6107x/jl6107x_chip.h"

CONST_T jl_uint8 jl6107x_acl_field_bit_size[ACL_FIELD_NUM] = {
	0, 3, 48, 48, 12, 1, 1, 1, 3, 1,
	12, 3, 1, 16, 2, 32, 32, 128, 128, 20,
	8, 8, 8, 3, 16, 16, 128, 8, 8, 8,
	8, 8, 32, 3
};

static jl6107x_acl_template_t *gp_acl_template[JL_MAX_CHIP_NUM] = {0};
static jl6107x_acl_rule_info_t *gp_acl_rule_info[JL_MAX_CHIP_NUM] = {0};

static jl_ret_t jl6107x_acl_templ_info_set(jl_device_t *dev,
										jl_uint8 index,
										jl_acl_template_t *acl_templ);
static jl_uint8 jl6107x_acl_calc_hash_idx(jl6107x_acl_hash_key_t* key, jl_uint8 is_large);
static jl_ret_t jl6107x_acl_add_key_to_tcam(jl6107x_acl_hash_key_t* key , jl_uint8* tcam_tbl);
static jl_ret_t jl6107x_acl_add_key_to_hash(jl6107x_acl_hash_key_t* key, jl_uint8* hash_tbl);
static jl_ret_t jl6107x_acl_add_action_to_tcam(jl_device_t *dev, jl_acl_action_t *acl_action,
											SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_t* tcam_tbl);
static jl_ret_t jl6107x_acl_add_action_to_hash_large(jl_device_t *dev, jl_acl_action_t *acl_action,
											SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t* hash_tbl);
static jl_ret_t jl6107x_acl_add_action_to_hash_small(jl_device_t *dev, jl_acl_action_t *acl_action,
											SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t * hash_tbl);
static jl_ret_t jl6107x_acl_insert_val_to_rule(jl_device_t *dev,
											jl_uint8 field_type,
											jl6107x_acl_val_t* val,
											jl_acl_rule_t *acl);
static jl_ret_t jl6107x_acl_get_rule_from_tcam(jl_device_t *dev,
												SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_t *tcam_tbl,
												jl_acl_rule_t *acl_rule);
static jl_ret_t jl6107x_acl_get_rule_from_hash_large(jl_device_t *dev,
													SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t* hash_tbl,
													jl_acl_rule_t *acl_rule);
static jl_ret_t jl6107x_acl_get_rule_from_hash_small(jl_device_t *dev,
													SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t* hash_tbl,
													jl_acl_rule_t *acl_rule);
static jl_ret_t jl6107x_acl_get_action_from_tcam(jl_device_t *dev,
												SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_t *tcam_tbl,
												jl_acl_action_t *acl_action);
static jl_ret_t jl6107x_acl_get_action_from_hash_large(jl_device_t *dev,
													SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t *hash_tbl,
													jl_acl_action_t *acl_action);
static jl_ret_t jl6107x_acl_get_action_from_hash_small(jl_device_t *dev,
													SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t *hash_tbl,
													jl_acl_action_t *acl_action);
static jl_ret_t jl6107x_acl_rule_info_set(jl_device_t *dev,
										jl_uint8 index,
										jl_acl_rule_t *acl_rule,
										jl_acl_action_t *acl_action);
static jl_ret_t jl6107x_acl_rule_info_get(jl_device_t *dev,
										jl_uint8 index,
										jl_acl_rule_t *acl_rule,
										jl_acl_action_t *acl_action);
static jl_ret_t jl6107x_acl_rule_pre_process(jl_device_t *dev, jl_acl_rule_t *acl_rule);
static jl_ret_t jl6107x_acl_write_tcam(jl_device_t *dev,
										jl_uint8 index,
										jl6107x_acl_hash_key_t *key,
										jl_acl_action_t *acl_action,
										jl_uint8 *idx_out);
static jl_ret_t jl6107x_acl_write_hash_large(jl_device_t	 *dev,
											jl_uint8 index,
											jl6107x_acl_hash_key_t *key,
											jl_acl_action_t *acl_action,
											jl_uint8 *idx_out);
static jl_ret_t jl6107x_acl_write_hash_small(jl_device_t *dev,
											jl_uint8 index,
											jl6107x_acl_hash_key_t *key,
											jl_acl_action_t *acl_action,
											jl_uint8* idx_out);
static jl_ret_t jl6107x_acl_read_tcam(jl_device_t *dev,
									jl_uint8 index,
									jl_acl_rule_t *rule_out,
									jl_acl_action_t *act_out);

static jl_ret_t jl6107x_acl_read_hash_large(jl_device_t *dev,
											jl_uint8 index,
											jl_acl_rule_t *rule_out,
											jl_acl_action_t *act_out);
static jl_ret_t jl6107x_acl_read_hash_mall(jl_device_t *dev,
											jl_uint8 index,
											jl_acl_rule_t *rule_out,
											jl_acl_action_t *act_out);
static jl_ret_t jl6107x_acl_clear_table(jl_device_t *dev, jl_uint8 index);
static jl_ret_t jl6107x_acl_clear_tcam(jl_device_t *dev, jl_uint8 index);
static jl_ret_t jl6107x_acl_clear_hash_large(jl_device_t *dev, jl_uint8 index);
static jl_ret_t jl6107x_acl_clear_hash_small(jl_device_t *dev, jl_uint8 index);
static jl_ret_t jl6107x_acl_l2v(void *val_in, jl_uint8 len, jl6107x_acl_val_t* val_out);
static jl_ret_t jl6107x_acl_v2l(jl6107x_acl_val_t* val_in, jl_uint8 len, void *val_out);
static jl_ret_t jl6107x_acl_get_mask_val(jl_device_t* dev,
										jl_acl_field_data_t* field_rule,
										jl6107x_acl_val_t* val_out);
static jl_ret_t jl6107x_acl_rule_get_key(jl_device_t *dev,
										jl_acl_rule_t *acl_rule,
										jl6107x_acl_hash_key_t *key);
static jl_ret_t jl6107x_acl_field_chk(jl_device_t *dev,
									jl_acl_rule_t *acl_rule,
									jl_acl_field_data_t *acl_field);
static jl_ret_t jl6107x_acl_templ_field_get(jl_device_t *dev,
											jl_uint8 index,
											jl_acl_field_t *acl_filed);
static jl_ret_t jl6107x_acl_stat_enable_tcam(jl_device_t *dev,
											jl_uint8 index,
											jl_uint8 counter,
											jl_uint8 enable);
static jl_ret_t jl6107x_acl_stat_enable_hash_large(jl_device_t *dev,
													jl_uint8 index,
													jl_uint8 counter,
													jl_uint8 enable);
static jl_ret_t jl6107x_acl_stat_enable_hash_small(jl_device_t *dev,
													jl_uint8 index,
													jl_uint8 counter,
													jl_uint8 enable);


static jl_ret_t jl6107x_acl_templ_info_set(jl_device_t *dev,
										jl_uint8 index,
										jl_acl_template_t *acl_templ)
{
	jl_uint8 field_idx = 0;
	jl_uint8 field_type = 0;
	jl_uint8 with_pointer = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_templ);

	gp_acl_template[dev->id]->templ_mask[index] = 0;

	for (; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		gp_acl_template[dev->id]->acl_templ[index][field_idx] = ACL_FIELD_NO_FIELD;

		field_type = acl_templ->field[field_idx].field_type;
		gp_acl_template[dev->id]->acl_templ[index][field_idx] = field_type;

		if (field_type != ACL_FIELD_NO_FIELD)
		{
			gp_acl_template[dev->id]->templ_mask[index] |= BIT(field_idx);
		}
		if (field_type == ACL_FIELD_RULE_POINTER)
			with_pointer = 1;
	}

	if (with_pointer)
		SET_BIT(gp_acl_template[dev->id]->rule_pointer_mask, index);
	else
		CLR_BIT(gp_acl_template[dev->id]->rule_pointer_mask, index);

	return JL_ERR_OK;
}


jl_ret_t jl6107x_acl_template_set(jl_device_t *dev,
							jl_uint8 index,
							jl_acl_template_t *acl_templ)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 idx = 0;
	jl_uint8 field_idx = 0;
	jl_uint8 field_type = 0;
	jl_uint8 field_size = 0;
	jl_uint8 size_all = 0;

	jl_acl_field_t acl_field;
	jl_acl_template_t acl_template;
	SWCORE_INGRESS_CONFIGURABLE_ACL_RULES_SETUP_t field_rule;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_templ);

	memset((void*)&acl_template,0,sizeof(jl_acl_template_t));
	memset((void*)&field_rule,0,sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_RULES_SETUP_t));

	for (field_idx = 0; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		field_type = acl_templ->field[field_idx].field_type;

		if (field_type >= ACL_FIELD_NUM)
			return JL_ERR_PARAM;

		field_size = jl6107x_acl_field_bit_size[field_type];

		acl_templ->field[field_idx].field_size = field_size;
		size_all += field_size;

		if (field_type != ACL_FIELD_NO_FIELD)
		{
			acl_template.field[idx] = acl_templ->field[field_idx];

			if (field_type == ACL_FIELD_RULE_POINTER && idx != 0)   //RULE_POINTER must be the first
			{
				acl_field = acl_template.field[0];
				acl_template.field[0] = acl_template.field[idx];
				acl_template.field[idx] = acl_field;
			}
			idx++;
		}
	}

	if (size_all > 202)
		return JL_ERR_CONFIG;

	for (field_idx = 0; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		field_type = acl_template.field[field_idx].field_type;
		field_size = acl_template.field[field_idx].field_size;

		field_rule.BF.field_select = field_type &
			BITS_WIDTH(SWCORE_INGRESS_CONFIGURABLE_ACL_RULES_SETUP_OFFSET_FIELD_SELECT_WIDTH);
		field_rule.BF.bits_selected = field_size &
			BITS_WIDTH(SWCORE_INGRESS_CONFIGURABLE_ACL_RULES_SETUP_OFFSET_BITS_SELECTED_WIDTH);

		idx = (index * ACL_RULE_FIELD_NUM + field_idx);

		REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_RULES_SETUP, field_rule, idx, 0);
	}

	ret = jl6107x_acl_templ_info_set(dev, index, &acl_template);

	return ret;
}

static jl_ret_t jl6107x_acl_templ_field_get(jl_device_t *dev,
						jl_uint8 index,
						jl_acl_field_t *acl_filed)

{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_filed);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_RULES_SETUP, field_rule, index, 0);

	acl_filed->field_type = field_rule.BF.field_select;
	acl_filed->field_size = field_rule.BF.bits_selected;

	return ret;
}

jl_ret_t jl6107x_acl_template_get(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_template_t *acl_templ)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 idx = 0;
	jl_uint8 field_idx = 0;
	jl_acl_field_t acl_filed;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_templ);

	for (idx = 0; idx < ACL_RULE_FIELD_NUM; idx++)
	{
		field_idx = (index * ACL_RULE_FIELD_NUM + idx);

		ret = jl6107x_acl_templ_field_get(dev, field_idx, &acl_filed);
		if (ret)
			return ret;

		acl_templ->field[idx] = acl_filed;
	}

	return JL_ERR_OK;
}

static jl_uint8 jl6107x_acl_calc_hash_idx(jl6107x_acl_hash_key_t* key, jl_uint8 is_large)
{
	jl_uint8 i,j;
	jl_uint8 hash_val = 0;
	jl_uint32 value = 0;

	JL_CHECK_POINTER(key);

	if (is_large)
	{
		//calc 4bit hash
		hash_val = key->value[0] & 0xf;
		hash_val ^= (key->value[0] >> 4);
		hash_val &= 0xf;

		for (i = 1; i < 26; i++)
		{
			hash_val ^= key->value[i];
			hash_val &= 0xf;
			hash_val ^= (key->value[i] >> 4);
			hash_val &= 0xf;
		}
	}
	else
	{
		//calc 3bit hash
		value = *(jl_uint32*)&key->value[0];
		value &= 0xffffff;
		hash_val = value & 0x7;

		for (i = 1; i < 8; i++)
		{
			value >>= 3;
			hash_val ^= value;
			hash_val &= 0x7;
		}

		for (i = 1; i < 8; i++)
		{
			value = *(jl_uint32*)&key->value[3*i];
			value &= 0xffffff;

			for (j = 0; j < 8; j++)
			{
				hash_val ^= value;
				hash_val &= 0x7;
				value >>= 3;
			}
		}

		value = *(jl_uint16*)&key->value[24];
		value &= 0xffff;

		for (j = 0; j < 6; j++)
		{
			hash_val ^= value;
			hash_val &= 0x7;
			value >>= 3;
		}
	}

	return hash_val;
}

static jl_ret_t jl6107x_acl_add_key_to_tcam(jl6107x_acl_hash_key_t* key , jl_uint8* tcam_tbl)
{
	jl_uint8 idx = 0;

	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(tcam_tbl);

	for (; idx < 26; idx++)
	{
		key->value[idx] &= key->mask[idx];

		jl_write_bits(tcam_tbl, key->mask[idx], (8 * idx) + 1, 8);
		jl_write_bits(tcam_tbl + 26, key->value[idx], (8 * idx) + 1, 8);  //begin from 209bit
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_add_key_to_hash(jl6107x_acl_hash_key_t* key, jl_uint8* hash_tbl)
{
	jl_uint32 idx = 0;

	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(hash_tbl);

	for (; idx < 26; idx++)
	{
		jl_write_bits(hash_tbl, key->value[idx], (8 * idx) + 1, 8);
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_add_action_to_tcam(jl_device_t *dev,
										jl_acl_action_t *acl_action,
										SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_t* tcam_tbl)
{
	jl_port_t cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(tcam_tbl);

	tcam_tbl->BF.drop_enable = acl_action->drop_enable;
	tcam_tbl->BF.send_to_cpu = acl_action->send2cpu_enable;

	tcam_tbl->BF.send_to_port = acl_action->send2port_enable;
	if (tcam_tbl->BF.send_to_port)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_port);
		tcam_tbl->BF.dest_port = cport;
	}

	tcam_tbl->BF.force_vid_valid = acl_action->force_vid_enable;
	if (tcam_tbl->BF.force_vid_valid)
		tcam_tbl->BF.force_vid = acl_action->force_vid;

	tcam_tbl->BF.force_queue = acl_action->force_queue_enable;
	if (tcam_tbl->BF.force_queue)
		tcam_tbl->BF.e_queue = acl_action->e_queue;

	tcam_tbl->BF.force_color = acl_action->force_color_enable;
	if (tcam_tbl->BF.force_color)
		tcam_tbl->BF.color = acl_action->color;

	tcam_tbl->BF.mmp_valid = acl_action->mmp_valid;
	if (tcam_tbl->BF.mmp_valid)
	{
		tcam_tbl->BF.mmp_ptr = acl_action->mmp_ptr;
		tcam_tbl->BF.mmp_order_0 = acl_action->mmp_order;
		tcam_tbl->BF.mmp_order_1 = acl_action->mmp_order >> 
					SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_OFFSET_MMP_ORDER_0_WIDTH;
	}

	tcam_tbl->BF.update_counter = acl_action->update_counter_enable;
	if (tcam_tbl->BF.update_counter)
		tcam_tbl->BF.counter = acl_action->counter;

	tcam_tbl->BF.no_learning = acl_action->no_learning_enable;

	tcam_tbl->BF.update_cfi_dei = acl_action->update_cfi_dei_enable;
	if (tcam_tbl->BF.update_cfi_dei)
		tcam_tbl->BF.new_cfi_dei_value = acl_action->new_cfi_dei;

	tcam_tbl->BF.update_pcp = acl_action->update_pcp_enable;
	if (tcam_tbl->BF.update_pcp)
		tcam_tbl->BF.new_pcp_value = acl_action->new_pcp;

	tcam_tbl->BF.update_vid = acl_action->update_vid_enable;
	if (tcam_tbl->BF.update_vid)
		tcam_tbl->BF.new_vid_value = acl_action->new_vid;

	tcam_tbl->BF.update_etype = acl_action->update_etype_enable;
	if (tcam_tbl->BF.update_etype)
		tcam_tbl->BF.new_eth_type = acl_action->new_etype;

	tcam_tbl->BF.input_mirror = acl_action->input_mirror_enable;
	if (tcam_tbl->BF.input_mirror)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_input_mirror);
		tcam_tbl->BF.dest_input_mirror_0 = cport;
		tcam_tbl->BF.dest_input_mirror_1_2 = cport >> 
				SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_OFFSET_DEST_INPUT_MIRROR_0_WIDTH;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_add_action_to_hash_large(jl_device_t *dev,
											jl_acl_action_t *acl_action,
											SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t* hash_tbl)
{
	jl_port_t cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(hash_tbl);

	hash_tbl->BF.drop_enable = acl_action->drop_enable;
	hash_tbl->BF.send_to_cpu = acl_action->send2cpu_enable;

	hash_tbl->BF.send_to_port = acl_action->send2port_enable;
	if (hash_tbl->BF.send_to_port)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_port);
		hash_tbl->BF.dest_port = cport;
	}

	hash_tbl->BF.force_vid_valid = acl_action->force_vid_enable;
	if (hash_tbl->BF.force_vid_valid)
	{
		hash_tbl->BF.force_vid_0_7 = acl_action->force_vid;
		hash_tbl->BF.force_vid_8_11 = (acl_action->force_vid >> SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_OFFSET_FORCE_VID_0_7_WIDTH);
	}

	hash_tbl->BF.force_queue = acl_action->force_queue_enable;
	if (hash_tbl->BF.force_queue)
		hash_tbl->BF.e_queue = acl_action->e_queue;

	hash_tbl->BF.force_color = acl_action->force_color_enable;
	if (hash_tbl->BF.force_color)
		hash_tbl->BF.color = acl_action->color;

	hash_tbl->BF.mmp_valid = acl_action->mmp_valid;
	if (hash_tbl->BF.mmp_valid)
	{
		hash_tbl->BF.mmp_ptr = acl_action->mmp_ptr;
		hash_tbl->BF.mmp_order = acl_action->mmp_order;
	}

	hash_tbl->BF.update_counter = acl_action->update_counter_enable;
	if (hash_tbl->BF.update_counter)
		hash_tbl->BF.counter = acl_action->counter;

	hash_tbl->BF.no_learning = acl_action->no_learning_enable;

	hash_tbl->BF.update_cfi_dei = acl_action->update_cfi_dei_enable;
	if (hash_tbl->BF.update_cfi_dei)
		hash_tbl->BF.new_cfi_dei_value = acl_action->new_cfi_dei;

	hash_tbl->BF.update_pcp = acl_action->update_pcp_enable;
	if (hash_tbl->BF.update_pcp)
		hash_tbl->BF.new_pcp_value = acl_action->new_pcp;

	hash_tbl->BF.update_vid = acl_action->update_vid_enable;
	if (hash_tbl->BF.update_vid)
		hash_tbl->BF.new_vid_value = acl_action->new_vid;

	hash_tbl->BF.update_etype = acl_action->update_etype_enable;
	if (hash_tbl->BF.update_etype)
		hash_tbl->BF.new_eth_type = acl_action->new_etype;

	hash_tbl->BF.update_etype = acl_action->update_etype_enable;
	if (hash_tbl->BF.update_etype)
		hash_tbl->BF.new_eth_type = acl_action->new_etype;

	hash_tbl->BF.input_mirror = acl_action->input_mirror_enable;
	if (hash_tbl->BF.input_mirror)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_input_mirror);
		hash_tbl->BF.dest_input_mirror = cport;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_add_action_to_hash_small(jl_device_t *dev,
												jl_acl_action_t *acl_action,
												SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t* hash_tbl)
{
	jl_port_t cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(hash_tbl);

	hash_tbl->BF.drop_enable = acl_action->drop_enable;
	hash_tbl->BF.send_to_cpu = acl_action->send2cpu_enable;

	hash_tbl->BF.send_to_port = acl_action->send2port_enable;
	if (hash_tbl->BF.send_to_port)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_port);
		hash_tbl->BF.dest_port = cport;
	}


	hash_tbl->BF.force_vid_valid = acl_action->force_vid_enable;
	if (hash_tbl->BF.force_vid_valid)
	{
		hash_tbl->BF.force_vid_0_7 = acl_action->force_vid;
		hash_tbl->BF.force_vid_8_11 = (acl_action->force_vid >> SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_OFFSET_FORCE_VID_0_7_WIDTH);
	}

	hash_tbl->BF.force_queue = acl_action->force_queue_enable;
	if (hash_tbl->BF.force_queue)
		hash_tbl->BF.e_queue = acl_action->e_queue;

	hash_tbl->BF.force_color = acl_action->force_color_enable;
	if (hash_tbl->BF.force_color)
		hash_tbl->BF.color = acl_action->color;

	hash_tbl->BF.mmp_valid = acl_action->mmp_valid;
	if (hash_tbl->BF.mmp_valid)
	{
		hash_tbl->BF.mmp_ptr = acl_action->mmp_ptr;
		hash_tbl->BF.mmp_order = acl_action->mmp_order;
	}

	hash_tbl->BF.update_counter = acl_action->update_counter_enable;
	if (hash_tbl->BF.update_counter)
		hash_tbl->BF.counter = acl_action->counter;

	hash_tbl->BF.no_learning = acl_action->no_learning_enable;

	hash_tbl->BF.update_cfi_dei = acl_action->update_cfi_dei_enable;
	if (hash_tbl->BF.update_cfi_dei)
		hash_tbl->BF.new_cfi_dei_value = acl_action->new_cfi_dei;

	hash_tbl->BF.update_pcp = acl_action->update_pcp_enable;
	if (hash_tbl->BF.update_pcp)
		hash_tbl->BF.new_pcp_value = acl_action->new_pcp;

	hash_tbl->BF.update_vid = acl_action->update_vid_enable;
	if (hash_tbl->BF.new_vid_value)
		hash_tbl->BF.new_vid_value = acl_action->new_vid;

	hash_tbl->BF.update_etype = acl_action->update_etype_enable;
	if (hash_tbl->BF.update_etype)
		hash_tbl->BF.new_eth_type = acl_action->new_etype;

	hash_tbl->BF.input_mirror = acl_action->input_mirror_enable;
	if (hash_tbl->BF.input_mirror)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_input_mirror);
		hash_tbl->BF.dest_input_mirror = cport;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_insert_val_to_rule(jl_device_t *dev, jl_uint8 field_type,
												jl6107x_acl_val_t* val, jl_acl_rule_t *acl)
{
	jl_ret_t ret;
	jl_port_t lport;

	jl_acl_field_data_t* field_rule = NULL;
	jl_acl_field_data_t* field_tmp = NULL;
	jl_acl_field_data_t* field_nxt = NULL;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(val);
	JL_CHECK_POINTER(acl);

	field_rule = (jl_acl_field_data_t*)port_mem_malloc(sizeof(jl_acl_field_data_t));

	if(!field_rule)
	{
		ret = JL_ERR_MEMORY;
		goto ERR;
	}

	memset(field_rule, 0, sizeof(jl_acl_field_data_t));

	field_rule->field_type = field_type;
	field_rule->templ_idx = acl->templ_idx;

	switch (field_type)
	{
		case ACL_FIELD_SOURCE_PORT:
			lport = jlxx_port_c2l(dev, val->val_0_63);
			field_rule->field.source_port.value = lport;
			field_rule->field.source_port.mask = val->mask_0_63;
			break;
		case ACL_FIELD_MAC_DA:
			ret = jl6107x_acl_v2l(val, 48,(void*)&field_rule->field.d_mac);
			if (ret)
				goto ERR;
			break;
		case ACL_FIELD_MAC_SA:
			ret = jl6107x_acl_v2l(val, 48,(void*)&field_rule->field.s_mac);
			if (ret)
				goto ERR;
			break;
		case ACL_FIELD_OUTER_VID:
			field_rule->field.outer_vid.value = val->val_0_63;
			field_rule->field.outer_vid.mask = val->mask_0_63;
			break;
		case ACL_FIELD_HAS_VLAN:
			field_rule->field.has_vlans.value = val->val_0_63;
			break;
		case ACL_FIELD_OUTER_ETH_TYPE:
			field_rule->field.outer_cstag.value = val->val_0_63;
			break;
		case ACL_FIELD_INNER_ETH_TYPE:
			field_rule->field.inner_cstag.value = val->val_0_63;
			break;
		case ACL_FIELD_OUTER_PCP:
			field_rule->field.outer_pcp.value = val->val_0_63;
			field_rule->field.outer_pcp.mask = val->mask_0_63;
			break;
		case ACL_FIELD_OUTER_DEI:
			field_rule->field.outer_dei.value = val->val_0_63;
			field_rule->field.outer_dei.mask = val->mask_0_63;
			break;
		case ACL_FIELD_INNER_VID:
			field_rule->field.inner_vid.value = val->val_0_63;
			field_rule->field.inner_vid.mask = val->mask_0_63;
			break;
		case ACL_FIELD_INNER_PCP:
			field_rule->field.inner_pcp.value = val->val_0_63;
			field_rule->field.inner_pcp.mask = val->mask_0_63;
			break;
		case ACL_FIELD_INNER_DEI:
			field_rule->field.inner_dei.value = val->val_0_63;
			field_rule->field.inner_dei.mask = val->mask_0_63;
			break;
		case ACL_FIELD_ETH_TYPE:
			field_rule->field.eth_type.value = val->val_0_63;
			field_rule->field.eth_type.mask = val->mask_0_63;
			break;
		case ACL_FIELD_L3_TYPE:
			field_rule->field.l3_type.value = val->val_0_63;
			break;
		case ACL_FIELD_IP_SA:
			ret = jl6107x_acl_v2l(val, 32,(void*)&field_rule->field.s_ip);
			if (ret)
				goto ERR;
			break;
		case ACL_FIELD_IP_DA:
			ret = jl6107x_acl_v2l(val, 32,(void*)&field_rule->field.d_ip);
			if (ret)
				goto ERR;
			break;
		case ACL_FIELD_IP6_SA:
			ret = jl6107x_acl_v2l(val, 128,(void*)&field_rule->field.s_ip6);
			if (ret)
				goto ERR;
			break;
		case ACL_FIELD_IP6_DA:
			ret = jl6107x_acl_v2l(val, 128,(void*)&field_rule->field.d_ip6);
			if (ret)
				goto ERR;
			break;
		case ACL_FIELD_MPLS:
			field_rule->field.mpls.value = val->val_0_63;
			field_rule->field.mpls.mask = val->mask_0_63;
			break;
		case ACL_FIELD_TOS:
			field_rule->field.tos.value = val->val_0_63;
			field_rule->field.tos.mask = val->mask_0_63;
			break;
		case ACL_FIELD_TTL:
			field_rule->field.ttl.value = val->val_0_63;
			field_rule->field.ttl.mask = val->mask_0_63;
			break;
		case ACL_FIELD_L4_PROTOCOL:
			field_rule->field.l4_pro.value = val->val_0_63;
			field_rule->field.l4_pro.mask = val->mask_0_63;
			break;
		case ACL_FIELD_L4_TYPE:
			field_rule->field.l4_type.value = val->val_0_63;
			break;
		case ACL_FIELD_L4_SOURCE_PORT:
			field_rule->field.l4_s_port.value = val->val_0_63;
			field_rule->field.l4_s_port.mask = val->mask_0_63;
			break;
		case ACL_FIELD_L4_DEST_PORT:
			field_rule->field.l4_d_port.value = val->val_0_63;
			field_rule->field.l4_d_port.mask = val->mask_0_63;
			break;
		case ACL_FIELD_MLD_ADDR:
			ret = jl6107x_acl_v2l(val, 128,(void*)&field_rule->field.mld_addr);
			if (ret)
				goto ERR;
			break;
		case ACL_FIELD_ICMP_TYPE:
			field_rule->field.icmp_type.value = val->val_0_63;
			field_rule->field.icmp_type.mask = val->mask_0_63;
			break;
		case ACL_FIELD_ICMP_CODE:
			field_rule->field.icmp_code.value = val->val_0_63;
			field_rule->field.icmp_code.mask = val->mask_0_63;
			break;
		case ACL_FIELD_ICMP6_TYPE:
			field_rule->field.icmp6_type.value = val->val_0_63;
			field_rule->field.icmp6_type.mask = val->mask_0_63;
			break;
		case ACL_FIELD_ICMP6_CODE:
			field_rule->field.icmp6_code.value = val->val_0_63;
			field_rule->field.icmp6_code.mask = val->mask_0_63;
			break;
		case ACL_FIELD_IGMP_TYPE:
			field_rule->field.igmp_type.value = val->val_0_63;
			field_rule->field.igmp_type.mask = val->mask_0_63;
			break;
		case ACL_FIELD_IGMP_GROUP_ADDR:
			ret = jl6107x_acl_v2l(val, 32,(void*)&field_rule->field.igmp_group);
			if (ret)
				goto ERR;
			break;
		case ACL_FIELD_RULE_POINTER:
			field_rule->field.rule_pointer.value = val->val_0_63;
			acl->with_pointer = 1;
			break;
		default:
			ret = JL_ERR_PARAM;
			goto ERR;
	}

	if (acl->head == NULL)
		acl->head = field_rule;
	else
	{
		field_tmp = acl->head;
		while(field_tmp->next)
		{
			field_tmp = field_tmp->next;
		}
		field_tmp->next = field_rule;
	}
	return JL_ERR_OK;

ERR:
	field_tmp = acl->head;

	while (field_tmp)
	{
		field_nxt = field_tmp->next;
		port_mem_free(field_tmp);
		field_tmp = field_nxt;
	}
	acl->head = NULL;

	if (field_rule)
		port_mem_free(field_rule);

	return ret;
}


jl_ret_t jl6107x_acl_field_insert(jl_device_t *dev,
								jl_acl_field_data_t *acl_field,
								jl_acl_rule_t *acl_rule)
{
	jl_acl_field_data_t* field_buf = NULL;
	jl_acl_field_data_t* field_pre = NULL;
	jl_acl_field_data_t* field_tmp = NULL;
	jl_acl_field_data_t* field_nxt = NULL;
	jl_api_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_field);

	ret = jl6107x_acl_field_chk(dev, acl_rule, acl_field);
	if (ret)
		return ret;

	field_buf = (jl_acl_field_data_t*)port_mem_malloc(sizeof(jl_acl_field_data_t));

	if (!field_buf)
		return JL_ERR_MEMORY;

	*field_buf = *acl_field;

	if (acl_rule->head == NULL)
		acl_rule->head = field_buf;
	else
	{
		field_tmp = acl_rule->head;
		while (field_tmp != NULL)
		{
			if (field_tmp->field_type == acl_field->field_type)
			{
				field_nxt = field_tmp->next;
				port_mem_free(field_tmp);
				field_tmp = NULL;
				acl_rule->field_num--;
				break;
			}
			field_pre = field_tmp;
			field_tmp = field_tmp->next;
		}

		if (acl_rule->field_num == ACL_RULE_FIELD_NUM)
		{
			if (field_buf)
				port_mem_free(field_buf);
			return JL_ERR_FULL;
		}

		if (field_pre == NULL)
			acl_rule->head = field_buf;
		else
		{
			field_buf->next = field_nxt;
			field_pre->next = field_buf;
		}
	}

	acl_rule->field_mask |= BIT(acl_field->field_idx);
	acl_rule->field_num++;

	if (acl_field->field_type == ACL_FIELD_RULE_POINTER)
		acl_rule->with_pointer = 1;

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_field_chk(jl_device_t *dev, jl_acl_rule_t *acl_rule, jl_acl_field_data_t *acl_field)
{
	jl_uint8 field_idx = 0;
	jl_uint8 field_type = 0;
	jl_uint8 templ_type = 0;
	jl_uint8 bit_width = 0;
	jl_uint32 mask_val = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_field);

	field_type = acl_field->field_type;
	templ_type = acl_rule->templ_idx;

	if (field_type > ACL_FIELD_RULE_POINTER || field_type == ACL_FIELD_NO_FIELD)
		return JL_ERR_PARAM;

	for (field_idx = 0; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		if (field_type == gp_acl_template[dev->id]->acl_templ[templ_type][field_idx])
		{
			acl_field->templ_idx = templ_type;
			acl_field->field_idx = field_idx;

			bit_width = jl6107x_acl_field_bit_size[field_type];

			if (bit_width <= 32)
				mask_val = BITS_WIDTH(bit_width);

			switch (field_type)
			{
				case ACL_FIELD_SOURCE_PORT:
					JL_CHECK_PORT(dev, acl_field->field.source_port.value);
					break;
				case ACL_FIELD_MAC_DA:
					if (acl_field->field.d_mac.value.val > 0xffffffffffff)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_MAC_SA:
					if (acl_field->field.s_mac.value.val > 0xffffffffffff)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_OUTER_VID:
					if (acl_field->field.outer_vid.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_HAS_VLAN:
					if (acl_field->field.has_vlans.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_OUTER_ETH_TYPE:
					if (acl_field->field.outer_cstag.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_INNER_ETH_TYPE:
					if (acl_field->field.inner_cstag.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_OUTER_PCP:
					if (acl_field->field.outer_pcp.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_OUTER_DEI:
					if (acl_field->field.outer_dei.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_INNER_VID:
					if (acl_field->field.inner_vid.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_INNER_PCP:
					if (acl_field->field.inner_pcp.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_INNER_DEI:
					if (acl_field->field.inner_dei.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_ETH_TYPE:
					if (acl_field->field.eth_type.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_L3_TYPE:
					if (acl_field->field.l3_type.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_IP_SA:
				case ACL_FIELD_IP_DA:
				case ACL_FIELD_IP6_SA:
				case ACL_FIELD_IP6_DA:
					break;
				case ACL_FIELD_MPLS:
					if (acl_field->field.mpls.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_TOS:
					if (acl_field->field.tos.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_TTL:
					if (acl_field->field.ttl.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_L4_PROTOCOL:
					if (acl_field->field.l4_pro.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_L4_TYPE:
					if (acl_field->field.l4_type.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_L4_SOURCE_PORT:
					if (acl_field->field.l4_s_port.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_L4_DEST_PORT:
					if (acl_field->field.l4_d_port.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_MLD_ADDR:
					break;
				case ACL_FIELD_ICMP_TYPE:
					if (acl_field->field.icmp_type.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_ICMP_CODE:
					if (acl_field->field.icmp_code.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_ICMP6_TYPE:
					if (acl_field->field.icmp6_type.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_ICMP6_CODE:
					if (acl_field->field.icmp6_code.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_IGMP_TYPE:
					if (acl_field->field.igmp_type.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_IGMP_GROUP_ADDR:
				case ACL_FIELD_RULE_POINTER:
					break;
				default:
						return JL_ERR_PARAM;
			}
			return JL_ERR_OK;
		}
	}

	return JL_ERR_CONFIG;
}

jl_ret_t jl6107x_acl_field_clear(jl_device_t *dev, jl_acl_rule_t *acl_rule)
{
	jl_acl_field_data_t* field_tmp = NULL;
	jl_acl_field_data_t* field_nxt = NULL;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);

	field_tmp = acl_rule->head;

	while (field_tmp != NULL)
	{
		field_nxt = field_tmp->next;
		port_mem_free(field_tmp);
		field_tmp = field_nxt;
	}

	memset(acl_rule, 0, sizeof(jl_acl_rule_t));
	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_get_rule_from_tcam(jl_device_t *dev,
											SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_t *tcam_tbl,
											jl_acl_rule_t *acl_rule)
{
	jl_uint8 idx = 0;
	jl_uint8 templ_type = 0;
	jl_uint8 field_msk = 0;
	jl_uint8 field_type = 0;
	jl_uint8 field_size = 0;
	jl_uint8 field_offset = 7;

	jl_uint64 val = 0;
	jl_uint64 val_h = 0;

	jl6107x_acl_val_t acl_val;
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(tcam_tbl);
	JL_CHECK_POINTER(acl_rule);

	memset(&acl_val, 0 ,sizeof(jl6107x_acl_val_t));

	templ_type = acl_rule->templ_idx;
	field_msk = acl_rule->field_mask;

	for (; idx < ACL_RULE_FIELD_NUM; idx++)
	{
		field_type = gp_acl_template[dev->id]->acl_templ[templ_type][idx];
		field_size = jl6107x_acl_field_bit_size[field_type];

		if (GET_BIT(field_msk,idx) == 1)
		{
			if (field_type == ACL_FIELD_IP6_SA || field_type == ACL_FIELD_IP6_DA || field_type == ACL_FIELD_MLD_ADDR)
			{
				jl_read_bits((jl_uint8*)tcam_tbl->val, &val, field_offset, 64);
				jl_read_bits((jl_uint8*)tcam_tbl->val, &val_h, field_offset + 64, 64);

				acl_val.mask_0_63 = val;
				acl_val.mask_64_127 = val_h;

				jl_read_bits((jl_uint8*)tcam_tbl->val, &val, field_offset + 208, 64);
				jl_read_bits(((jl_uint8*)tcam_tbl->val) + 26, &val_h, field_offset + 64, 64);

				acl_val.val_0_63 = val;
				acl_val.val_64_127 = val_h;
			}
			else
			{
				jl_read_bits((jl_uint8*)tcam_tbl->val, &val, field_offset, field_size);
				acl_val.mask_0_63 = val;
				jl_read_bits(((jl_uint8*)tcam_tbl->val) + 26, &val, field_offset, field_size);
				acl_val.val_0_63 = val;
			}

			ret = jl6107x_acl_insert_val_to_rule(dev, field_type, &acl_val, acl_rule);
			if (ret)
				return ret;

		}
		field_offset += field_size;
	}

	return JL_ERR_OK;
}


static jl_ret_t jl6107x_acl_get_rule_from_hash_large(jl_device_t *dev,
													SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t* hash_tbl,
													jl_acl_rule_t *acl_rule)
{
	jl_uint8 idx = 0;
	jl_uint8 templ_type = 0;
	jl_uint8 field_msk = 0;
	jl_uint8 field_type = 0;
	jl_uint8 field_size = 0;
	jl_uint8 field_offset = 7;

	jl_uint64 val = 0;
	jl_uint64 val_h = 0;

	jl6107x_acl_val_t acl_val;
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(hash_tbl);
	JL_CHECK_POINTER(acl_rule);

	memset(&acl_val, 0 ,sizeof(jl6107x_acl_val_t));

	templ_type = acl_rule->templ_idx;
	field_msk = acl_rule->field_mask;

	for (; idx < ACL_RULE_FIELD_NUM; idx++)
	{
		field_type = gp_acl_template[dev->id]->acl_templ[templ_type][idx];
		field_size = jl6107x_acl_field_bit_size[field_type];
		if (GET_BIT(field_msk,idx) == 1)
		{
			if (field_type == ACL_FIELD_IP6_SA || field_type == ACL_FIELD_IP6_DA || field_type == ACL_FIELD_MLD_ADDR)
			{
				acl_val.mask_0_63 = 0xffffffffffffffff;
				acl_val.mask_64_127 = 0xffffffffffffffff;

				jl_read_bits((jl_uint8*)hash_tbl->val, &val, field_offset, 64);
				jl_read_bits((jl_uint8*)hash_tbl->val, &val_h, field_offset + 64, 64);

				acl_val.val_0_63 = val;
				acl_val.val_64_127 = val_h;
			}
			else
			{
				if (field_type == ACL_FIELD_MAC_DA || field_type == ACL_FIELD_MAC_SA)
					acl_val.mask_0_63 = 0xffffffffffff;
				else
					acl_val.mask_0_63 = BITS_WIDTH(field_size);

				jl_read_bits((jl_uint8*)hash_tbl->val, &val, field_offset, field_size);
				acl_val.val_0_63 = val;
			}

			ret = jl6107x_acl_insert_val_to_rule(dev, field_type, &acl_val, acl_rule);
			if (ret)
				return ret;

		}
		field_offset += field_size;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_get_rule_from_hash_small(jl_device_t *dev,
													SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t* hash_tbl,
													jl_acl_rule_t *acl_rule)
{
	jl_uint8 idx = 0;
	jl_uint8 templ_type = 0;
	jl_uint8 field_msk = 0;
	jl_uint8 field_type = 0;
	jl_uint8 field_size = 0;
	jl_uint8 field_offset = 7;
	jl_uint64 val = 0;
	jl_uint64 val_h = 0;
	jl6107x_acl_val_t acl_val;
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(hash_tbl);
	JL_CHECK_POINTER(acl_rule);

	memset(&acl_val, 0 ,sizeof(jl6107x_acl_val_t));

	templ_type = acl_rule->templ_idx;
	field_msk = acl_rule->field_mask;

	for (; idx < ACL_RULE_FIELD_NUM; idx++)
	{
		field_type = gp_acl_template[dev->id]->acl_templ[templ_type][idx];
		field_size = jl6107x_acl_field_bit_size[field_type];
		if (GET_BIT(field_msk,idx) == 1)
		{
			if (field_type == ACL_FIELD_IP6_SA || field_type == ACL_FIELD_IP6_DA || field_type == ACL_FIELD_MLD_ADDR)
			{
				acl_val.mask_0_63 = 0xffffffffffffffff;
				acl_val.mask_64_127 = 0xffffffffffffffff;

				jl_read_bits((jl_uint8*)hash_tbl->val, &val, field_offset, 64);
				jl_read_bits((jl_uint8*)hash_tbl->val, &val_h, field_offset + 64, 64);

				acl_val.val_0_63 = val;
				acl_val.val_64_127 = val_h;
			}
			else
			{
				if (field_type == ACL_FIELD_MAC_DA || field_type == ACL_FIELD_MAC_SA)
					acl_val.mask_0_63 = 0xffffffffffff;
				else
					acl_val.mask_0_63 = BITS_WIDTH(field_size);

				jl_read_bits((jl_uint8*)hash_tbl->val, &val, field_offset, field_size);
				acl_val.val_0_63 = val;
			}

			ret = jl6107x_acl_insert_val_to_rule(dev, field_type, &acl_val, acl_rule);
			if (ret)
				return ret;

		}
		field_offset += field_size;
	}

	return JL_ERR_OK;
}


static jl_ret_t jl6107x_acl_get_action_from_tcam(jl_device_t *dev,
											SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_t *tcam_tbl,
											jl_acl_action_t *acl_action)
{
	jl_port_t lport;
	jl_port_t cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(tcam_tbl);

	acl_action->drop_enable = tcam_tbl->BF.drop_enable;
	acl_action->send2cpu_enable = tcam_tbl->BF.send_to_cpu;
	acl_action->send2port_enable = tcam_tbl->BF.send_to_port;
	if (acl_action->send2port_enable)
	{
		lport = jlxx_port_c2l(dev, tcam_tbl->BF.dest_port);
		acl_action->dest_port = lport;
	}

	acl_action->force_vid_enable = tcam_tbl->BF.force_vid_valid;
	acl_action->force_vid = tcam_tbl->BF.force_vid;
	acl_action->force_queue_enable = tcam_tbl->BF.force_queue;
	acl_action->e_queue = tcam_tbl->BF.e_queue;
	acl_action->force_color_enable = tcam_tbl->BF.force_color;
	acl_action->color = tcam_tbl->BF.color;
	acl_action->mmp_valid = tcam_tbl->BF.mmp_valid;
	acl_action->mmp_ptr = tcam_tbl->BF.mmp_ptr;
	acl_action->mmp_order = tcam_tbl->BF.mmp_order_0 + 
							(tcam_tbl->BF.mmp_order_1 << 1);
	acl_action->update_counter_enable = tcam_tbl->BF.update_counter;
	acl_action->counter = tcam_tbl->BF.counter;
	acl_action->no_learning_enable = tcam_tbl->BF.no_learning;
	acl_action->update_cfi_dei_enable = tcam_tbl->BF.update_cfi_dei;
	acl_action->new_cfi_dei = tcam_tbl->BF.new_cfi_dei_value;
	acl_action->update_pcp_enable = tcam_tbl->BF.update_pcp;
	acl_action->new_pcp = tcam_tbl->BF.new_pcp_value;
	acl_action->update_vid_enable = tcam_tbl->BF.update_vid;
	acl_action->new_vid = tcam_tbl->BF.new_vid_value;
	acl_action->update_etype_enable = tcam_tbl->BF.update_etype;
	acl_action->new_etype = tcam_tbl->BF.new_eth_type;
	acl_action->input_mirror_enable = tcam_tbl->BF.input_mirror;
	if (acl_action->input_mirror_enable)
	{
		cport = tcam_tbl->BF.dest_input_mirror_0 + 
				(tcam_tbl->BF.dest_input_mirror_1_2 << 1);
		lport = jlxx_port_c2l(dev, cport);
		acl_action->dest_input_mirror = lport;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_get_action_from_hash_large(jl_device_t *dev,
										SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t *hash_tbl,
										jl_acl_action_t *acl_action)
{
	jl_port_t lport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(hash_tbl);

	acl_action->drop_enable = hash_tbl->BF.drop_enable;
	acl_action->send2cpu_enable = hash_tbl->BF.send_to_cpu;
	acl_action->send2port_enable = hash_tbl->BF.send_to_port;
	if (acl_action->send2port_enable)
	{
		lport = jlxx_port_c2l(dev, hash_tbl->BF.dest_port);
		acl_action->dest_port = lport;
	}

	acl_action->force_vid_enable = hash_tbl->BF.force_vid_valid;
	acl_action->force_vid = hash_tbl->BF.force_vid_0_7 +
							(hash_tbl->BF.force_vid_8_11 << 8);
	acl_action->force_queue_enable = hash_tbl->BF.force_queue;
	acl_action->e_queue = hash_tbl->BF.e_queue;
	acl_action->force_color_enable = hash_tbl->BF.force_color;
	acl_action->color = hash_tbl->BF.color;
	acl_action->mmp_valid = hash_tbl->BF.mmp_valid;
	acl_action->mmp_ptr = hash_tbl->BF.mmp_ptr;
	acl_action->mmp_order = hash_tbl->BF.mmp_order;
	acl_action->update_counter_enable = hash_tbl->BF.update_counter;
	acl_action->counter = hash_tbl->BF.counter;
	acl_action->no_learning_enable = hash_tbl->BF.no_learning;
	acl_action->update_cfi_dei_enable = hash_tbl->BF.update_cfi_dei;
	acl_action->new_cfi_dei = hash_tbl->BF.new_cfi_dei_value;
	acl_action->update_pcp_enable = hash_tbl->BF.update_pcp;
	acl_action->new_pcp = hash_tbl->BF.new_pcp_value;
	acl_action->update_vid_enable = hash_tbl->BF.update_vid;
	acl_action->new_vid = hash_tbl->BF.new_vid_value;
	acl_action->update_etype_enable = hash_tbl->BF.update_etype;
	acl_action->new_etype = hash_tbl->BF.new_eth_type;
	acl_action->input_mirror_enable = hash_tbl->BF.input_mirror;
	if (acl_action->input_mirror_enable)
	{
		lport = jlxx_port_c2l(dev, hash_tbl->BF.dest_input_mirror);
		acl_action->dest_input_mirror = lport;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_get_action_from_hash_small(jl_device_t *dev,
											SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t *hash_tbl,
											jl_acl_action_t *acl_action)
{
	jl_port_t lport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(hash_tbl);

	acl_action->drop_enable = hash_tbl->BF.drop_enable;
	acl_action->send2cpu_enable = hash_tbl->BF.send_to_cpu;
	acl_action->send2port_enable = hash_tbl->BF.send_to_port;
	if (acl_action->send2port_enable)
	{
		lport = jlxx_port_c2l(dev, hash_tbl->BF.dest_port);
		acl_action->dest_port = lport;
	}

	acl_action->force_vid_enable = hash_tbl->BF.force_vid_valid;
	acl_action->force_vid = hash_tbl->BF.force_vid_0_7 +
							(hash_tbl->BF.force_vid_8_11 << 8);
	acl_action->force_queue_enable = hash_tbl->BF.force_queue;
	acl_action->e_queue = hash_tbl->BF.e_queue;
	acl_action->force_color_enable = hash_tbl->BF.force_color;
	acl_action->color = hash_tbl->BF.color;
	acl_action->mmp_valid = hash_tbl->BF.mmp_valid;
	acl_action->mmp_ptr = hash_tbl->BF.mmp_ptr;
	acl_action->mmp_order = hash_tbl->BF.mmp_order;
	acl_action->update_counter_enable = hash_tbl->BF.update_counter;
	acl_action->counter = hash_tbl->BF.counter;
	acl_action->no_learning_enable = hash_tbl->BF.no_learning;
	acl_action->update_cfi_dei_enable = hash_tbl->BF.update_cfi_dei;
	acl_action->new_cfi_dei = hash_tbl->BF.new_cfi_dei_value;
	acl_action->update_pcp_enable = hash_tbl->BF.update_pcp;
	acl_action->new_pcp = hash_tbl->BF.new_pcp_value;
	acl_action->update_vid_enable = hash_tbl->BF.update_vid;
	acl_action->new_vid = hash_tbl->BF.new_vid_value;
	acl_action->update_etype_enable = hash_tbl->BF.update_etype;
	acl_action->new_etype = hash_tbl->BF.new_eth_type;
	acl_action->input_mirror_enable = hash_tbl->BF.input_mirror;
	if (acl_action->input_mirror_enable)
	{
		lport = jlxx_port_c2l(dev, hash_tbl->BF.dest_input_mirror);
		acl_action->dest_input_mirror  = lport;
	}

	return JL_ERR_OK;
}


static jl_ret_t jl6107x_acl_rule_info_set(jl_device_t *dev,
										jl_uint8 index,
										jl_acl_rule_t *acl_rule,
										jl_acl_action_t *acl_action)
{
	jl_ret_t ret;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);

	gp_acl_rule_info[dev->id]->acl_rule[index].valid = TRUE;
	gp_acl_rule_info[dev->id]->acl_rule[index].field_num = acl_rule->field_num;
	gp_acl_rule_info[dev->id]->acl_rule[index].field_mask = acl_rule->field_mask;
	gp_acl_rule_info[dev->id]->acl_rule[index].rule_templ = acl_rule->templ_idx;
	gp_acl_rule_info[dev->id]->acl_rule[index].rule_table = acl_rule->table_type;
	gp_acl_rule_info[dev->id]->acl_rule[index].rule_index = acl_rule->entry_idx;

	gp_acl_rule_info[dev->id]->acl_rule[index].counter_valid = acl_action->update_counter_enable;
	if(acl_action->update_counter_enable)
	{
		gp_acl_rule_info[dev->id]->acl_rule[index].counter_index = acl_action->counter;
		gp_acl_rule_info[dev->id]->counter[acl_action->counter]++;
	}

	if (acl_rule->table_type == ACL_TBL_TCAM)
		SET_BIT(gp_acl_rule_info[dev->id]->entry_mask_tcam, acl_rule->entry_idx);
	else if (acl_rule->table_type == ACL_TBL_LARGE)
		SET_BIT(gp_acl_rule_info[dev->id]->entry_mask_large, acl_rule->entry_idx);
	else
		SET_BIT(gp_acl_rule_info[dev->id]->entry_mask_small, acl_rule->entry_idx);


	if (acl_action->mmp_valid)
	{
		gp_acl_rule_info[dev->id]->acl_rule[index].mmp_index = acl_action->mmp_ptr;

		ret = jl6107x_set_mmp_pointer_info(dev, JL_MMP_MODULE_ACL, acl_action->mmp_ptr);
		if (ret)
			return ret;
	}
	else if (acl_action->mmp_valid == FALSE && gp_acl_rule_info[dev->id]->acl_rule[index].mmp_valid)
	{
		ret = jl6107x_clear_mmp_pointer_info(dev, JL_MMP_MODULE_ACL,
									gp_acl_rule_info[dev->id]->acl_rule[index].mmp_index);
	}
	gp_acl_rule_info[dev->id]->acl_rule[index].mmp_valid = acl_action->mmp_valid;

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_rule_info_clear(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 counter = 0;
	JL_CHECK_POINTER(dev);

	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid == TRUE)
	{
		if (gp_acl_rule_info[dev->id]->acl_rule[index].rule_table == ACL_TBL_TCAM)
			CLR_BIT(gp_acl_rule_info[dev->id]->entry_mask_tcam,gp_acl_rule_info[dev->id]->acl_rule[index].rule_index);
		else if (gp_acl_rule_info[dev->id]->acl_rule[index].rule_table == ACL_TBL_LARGE)
			CLR_BIT(gp_acl_rule_info[dev->id]->entry_mask_large,gp_acl_rule_info[dev->id]->acl_rule[index].rule_index);
		else
			CLR_BIT(gp_acl_rule_info[dev->id]->entry_mask_small,gp_acl_rule_info[dev->id]->acl_rule[index].rule_index);

		counter = gp_acl_rule_info[dev->id]->acl_rule[index].counter_index;

		if (gp_acl_rule_info[dev->id]->acl_rule[index].counter_valid)
		{
			if (--gp_acl_rule_info[dev->id]->counter[counter] == 0)
			{
				/*no rule use this counter,clear it*/
				ret = jl6107x_acl_stat_write(dev, counter, 0);
				if (ret)
					return ret;
			}
		}

		if (gp_acl_rule_info[dev->id]->acl_rule[index].mmp_valid)
		{
			ret = jl6107x_clear_mmp_pointer_info(dev, JL_MMP_MODULE_ACL,
									gp_acl_rule_info[dev->id]->acl_rule[index].mmp_index);
			if (ret)
				return ret;
		}

		memset(&gp_acl_rule_info[dev->id]->acl_rule[index], 0, sizeof(jl6107x_acl_entry_info_t));
	}

	return 	JL_ERR_OK;
}


static jl_ret_t jl6107x_acl_rule_info_get(jl_device_t *dev,
										jl_uint8 index,
										jl_acl_rule_t *acl_rule,
										jl_acl_action_t *acl_action)
{
	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);

	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid == FALSE)
		return JL_ERR_NOT_FOUND;
	acl_rule->templ_idx = gp_acl_rule_info[dev->id]->acl_rule[index].rule_templ;
	acl_rule->field_mask = gp_acl_rule_info[dev->id]->acl_rule[index].field_mask;
	acl_rule->field_num = gp_acl_rule_info[dev->id]->acl_rule[index].field_num;
	acl_rule->table_type = gp_acl_rule_info[dev->id]->acl_rule[index].rule_table;
	acl_rule->entry_idx = gp_acl_rule_info[dev->id]->acl_rule[index].rule_index;

	acl_action->update_counter_enable = gp_acl_rule_info[dev->id]->acl_rule[index].counter_valid;
	acl_action->counter = gp_acl_rule_info[dev->id]->acl_rule[index].counter_index;
	acl_action->mmp_valid = gp_acl_rule_info[dev->id]->acl_rule[index].mmp_valid;
	acl_action->mmp_ptr = gp_acl_rule_info[dev->id]->acl_rule[index].mmp_index;

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_action_chk(jl_device_t *dev, jl_acl_action_t *acl_action)
{
	JL_CHECK_POINTER(acl_action);

	if (acl_action->drop_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->send2cpu_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->force_vid_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->force_vid_enable && (acl_action->force_vid > 0xfff))
		return JL_ERR_PARAM;

	if (acl_action->force_queue_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->force_queue_enable && (acl_action->e_queue > 7))
		return JL_ERR_PARAM;

	if (acl_action->force_color_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->force_color_enable && (acl_action->color > 2))
		return JL_ERR_PARAM;

	if (acl_action->mmp_valid > 1)
		return JL_ERR_PARAM;

	if (acl_action->mmp_valid &&
			(acl_action->mmp_ptr > 15 || acl_action->mmp_order > 3))
		return JL_ERR_PARAM;

	if (acl_action->update_counter_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_counter_enable && (acl_action->counter > 31))
		return JL_ERR_PARAM;

	if (acl_action->no_learning_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_cfi_dei_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_cfi_dei_enable  && (acl_action->new_cfi_dei > 1))
		return JL_ERR_PARAM;

	if (acl_action->update_pcp_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_pcp_enable && (acl_action->new_pcp > 7))
		return JL_ERR_PARAM;

	if (acl_action->update_vid_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_vid_enable && (acl_action->new_vid > 0xfff))
		return JL_ERR_PARAM;

	if (acl_action->update_etype_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_etype_enable && (acl_action->new_etype > 3))
		return JL_ERR_PARAM;

	if (acl_action->input_mirror_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->input_mirror_enable)
	{
		JL_CHECK_PORT(dev, acl_action->dest_input_mirror);
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_rule_pre_process(jl_device_t *dev, jl_acl_rule_t *acl_rule)
{
	jl_acl_field_data_t* field_buf = NULL;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);

	if (CHECK_BIT(gp_acl_template[dev->id]->rule_pointer_mask, acl_rule->templ_idx) &&
															(acl_rule->with_pointer == 0))
	{
		field_buf = (jl_acl_field_data_t*)port_mem_malloc(sizeof(jl_acl_field_data_t));
		if (!field_buf)
			return JL_ERR_MEMORY;

		field_buf->field_type = ACL_FIELD_RULE_POINTER;
		field_buf->field_idx = 0;
		field_buf->templ_idx = acl_rule->templ_idx;
		field_buf->field.rule_pointer.value = acl_rule->templ_idx;
		field_buf->next = acl_rule->head;
		acl_rule->head = field_buf;
		acl_rule->with_pointer = 1;
		acl_rule->field_num++;
		SET_BIT(acl_rule->field_mask, 0);
	}

	if (acl_rule->field_mask != gp_acl_template[dev->id]->templ_mask[acl_rule->templ_idx])
		acl_rule->table_type= ACL_TBL_TCAM;
	else
		acl_rule->table_type = ACL_TBL_LARGE;

	return JL_ERR_OK;
}


static jl_ret_t jl6107x_acl_write_tcam(jl_device_t *dev,
					jl_uint8 index,
					jl6107x_acl_hash_key_t *key,
					jl_acl_action_t *acl_action,
					jl_uint8 *idx_out)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 tcam_mask = 0;
	jl_uint8 tcam_used = 0;
	jl_uint8 idx = 0;
	jl_uint8 tcam_valid = FALSE;

	SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_t	tcam_table;
	SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_t answer_table;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(acl_action);

	memset((void*)&tcam_table, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_t));
	memset((void*)&answer_table, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_t));

	tcam_mask = gp_acl_rule_info[dev->id]->entry_mask_tcam;

	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid == TRUE &&
		gp_acl_rule_info[dev->id]->acl_rule[index].rule_table == ACL_TBL_TCAM)
	{
		tcam_used = gp_acl_rule_info[dev->id]->acl_rule[index].rule_index;
		CLR_BIT(tcam_mask,tcam_used);
	}

	for (; idx < 32; idx++)
	{
		if (!(tcam_mask & 0x1))
		{
			tcam_valid = TRUE;
			break;
		}
		tcam_mask >>= 1;
	}

	if (tcam_valid == FALSE)
		return JL_ERR_RESOURCE;

	ret = jl6107x_acl_add_key_to_tcam(key, (jl_uint8*)&tcam_table);  //pack rule data
	if (ret)
		return ret;

	ret = jl6107x_acl_add_action_to_tcam(dev, acl_action, &answer_table);  //pack action data
	if (ret)
		return ret;

	tcam_table.BF.valid = 1;

	ret = jl6107x_acl_clear_table(dev, index);
	if (ret)
		return ret;

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER, answer_table, idx, 0);
	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_TCAM, tcam_table, idx, 0);

	*idx_out = idx;

	return JL_ERR_OK;
}


static jl_ret_t jl6107x_acl_write_hash_large(jl_device_t	 *dev,
						jl_uint8 index,
						jl6107x_acl_hash_key_t *key,
						jl_acl_action_t *acl_action,
						jl_uint8 *idx_out)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl_uint8	hash_idx = 0;
	jl_uint8	key_idx = 0;
	jl_uint8	hash_used = 0;
	jl_uint8	buckets_idx = 0;
	jl_uint64	hash_mask = 0;
	jl_bool		hash_valid = FALSE;

	SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t hash_table;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(idx_out);

	hash_mask = gp_acl_rule_info[dev->id]->entry_mask_large;

	key_idx = jl6107x_acl_calc_hash_idx(key, 1);

	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid == TRUE &&
		gp_acl_rule_info[dev->id]->acl_rule[index].rule_table == ACL_TBL_LARGE)
	{
		hash_used = gp_acl_rule_info[dev->id]->acl_rule[index].rule_index;
		CLR_BIT(hash_mask,hash_used);
	}

	for (buckets_idx = 0; buckets_idx < 4; buckets_idx++)
	{
		hash_idx = (buckets_idx << 4) + key_idx;

		if (GET_BIT(hash_mask,hash_idx) == 0)
		{
			hash_valid = TRUE;
			break;
		}
	}

	if (hash_valid == FALSE)
		return JL_ERR_RESOURCE;

	memset((void*)&hash_table, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t));

	ret = jl6107x_acl_add_key_to_hash(key, (jl_uint8*)&hash_table);  //pack rule data
	if (ret)
		return ret;

	ret = jl6107x_acl_add_action_to_hash_large(dev, acl_action, &hash_table);  //pack action data
	if (ret)
		return ret;

	hash_table.BF.valid = 1;

	ret = jl6107x_acl_clear_table(dev, index);
	if (ret)
		return ret;

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_LARGE_TABLE, hash_table, hash_idx, 0);

	*idx_out = hash_idx;

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_write_hash_small(jl_device_t *dev,
					jl_uint8 index,
					jl6107x_acl_hash_key_t *key,
					jl_acl_action_t *acl_action,
					jl_uint8* idx_out)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl_uint8	hash_idx = 0;
	jl_uint8	key_idx = 0;
	jl_uint8	hash_used = 0;
	jl_uint8	buckets_idx = 0;
	jl_uint32	hash_mask = 0;
	jl_bool		hash_valid = FALSE;

	SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t hash_table;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(idx_out);

	hash_mask = gp_acl_rule_info[dev->id]->entry_mask_small;

	key_idx = jl6107x_acl_calc_hash_idx(key, 0);

	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid == TRUE &&
		gp_acl_rule_info[dev->id]->acl_rule[index].rule_table == ACL_TBL_SMALL)
	{
		hash_used = gp_acl_rule_info[dev->id]->acl_rule[index].rule_index;
		CLR_BIT(hash_mask,hash_used);
	}

	for (buckets_idx = 0; buckets_idx < 4; buckets_idx++)
	{
		hash_idx = (buckets_idx << 3) + key_idx;

		if (GET_BIT(hash_mask,hash_idx) == 0)
		{
			hash_valid = TRUE;
			break;
		}
	}

	if (hash_valid == FALSE)
		return JL_ERR_RESOURCE;

	memset((void*)&hash_table, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t));

	ret = jl6107x_acl_add_key_to_hash(key, (jl_uint8*)&hash_table);  //pack rule data
	if (ret)
		return ret;

	ret = jl6107x_acl_add_action_to_hash_small(dev, acl_action, &hash_table);  //pack action data
	if (ret)
		return ret;

	hash_table.BF.valid = 1;

	ret = jl6107x_acl_clear_table(dev, index);
	if (ret)
		return ret;

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_SMALL_TABLE, hash_table, hash_idx, 0);

	*idx_out = hash_idx;

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_read_tcam(jl_device_t *dev,
					jl_uint8 index,
					jl_acl_rule_t *rule_out,
					jl_acl_action_t *act_out)

{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(rule_out);
	JL_CHECK_POINTER(act_out);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER, answer_tbl, index, 0);
	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_TCAM, tcam_tbl, index, 0);

	ret = jl6107x_acl_get_rule_from_tcam(dev, &tcam_tbl, rule_out);
	if (ret)
		return ret;

	ret = jl6107x_acl_get_action_from_tcam(dev, &answer_tbl, act_out);

	return ret;
}


static jl_ret_t jl6107x_acl_read_hash_large(jl_device_t *dev,
					jl_uint8 index,
					jl_acl_rule_t *rule_out,
					jl_acl_action_t *act_out)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(rule_out);
	JL_CHECK_POINTER(act_out);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_LARGE_TABLE, hash_tbl, index, 0);

	ret = jl6107x_acl_get_rule_from_hash_large(dev, &hash_tbl, rule_out);
	if (ret)
		return ret;

	ret = jl6107x_acl_get_action_from_hash_large(dev, &hash_tbl, act_out);

	return ret;
}

static jl_ret_t jl6107x_acl_read_hash_mall(jl_device_t *dev,
					jl_uint8 index,
					jl_acl_rule_t *rule_out,
					jl_acl_action_t *act_out)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(rule_out);
	JL_CHECK_POINTER(act_out);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_SMALL_TABLE, hash_tbl, index, 0);

	ret = jl6107x_acl_get_rule_from_hash_small(dev, &hash_tbl, rule_out);
	if (ret)
		return ret;

	ret = jl6107x_acl_get_action_from_hash_small(dev, &hash_tbl, act_out);

	return ret;
}

static jl_ret_t jl6107x_acl_clear_table(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid == FALSE)
		return JL_ERR_OK;

	if (gp_acl_rule_info[dev->id]->acl_rule[index].rule_table == ACL_TBL_TCAM)
		ret = jl6107x_acl_clear_tcam(dev, gp_acl_rule_info[dev->id]->acl_rule[index].rule_index);
	else if (gp_acl_rule_info[dev->id]->acl_rule[index].rule_table == ACL_TBL_LARGE)
		ret = jl6107x_acl_clear_hash_large(dev, gp_acl_rule_info[dev->id]->acl_rule[index].rule_index);
	else
		ret = jl6107x_acl_clear_hash_small(dev, gp_acl_rule_info[dev->id]->acl_rule[index].rule_index);

	if (ret)
		return ret;

	ret = jl6107x_acl_rule_info_clear(dev, index);

	return ret;
}

static jl_ret_t jl6107x_acl_clear_tcam(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_t	tcam_tbl;
	SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_t answer_tbl;

	memset((void*)&tcam_tbl, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_t));
	memset((void*)&answer_tbl, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER_t));

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_TCAM, tcam_tbl, index, 0);
	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER, answer_tbl, index, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_clear_hash_large(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t hash_tbl;

	memset((void*)&hash_tbl, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_LARGE_TABLE_t));

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_LARGE_TABLE, hash_tbl, index, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_clear_hash_small(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t hash_tbl;

	memset((void*)&hash_tbl, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_SMALL_TABLE_t));

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_SMALL_TABLE, hash_tbl, index, 0);

	return JL_ERR_OK;
}


static jl_ret_t jl6107x_acl_l2v(void *val_in, jl_uint8 len, jl6107x_acl_val_t* val_out)
{
	jl_acl_mac_t *p_mac = NULL;
	jl_acl_ip_t *p_ip = NULL;
	jl_acl_ip6_t *p_ip6 = NULL;

	jl_uint8 i = 0;
	jl_uint64 val = 0;

	JL_CHECK_POINTER(val_in);
	JL_CHECK_POINTER(val_out);

	memset(val_out, 0, sizeof(jl6107x_acl_val_t));

	if (len == 32) //ipv4
	{
		p_ip = (jl_acl_ip_t*)val_in;

		for (i = 0; i < 4; i++)
		{
			val = p_ip->value.addr[i];
			val <<= ((3 - i) * 8);
			val_out->val_0_63 |= val;
			val = p_ip->mask.addr[i];
			val <<= ((3 - i) * 8);
			val_out->mask_0_63 |= val;
		}
	}
	else if (len == 48) //mac
	{
		p_mac = (jl_acl_mac_t*)val_in;

		for (i = 0; i < 6; i++)
		{
			val = p_mac->value.addr[i];
			val <<= ((5 - i) * 8);
			val_out->val_0_63 |= val;
			val = p_mac->mask.addr[i];
			val <<= ((5 - i) * 8);
			val_out->mask_0_63 |= val;
		}
	}
	else if (len == 128) //ipv6
	{
		p_ip6 = (jl_acl_ip6_t*)val_in;

		val_out->val_0_63 = *(jl_uint64*)&p_ip6->value.addr[0];
		val_out->val_64_127 = *(jl_uint64*)&p_ip6->value.addr[4];
		val_out->mask_0_63 = *(jl_uint64*)&p_ip6->mask.addr[0];
		val_out->mask_64_127 = *(jl_uint64*)&p_ip6->mask.addr[4];
	}
	else
		return JL_ERR_PARAM;

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_v2l(jl6107x_acl_val_t* val_in, jl_uint8 len, void *val_out)
{
	jl_acl_mac_t *p_mac = NULL;
	jl_acl_ip_t *p_ip = NULL;
	jl_acl_ip6_t *p_ip6 = NULL;

	jl_uint8 i = 0;
	jl_uint64 val = 0;

	JL_CHECK_POINTER(val_in);
	JL_CHECK_POINTER(val_out);

	if (len == 32) //ipv4
	{
		p_ip = (jl_acl_ip_t*)val_out;
		memset(p_ip, 0, sizeof(jl_acl_ip_t));

		for (i = 0; i < 4; i++)
		{
			val =  (val_in->val_0_63 >> ((3 - i) * 8));
			p_ip->value.addr[i] = val & 0xff;
			val =  (val_in->mask_0_63 >> ((3 - i) * 8));
			p_ip->mask.addr[i] = val & 0xff;
		}
	}
	else if (len == 48) //mac
	{
		p_mac = (jl_acl_mac_t*)val_out;
		memset(p_mac, 0, sizeof(jl_acl_mac_t));

		for (i = 0; i < 6; i++)
		{
			val = (val_in->val_0_63 >> ((5 - i) * 8));
			p_mac->value.addr[i] = val & 0xff;
			val =  (val_in->mask_0_63 >> ((5 - i) * 8));
			p_mac->mask.addr[i] = val & 0xff;
		}
	}
	else if (len == 128) //ipv6
	{
		p_ip6 = (jl_acl_ip6_t*)val_out;

		*(jl_uint64*)&p_ip6->value.addr[0] = val_in->val_0_63;
		*(jl_uint64*)&p_ip6->value.addr[4] = val_in->val_64_127;
		*(jl_uint64*)&p_ip6->mask.addr[0] = val_in->mask_0_63;
		*(jl_uint64*)&p_ip6->mask.addr[4] = val_in->mask_64_127;
	}
	else
		return JL_ERR_PARAM;

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_get_mask_val(jl_device_t* dev,
								jl_acl_field_data_t* field_rule,
									jl6107x_acl_val_t* val_out)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport = 0;
	jl_uint8 bit_width = 0;
	jl_uint32 mask_val = 0;

	val_out->val_0_63 = 0;
	val_out->val_64_127 = 0;
	val_out->mask_0_63 = 0;
	val_out->mask_64_127 = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(field_rule);
	JL_CHECK_POINTER(val_out);

	if (field_rule->field_type >= ACL_FIELD_NUM)
	return JL_ERR_PARAM;

	bit_width = jl6107x_acl_field_bit_size[field_rule->field_type];

	if (bit_width <= 32)
		mask_val = BITS_WIDTH(bit_width);

	switch (field_rule->field_type)
	{
		case ACL_FIELD_SOURCE_PORT:
			cport = jlxx_port_l2c(dev, field_rule->field.source_port.value);
			val_out->val_0_63 = cport;
			val_out->mask_0_63 = field_rule->field.source_port.mask & mask_val;
			break;
		case ACL_FIELD_MAC_DA:
			ret = jl6107x_acl_l2v((void*)&field_rule->field.d_mac, 48, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_MAC_SA:
			ret = jl6107x_acl_l2v((void*)&field_rule->field.s_mac, 48, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_OUTER_VID:
			val_out->val_0_63 = field_rule->field.outer_vid.value;
			val_out->mask_0_63 = field_rule->field.outer_vid.mask & mask_val;
			break;
		case ACL_FIELD_HAS_VLAN:
			val_out->val_0_63 = field_rule->field.has_vlans.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_OUTER_ETH_TYPE:
			val_out->val_0_63 = field_rule->field.outer_cstag.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_INNER_ETH_TYPE:
			val_out->val_0_63 = field_rule->field.inner_cstag.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_OUTER_PCP:
			val_out->val_0_63 = field_rule->field.outer_pcp.value;
			val_out->mask_0_63 = field_rule->field.outer_pcp.mask & mask_val;
			break;
		case ACL_FIELD_OUTER_DEI:
			val_out->val_0_63 = field_rule->field.outer_dei.value;
			val_out->mask_0_63 = field_rule->field.outer_dei.mask & mask_val;
			break;
		case ACL_FIELD_INNER_VID:
			val_out->val_0_63 = field_rule->field.inner_vid.value;
			val_out->mask_0_63 = field_rule->field.inner_vid.mask & mask_val;
			break;
		case ACL_FIELD_INNER_PCP:
			val_out->val_0_63 = field_rule->field.inner_pcp.value;
			val_out->mask_0_63 = field_rule->field.inner_pcp.mask & mask_val;
			break;
		case ACL_FIELD_INNER_DEI:
			val_out->val_0_63 = field_rule->field.inner_dei.value;
			val_out->mask_0_63 = field_rule->field.inner_dei.mask & mask_val;
			break;
		case ACL_FIELD_ETH_TYPE:
			val_out->val_0_63 = field_rule->field.eth_type.value;
			val_out->mask_0_63 = field_rule->field.eth_type.mask & mask_val;
			break;
		case ACL_FIELD_L3_TYPE:
			val_out->val_0_63 = field_rule->field.l3_type.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_IP_SA:
			ret = jl6107x_acl_l2v((void*)&field_rule->field.s_ip, 32, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_IP_DA:
			ret = jl6107x_acl_l2v((void*)&field_rule->field.d_ip, 32, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_IP6_SA:
			ret =jl6107x_acl_l2v((void*)&field_rule->field.s_ip6, 128, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_IP6_DA:
			ret =jl6107x_acl_l2v((void*)&field_rule->field.d_ip6, 128, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_MPLS:
			val_out->val_0_63 = field_rule->field.mpls.value;
			val_out->mask_0_63 = field_rule->field.mpls.mask & mask_val;
			break;
		case ACL_FIELD_TOS:
			val_out->val_0_63 = field_rule->field.tos.value;
			val_out->mask_0_63 = field_rule->field.tos.mask & mask_val;
			break;
		case ACL_FIELD_TTL:
			val_out->val_0_63 = field_rule->field.ttl.value;
			val_out->mask_0_63 = field_rule->field.ttl.mask & mask_val;
			break;
		case ACL_FIELD_L4_PROTOCOL:
			val_out->val_0_63 = field_rule->field.l4_pro.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_L4_TYPE:
			val_out->val_0_63 = field_rule->field.l4_type.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_L4_SOURCE_PORT:
			val_out->val_0_63 = field_rule->field.l4_s_port.value;
			val_out->mask_0_63 = field_rule->field.l4_s_port.mask & mask_val;
			break;
		case ACL_FIELD_L4_DEST_PORT:
			val_out->val_0_63 = field_rule->field.l4_d_port.value;
			val_out->mask_0_63 = field_rule->field.l4_d_port.mask & mask_val;
			break;
		case ACL_FIELD_MLD_ADDR:
			ret =jl6107x_acl_l2v((void*)&field_rule->field.mld_addr, 128, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_ICMP_TYPE:
			val_out->val_0_63 = field_rule->field.icmp_type.value;
			val_out->mask_0_63 = field_rule->field.icmp_type.mask & mask_val;
			break;
		case ACL_FIELD_ICMP_CODE:
			val_out->val_0_63 = field_rule->field.icmp_code.value;
			val_out->mask_0_63 = field_rule->field.icmp_code.mask & mask_val;
			break;
		case ACL_FIELD_ICMP6_TYPE:
			val_out->val_0_63 = field_rule->field.icmp6_type.value;
			val_out->mask_0_63 = field_rule->field.icmp6_type.mask & mask_val;
			break;
		case ACL_FIELD_ICMP6_CODE:
			val_out->val_0_63 = field_rule->field.icmp6_code.value;
			val_out->mask_0_63 = field_rule->field.icmp6_code.mask & mask_val;
			break;
		case ACL_FIELD_IGMP_TYPE:
			val_out->val_0_63 = field_rule->field.igmp_type.value;
			val_out->mask_0_63 = field_rule->field.igmp_type.mask & mask_val;
			break;
		case ACL_FIELD_IGMP_GROUP_ADDR:
			ret = jl6107x_acl_l2v((void*)&field_rule->field.igmp_group, 32, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_RULE_POINTER:
			val_out->val_0_63 = field_rule->templ_idx;	//rule pointer is the template index
			val_out->mask_0_63 = mask_val;
			break;
		default:
				return JL_ERR_PARAM;
	}

	return 	JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_rule_get_key(jl_device_t *dev,
										jl_acl_rule_t *acl_rule,
										jl6107x_acl_hash_key_t *key)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl_uint8	entry_idx = 0;
	jl_uint8	bit_offset = 0;
	jl_uint8	bit_width = 0;
	jl_uint8	field_type = 0;
	jl_uint8	field_mask = 0;
	jl6107x_acl_val_t	val;
	jl_acl_field_data_t	*pfield = NULL;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(key);

	pfield = acl_rule->head;
	if (pfield == NULL)
		return JL_ERR_CONFIG;

	field_mask = gp_acl_template[dev->id]->templ_mask[acl_rule->templ_idx] & 0x3f;

	jl_write_bits(key->value, field_mask, 0, 6);		//set field valid bits
	jl_write_bits(key->mask, field_mask, 0, 6);

	while (pfield)
	{
		bit_width = jl6107x_acl_field_bit_size[pfield->field_type];
		bit_offset = 0;

		for(entry_idx = 0; entry_idx < pfield->field_idx; entry_idx++)
		{
			field_type = gp_acl_template[dev->id]->acl_templ[acl_rule->templ_idx][entry_idx];
			bit_offset += jl6107x_acl_field_bit_size[field_type];
		}

		ret = jl6107x_acl_get_mask_val(dev, pfield, &val);
		if (ret)
			return ret;

		if (pfield->field_type == ACL_FIELD_IP6_SA ||
			pfield->field_type == ACL_FIELD_IP6_DA ||
			pfield->field_type == ACL_FIELD_MLD_ADDR)
		{
			jl_write_bits(key->mask, val.mask_0_63, bit_offset + 6, 64);  // 6 field valid bits
			jl_write_bits(key->mask, val.mask_64_127, bit_offset + 64 + 6, bit_width - 64);

			jl_write_bits(key->value, val.val_0_63, bit_offset + 6, 64);
			jl_write_bits(key->value, val.val_64_127, bit_offset + 64 + 6, bit_width - 64);

			if (val.mask_0_63 != 0xffffffffffffffff ||
				val.mask_64_127 != 0xffffffffffffffff)
			{
				acl_rule->table_type = ACL_TBL_TCAM;
			}
		}
		else if (pfield->field_type == ACL_FIELD_MAC_DA ||
					pfield->field_type == ACL_FIELD_MAC_SA)
		{
			jl_write_bits(key->mask, val.mask_0_63, bit_offset + 6, 48);
			jl_write_bits(key->value, val.val_0_63, bit_offset + 6, 48);

			if (val.mask_0_63 != 0xffffffffffff)
				acl_rule->table_type = ACL_TBL_TCAM;
		}
		else
		{
			jl_write_bits(key->mask, val.mask_0_63, bit_offset + 6, bit_width);
			jl_write_bits(key->value, val.val_0_63, bit_offset + 6, bit_width);

			if (val.mask_0_63 != BITS(0, bit_width - 1))
				acl_rule->table_type = ACL_TBL_TCAM;
		}

		pfield = pfield->next;
	}

	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_rule_add(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_rule_t *acl_rule,
				jl_acl_action_t *acl_action)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl_uint8	entry_idx = 0;
	jl6107x_acl_hash_key_t key;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);

	if (acl_rule->field_num == 0)
		return JL_ERR_CONFIG;

	memset((void*)&key, 0, sizeof(jl6107x_acl_hash_key_t));

	ret = jl6107x_acl_action_chk(dev, acl_action);
	if (ret)
		return ret;

	ret = jl6107x_acl_rule_pre_process(dev, acl_rule);
	if (ret)
		return ret;

	ret = jl6107x_acl_rule_get_key(dev, acl_rule, &key);
	if (ret)
		return ret;

	if (acl_rule->table_type == ACL_TBL_LARGE)
	{
		ret = jl6107x_acl_write_hash_large(dev, index, &key, acl_action, &entry_idx);

		if (ret == JL_ERR_RESOURCE)
		{
			acl_rule->table_type = ACL_TBL_SMALL;
			ret = jl6107x_acl_write_hash_small(dev, index, &key, acl_action, &entry_idx);
			if (ret == JL_ERR_RESOURCE)
				acl_rule->table_type = ACL_TBL_TCAM;
			else if (ret)
				return ret;
		}
		else if (ret)
			return ret;
	}

	if (acl_rule->table_type == ACL_TBL_TCAM)
	{
		ret = jl6107x_acl_write_tcam(dev, index, &key, acl_action, &entry_idx);
		if (ret)
			return ret;
	}

	acl_rule->entry_idx = entry_idx;

	ret = jl6107x_acl_rule_info_set(dev, index, acl_rule, acl_action);

	return ret;
}


jl_ret_t jl6107x_acl_rule_get(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_rule_t *acl_rule,
				jl_acl_action_t *acl_action)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);

	ret = jl6107x_acl_rule_info_get(dev, index, acl_rule, acl_action);
	if (ret)
		return ret;

	if (acl_rule->table_type== ACL_TBL_TCAM)
		ret = jl6107x_acl_read_tcam(dev, acl_rule->entry_idx, acl_rule, acl_action);
	else if (acl_rule->table_type == ACL_TBL_LARGE)
		ret = jl6107x_acl_read_hash_large(dev, acl_rule->entry_idx, acl_rule, acl_action);
	else
		ret = jl6107x_acl_read_hash_mall(dev, acl_rule->entry_idx, acl_rule, acl_action);

	return ret;
}


jl_ret_t jl6107x_acl_rule_del(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	ret = jl6107x_acl_clear_table(dev, index);

	return ret;
}


jl_ret_t jl6107x_acl_selection_set(jl_device_t *dev, jl_uint8 val)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_INGRESS_CONFIGURABLE_ACL_SELECTION_t selec_tbl;

	memset(&selec_tbl, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_SELECTION_t));

	selec_tbl.BF.select_tcam_or_table = (val & 0x1);
	selec_tbl.BF.select_small_or_large = ((val >> 1) & 0x1);

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_SELECTION, selec_tbl, 0, 0);

	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_selection_get(jl_device_t *dev, jl_uint8 *val)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_SELECTION, selec_tbl, 0, 0);

	*val = selec_tbl.val[0];

	return JL_ERR_OK;
}


jl_ret_t jl6107x_acl_stat_read(jl_device_t *dev,
					jl_uint8 counter,
					jl_uint32 *packets)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(packets);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_MATCH_COUNTER, counter_tbl, counter, 0);

	*packets = counter_tbl.BF.packets;

	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_stat_write(jl_device_t *dev,
					jl_uint8 counter,
					jl_uint32 packets)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_INGRESS_CONFIGURABLE_ACL_MATCH_COUNTER_t counter_tbl;

	memset(&counter_tbl, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_MATCH_COUNTER_t));

	counter_tbl.BF.packets = packets & 0xffffff;

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_MATCH_COUNTER, counter_tbl, counter, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_stat_enable_tcam(jl_device_t *dev,
						jl_uint8 index,
						jl_uint8 counter,
						jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER, answer_tbl, index, 0);

	if (enable)
	{
		answer_tbl.BF.update_counter = 1;
		answer_tbl.BF.counter = counter;
	}
	else
	{
		answer_tbl.BF.update_counter = 0;
		answer_tbl.BF.counter = 0;
	}

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_TCAM_ANSWER, answer_tbl, index, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_stat_enable_hash_large(jl_device_t *dev,
						jl_uint8 index,
						jl_uint8 counter,
						jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_LARGE_TABLE, hash_tbl, index, 0);

	if (enable)
	{
		hash_tbl.BF.update_counter = 1;
		hash_tbl.BF.counter = counter;
	}
	else
	{
		hash_tbl.BF.update_counter = 0;
		hash_tbl.BF.counter = 0;
	}

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_LARGE_TABLE, hash_tbl, index, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_acl_stat_enable_hash_small(jl_device_t *dev,
						jl_uint8 index,
						jl_uint8 counter,
						jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_SMALL_TABLE, hash_tbl, index, 0);

	if (enable)
	{
		hash_tbl.BF.update_counter = 1;
		hash_tbl.BF.counter = counter;
	}
	else
	{
		hash_tbl.BF.update_counter = 0;
		hash_tbl.BF.counter = 0;
	}

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_SMALL_TABLE, hash_tbl, index, 0);

	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_stat_enable(jl_device_t *dev,
								jl_uint8 index,
								jl_uint8 counter)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_acl_rule_t acl_rule;
	jl_acl_action_t acl_act;
	jl_uint8 enable = TRUE;

	JL_CHECK_POINTER(dev);

	memset(&acl_rule, 0, sizeof(jl_acl_rule_t));
	memset(&acl_act, 0, sizeof(jl_acl_action_t));

	ret = jl6107x_acl_rule_info_get(dev, index, &acl_rule, &acl_act);
	if (ret)
		return ret;

	if (acl_act.update_counter_enable &&
		(acl_act.counter == counter))
	{
		/*none change*/
		return JL_ERR_OK;
	}

	if (acl_rule.table_type == ACL_TBL_TCAM)
		ret = jl6107x_acl_stat_enable_tcam(dev, acl_rule.entry_idx, counter, enable);
	else if (acl_rule.table_type == ACL_TBL_LARGE)
		ret = jl6107x_acl_stat_enable_hash_large(dev, acl_rule.entry_idx, counter, enable);
	else
		ret = jl6107x_acl_stat_enable_hash_small(dev, acl_rule.entry_idx, counter, enable);

	if (ret)
		return ret;

	/*update rule info*/
	acl_act.update_counter_enable = TRUE;
	acl_act.counter = counter;

	ret = jl6107x_acl_rule_info_clear(dev, index);
	if (ret)
		return ret;

	ret = jl6107x_acl_rule_info_set(dev, index, &acl_rule, &acl_act);

	return ret;
}

jl_ret_t jl6107x_acl_stat_disable(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_acl_rule_t acl_rule;
	jl_acl_action_t acl_act;
	jl_uint8 enable = FALSE;

	JL_CHECK_POINTER(dev);

	memset(&acl_rule, 0, sizeof(jl_acl_rule_t));
	memset(&acl_act, 0, sizeof(jl_acl_action_t));

	ret = jl6107x_acl_rule_info_get(dev, index, &acl_rule, &acl_act);
	if (ret)
		return ret;

	if (!acl_act.update_counter_enable)
	{
		/*none change*/
		return JL_ERR_OK;
	}

	if (acl_rule.table_type == ACL_TBL_TCAM)
		ret = jl6107x_acl_stat_enable_tcam(dev, acl_rule.entry_idx, 0, enable);
	else if (acl_rule.table_type == ACL_TBL_LARGE)
		ret = jl6107x_acl_stat_enable_hash_large(dev, acl_rule.entry_idx, 0, enable);
	else
		ret = jl6107x_acl_stat_enable_hash_small(dev, acl_rule.entry_idx, 0, enable);

	if (ret)
		return ret;
	/*update rule info*/
	acl_act.update_counter_enable = FALSE;
	acl_act.counter = 0;

	ret = jl6107x_acl_rule_info_clear(dev, index);
	if (ret)
		return ret;

	ret = jl6107x_acl_rule_info_set(dev, index, &acl_rule, &acl_act);

	return ret;
}

jl_ret_t jl6107x_acl_stat_counter_get(jl_device_t *dev, jl_uint8 *counter)
{
	jl_uint8 idx = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(counter);

	for (; idx < ACL_MATCH_COUNTER; idx++)
	{
		if (gp_acl_rule_info[dev->id]->counter[idx] == 0)
		{
			*counter = idx;
			return JL_ERR_OK;
		}
	}

	return JL_ERR_RESOURCE;
}

static jl_ret_t jl6107x_acl_mac_action_chk(jl_device_t *dev, jl_acl_mac_action_t *mac_action)
{
	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(mac_action);

	if (mac_action->drop_enable > 1)
		return JL_ERR_PARAM;

	if (mac_action->send2cpu_enable > 1)
		return JL_ERR_PARAM;

	if (mac_action->force_queue_enable > 1)
		return JL_ERR_PARAM;

	if (mac_action->force_queue_enable && mac_action->e_queue >= ACL_QUEUE_NUM)
		return JL_ERR_PARAM;

	if (mac_action->force_color_enable > 1)
		return JL_ERR_PARAM;

	if (mac_action->force_color_enable && mac_action->color > ACL_COLOR_RED)
		return JL_ERR_PARAM;

	if (mac_action->mmp_valid > 1)
		return JL_ERR_PARAM;

	if (mac_action->mmp_valid &&
				(mac_action->mmp_order > 3 || mac_action->mmp_ptr >= ACL_MMP_POINTER_NUM))
		return JL_ERR_PARAM;

	JL_CHECK_PORTMASK(dev, mac_action->port_mask);

	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_mac_rule_add(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_mac_range_t *mac_range,
				jl_acl_mac_action_t *mac_action)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cportmask = 0;
	jl_uint64 val, vals = 0, vale = 0;
	jl_uint8 i;
	SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_t smac_tbl;
	SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_t dmac_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(mac_range);
	JL_CHECK_POINTER(mac_action);

	for (i = 0; i < 6; i++)
	{
		val = mac_range->mac_s.addr[i];
		val <<= ((5 - i) * 8);
		vals |= val;

		val = mac_range->mac_e.addr[i];
		val <<= ((5 - i) * 8);
		vale |= val;
	}

	if (vals > vale)
		return JL_ERR_CONFIG;

	ret = jl6107x_acl_mac_action_chk(dev, mac_action);
	if (ret)
		return ret;

	ret = jlxx_portmask_l2c(dev, mac_action->port_mask, &cportmask);
	if (ret)
		return ret;

	if (mac_range->mac_type == ACL_MAC_SOURCE)
	{
		memset(&smac_tbl, 0, sizeof(SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_t));

		smac_tbl.BF.start_addr_0_31 = vals & 0xffffffff;

		smac_tbl.BF.start_addr_32_47 = (vals >> SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_OFFSET_START_ADDR_0_31_WIDTH);

		smac_tbl.BF.stop_addr_0_15 = vale & 0xffff;

		smac_tbl.BF.stop_addr_16_47 = (vale >> SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_OFFSET_STOP_ADDR_0_15_WIDTH);

		smac_tbl.BF.drop_enable = mac_action->drop_enable;
		smac_tbl.BF.send_to_cpu = mac_action->send2cpu_enable;
		smac_tbl.BF.force_queue = mac_action->force_queue_enable;

		smac_tbl.BF.e_queue = mac_action->e_queue;
		smac_tbl.BF.force_color = mac_action->force_color_enable;
		smac_tbl.BF.color = mac_action->color;
		smac_tbl.BF.mmp_valid = mac_action->mmp_valid;
		smac_tbl.BF.mmp_ptr = mac_action->mmp_ptr;
		smac_tbl.BF.mmp_order = mac_action->mmp_order;
		smac_tbl.BF.enable = cportmask;

		REGISTER_WRITE(dev, SWCORE, RESERVED_SOURCE_MAC_ADDRESS_RANGE, smac_tbl, index, 0);

		if (gp_acl_rule_info[dev->id]->smac_rule[index].mmp_valid)
		{
			ret = jl6107x_clear_mmp_pointer_info(dev, JL_MMP_MODULE_SOURCE_MAC,
											gp_acl_rule_info[dev->id]->smac_rule[index].mmp_index);
			if (ret)
				return ret;
		}
		if (mac_action->mmp_valid)
		{
			ret = jl6107x_set_mmp_pointer_info(dev, JL_MMP_MODULE_SOURCE_MAC, mac_action->mmp_ptr);
			if (ret)
				return ret;

			gp_acl_rule_info[dev->id]->smac_rule[index].mmp_index = mac_action->mmp_ptr;
		}
		gp_acl_rule_info[dev->id]->smac_rule[index].mmp_valid = mac_action->mmp_valid;
	}
	else if (mac_range->mac_type == ACL_MAC_DEST)
	{
		memset(&dmac_tbl, 0, sizeof(SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_t));

		dmac_tbl.BF.start_addr_0_31 = vals & 0xffffffff;

		dmac_tbl.BF.start_addr_32_47 =
				(vals >> SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_OFFSET_START_ADDR_0_31_WIDTH);

		dmac_tbl.BF.stop_addr_0_15 = vale & 0xffff;

		dmac_tbl.BF.stop_addr_16_47 =
				(vale >> SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_OFFSET_STOP_ADDR_0_15_WIDTH);

		dmac_tbl.BF.drop_enable = mac_action->drop_enable;
		dmac_tbl.BF.send_to_cpu = mac_action->send2cpu_enable;
		dmac_tbl.BF.force_queue = mac_action->force_queue_enable;

		dmac_tbl.BF.e_queue = mac_action->e_queue;
		dmac_tbl.BF.force_color = mac_action->force_color_enable;
		dmac_tbl.BF.color = mac_action->color;
		dmac_tbl.BF.mmp_valid = mac_action->mmp_valid;
		dmac_tbl.BF.mmp_ptr = mac_action->mmp_ptr;
		dmac_tbl.BF.mmp_order = mac_action->mmp_order;
		dmac_tbl.BF.enable = cportmask;

		REGISTER_WRITE(dev, SWCORE, RESERVED_DESTINATION_MAC_ADDRESS_RANGE, dmac_tbl, index, 0);
		
		if (gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_valid)
		{
			ret = jl6107x_clear_mmp_pointer_info(dev, JL_MMP_MODULE_DEST_MAC,
											gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_index);
			if (ret)
				return ret;
		}
		if (mac_action->mmp_valid)
		{
			ret = jl6107x_set_mmp_pointer_info(dev, JL_MMP_MODULE_DEST_MAC, mac_action->mmp_ptr);
			if (ret)
				return ret;

			gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_index = mac_action->mmp_ptr;
		}
		gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_valid = mac_action->mmp_valid;
	}
	else
		return JL_ERR_PARAM;

	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_mac_rule_del(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_mac_type_t mac_type)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_t smac_tbl;
	SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_t dmac_tbl;

	JL_CHECK_POINTER(dev);

	if (mac_type == ACL_MAC_SOURCE)
	{
		memset(&smac_tbl, 0, sizeof(SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_t));
		REGISTER_WRITE(dev, SWCORE, RESERVED_SOURCE_MAC_ADDRESS_RANGE, smac_tbl,index, 0);

		if (gp_acl_rule_info[dev->id]->smac_rule[index].mmp_valid)
		{
			ret = jl6107x_set_mmp_pointer_info(dev, JL_MMP_MODULE_SOURCE_MAC,
										gp_acl_rule_info[dev->id]->smac_rule[index].mmp_index);
			if (ret)
				return ret;

			gp_acl_rule_info[dev->id]->smac_rule[index].mmp_valid = 0;
		}
	}
	else
	{
		memset(&dmac_tbl, 0, sizeof(SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_t));
		REGISTER_WRITE(dev, SWCORE, RESERVED_DESTINATION_MAC_ADDRESS_RANGE, dmac_tbl,index, 0);

		if (gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_valid)
		{
			ret = jl6107x_set_mmp_pointer_info(dev, JL_MMP_MODULE_DEST_MAC,
										gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_index);
			if (ret)
				return ret;

			gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_valid = 0;
		}
	}
	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_mac_rule_get(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_mac_type_t mac_type,
				jl_acl_mac_range_t *mac_range,
				jl_acl_mac_action_t *mac_action)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 value = 0, vals = 0,vale = 0;
	jl_uint32 lportmask = 0;
	jl_uint8 i = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(mac_range);
	JL_CHECK_POINTER(mac_action);

	if (mac_type == ACL_MAC_SOURCE)
	{
		REGISTER_READ(dev, SWCORE, RESERVED_SOURCE_MAC_ADDRESS_RANGE, smac_tbl, index, INDEX_ZERO);

		vals = smac_tbl.BF.start_addr_0_31;
		value = smac_tbl.BF.start_addr_32_47;
		value <<= SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_OFFSET_START_ADDR_0_31_WIDTH;
		vals |= value;

		vale = smac_tbl.BF.stop_addr_0_15;
		value = smac_tbl.BF.stop_addr_16_47;
		value <<= SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_OFFSET_STOP_ADDR_0_15_WIDTH;
		vale |= value;

		mac_action->drop_enable = smac_tbl.BF.drop_enable;
		mac_action->send2cpu_enable = smac_tbl.BF.send_to_cpu;
		mac_action->force_queue_enable = smac_tbl.BF.force_queue;
		mac_action->e_queue = smac_tbl.BF.e_queue;
		mac_action->force_color_enable = smac_tbl.BF.force_color;
		mac_action->color = smac_tbl.BF.color;
		mac_action->mmp_valid = smac_tbl.BF.mmp_valid;
		mac_action->mmp_ptr = smac_tbl.BF.mmp_ptr;
		mac_action->mmp_order = smac_tbl.BF.mmp_order;

		ret = jlxx_portmask_c2l(dev, smac_tbl.BF.enable, &lportmask);
		if (ret)
			return ret;

		mac_action->port_mask = lportmask;
	}
	else if (mac_type == ACL_MAC_DEST)
	{
		REGISTER_READ(dev, SWCORE, RESERVED_DESTINATION_MAC_ADDRESS_RANGE, dmac_tbl, index, INDEX_ZERO);

		vals = dmac_tbl.BF.start_addr_0_31;
		value = dmac_tbl.BF.start_addr_32_47;
		value <<= SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_OFFSET_START_ADDR_0_31_WIDTH;
		vals |= value;

		vale = dmac_tbl.BF.stop_addr_0_15;
		value = dmac_tbl.BF.stop_addr_16_47;
		value <<= SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_OFFSET_STOP_ADDR_0_15_WIDTH;
		vale |= value;

		mac_action->drop_enable = dmac_tbl.BF.drop_enable;
		mac_action->send2cpu_enable = dmac_tbl.BF.send_to_cpu;
		mac_action->force_queue_enable = dmac_tbl.BF.force_queue;
		mac_action->e_queue = dmac_tbl.BF.e_queue;
		mac_action->force_color_enable = dmac_tbl.BF.force_color;
		mac_action->color = dmac_tbl.BF.color;
		mac_action->mmp_valid = dmac_tbl.BF.mmp_valid;
		mac_action->mmp_ptr = dmac_tbl.BF.mmp_ptr;
		mac_action->mmp_order = dmac_tbl.BF.mmp_order;

		ret = jlxx_portmask_c2l(dev, dmac_tbl.BF.enable, &lportmask);
		if (ret)
			return ret;

		mac_action->port_mask = lportmask;
	}
	else
		return JL_ERR_PARAM;

	mac_range->mac_type = mac_type;

	for (i = 0; i < 6; i++)
	{
		mac_range->mac_s.addr[i] = (vals >> ((5 - i) * 8)) & 0xff;
		mac_range->mac_e.addr[i] = (vale >> ((5 - i) * 8)) & 0xff;
	}

	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_source_port_set(jl_device_t *dev,
					jl_uint8 lport,
					jl_acl_port_t *port_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(port_state);
	JL_CHECK_PORT(dev, lport);

	if (port_state->valid == FALSE)
		return JL_ERR_OK;

	cport = jlxx_port_l2c(dev, lport);

	REGISTER_READ(dev, SWCORE, SOURCE_PORT_TABLE, port_tbl, cport, 0);

	if (port_state->enable == DISABLED)
	{
		port_tbl.BF.use_acl = 0;
		port_tbl.BF.acl_rule = 0;
		port_tbl.BF.acl_rule_icmp = 0;
		port_tbl.BF.acl_rule_icmpv6 = 0;
		port_tbl.BF.acl_rule_igmp = 0;
	}
	else
	{
		if (port_state->acl_tpl_type >= ACL_TEMPLATE_NUM ||
			port_state->icmp_tpl_type >= ACL_TEMPLATE_NUM ||
			port_state->icmpv6_tpl_type >= ACL_TEMPLATE_NUM ||
			port_state->igmp_tpl_type >= ACL_TEMPLATE_NUM)
		{
			return JL_ERR_PARAM;
		}

		port_tbl.BF.use_acl = 1;
		port_tbl.BF.acl_rule = port_state->acl_tpl_type;
		port_tbl.BF.acl_rule_icmp = port_state->icmp_tpl_type;
		port_tbl.BF.acl_rule_icmpv6 = port_state->icmpv6_tpl_type;
		port_tbl.BF.acl_rule_igmp = port_state->igmp_tpl_type;
	}

	REGISTER_WRITE(dev, SWCORE, SOURCE_PORT_TABLE, port_tbl, cport, 0);

	return JL_ERR_OK;
}


jl_ret_t jl6107x_acl_source_port_get(jl_device_t *dev,
					jl_uint8 lport,
					jl_acl_port_t *port_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(port_state);
	JL_CHECK_PORT(dev, lport);

	cport = jlxx_port_l2c(dev, lport);

	REGISTER_READ(dev, SWCORE, SOURCE_PORT_TABLE, port_tbl, cport, 0);

	memset(port_state, 0, sizeof(jl_acl_port_t));

	if (port_tbl.BF.use_acl == 0)
		port_state->enable = DISABLED;
	else
		port_state->enable = ENABLED;

	port_state->acl_tpl_type = port_tbl.BF.acl_rule;
	port_state->icmp_tpl_type = port_tbl.BF.acl_rule_icmp;
	port_state->icmpv6_tpl_type = port_tbl.BF.acl_rule_icmpv6;
	port_state->igmp_tpl_type= port_tbl.BF.acl_rule_igmp;
	port_state->valid = TRUE;

	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_init(jl_device_t *dev)
{
	JL_CHECK_POINTER(dev);

	if (gp_acl_template[dev->id] == NULL)
	{
		gp_acl_template[dev->id] = (jl6107x_acl_template_t*)port_mem_malloc(sizeof(jl6107x_acl_template_t));

		if (!gp_acl_template[dev->id])
			return JL_ERR_MEMORY;

		memset((void*)gp_acl_template[dev->id], 0, sizeof(jl6107x_acl_template_t));
	}

	if (gp_acl_rule_info[dev->id] == NULL)
	{
		gp_acl_rule_info[dev->id] = (jl6107x_acl_rule_info_t*)port_mem_malloc(sizeof(jl6107x_acl_rule_info_t));

		if (!gp_acl_rule_info[dev->id])
			return JL_ERR_MEMORY;

		memset((void*)gp_acl_rule_info[dev->id], 0, sizeof(jl6107x_acl_rule_info_t));
	}

	return JL_ERR_OK;
}

jl_ret_t jl6107x_acl_deinit(jl_device_t *dev)
{
	JL_CHECK_POINTER(dev);

	if (gp_acl_template[dev->id])
		port_mem_free(gp_acl_template[dev->id]);

	gp_acl_template[dev->id] = NULL;

	if (gp_acl_rule_info[dev->id])
		port_mem_free(gp_acl_rule_info[dev->id]);

	gp_acl_rule_info[dev->id] = NULL;

	return JL_ERR_OK;
}
