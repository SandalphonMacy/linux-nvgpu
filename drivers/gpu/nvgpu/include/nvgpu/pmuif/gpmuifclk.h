/*
* Copyright (c) 2016-2018, NVIDIA CORPORATION.  All rights reserved.
*
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
*/

#ifndef _GPMUIFCLK_H_
#define _GPMUIFCLK_H_

#include "ctrl/ctrlboardobj.h"
#include "ctrl/ctrlvolt.h"
#include "ctrl/ctrlperf.h"
#include "ctrl/ctrlclk.h"
#include "gpmuifboardobj.h"
#include "gpmuifvolt.h"
#include <nvgpu/flcnif_cmn.h>


/*
 *  Try to get gpc2clk, mclk, sys2clk, xbar2clk work for Pascal
 *
 *  mclk is same for both
 *  gpc2clk is 17 for Pascal and 13 for Volta, making it 17
 *    as volta uses gpcclk
 *  sys2clk is 20 in Pascal and 15 in Volta.
 *    Changing for Pascal would break nvdclk of Volta
 *  xbar2clk is 19 in Pascal and 14 in Volta
 *    Changing for Pascal would break pwrclk of Volta
 */
enum nv_pmu_clk_clkwhich {
	clkwhich_gpcclk         = 1,
	clkwhich_xbarclk        = 2,
	clkwhich_sysclk         = 3,
	clkwhich_hubclk         = 4,
	clkwhich_mclk           = 5,
	clkwhich_hostclk        = 6,
	clkwhich_dispclk        = 7,
	clkwhich_xclk           = 12,
	clkwhich_gpc2clk        = 17,
	clkwhich_xbar2clk       = 14,
	clkwhich_sys2clk        = 15,
	clkwhich_hub2clk        = 16,
	clkwhich_pwrclk         = 19,
	clkwhich_nvdclk         = 20,
	clkwhich_pciegenclk     = 26,
};

/*
 * Enumeration of BOARDOBJGRP class IDs within OBJCLK.  Used as "classId"
 * argument for communications between Kernel and PMU via the various generic
 * BOARDOBJGRP interfaces.
 */
#define NV_PMU_CLK_BOARDOBJGRP_CLASS_ID_CLK_DOMAIN                          0x00
#define NV_PMU_CLK_BOARDOBJGRP_CLASS_ID_CLK_PROG                            0x01
#define NV_PMU_CLK_BOARDOBJGRP_CLASS_ID_VIN_DEVICE                          0x02
#define NV_PMU_CLK_BOARDOBJGRP_CLASS_ID_FLL_DEVICE                          0x03
#define NV_PMU_CLK_BOARDOBJGRP_CLASS_ID_CLK_VF_POINT                        0x04
#define NV_PMU_CLK_BOARDOBJGRP_CLASS_ID_CLK_FREQ_CONTROLLER                 0x05

/*!
* CLK_DOMAIN BOARDOBJGRP Header structure.  Describes global state about the
* CLK_DOMAIN feature.
*/
struct nv_pmu_clk_clk_domain_boardobjgrp_set_header {
	struct nv_pmu_boardobjgrp_e32 super;
	u32 vbios_domains;
	struct ctrl_boardobjgrp_mask_e32 prog_domains_mask;
	struct ctrl_boardobjgrp_mask_e32 master_domains_mask;
	u16 cntr_sampling_periodms;
	u8 version;
	bool b_override_o_v_o_c;
	bool b_debug_mode;
	bool b_enforce_vf_monotonicity;
	bool b_enforce_vf_smoothening;
	u8 volt_rails_max;
	struct ctrl_clk_clk_delta deltas;
};

struct nv_pmu_clk_clk_domain_boardobj_set {
	struct nv_pmu_boardobj super;
	enum nv_pmu_clk_clkwhich domain;
	u32 api_domain;
	u8 perf_domain_grp_idx;
};

struct nv_pmu_clk_clk_domain_3x_boardobj_set {
	struct nv_pmu_clk_clk_domain_boardobj_set super;
	bool b_noise_aware_capable;
};

struct nv_pmu_clk_clk_domain_3x_fixed_boardobj_set {
	struct nv_pmu_clk_clk_domain_3x_boardobj_set super;
	u16 freq_mhz;
};

struct nv_pmu_clk_clk_domain_3x_prog_boardobj_set {
	struct nv_pmu_clk_clk_domain_3x_boardobj_set super;
	u8 clk_prog_idx_first;
	u8 clk_prog_idx_last;
	bool b_force_noise_unaware_ordering;
	struct ctrl_clk_freq_delta factory_delta;
	short freq_delta_min_mhz;
	short freq_delta_max_mhz;
	struct ctrl_clk_clk_delta deltas;
	u8 noise_unaware_ordering_index;
	u8 noise_aware_ordering_index;
};

struct nv_pmu_clk_clk_domain_3x_master_boardobj_set {
	struct nv_pmu_clk_clk_domain_3x_prog_boardobj_set super;
	u8 rsvd;	/* Stubbing for RM_PMU_BOARDOBJ_INTERFACE */
	u32 slave_idxs_mask;
};

struct nv_pmu_clk_clk_domain_3x_slave_boardobj_set {
	struct nv_pmu_clk_clk_domain_3x_prog_boardobj_set super;
	u8 rsvd;	/* Stubbing for RM_PMU_BOARDOBJ_INTERFACE */
	u8 master_idx;
};

union nv_pmu_clk_clk_domain_boardobj_set_union {
	struct nv_pmu_boardobj board_obj;
	struct nv_pmu_clk_clk_domain_boardobj_set super;
	struct nv_pmu_clk_clk_domain_3x_boardobj_set v3x;
	struct nv_pmu_clk_clk_domain_3x_fixed_boardobj_set v3x_fixed;
	struct nv_pmu_clk_clk_domain_3x_prog_boardobj_set v3x_prog;
	struct nv_pmu_clk_clk_domain_3x_master_boardobj_set v3x_master;
	struct nv_pmu_clk_clk_domain_3x_slave_boardobj_set v3x_slave;
};

NV_PMU_BOARDOBJ_GRP_SET_MAKE_E32(clk, clk_domain);

struct nv_pmu_clk_clk_prog_boardobjgrp_set_header {
	struct nv_pmu_boardobjgrp_e255 super;
	u8 slave_entry_count;
	u8 vf_entry_count;
};

struct nv_pmu_clk_clk_prog_boardobj_set {
	struct nv_pmu_boardobj super;
};

struct nv_pmu_clk_clk_prog_1x_boardobj_set {
	struct nv_pmu_clk_clk_prog_boardobj_set super;
	u8 source;
	u16 freq_max_mhz;
	union ctrl_clk_clk_prog_1x_source_data source_data;
};

struct nv_pmu_clk_clk_prog_1x_master_boardobj_set {
	struct nv_pmu_clk_clk_prog_1x_boardobj_set super;
	u8 rsvd;	/* Stubbing for RM_PMU_BOARDOBJ_INTERFACE */
	bool b_o_c_o_v_enabled;
	struct ctrl_clk_clk_prog_1x_master_vf_entry vf_entries[
		CTRL_CLK_CLK_PROG_1X_MASTER_VF_ENTRY_MAX_ENTRIES];
	struct ctrl_clk_clk_delta deltas;
	union ctrl_clk_clk_prog_1x_master_source_data source_data;
};

struct nv_pmu_clk_clk_prog_1x_master_ratio_boardobj_set {
	struct nv_pmu_clk_clk_prog_1x_master_boardobj_set super;
	u8 rsvd;	/* Stubbing for RM_PMU_BOARDOBJ_INTERFACE */
	struct ctrl_clk_clk_prog_1x_master_ratio_slave_entry slave_entries[
		CTRL_CLK_PROG_1X_MASTER_MAX_SLAVE_ENTRIES];
};

struct nv_pmu_clk_clk_prog_1x_master_table_boardobj_set {
	struct nv_pmu_clk_clk_prog_1x_master_boardobj_set super;
	u8 rsvd;	/* Stubbing for RM_PMU_BOARDOBJ_INTERFACE */
	struct ctrl_clk_clk_prog_1x_master_table_slave_entry
	slave_entries[CTRL_CLK_PROG_1X_MASTER_MAX_SLAVE_ENTRIES];
};

union nv_pmu_clk_clk_prog_boardobj_set_union {
	struct nv_pmu_boardobj board_obj;
	struct nv_pmu_clk_clk_prog_boardobj_set super;
	struct nv_pmu_clk_clk_prog_1x_boardobj_set v1x;
	struct nv_pmu_clk_clk_prog_1x_master_boardobj_set v1x_master;
	struct nv_pmu_clk_clk_prog_1x_master_ratio_boardobj_set v1x_master_ratio;
	struct nv_pmu_clk_clk_prog_1x_master_table_boardobj_set v1x_master_table;
};

NV_PMU_BOARDOBJ_GRP_SET_MAKE_E255(clk, clk_prog);

struct nv_pmu_clk_lut_device_desc {
	u8 vselect_mode;
	u16 hysteresis_threshold;
};

struct nv_pmu_clk_regime_desc {
	u8 regime_id;
	u16 fixed_freq_regime_limit_mhz;
};

struct nv_pmu_clk_clk_fll_device_boardobjgrp_set_header {
	struct nv_pmu_boardobjgrp_e32 super;
	struct ctrl_boardobjgrp_mask_e32 lut_prog_master_mask;
	u32 lut_step_size_uv;
	u32 lut_min_voltage_uv;
	u8 lut_num_entries;
	u16 max_min_freq_mhz;
};

struct nv_pmu_clk_clk_fll_device_boardobj_set {
	struct nv_pmu_boardobj super;
	u8 id;
	u8 mdiv;
	u8 vin_idx_logic;
	u8 vin_idx_sram;
	u8 rail_idx_for_lut;
	u16 input_freq_mhz;
	u32 clk_domain;
	struct nv_pmu_clk_lut_device_desc lut_device;
	struct nv_pmu_clk_regime_desc regime_desc;
	u8 min_freq_vfe_idx;
	u8 freq_ctrl_idx;
	bool b_skip_pldiv_below_dvco_min;
	bool b_dvco_1x;
	struct ctrl_boardobjgrp_mask_e32 lut_prog_broadcast_slave_mask;
};

union nv_pmu_clk_clk_fll_device_boardobj_set_union {
	struct nv_pmu_boardobj board_obj;
	struct nv_pmu_clk_clk_fll_device_boardobj_set super;
};

NV_PMU_BOARDOBJ_GRP_SET_MAKE_E32(clk, clk_fll_device);

struct nv_pmu_clk_clk_vin_device_boardobjgrp_set_header {
	struct nv_pmu_boardobjgrp_e32 super;
	bool b_vin_is_disable_allowed;
};

struct nv_pmu_clk_clk_vin_device_boardobj_set {
	struct nv_pmu_boardobj super;
	u8 id;
	u8 volt_domain;
	u32 flls_shared_mask;
};

struct nv_pmu_clk_clk_vin_device_v10_boardobj_set {
	struct nv_pmu_clk_clk_vin_device_boardobj_set super;
	struct ctrl_clk_vin_device_info_data_v10 data;
};

struct nv_pmu_clk_clk_vin_device_v20_boardobj_set {
	struct nv_pmu_clk_clk_vin_device_boardobj_set super;
	struct ctrl_clk_vin_device_info_data_v20 data;
};

union nv_pmu_clk_clk_vin_device_boardobj_set_union {
	struct nv_pmu_boardobj board_obj;
	struct nv_pmu_clk_clk_vin_device_boardobj_set super;
	struct nv_pmu_clk_clk_vin_device_v10_boardobj_set v10;
	struct nv_pmu_clk_clk_vin_device_v20_boardobj_set v20;
};

NV_PMU_BOARDOBJ_GRP_SET_MAKE_E32(clk, clk_vin_device);

struct nv_pmu_clk_clk_vf_point_boardobjgrp_set_header {
	struct nv_pmu_boardobjgrp_e255 super;
};

struct nv_pmu_clk_clk_vf_point_boardobj_set {
	struct nv_pmu_boardobj super;
	u8 vfe_equ_idx;
	u8 volt_rail_idx;
};

struct nv_pmu_clk_clk_vf_point_freq_boardobj_set {
	struct nv_pmu_clk_clk_vf_point_boardobj_set super;
	u16 freq_mhz;
	int volt_delta_uv;
};

struct nv_pmu_clk_clk_vf_point_volt_boardobj_set {
	struct nv_pmu_clk_clk_vf_point_boardobj_set super;
	u32 source_voltage_uv;
	struct ctrl_clk_freq_delta freq_delta;
};

union nv_pmu_clk_clk_vf_point_boardobj_set_union {
	struct nv_pmu_boardobj board_obj;
	struct nv_pmu_clk_clk_vf_point_boardobj_set super;
	struct nv_pmu_clk_clk_vf_point_freq_boardobj_set freq;
	struct nv_pmu_clk_clk_vf_point_volt_boardobj_set volt;
};

NV_PMU_BOARDOBJ_GRP_SET_MAKE_E255(clk, clk_vf_point);

struct nv_pmu_clk_clk_vf_point_boardobjgrp_get_status_header {
	struct nv_pmu_boardobjgrp_e255 super;
	u32 vf_points_cahce_counter;
};

struct nv_pmu_clk_clk_vf_point_boardobj_get_status {
	struct nv_pmu_boardobj super;
	struct ctrl_clk_vf_pair pair;
	u8 dummy[38];
};

struct nv_pmu_clk_clk_vf_point_volt_boardobj_get_status {
	struct nv_pmu_clk_clk_vf_point_boardobj_get_status super;
	u16 vf_gain_value;
};

union nv_pmu_clk_clk_vf_point_boardobj_get_status_union {
	struct nv_pmu_boardobj board_obj;
	struct nv_pmu_clk_clk_vf_point_boardobj_get_status super;
	struct nv_pmu_clk_clk_vf_point_volt_boardobj_get_status volt;
};

NV_PMU_BOARDOBJ_GRP_GET_STATUS_MAKE_E255(clk, clk_vf_point);

#define NV_PMU_VF_INJECT_MAX_CLOCK_DOMAINS                                  (12)

struct nv_pmu_clk_clk_domain_list {
	u8 num_domains;
	struct ctrl_clk_clk_domain_list_item clk_domains[
		NV_PMU_VF_INJECT_MAX_CLOCK_DOMAINS];
};

struct nv_pmu_clk_clk_domain_list_v1 {
	u8 num_domains;
	struct ctrl_clk_clk_domain_list_item_v1 clk_domains[
		NV_PMU_VF_INJECT_MAX_CLOCK_DOMAINS];
};

struct nv_pmu_clk_vf_change_inject {
	u8 flags;
	struct nv_pmu_clk_clk_domain_list clk_list;
	struct nv_pmu_volt_volt_rail_list volt_list;
};

struct nv_pmu_clk_vf_change_inject_v1 {
	u8 flags;
	struct nv_pmu_clk_clk_domain_list_v1 clk_list;
	struct nv_pmu_volt_volt_rail_list_v1 volt_list;
};

#define NV_NV_PMU_CLK_LOAD_FEATURE_VIN                              (0x00000002)
#define NV_NV_PMU_CLK_LOAD_ACTION_MASK_VIN_HW_CAL_PROGRAM_YES       (0x00000001)

struct nv_pmu_clk_load_payload_freq_controllers {
	struct ctrl_boardobjgrp_mask_e32 load_mask;
};

struct nv_pmu_clk_load {
	u8 feature;
	u32 action_mask;
	union {
		struct nv_pmu_clk_load_payload_freq_controllers freq_controllers;
	} payload;
};
/* CLK_FREQ_CONTROLLER */
#define NV_NV_PMU_CLK_LOAD_FEATURE_FREQ_CONTROLLER (0x00000003)

#define NV_NV_PMU_CLK_LOAD_ACTION_MASK_FREQ_CONTROLLER_CALLBACK_NO  (0x00000000)
#define NV_NV_PMU_CLK_LOAD_ACTION_MASK_FREQ_CONTROLLER_CALLBACK_YES (0x00000002)

struct nv_pmu_clk_clk_freq_controller_boardobjgrp_set_header {
	struct nv_pmu_boardobjgrp_e32  super;
	u32 sampling_period_ms;
	u8 volt_policy_idx;
};

struct nv_pmu_clk_clk_freq_controller_boardobj_set {
	struct nv_pmu_boardobj super;
	u8   controller_id;
	u8   parts_freq_mode;
	bool bdisable;
	u32  clk_domain;
	s16  freq_cap_noise_unaware_vmin_above;
	s16  freq_cap_noise_unaware_vmin_below;
	s16  freq_hyst_pos_mhz;
	s16  freq_hyst_neg_mhz;
};

struct nv_pmu_clk_clk_freq_controller_pi_boardobj_set {
	struct nv_pmu_clk_clk_freq_controller_boardobj_set super;
	s32 prop_gain;
	s32 integ_gain;
	s32 integ_decay;
	s32 volt_delta_min;
	s32 volt_delta_max;
	u8  slowdown_pct_min;
	bool bpoison;
};

union nv_pmu_clk_clk_freq_controller_boardobj_set_union {
	struct nv_pmu_boardobj board_obj;
	struct nv_pmu_clk_clk_freq_controller_boardobj_set super;
	struct nv_pmu_clk_clk_freq_controller_pi_boardobj_set pi;
};

NV_PMU_BOARDOBJ_GRP_SET_MAKE_E32(clk, clk_freq_controller);

/* CLK CMD ID definitions.  */
#define NV_PMU_CLK_CMD_ID_BOARDOBJ_GRP_SET                          (0x00000001)
#define NV_PMU_CLK_CMD_ID_RPC                                       (0x00000000)
#define NV_PMU_CLK_CMD_ID_BOARDOBJ_GRP_GET_STATUS                   (0x00000002)

#define NV_PMU_CLK_RPC_ID_LOAD                                      (0x00000001)
#define NV_PMU_CLK_RPC_ID_CLK_VF_CHANGE_INJECT                      (0x00000000)
#define NV_PMU_CLK_RPC_ID_CLK_FREQ_EFF_AVG                          (0x00000002)


struct nv_pmu_clk_cmd_rpc {
	u8 cmd_type;
	u8 pad[3];
	struct nv_pmu_allocation request;
};

#define NV_PMU_CLK_CMD_RPC_ALLOC_OFFSET                                        \
	(offsetof(struct nv_pmu_clk_cmd_rpc, request))

struct nv_pmu_clk_cmd {
	union {
		u8 cmd_type;
		struct nv_pmu_boardobj_cmd_grp grp_set;
		struct nv_pmu_clk_cmd_rpc rpc;
		struct nv_pmu_boardobj_cmd_grp grp_get_status;
	};
};

struct nv_pmu_clk_rpc {
	u8 function;
	bool b_supported;
	bool b_success;
	flcn_status flcn_status;
	union {
		struct nv_pmu_clk_vf_change_inject clk_vf_change_inject;
		struct nv_pmu_clk_vf_change_inject_v1 clk_vf_change_inject_v1;
		struct nv_pmu_clk_load clk_load;
	} params;
};

/* CLK MSG ID definitions */
#define NV_PMU_CLK_MSG_ID_BOARDOBJ_GRP_SET                          (0x00000001)
#define NV_PMU_CLK_MSG_ID_RPC                                       (0x00000000)
#define NV_PMU_CLK_MSG_ID_BOARDOBJ_GRP_GET_STATUS                   (0x00000002)

struct nv_pmu_clk_msg_rpc {
	u8 msg_type;
	u8 rsvd[3];
	struct nv_pmu_allocation response;
};

#define NV_PMU_CLK_MSG_RPC_ALLOC_OFFSET       \
	offsetof(struct nv_pmu_clk_msg_rpc, response)

struct nv_pmu_clk_msg {
	union {
		u8 msg_type;
		struct nv_pmu_boardobj_msg_grp grp_set;
		struct nv_pmu_clk_msg_rpc rpc;
		struct nv_pmu_boardobj_msg_grp grp_get_status;
	};
};

struct nv_pmu_clk_clk_vin_device_boardobjgrp_get_status_header {
	struct nv_pmu_boardobjgrp_e32 super;
};

struct nv_pmu_clk_clk_vin_device_boardobj_get_status {
	struct nv_pmu_boardobj_query super;
	u32 actual_voltage_uv;
	u32 corrected_voltage_uv;
	u8 sampled_code;
	u8 override_code;
};

union nv_pmu_clk_clk_vin_device_boardobj_get_status_union {
	struct nv_pmu_boardobj_query board_obj;
	struct nv_pmu_clk_clk_vin_device_boardobj_get_status super;
};

NV_PMU_BOARDOBJ_GRP_GET_STATUS_MAKE_E32(clk, clk_vin_device);

struct nv_pmu_clk_lut_vf_entry {
	u32 entry;
};

struct nv_pmu_clk_clk_fll_device_boardobjgrp_get_status_header {
	struct nv_pmu_boardobjgrp_e32 super;
};

struct nv_pmu_clk_clk_fll_device_boardobj_get_status {
	struct nv_pmu_boardobj_query super;
	u8 current_regime_id;
	u16 min_freq_mhz;
	struct nv_pmu_clk_lut_vf_entry lut_vf_curve[NV_UNSIGNED_ROUNDED_DIV(CTRL_CLK_LUT_NUM_ENTRIES, 2)];
};

union nv_pmu_clk_clk_fll_device_boardobj_get_status_union {
	struct nv_pmu_boardobj_query board_obj;
	struct nv_pmu_clk_clk_fll_device_boardobj_get_status super;
};

NV_PMU_BOARDOBJ_GRP_GET_STATUS_MAKE_E32(clk, clk_fll_device);

#endif /*_GPMUIFCLK_H_*/
