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

#include "jl_base.h"
#include "jl_ops.h"
#include "jl_version.h"
#include "jl6107x/jl6107x_drv_switch.h"
#include "jl6107x/jl6107x_chip.h"
#include "portable/jl_portable.h"
#ifdef CONFIG_JL6107X_DRV_RWREG
#include "jl6107x/jl6107x_drv_rwreg.h"
#endif
#ifdef CONFIG_JL6107X_DRV_PORT
#include "jl6107x/jl6107x_drv_port.h"
#endif
#ifdef CONFIG_JL6107X_DRV_MIB
#include "jl6107x/jl6107x_drv_mib.h"
#endif
#ifdef CONFIG_JL6107X_DRV_LPD
#include "jl6107x/jl6107x_drv_lpd.h"
#endif
#ifdef CONFIG_JL6107X_DRV_PTI
#include "jl6107x/jl6107x_drv_pti.h"
#endif
#ifdef CONFIG_JL6107X_DRV_STP
#include "jl6107x/jl6107x_drv_stp.h"
#endif
#ifdef CONFIG_JL6107X_DRV_STORM
#include "jl6107x/jl6107x_drv_storm.h"
#endif
#ifdef CONFIG_JL6107X_DRV_L2
#include "jl6107x/jl6107x_drv_l2.h"
#endif
#ifdef CONFIG_JL6107X_DRV_VLAN
#include "jl6107x/jl6107x_drv_vlan.h"
#endif
#ifdef CONFIG_JL6107X_DRV_CPU
#include "jl6107x/jl6107x_drv_cpu.h"
#endif
#ifdef CONFIG_JL6107X_DRV_FC
#include "jl6107x/jl6107x_drv_fc.h"
#endif
#ifdef CONFIG_JL6107X_DRV_ACL
#include "jl6107x/jl6107x_drv_acl.h"
#endif
#ifdef CONFIG_JL6107X_DRV_LED
#include "jl6107x/jl6107x_drv_led.h"
#endif
#ifdef CONFIG_JL6107X_DRV_MIRROR
#include "jl6107x/jl6107x_drv_mirror.h"
#endif
#ifdef CONFIG_JL6107X_DRV_TRUNK
#include "jl6107x/jl6107x_drv_trunk.h"
#endif
#ifdef CONFIG_JL6107X_DRV_QOS
#include "jl6107x/jl6107x_drv_qos.h"
#endif

/*macro define*/

jl_uint32 g_tick_freq_list_6107[JL_SYSTIME_MAX_STEP] = { 1000000, 100000, 10000, 1000, 100 };
static jl_mmp_pointer_t *gp_mmp_ptr[JL_MAX_CHIP_NUM] = {0};

struct jl_driver_s g_drv_6107x = {
	.compat_id = JL_CHIP_6107X,
	.name = "jl6107x-driver",
	.probe = jl6107x_chip_probe,
	.remove = jl6107x_chip_remove,
	.drv_data = NULL
};

static jl_ret_t __ops_regist(jl_driver_t *driver)
{
	jl_ops_t *ops = NULL;

	ops = (jl_ops_t *) port_mem_malloc(sizeof(jl_ops_t));
	if (ops == NULL)
		return JL_ERR_MEMORY;

	ops->switch_info = jl6107x_switch_info;
	ops->lport_check = jlxx_lport_check;
	ops->utpport_check = jlxx_utpport_check;
	ops->extport_check = jlxx_extport_check;
	ops->cpuport_check = jlxx_cpuport_check;
	ops->port_l2c_get = jlxx_port_l2c;
	ops->port_c2l_get = jlxx_port_c2l;
	ops->lportmask_get = jlxx_lportmask_get;
	ops->cportmask_get = jlxx_cportmask_get;
	ops->portmask_l2c_get = jlxx_portmask_l2c;
	ops->portmask_c2l_get = jlxx_portmask_c2l;
	ops->show_switchport_info = jl6107x_show_switchport_info;
#ifdef CONFIG_JL6107X_DRV_RWREG
	ops->reg_io_write = jl6107x_reg_io_write;
	ops->reg_io_read = jl6107x_reg_io_read;
	ops->reg_io_burst_write = jl6107x_reg_io_burst_write;
	ops->reg_io_burst_read = jl6107x_reg_io_burst_read;
#endif
#ifdef CONFIG_JL6107X_DRV_PORT
	ops->port_phy_autoneg_ability_set = jl6107x_port_phy_autoneg_ability_set;
	ops->port_phy_autoneg_ability_get = jl6107x_port_phy_autoneg_ability_get;
	ops->port_phy_force_ability_set = jl6107x_port_phy_force_ability_set;
	ops->port_phy_force_ability_get = jl6107x_port_phy_force_ability_get;
	ops->port_phy_status_get = jl6107x_port_phy_status_get;
	ops->port_phy_isolate_set = jl6107x_port_phy_isolate_set;
	ops->port_phy_isolate_get = jl6107x_port_phy_isolate_get;
	ops->port_phy_enable_set = jl6107x_port_phy_enable_set;
	ops->port_phy_enable_get = jl6107x_port_phy_enable_get;
	ops->port_phy_all_enable_set = jl6107x_port_phy_all_enable_set;
	ops->port_mac_force_link_set = jl6107x_port_mac_force_link_set;
	ops->port_mac_force_link_get = jl6107x_port_mac_force_link_get;
	ops->port_mac_force_link_ext_set = jl6107x_port_mac_force_link_ext_set;
	ops->port_mac_force_link_ext_get = jl6107x_port_mac_force_link_ext_get;
	ops->port_mac_status_get = jl6107x_port_mac_status_get;
	ops->port_mac_loopback_set = jl6107x_port_mac_loopback_set;
	ops->port_mac_loopback_get = jl6107x_port_mac_loopback_get;
	ops->port_fiber_ability_set = jl6107x_port_fiber_ability_set;
	ops->port_fiber_ability_get = jl6107x_port_fiber_ability_get;
	ops->port_sds_reset = jl6107x_port_sds_reset;
#endif
#ifdef CONFIG_JL6107X_DRV_MIB
	ops->mib_port_rc_mode_get = jl6107x_mib_port_rc_mode_get;
	ops->mib_port_rc_mode_set = jl6107x_mib_port_rc_mode_set;
	ops->mib_port_clear = jl6107x_mib_port_clear;
	ops->mib_port_clear_all = jl6107x_mib_port_clear_all;
	ops->mib_port_get = jl6107x_mib_port_get;
	ops->mib_port_get_all = jl6107x_mib_port_get_all;
	ops->mib_swc_global_drop_get = jl6107x_mib_swc_global_drop_get;
	ops->mib_swc_global_drop_clear = jl6107x_mib_swc_global_drop_clear;
	ops->mib_swc_port_drop_get = jl6107x_mib_swc_port_drop_get;
	ops->mib_swc_port_drop_clear = jl6107x_mib_swc_port_drop_clear;
#endif
#ifdef CONFIG_JL6107X_DRV_LPD
	ops->lpd_config_get = jl6107x_lpd_config_get;
	ops->lpd_config_set = jl6107x_lpd_config_set;
	ops->lpd_enable_set = jl6107x_lpd_enable_set;
	ops->lpd_enable_get = jl6107x_lpd_enable_get;
	ops->lpd_port_enable_set = jl6107x_lpd_port_enable_set;
	ops->lpd_port_enable_get = jl6107x_lpd_port_enable_get;
	ops->lpd_port_status_get = jl6107x_lpd_port_status_get;
	ops->lpd_status_history_get = jl6107x_lpd_status_history_get;
#endif
#ifdef CONFIG_JL6107X_DRV_PTI
	ops->port_isolation_dst_set = jl6107x_port_isolation_dst_set;
	ops->port_isolation_dst_get = jl6107x_port_isolation_dst_get;
	ops->port_isolation_src_set = jl6107x_port_isolation_src_set;
	ops->port_isolation_src_get = jl6107x_port_isolation_src_get;
	ops->port_isolation_group_set = jl6107x_port_isolation_group_set;
	ops->port_isolation_group_get = jl6107x_port_isolation_group_get;
	ops->port_isolation_group_clear = jl6107x_port_isolation_group_clear;
#endif
#ifdef CONFIG_JL6107X_DRV_STP
	ops->stp_port_ingress_state_set = jl6107x_stp_port_ingress_state_set;
	ops->stp_port_ingress_state_get = jl6107x_stp_port_ingress_state_get;
	ops->stp_port_egress_state_set = jl6107x_stp_port_egress_state_set;
	ops->stp_port_egress_state_get = jl6107x_stp_port_egress_state_get;
	ops->stp_ingress_state_drop_get = jl6107x_stp_ingress_state_drop_get;
	ops->stp_ingress_state_drop_clear = jl6107x_stp_ingress_state_drop_clear;
	ops->stp_port_egress_drop_get = jl6107x_stp_port_egress_drop_get;
	ops->stp_port_egress_drop_clear = jl6107x_stp_port_egress_drop_clear;
	ops->mstp_port_ingress_state_set = jl6107x_mstp_port_ingress_state_set;
	ops->mstp_port_ingress_state_get = jl6107x_mstp_port_ingress_state_get;
	ops->mstp_port_egress_state_set = jl6107x_mstp_port_egress_state_set;
	ops->mstp_port_egress_state_get = jl6107x_mstp_port_egress_state_get;
	ops->mstp_ingress_state_drop_get = jl6107x_mstp_ingress_state_drop_get;
	ops->mstp_ingress_state_drop_clear = jl6107x_mstp_ingress_state_drop_clear;
	ops->mstp_port_egress_drop_get = jl6107x_mstp_port_egress_drop_get;
	ops->mstp_port_egress_drop_clear = jl6107x_mstp_port_egress_drop_clear;
	ops->mstp_vlan_msti_set = jl6107x_mstp_vlan_msti_set;
	ops->mstp_vlan_msti_get = jl6107x_mstp_vlan_msti_get;
#endif
#ifdef CONFIG_JL6107X_DRV_STORM
	ops->stormctrl_enable_set = jl6107x_stormctrl_enable_set;
	ops->stormctrl_enable_get = jl6107x_stormctrl_enable_get;
	ops->stormctrl_rate_set = jl6107x_stormctrl_rate_set;
	ops->stormctrl_rate_get = jl6107x_stormctrl_rate_get;
	ops->stormctrl_drop_counter_get = NULL;
#endif
#ifdef CONFIG_JL6107X_DRV_CPU
	ops->cpu_allow_bpdu_enable_set = jl6107x_cpu_allow_bpdu_enable_set,
	ops->cpu_allow_bpdu_enable_get = jl6107x_cpu_allow_bpdu_enable_get,
	ops->cpu_allow_rst_bpdu_enable_set = jl6107x_cpu_allow_rst_bpdu_enable_set,
	ops->cpu_allow_rst_bpdu_enable_get = jl6107x_cpu_allow_rst_bpdu_enable_get,
	ops->cpu_tag_enable_set = jl6107x_cpu_tag_enable_set,
	ops->cpu_tag_enable_get = jl6107x_cpu_tag_enable_get,
	ops->cpu_allow_lldp_enable_set = jl6107x_cpu_allow_lldp_enable_set,
	ops->cpu_allow_lldp_enable_get = jl6107x_cpu_allow_lldp_enable_get,
	ops->cpu_rma_action_set = jl6107x_cpu_rma_action_set,
	ops->cpu_rma_action_get = jl6107x_cpu_rma_action_get,
	ops->cpu_cpumac_set = jl6107x_cpu_cpumac_set,
	ops->cpu_cpumac_get = jl6107x_cpu_cpumac_get,
	ops->cpu_detection_cpumac_enable_set = jl6107x_cpu_detection_cpumac_enable_set,
	ops->cpu_detection_cpumac_enable_get = jl6107x_cpu_detection_cpumac_enable_get,
#endif
#ifdef CONFIG_JL6107X_DRV_FC
	ops->taildrop_enable_set = jl6107x_taildrop_enable_set,
	ops->taildrop_enable_get = jl6107x_taildrop_enable_get,
	ops->taildrop_counter_get = jl6107x_ingress_resource_manager_drop_counter_get,
	ops->port_fc_force_mode_set = jl6107x_port_fc_force_mode_set,
	ops->port_fc_force_mode_get = jl6107x_port_fc_force_mode_get,
	ops->port_fc_force_enable_set = jl6107x_port_fc_force_enable_set,
	ops->port_fc_force_enable_get = jl6107x_port_fc_force_enable_get,
	ops->port_fc_threshold_set = jl6107x_port_fc_threshold_set,
	ops->port_fc_threshold_get = jl6107x_port_fc_threshold_get,
#endif
#ifdef CONFIG_JL6107X_DRV_L2
	ops->l2_init = jl6107x_l2_init;
	ops->l2_deinit = jl6107x_l2_deinit;
	ops->l2_table_set = jl6107x_l2_table_set;
	ops->l2_table_search = jl6107x_l2_table_search;
	ops->l2_table_clear = jl6107x_l2_table_clear;
	ops->l2_dynamic_fast_clear = NULL;
	ops->l2_hash_col_table_set = jl6107x_l2_hash_col_table_set;
	ops->l2_hash_col_table_get = jl6107x_l2_hash_col_table_get;
	ops->l2_set_agetime = jl6107x_l2_set_agetime;
	ops->l2_get_agetime = jl6107x_l2_get_agetime;
	ops->l2_mc_table_set = jl6107x_l2_mc_table_set;
	ops->l2_mc_table_get = jl6107x_l2_mc_table_get;
	ops->l2_mc_handle_set = jl6107x_l2_mc_handle_set;
#endif
#ifdef CONFIG_JL6107X_DRV_ACL
	ops->acl_template_set = jl6107x_acl_template_set;
	ops->acl_template_get = jl6107x_acl_template_get;
	ops->acl_rule_add = jl6107x_acl_rule_add;
	ops->acl_rule_get = jl6107x_acl_rule_get;
	ops->acl_rule_del = jl6107x_acl_rule_del;
	ops->acl_field_insert = jl6107x_acl_field_insert;
	ops->acl_field_clear = jl6107x_acl_field_clear;
	ops->acl_selection_set = jl6107x_acl_selection_set;
	ops->acl_selection_get = jl6107x_acl_selection_get;
	ops->acl_stat_enable = jl6107x_acl_stat_enable;
	ops->acl_stat_disable = jl6107x_acl_stat_disable;
	ops->acl_stat_read = jl6107x_acl_stat_read;
	ops->acl_stat_write = jl6107x_acl_stat_write;
	ops->acl_stat_counter_get = jl6107x_acl_stat_counter_get;
	ops->acl_mac_rule_add = jl6107x_acl_mac_rule_add;
	ops->acl_mac_rule_get = jl6107x_acl_mac_rule_get;
	ops->acl_mac_rule_del = jl6107x_acl_mac_rule_del;
	ops->acl_source_port_set = jl6107x_acl_source_port_set;
	ops->acl_source_port_get = jl6107x_acl_source_port_get;
#endif
#ifdef CONFIG_JL6107X_DRV_LED
	ops->led_enable_set = jl6107x_led_enable_set;
	ops->led_enable_get = jl6107x_led_enable_get;
	ops->led_group_config_set = jl6107x_led_group_config_set;
	ops->led_group_config_get = jl6107x_led_group_config_get;
	ops->led_blink_rate_set = jl6107x_led_blink_rate_set;
	ops->led_blink_rate_get = jl6107x_led_blink_rate_get;
#endif
#ifdef CONFIG_JL6107X_DRV_MIRROR
	ops->mirror_input_mr_get = jl6107x_mirror_input_mr_get;
	ops->mirror_input_mr_set = jl6107x_mirror_input_mr_set;
	ops->mirror_output_mr_set = jl6107x_mirror_output_mr_set;
	ops->mirror_output_mr_get = jl6107x_mirror_output_mr_get;
	ops->mirror_input_mr_clear = jl6107x_mirror_input_mr_clear;
	ops->mirror_output_mr_clear = jl6107x_mirror_output_mr_clear;
#endif
#ifdef CONFIG_JL6107X_DRV_TRUNK
	ops->link_aggregation_enable_set = jl6107x_link_aggregation_enable_set;
	ops->link_aggregation_enable_get = jl6107x_link_aggregation_enable_get;
	ops->link_aggregation_portmask_set = jl6107x_link_aggregation_portmask_set;
	ops->link_aggregation_portmask_get = jl6107x_link_aggregation_portmask_get;
	ops->link_aggregation_port_del = jl6107x_link_aggregation_port_del;
	ops->link_aggregation_group_del = jl6107x_link_aggregation_group_del;
	ops->link_aggregation_hashfactor_set = jl6107x_link_aggregation_hashfactor_set;
	ops->link_aggregation_hashfactor_get = jl6107x_link_aggregation_hashfactor_get;
	ops->link_aggregation_weight_set = jl6107x_link_aggregation_hash_map_set;
	ops->link_aggregation_weight_get = jl6107x_link_aggregation_hash_map_get;
	ops->link_aggregation_weight_clear = jl6107x_link_aggregation_hash_map_clear;
#endif
#ifdef CONFIG_JL6107X_DRV_VLAN
	ops->vlan_init = jl6107x_vlan_init;
	ops->vlan_deinit = jl6107x_vlan_deinit;
	ops->vlan_ingressport_mode_set = jl6107x_vlan_ingressport_mode_set;
	ops->vlan_ingressport_default_set = jl6107x_vlan_ingressport_default_set;
	ops->vlan_egressport_mode_set = jl6107x_vlan_egressport_mode_set;
	ops->vlan_egressport_default_set = jl6107x_vlan_egressport_default_set;
	ops->vlan_preprocess_mac_set = jl6107x_vlan_preprocess_mac_set;
	ops->vlan_preprocess_mac_get = jl6107x_vlan_preprocess_mac_get;
	ops->vlan_preprocess_outvid_set = jl6107x_vlan_preprocess_outvid_set;
	ops->vlan_preprocess_outvid_get = jl6107x_vlan_preprocess_outvid_get;
	ops->vlan_preprocess_invid_set = jl6107x_vlan_preprocess_invid_set;
	ops->vlan_preprocess_invid_get = jl6107x_vlan_preprocess_invid_get;
	ops->vlan_preprocess_ethtype_set = jl6107x_vlan_preprocess_ethtype_set;
	ops->vlan_preprocess_ethtype_get = jl6107x_vlan_preprocess_ethtype_get;
	ops->vlan_tbl_set = jl6107x_vlan_tbl_set;
	ops->vlan_tbl_get = jl6107x_vlan_tbl_get;
	ops->vlan_tbl_del = jl6107x_vlan_tbl_del;
	ops->vlan_tbl_set_fid = jl6107x_vlan_tbl_set_fid;
	ops->vlan_tbl_get_fid = jl6107x_vlan_tbl_get_fid;
	ops->vlan_tbl_set_memberPortmsk = jl6107x_vlan_tbl_set_memberPortmsk;
	ops->vlan_tbl_get_memberPortmsk = jl6107x_vlan_tbl_get_memberPortmsk;
	ops->vlan_set_operation_selection = jl6107x_vlan_set_operation_selection;
	ops->vlan_get_operation_selection = jl6107x_vlan_get_operation_selection;
	ops->vlan_set_tpid_val = jl6107x_vlan_set_tpid_val;
	ops->vlan_get_tpid_val = jl6107x_vlan_get_tpid_val;
#endif
#ifdef CONFIG_JL6107X_DRV_QOS
	ops->qos_nonvlan_map2q_set = jl6107x_qos_map_2queue_nonvlan_set;
	ops->qos_nonvlan_map2q_get = jl6107x_qos_map_2queue_nonvlan_get;
	ops->qos_unknownl3_map2q_set = jl6107x_qos_map_2queue_unknown_set;
	ops->qos_unknownl3_map2q_get = jl6107x_qos_map_2queue_unknown_get;
	ops->qos_mac_map2q_set = jl6107x_qos_map_2queue_mac_set;
	ops->qos_mac_map2q_get = jl6107x_qos_map_2queue_mac_get;
	ops->qos_vid_map2q_set = jl6107x_qos_map_2queue_vid_set;
	ops->qos_vid_map2q_get = jl6107x_qos_map_2queue_vid_get;
	ops->qos_ip_map2q_set = jl6107x_qos_map_2queue_ip_set;
	ops->qos_ip_map2q_get = jl6107x_qos_map_2queue_ip_get;
	ops->qos_ethtype_map2q_set = jl6107x_qos_map_2queue_ethtype_set;
	ops->qos_ethtype_map2q_get = jl6107x_qos_map_2queue_ethtype_get;
	ops->qos_tos_map2q_set = jl6107x_qos_map_2queue_tos_set;
	ops->qos_tos_map2q_get = jl6107x_qos_map_2queue_tos_get;
	ops->qos_exp_map2q_set = jl6107x_qos_map_2queue_exp_set;
	ops->qos_exp_map2q_get = jl6107x_qos_map_2queue_exp_get;
	ops->qos_pcp_map2q_set = jl6107x_qos_map_2queue_pcp_set;
	ops->qos_pcp_map2q_get = jl6107x_qos_map_2queue_pcp_get;
	ops->qos_l4port_map2q_set = jl6107x_qos_map_2queue_l4port_range_set;
	ops->qos_l4port_map2q_get = jl6107x_qos_map_2queue_l4port_range_get;
	ops->qos_l4protocol_map2q_set = jl6107x_qos_map_2queue_l4_protocol_set;
	ops->qos_l4protocol_map2q_get = jl6107x_qos_map_2queue_l4_protocol_get;
	ops->qos_nonvlan_map2color_set = jl6107x_qos_map_2color_nonvlan_set;
	ops->qos_nonvlan_map2color_get = jl6107x_qos_map_2color_nonvlan_get;
	ops->qos_unknownl3_map2color_set = jl6107x_qos_map_2color_unknown_set;
	ops->qos_unknownl3_map2color_get = jl6107x_qos_map_2color_unknown_get;
	ops->qos_tos_map2color_set = jl6107x_qos_map_2color_tos_set;
	ops->qos_tos_map2color_get = jl6107x_qos_map_2color_tos_get;
	ops->qos_exp_map2color_set = jl6107x_qos_map_2color_exp_set;
	ops->qos_exp_map2color_get = jl6107x_qos_map_2color_exp_get;
	ops->qos_pcpdei_map2color_set = jl6107x_qos_map_2color_pcpdei_set;
	ops->qos_pcpdei_map2color_get = jl6107x_qos_map_2color_pcpdei_get;
	ops->qos_remap_vlan_set = jl6107x_qos_remap_vlan_set;
	ops->qos_remap_vlan_get = jl6107x_qos_remap_vlan_get;
	ops->qos_color_remap_set = jl6107x_qos_color_remap_set;
	ops->qos_color_remap_get = jl6107x_qos_color_remap_get;
	ops->qos_mmp_cfg_set = jl6107x_qos_mmp_cfg_set;
	ops->qos_mmp_cfg_get = jl6107x_qos_mmp_cfg_get;
	ops->qos_mmp_unused_pointer_get = jl6107x_qos_mmp_unused_pointer_get;
	ops->qos_mmp_used_pointer_info_get = jl6107x_qos_mmp_used_pointer_info_get;
	ops->qos_ingress_initial_mmp_set = jl6107x_qos_ingress_initial_mmp_set;
	ops->qos_ingress_initial_mmp_get = jl6107x_qos_ingress_initial_mmp_get;
	ops->qos_queue_map_priority_set = jl6107x_qos_queue_map_trust_l3_set;
	ops->qos_queue_map_priority_get = jl6107x_qos_queue_map_trust_l3_get;
	ops->qos_color_map_priority_set = jl6107x_qos_color_map_trust_l3_set;
	ops->qos_color_map_priority_get = jl6107x_qos_color_map_trust_l3_get;
	ops->qos_priority_to_queue_set = jl6107x_qos_queue_priority_set;
	ops->qos_priority_to_queue_get = jl6107x_qos_queue_priority_get;
	ops->qos_queue_shaper_set = jl6107x_qos_queue_shaper_set;
	ops->qos_queue_shaper_get = jl6107x_qos_queue_shaper_get;
	ops->qos_priority_shaper_set = jl6107x_qos_prio_shaper_set;
	ops->qos_priority_shaper_get = jl6107x_qos_prio_shaper_get;
	ops->qos_port_shaper_set = jl6107x_qos_port_shaper_set;
	ops->qos_port_shaper_get = jl6107x_qos_port_shaper_get;
	ops->qos_queue_dwrr_set = jl6107x_qos_queue_dwrr_set;
	ops->qos_queue_dwrr_get = jl6107x_qos_queue_dwrr_get;
	ops->qos_enqueue_enable_set = jl6107x_qos_enqueue_enable_set;
	ops->qos_enqueue_enable_get = jl6107x_qos_enqueue_enable_get;
	ops->qos_schedule_set = jl6107x_qos_schedule_set;
	ops->qos_schedule_get = jl6107x_qos_schedule_get;
	ops->qos_res_init = jl6107x_qos_resouce_init;
	ops->qos_res_deinit = jl6107x_qos_resouce_deinit;
#endif
	driver->ops = ops;

	return JL_ERR_OK;
}

static void __ops_clear(jl_driver_t *driver)
{
	port_mem_free(driver->ops);
	driver->ops = NULL;
}


jl_ret_t jl6107x_get_unused_mmp_pointer(jl_device_t *device, jl_uint8 *pptr)
{
	jl_uint8 idx = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pptr);

	for (; idx < JL_MMP_POINTER_NUM; idx++)
	{
		if (!gp_mmp_ptr[device->id]->used_info[idx].is_used)
		{
			*pptr = idx;
			return JL_ERR_OK;
		}
	}
	return JL_ERR_UNAVAIL;
}

jl_ret_t jl6107x_set_mmp_pointer_info(jl_device_t *device,
								jl_uint8 module,
								jl_uint8 mmp_ptr)
{
	jl_mmp_used_info_t* pinfo = NULL;

	JL_CHECK_POINTER(device);

	if (mmp_ptr >= JL_MMP_POINTER_NUM)
		return JL_ERR_PARAM;

	pinfo = &(gp_mmp_ptr[device->id]->used_info[mmp_ptr]);

	switch(module)
	{
		case JL_MMP_MODULE_INITIAL_ADM:
			pinfo->adm_used_num++;
			break;
		case JL_MMP_MODULE_SOURCE_MAC:
			pinfo->smac_used_num++;
			break;
		case JL_MMP_MODULE_DEST_MAC:
			pinfo->dmac_used_num++;
			break;
		case JL_MMP_MODULE_ACL:
			pinfo->acl_used_num++;
			break;
		case JL_MMP_MODULE_VID:
			pinfo->vlan_used_num++;
			break;
		default:
			return JL_ERR_PARAM;
	}
	pinfo->is_used = TRUE;

	return JL_ERR_OK;
}

jl_ret_t jl6107x_get_mmp_pointer_info(jl_device_t *device,
								jl_uint8 mmp_ptr,
								jl_mmp_used_info_t *pptr_info)
{
	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pptr_info);

	if (mmp_ptr >= JL_MMP_POINTER_NUM)
		return JL_ERR_PARAM;

	*pptr_info = gp_mmp_ptr[device->id]->used_info[mmp_ptr];

	return JL_ERR_OK;
}

jl_ret_t jl6107x_clear_mmp_pointer_info(jl_device_t *device,
								jl_uint8 module,
								jl_uint8 mmp_ptr)
{
	jl_mmp_used_info_t* pinfo = NULL;

	JL_CHECK_POINTER(device);

	pinfo = &(gp_mmp_ptr[device->id]->used_info[mmp_ptr]);

	switch (module)
	{
	case JL_MMP_MODULE_INITIAL_ADM :
		pinfo->adm_used_num--;
		break;
	case JL_MMP_MODULE_SOURCE_MAC :
		pinfo->smac_used_num--;
		break;
	case JL_MMP_MODULE_DEST_MAC :
		pinfo->dmac_used_num--;
		break;
	case JL_MMP_MODULE_ACL :
		pinfo->acl_used_num--;
		break;
	case JL_MMP_MODULE_VID :
		pinfo->vlan_used_num--;
		break;
	default:
		return JL_ERR_PARAM;
	}

	if (pinfo->adm_used_num == 0 &&
		pinfo->smac_used_num == 0 &&
		pinfo->dmac_used_num == 0 &&
		pinfo->acl_used_num == 0 &&
		pinfo->vlan_used_num == 0)
		pinfo->is_used = FALSE;

	return JL_ERR_OK;
}


jl_ret_t jl6107x_switch_info(jl_device_t *device, jl_switch_info_t *pinfo)
{
	jl_ret_t ret = JL_ERR_OK;

	pinfo->compat_id = device->compat_id;
	pinfo->sdk_ver = (VERSION_JL_SDK_MAJOR << 28)
		| (VERSION_JL_SDK_MINOR << 16)
		| (VERSION_JL_SDK_MAINTENANCE);

	return ret;
}

jl_ret_t jl6107x_show_switchport_info(jl_device_t *pdevice)
{
	jl_ret_t ret = JL_ERR_OK;
	struct jl_switch_dev_s *pswitch;
	int i = 0;

	pswitch = (struct jl_switch_dev_s *)pdevice->switch_dev;
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_WARNING, "Dump Switch port info:\n");

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_WARNING, "Chip version is: ");
	switch (pdevice->compat_id) {
	case JL_CHIP_6105:
		JL_PRINT("6105\n");
		break;
	case JL_CHIP_6107:
		JL_PRINT("6107\n");
		break;
	case JL_CHIP_6107S:
		JL_PRINT("6107S\n");
		break;
	case JL_CHIP_6107SC:
		JL_PRINT("6107SC\n");
		break;
	default:
		JL_PRINT("Unknown\n");
		break;
	};

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_WARNING, "Port info is:\n");

	JL_PRINT("Port ID:\tType:\tPhymode:\tStatus: \tSWcoreid:\t\n");
	for (; i < JL_PORT_MAX; i++) {
		JL_PRINT("%02d \t\t", i);

		if (PORT_TYPE_UTP == pswitch->lport_type[i])
			JL_PRINT("UTP    \t");
		else if (PORT_TYPE_EXT == pswitch->lport_type[i])
			JL_PRINT("EXT    \t");
		else if (PORT_TYPE_CPU == pswitch->lport_type[i])
			JL_PRINT("CPU    \t");
		else if (PORT_TYPE_UNDEF == pswitch->lport_type[i])
			JL_PRINT("Undef  \t");

		if (PORT_TYPE_EXT == pswitch->lport_type[i])
			JL_PRINT("SerdesPHY \t");
		else if (PORT_TYPE_UTP == pswitch->lport_type[i] || PORT_TYPE_UNDEF == pswitch->lport_type[i])
			JL_PRINT("GEPHY     \t");
		else if (PORT_TYPE_CPU == pswitch->lport_type[i])
			JL_PRINT("Internal  \t");

		if (UNDEF_PORT == pswitch->l2c_port[i])
			JL_PRINT("UNUSE \t\t");
		else
			JL_PRINT("USE   \t\t");

		JL_PRINT("%d   \t\n", pswitch->l2c_port[i]);
	}
	return ret;
}

static jl_ret_t jl6107x_sw_res_init(jl_device_t *device)
{
	JL_CHECK_POINTER(device);

	if (gp_mmp_ptr[device->id] == NULL)
	{
		gp_mmp_ptr[device->id] = (jl_mmp_pointer_t*)port_mem_malloc(sizeof(jl_mmp_pointer_t));

		if (!gp_mmp_ptr[device->id])
			return JL_ERR_MEMORY;

		memset(gp_mmp_ptr[device->id], 0, sizeof(jl_mmp_pointer_t));
	}

	return JL_ERR_OK;
}

static jl_ret_t jl6107x_sw_res_deinit(jl_device_t *device)
{
	JL_CHECK_POINTER(device);

	if (gp_mmp_ptr[device->id] != NULL)
	{
		port_mem_free(gp_mmp_ptr[device->id]);
		gp_mmp_ptr[device->id] = NULL;
	}
	return JL_ERR_OK;
}

/* #define CONFIG_JL_CHIP_ID_6107_FPGA */
#ifdef CONFIG_JL_CHIP_ID_6107_FPGA
#ifndef CONFIG_JL_CHIP_ID_6107
#error "Please select chip jl6107 for your fpga platform!!!"
#endif
jl_ret_t jl6107x_check_fw_stat(jl_device_t *device)
{
	(void) device;

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "fpga init skip %s\n", __func__);

	return JL_ERR_OK;
}

jl_ret_t jl6107x_check_hw_info(jl_device_t *device)
{
	JL_ASSERT(device);

	if (device->compat_id != JL_CHIP_6107) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
			"Wrong chip id 0x%x, expect 0x%x\n",
			device->compat_id, JL_CHIP_6107);
	}

	device->switch_dev = (void *)&g_switch_dev_6107;

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "fpga init skip %s\n", __func__);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch Chip HW information: jl6107_fpga\n");

	return JL_ERR_OK;
}

jl_ret_t jl6107x_watchdog_reset(jl_device_t *device)
{
	(void) device;

	return JL_ERR_OK;
}
#else
jl_ret_t jl6107x_check_fw_stat(jl_device_t *device)
{
	jl_ret_t ret = JL_ERR_OK;

	/* TOP FW_RESERVED7 Register setting */
	REGISTER_READ(device, TOP, FW_RESERVED7, fwstat, INDEX_ZERO, INDEX_ZERO);
	/* TOP FW_RESERVED15 Register setting */
	REGISTER_READ(device, TOP, FW_RESERVED15, fwver, INDEX_ZERO, INDEX_ZERO);

	/* load patch boot up */
	if (((fwstat.BF.fw_reserved7 == 0x40000) && (fwver.BF.fw_reserved15 != 0))
	/* normal boot up */
			|| (fwstat.BF.fw_reserved7 == 0x160000)
	/* rtos boot up */
			|| (fwstat.BF.fw_reserved7 == 0x130000))
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch Initialize with right firmware flag: 0x%x, version: 0x%x\n",
						fwstat.val[0], fwver.val[0]);
	else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Switch Initialize fail with wrong firmware flag: 0x%x \n",
						fwstat.val[0]);
		return JL_ERR_NOT_READY;
	}


	return JL_ERR_OK;
}

jl_ret_t jl6107x_check_pkg(jl_device_t *device, jl_uint32 chipid)
{
	jl_uint32 _chipid = 0;

	switch (device->compat_id) {
	case JL_CHIP_6105:
		_chipid = CHIP_ID_JL6105;
		break;
	case JL_CHIP_6107:
		_chipid = CHIP_ID_JL6107;
		break;
	case JL_CHIP_6107S:
		_chipid = CHIP_ID_JL6107S;
		break;
	case JL_CHIP_6107SC:
		_chipid = CHIP_ID_JL6107SC;
		break;
	default:
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"Unknown device compatible chip id 0x%x\n",
				device->compat_id);

		return JL_ERR_PARAM;
	}

	if (_chipid != chipid) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"Wrong device compatible chip id 0x%x\n",
				device->compat_id);

		return JL_ERR_BADID;
	}


	switch (chipid) {
#ifdef CONFIG_JL_CHIP_ID_6105
	case (CHIP_ID_JL6105):
		device->switch_dev = (void *)&g_switch_dev_6105;
		break;
#endif
#ifdef CONFIG_JL_CHIP_ID_6107
	case (CHIP_ID_JL6107):
		device->switch_dev = (void *)&g_switch_dev_6107;
		break;
#endif
#ifdef CONFIG_JL_CHIP_ID_6107S
	case (CHIP_ID_JL6107S):
		device->switch_dev = (void *)&g_switch_dev_6107s;
		break;
#endif
#ifdef CONFIG_JL_CHIP_ID_6107SC
	case (CHIP_ID_JL6107SC):
		device->switch_dev = (void *)&g_switch_dev_6107sc;
		break;
#endif
	default:
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, \
			"Unknown package mode [0x%x]!!!\n", chipid);
		return JL_ERR_BADID;
	}

	return JL_ERR_OK;
}

jl_ret_t jl6107x_check_hw_info(jl_device_t *device)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 pkg;
	jl_uint32 core_ver;
	jl_uint32 eco_ver;
	struct jl_switch_dev_s *p_switch_dev = NULL;

	/* TOP FW_RESERVED5 Register setting */
	REGISTER_READ(device, TOP, CHIP_ID, hw_info, INDEX_ZERO, INDEX_ZERO);

	core_ver = GET_BITS(hw_info.BF.chip_id, 0, 15);
	pkg = GET_BITS(hw_info.BF.chip_id, 16, 23);
	eco_ver = GET_BITS(hw_info.BF.chip_id, 24, 31);

	ret = jl6107x_check_pkg(device, pkg);
	if (ret)
		return ret;

	p_switch_dev = (struct jl_switch_dev_s *)(device->switch_dev);

	if (core_ver != p_switch_dev->core_ver) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, \
			"Wrong core version [0x%x], expect [0x%x]!!!\n", core_ver, p_switch_dev->core_ver);
		return JL_ERR_BADID;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch Chip HW information:\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "package mode: [0x%x]\n", pkg);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "core version: [0x%x]\n", core_ver);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "eco version: [0x%x]\n", eco_ver);

	return ret;
}

jl_ret_t jl6107x_watchdog_reset(jl_device_t *device)
{
	jl_ret_t ret = JL_ERR_OK;

	/* TOP FW_RESERVED12 Watchdog Register setting */
	REGISTER_READ(device, TOP, FW_RESERVED12, wdg, INDEX_ZERO, INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "watchdog dump: 0x%x \n", wdg.val[0]);

	/* stop feed the watchdog for chip reset */
	CLR_BIT(wdg.BF.fw_reserved12, 2);
	SET_BIT(wdg.BF.fw_reserved12, 0);

	/* TOP FW_RESERVED12 Watchdog Register setting */
	REGISTER_WRITE(device, TOP, FW_RESERVED12, wdg, INDEX_ZERO, INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "SYS: wdg: 0x%x \n", wdg.val[0]);

	return ret;
}
#endif

jl_ret_t jl6107x_sw_core_init(jl_device_t *device)
{
	jl_ret_t ret;

	JL_CHECK_POINTER(device);

	ret = jl6107x_check_fw_stat(device);
	if (ret)
		return ret;

	ret = jl6107x_check_hw_info(device);
	if (ret)
		return ret;

	ret = jl6107x_sw_res_init(device);
	if (ret)
		return ret;


#ifdef CONFIG_JL6107X_DRV_ACL
	ret = jl6107x_acl_init(device);
	if (ret)
		return ret;
#endif
#ifdef CONFIG_JL6107X_DRV_QOS
	ret = jl6107x_qos_init(device);
	if (ret)
		return ret;
#endif
	return JL_ERR_OK;
}

jl_ret_t jl6107x_sw_core_deinit(jl_device_t *device)
{
	jl_ret_t ret;

	JL_CHECK_POINTER(device);

#ifdef CONFIG_JL6107X_DRV_ACL
	ret = jl6107x_acl_deinit(device);
	if (ret)
		return ret;
#endif

#ifdef CONFIG_JL6107X_DRV_QOS
	ret = jl6107x_qos_deinit(device);
	if (ret)
		return ret;
#endif

	ret = jl6107x_sw_res_deinit(device);
	if (ret)
		return ret;

	ret = jl6107x_watchdog_reset(device);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl6107x_chip_probe(struct jl_device_s *device)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
		   "Module probe: driver[%s]  device[%s]\n", device->driver->name, device->name);

	if (!device->driver->ref_count) {
		ret = __ops_regist(device->driver);
		if (ret)
			return ret;
	}

	ret = jl_reg_io_init(&device->io_desc);
	if (ret)
		goto err_io_init;

	ret = jl6107x_sw_core_init(device);
	if (ret)
		goto err_sw_core_init;

	JL_API_MUTEX_INIT(device->mutex);
	device->driver->ref_count++;

	return ret;

err_sw_core_init:
	jl_reg_io_deinit(&device->io_desc);

err_io_init:
	if (!device->driver->ref_count)
		__ops_clear(device->driver);

	return ret;
}

jl_ret_t jl6107x_chip_remove(struct jl_device_s *device)
{
	jl_ret_t ret;

	JL_CHECK_POINTER(device);

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
		"Module remove: driver[%s]  device[%s]\n",
		device->driver->name, device->name);

	ret = jl6107x_sw_core_deinit(device);
	if (ret)
		return ret;

	if (device->driver->ref_count == 1)
		__ops_clear(device->driver);

	ret = jl_reg_io_deinit(&device->io_desc);

	JL_API_MUTEX_DEINIT(device->mutex);
	device->driver->ref_count--;

	return ret;
}
