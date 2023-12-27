//reg_cmac.h
/*
* Automatically generated by gen_c_api.py, don't edit it
*/
#ifndef __JL_REG_CMAC_H__
#define __JL_REG_CMAC_H__

#include <jl_types.h>

#define CMAC_BASE                                                                              ((jl_uint32)0x00140E00U)
#define CMAC_BLOCK_SIZE                                                                        ((jl_uint32)0x00000200U)

#define CMAC_INT_STATUS                                                                        ((jl_uint32)0x00140E00U)
#define CMAC_INT_STATUS_NUM                                                                    ((jl_uint32)0x00000001U)
#define CMAC_INT_STATUS_ENTRY_SIZE                                                                                  (1)
#define CMAC_INT_STATUS_OFFSET_RSVD0                                                                                (5)
#define CMAC_INT_STATUS_OFFSET_RSVD0_WIDTH                                                                         (27)
#define CMAC_INT_STATUS_OFFSET_INT_RX_TRANS_UDF                                                                     (0)
#define CMAC_INT_STATUS_OFFSET_INT_RX_TRANS_UDF_WIDTH                                                               (1)
#define CMAC_INT_STATUS_OFFSET_INT_RX_TRANS_OVF                                                                     (1)
#define CMAC_INT_STATUS_OFFSET_INT_RX_TRANS_OVF_WIDTH                                                               (1)
#define CMAC_INT_STATUS_OFFSET_INT_TX_TRANS_UDF                                                                     (2)
#define CMAC_INT_STATUS_OFFSET_INT_TX_TRANS_UDF_WIDTH                                                               (1)
#define CMAC_INT_STATUS_OFFSET_INT_TX_TRANS_OVF                                                                     (3)
#define CMAC_INT_STATUS_OFFSET_INT_TX_TRANS_OVF_WIDTH                                                               (1)
#define CMAC_INT_STATUS_OFFSET_INT_REG_ACCESS_ERR                                                                   (4)
#define CMAC_INT_STATUS_OFFSET_INT_REG_ACCESS_ERR_WIDTH                                                             (1)
typedef union CMAC_INT_STATUS_u {
    struct __attribute__ ((packed)) {
        jl_uint32 int_rx_trans_udf:CMAC_INT_STATUS_OFFSET_INT_RX_TRANS_UDF_WIDTH;                                         //[ 0: 0]
        jl_uint32 int_rx_trans_ovf:CMAC_INT_STATUS_OFFSET_INT_RX_TRANS_OVF_WIDTH;                                         //[ 1: 1]
        jl_uint32 int_tx_trans_udf:CMAC_INT_STATUS_OFFSET_INT_TX_TRANS_UDF_WIDTH;                                         //[ 2: 2]
        jl_uint32 int_tx_trans_ovf:CMAC_INT_STATUS_OFFSET_INT_TX_TRANS_OVF_WIDTH;                                         //[ 3: 3]
        jl_uint32 int_reg_access_err:CMAC_INT_STATUS_OFFSET_INT_REG_ACCESS_ERR_WIDTH;                                     //[ 4: 4]
        jl_uint32 rsvd0:CMAC_INT_STATUS_OFFSET_RSVD0_WIDTH;                                                               //[31: 5]
    } BF;
    jl_uint32 val[CMAC_INT_STATUS_ENTRY_SIZE];
} CMAC_INT_STATUS_t;

#define CMAC_INT_ENABLE                                                                        ((jl_uint32)0x00140E04U)
#define CMAC_INT_ENABLE_NUM                                                                    ((jl_uint32)0x00000001U)
#define CMAC_INT_ENABLE_ENTRY_SIZE                                                                                  (1)
#define CMAC_INT_ENABLE_OFFSET_RSVD0                                                                                (5)
#define CMAC_INT_ENABLE_OFFSET_RSVD0_WIDTH                                                                         (27)
#define CMAC_INT_ENABLE_OFFSET_INT_RX_TRANS_UDF_EN                                                                  (0)
#define CMAC_INT_ENABLE_OFFSET_INT_RX_TRANS_UDF_EN_WIDTH                                                            (1)
#define CMAC_INT_ENABLE_OFFSET_INT_RX_TRANS_OVF_EN                                                                  (1)
#define CMAC_INT_ENABLE_OFFSET_INT_RX_TRANS_OVF_EN_WIDTH                                                            (1)
#define CMAC_INT_ENABLE_OFFSET_INT_TX_TRANS_UDF_EN                                                                  (2)
#define CMAC_INT_ENABLE_OFFSET_INT_TX_TRANS_UDF_EN_WIDTH                                                            (1)
#define CMAC_INT_ENABLE_OFFSET_INT_TX_TRANS_OVF_EN                                                                  (3)
#define CMAC_INT_ENABLE_OFFSET_INT_TX_TRANS_OVF_EN_WIDTH                                                            (1)
#define CMAC_INT_ENABLE_OFFSET_INT_REG_ACCESS_ERR_EN                                                                (4)
#define CMAC_INT_ENABLE_OFFSET_INT_REG_ACCESS_ERR_EN_WIDTH                                                          (1)
typedef union CMAC_INT_ENABLE_u {
    struct __attribute__ ((packed)) {
        jl_uint32 int_rx_trans_udf_en:CMAC_INT_ENABLE_OFFSET_INT_RX_TRANS_UDF_EN_WIDTH;                                   //[ 0: 0]
        jl_uint32 int_rx_trans_ovf_en:CMAC_INT_ENABLE_OFFSET_INT_RX_TRANS_OVF_EN_WIDTH;                                   //[ 1: 1]
        jl_uint32 int_tx_trans_udf_en:CMAC_INT_ENABLE_OFFSET_INT_TX_TRANS_UDF_EN_WIDTH;                                   //[ 2: 2]
        jl_uint32 int_tx_trans_ovf_en:CMAC_INT_ENABLE_OFFSET_INT_TX_TRANS_OVF_EN_WIDTH;                                   //[ 3: 3]
        jl_uint32 int_reg_access_err_en:CMAC_INT_ENABLE_OFFSET_INT_REG_ACCESS_ERR_EN_WIDTH;                               //[ 4: 4]
        jl_uint32 rsvd0:CMAC_INT_ENABLE_OFFSET_RSVD0_WIDTH;                                                               //[31: 5]
    } BF;
    jl_uint32 val[CMAC_INT_ENABLE_ENTRY_SIZE];
} CMAC_INT_ENABLE_t;

#define CMAC_INT_SET                                                                           ((jl_uint32)0x00140E08U)
#define CMAC_INT_SET_NUM                                                                       ((jl_uint32)0x00000001U)
#define CMAC_INT_SET_ENTRY_SIZE                                                                                     (1)
#define CMAC_INT_SET_OFFSET_RSVD0                                                                                   (5)
#define CMAC_INT_SET_OFFSET_RSVD0_WIDTH                                                                            (27)
#define CMAC_INT_SET_OFFSET_INT_RX_TRANS_UDF_SET                                                                    (0)
#define CMAC_INT_SET_OFFSET_INT_RX_TRANS_UDF_SET_WIDTH                                                              (1)
#define CMAC_INT_SET_OFFSET_INT_RX_TRANS_OVF_SET                                                                    (1)
#define CMAC_INT_SET_OFFSET_INT_RX_TRANS_OVF_SET_WIDTH                                                              (1)
#define CMAC_INT_SET_OFFSET_INT_TX_TRANS_UDF_SET                                                                    (2)
#define CMAC_INT_SET_OFFSET_INT_TX_TRANS_UDF_SET_WIDTH                                                              (1)
#define CMAC_INT_SET_OFFSET_INT_TX_TRANS_OVF_SET                                                                    (3)
#define CMAC_INT_SET_OFFSET_INT_TX_TRANS_OVF_SET_WIDTH                                                              (1)
#define CMAC_INT_SET_OFFSET_INT_REG_ACCESS_ERR_SET                                                                  (4)
#define CMAC_INT_SET_OFFSET_INT_REG_ACCESS_ERR_SET_WIDTH                                                            (1)
typedef union CMAC_INT_SET_u {
    struct __attribute__ ((packed)) {
        jl_uint32 int_rx_trans_udf_set:CMAC_INT_SET_OFFSET_INT_RX_TRANS_UDF_SET_WIDTH;                                    //[ 0: 0]
        jl_uint32 int_rx_trans_ovf_set:CMAC_INT_SET_OFFSET_INT_RX_TRANS_OVF_SET_WIDTH;                                    //[ 1: 1]
        jl_uint32 int_tx_trans_udf_set:CMAC_INT_SET_OFFSET_INT_TX_TRANS_UDF_SET_WIDTH;                                    //[ 2: 2]
        jl_uint32 int_tx_trans_ovf_set:CMAC_INT_SET_OFFSET_INT_TX_TRANS_OVF_SET_WIDTH;                                    //[ 3: 3]
        jl_uint32 int_reg_access_err_set:CMAC_INT_SET_OFFSET_INT_REG_ACCESS_ERR_SET_WIDTH;                                //[ 4: 4]
        jl_uint32 rsvd0:CMAC_INT_SET_OFFSET_RSVD0_WIDTH;                                                                  //[31: 5]
    } BF;
    jl_uint32 val[CMAC_INT_SET_ENTRY_SIZE];
} CMAC_INT_SET_t;

#define CMAC_TX_TRANSFER_CTRL                                                                  ((jl_uint32)0x00140E0CU)
#define CMAC_TX_TRANSFER_CTRL_NUM                                                              ((jl_uint32)0x00000001U)
#define CMAC_TX_TRANSFER_CTRL_ENTRY_SIZE                                                                            (1)
#define CMAC_TX_TRANSFER_CTRL_OFFSET_RSVD0                                                                          (1)
#define CMAC_TX_TRANSFER_CTRL_OFFSET_RSVD0_WIDTH                                                                   (31)
#define CMAC_TX_TRANSFER_CTRL_OFFSET_CFG_TX_JUMBO_PKT_EN                                                            (0)
#define CMAC_TX_TRANSFER_CTRL_OFFSET_CFG_TX_JUMBO_PKT_EN_WIDTH                                                      (1)
typedef union CMAC_TX_TRANSFER_CTRL_u {
    struct __attribute__ ((packed)) {
        jl_uint32 cfg_tx_jumbo_pkt_en:CMAC_TX_TRANSFER_CTRL_OFFSET_CFG_TX_JUMBO_PKT_EN_WIDTH;                             //[ 0: 0]
        jl_uint32 rsvd0:CMAC_TX_TRANSFER_CTRL_OFFSET_RSVD0_WIDTH;                                                         //[31: 1]
    } BF;
    jl_uint32 val[CMAC_TX_TRANSFER_CTRL_ENTRY_SIZE];
} CMAC_TX_TRANSFER_CTRL_t;

#define CMAC_RX_MEM_STATUS_CTRL                                                                ((jl_uint32)0x00140E10U)
#define CMAC_RX_MEM_STATUS_CTRL_NUM                                                            ((jl_uint32)0x00000001U)
#define CMAC_RX_MEM_STATUS_CTRL_ENTRY_SIZE                                                                          (1)
#define CMAC_RX_MEM_STATUS_CTRL_OFFSET_RSVD0                                                                        (9)
#define CMAC_RX_MEM_STATUS_CTRL_OFFSET_RSVD0_WIDTH                                                                 (23)
#define CMAC_RX_MEM_STATUS_CTRL_OFFSET_CFG_RX_FREE_SPACE_OFFSET                                                     (0)
#define CMAC_RX_MEM_STATUS_CTRL_OFFSET_CFG_RX_FREE_SPACE_OFFSET_WIDTH                                               (9)
typedef union CMAC_RX_MEM_STATUS_CTRL_u {
    struct __attribute__ ((packed)) {
        jl_uint32 cfg_rx_free_space_offset:CMAC_RX_MEM_STATUS_CTRL_OFFSET_CFG_RX_FREE_SPACE_OFFSET_WIDTH;                 //[ 8: 0]
        jl_uint32 rsvd0:CMAC_RX_MEM_STATUS_CTRL_OFFSET_RSVD0_WIDTH;                                                       //[31: 9]
    } BF;
    jl_uint32 val[CMAC_RX_MEM_STATUS_CTRL_ENTRY_SIZE];
} CMAC_RX_MEM_STATUS_CTRL_t;

#define CMAC_TRANSFER_PKT_IPG                                                                  ((jl_uint32)0x00140E14U)
#define CMAC_TRANSFER_PKT_IPG_NUM                                                              ((jl_uint32)0x00000001U)
#define CMAC_TRANSFER_PKT_IPG_ENTRY_SIZE                                                                            (1)
#define CMAC_TRANSFER_PKT_IPG_OFFSET_RSVD0                                                                         (10)
#define CMAC_TRANSFER_PKT_IPG_OFFSET_RSVD0_WIDTH                                                                   (22)
#define CMAC_TRANSFER_PKT_IPG_OFFSET_CFG_RX_PKT_IPG                                                                 (0)
#define CMAC_TRANSFER_PKT_IPG_OFFSET_CFG_RX_PKT_IPG_WIDTH                                                           (5)
#define CMAC_TRANSFER_PKT_IPG_OFFSET_CFG_TX_PKT_IPG                                                                 (5)
#define CMAC_TRANSFER_PKT_IPG_OFFSET_CFG_TX_PKT_IPG_WIDTH                                                           (5)
typedef union CMAC_TRANSFER_PKT_IPG_u {
    struct __attribute__ ((packed)) {
        jl_uint32 cfg_rx_pkt_ipg:CMAC_TRANSFER_PKT_IPG_OFFSET_CFG_RX_PKT_IPG_WIDTH;                                       //[ 4: 0]
        jl_uint32 cfg_tx_pkt_ipg:CMAC_TRANSFER_PKT_IPG_OFFSET_CFG_TX_PKT_IPG_WIDTH;                                       //[ 9: 5]
        jl_uint32 rsvd0:CMAC_TRANSFER_PKT_IPG_OFFSET_RSVD0_WIDTH;                                                         //[31:10]
    } BF;
    jl_uint32 val[CMAC_TRANSFER_PKT_IPG_ENTRY_SIZE];
} CMAC_TRANSFER_PKT_IPG_t;

#define CMAC_MEM_CTRL                                                                          ((jl_uint32)0x00140E18U)
#define CMAC_MEM_CTRL_NUM                                                                      ((jl_uint32)0x00000001U)
#define CMAC_MEM_CTRL_ENTRY_SIZE                                                                                    (1)
#define CMAC_MEM_CTRL_OFFSET_RSVD0                                                                                  (3)
#define CMAC_MEM_CTRL_OFFSET_RSVD0_WIDTH                                                                           (29)
#define CMAC_MEM_CTRL_OFFSET_CFG_MEM_EMA                                                                            (0)
#define CMAC_MEM_CTRL_OFFSET_CFG_MEM_EMA_WIDTH                                                                      (3)
typedef union CMAC_MEM_CTRL_u {
    struct __attribute__ ((packed)) {
        jl_uint32 cfg_mem_ema:CMAC_MEM_CTRL_OFFSET_CFG_MEM_EMA_WIDTH;                                                     //[ 2: 0]
        jl_uint32 rsvd0:CMAC_MEM_CTRL_OFFSET_RSVD0_WIDTH;                                                                 //[31: 3]
    } BF;
    jl_uint32 val[CMAC_MEM_CTRL_ENTRY_SIZE];
} CMAC_MEM_CTRL_t;

#define CMAC_CMAC_FLOW_CTRL                                                                    ((jl_uint32)0x00140E1CU)
#define CMAC_CMAC_FLOW_CTRL_NUM                                                                ((jl_uint32)0x00000001U)
#define CMAC_CMAC_FLOW_CTRL_ENTRY_SIZE                                                                              (1)
#define CMAC_CMAC_FLOW_CTRL_OFFSET_RSVD0                                                                            (2)
#define CMAC_CMAC_FLOW_CTRL_OFFSET_RSVD0_WIDTH                                                                     (30)
#define CMAC_CMAC_FLOW_CTRL_OFFSET_CFG_CMAC_RX_EN                                                                   (0)
#define CMAC_CMAC_FLOW_CTRL_OFFSET_CFG_CMAC_RX_EN_WIDTH                                                             (1)
#define CMAC_CMAC_FLOW_CTRL_OFFSET_CFG_CMAC_TX_EN                                                                   (1)
#define CMAC_CMAC_FLOW_CTRL_OFFSET_CFG_CMAC_TX_EN_WIDTH                                                             (1)
typedef union CMAC_CMAC_FLOW_CTRL_u {
    struct __attribute__ ((packed)) {
        jl_uint32 cfg_cmac_rx_en:CMAC_CMAC_FLOW_CTRL_OFFSET_CFG_CMAC_RX_EN_WIDTH;                                         //[ 0: 0]
        jl_uint32 cfg_cmac_tx_en:CMAC_CMAC_FLOW_CTRL_OFFSET_CFG_CMAC_TX_EN_WIDTH;                                         //[ 1: 1]
        jl_uint32 rsvd0:CMAC_CMAC_FLOW_CTRL_OFFSET_RSVD0_WIDTH;                                                           //[31: 2]
    } BF;
    jl_uint32 val[CMAC_CMAC_FLOW_CTRL_ENTRY_SIZE];
} CMAC_CMAC_FLOW_CTRL_t;

#define CMAC_TX_IHALT                                                                          ((jl_uint32)0x00140E20U)
#define CMAC_TX_IHALT_NUM                                                                      ((jl_uint32)0x00000001U)
#define CMAC_TX_IHALT_ENTRY_SIZE                                                                                    (1)
#define CMAC_TX_IHALT_OFFSET_RSVD0                                                                                  (1)
#define CMAC_TX_IHALT_OFFSET_RSVD0_WIDTH                                                                           (31)
#define CMAC_TX_IHALT_OFFSET_DBG_CMAC2SWC_IHALT                                                                     (0)
#define CMAC_TX_IHALT_OFFSET_DBG_CMAC2SWC_IHALT_WIDTH                                                               (1)
typedef union CMAC_TX_IHALT_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_cmac2swc_ihalt:CMAC_TX_IHALT_OFFSET_DBG_CMAC2SWC_IHALT_WIDTH;                                       //[ 0: 0]
        jl_uint32 rsvd0:CMAC_TX_IHALT_OFFSET_RSVD0_WIDTH;                                                                 //[31: 1]
    } BF;
    jl_uint32 val[CMAC_TX_IHALT_ENTRY_SIZE];
} CMAC_TX_IHALT_t;

#define CMAC_STATISTIC_CTRL                                                                    ((jl_uint32)0x00140E24U)
#define CMAC_STATISTIC_CTRL_NUM                                                                ((jl_uint32)0x00000001U)
#define CMAC_STATISTIC_CTRL_ENTRY_SIZE                                                                              (1)
#define CMAC_STATISTIC_CTRL_OFFSET_RSVD0                                                                            (3)
#define CMAC_STATISTIC_CTRL_OFFSET_RSVD0_WIDTH                                                                     (29)
#define CMAC_STATISTIC_CTRL_OFFSET_DBG_STATISTIC_SMODE                                                              (0)
#define CMAC_STATISTIC_CTRL_OFFSET_DBG_STATISTIC_SMODE_WIDTH                                                        (1)
#define CMAC_STATISTIC_CTRL_OFFSET_DBG_STATISTIC_CLEAR                                                              (1)
#define CMAC_STATISTIC_CTRL_OFFSET_DBG_STATISTIC_CLEAR_WIDTH                                                        (1)
#define CMAC_STATISTIC_CTRL_OFFSET_DBG_STATISTIC_RC_EN                                                              (2)
#define CMAC_STATISTIC_CTRL_OFFSET_DBG_STATISTIC_RC_EN_WIDTH                                                        (1)
typedef union CMAC_STATISTIC_CTRL_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_statistic_smode:CMAC_STATISTIC_CTRL_OFFSET_DBG_STATISTIC_SMODE_WIDTH;                               //[ 0: 0]
        jl_uint32 dbg_statistic_clear:CMAC_STATISTIC_CTRL_OFFSET_DBG_STATISTIC_CLEAR_WIDTH;                               //[ 1: 1]
        jl_uint32 dbg_statistic_rc_en:CMAC_STATISTIC_CTRL_OFFSET_DBG_STATISTIC_RC_EN_WIDTH;                               //[ 2: 2]
        jl_uint32 rsvd0:CMAC_STATISTIC_CTRL_OFFSET_RSVD0_WIDTH;                                                           //[31: 3]
    } BF;
    jl_uint32 val[CMAC_STATISTIC_CTRL_ENTRY_SIZE];
} CMAC_STATISTIC_CTRL_t;

#define CMAC_TX_SOP_CNT_FR_SWC                                                                 ((jl_uint32)0x00140E28U)
#define CMAC_TX_SOP_CNT_FR_SWC_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_TX_SOP_CNT_FR_SWC_ENTRY_SIZE                                                                           (1)
#define CMAC_TX_SOP_CNT_FR_SWC_OFFSET_DBG_SWC2CMAC_SOP_CNT                                                          (0)
#define CMAC_TX_SOP_CNT_FR_SWC_OFFSET_DBG_SWC2CMAC_SOP_CNT_WIDTH                                                   (32)
typedef union CMAC_TX_SOP_CNT_FR_SWC_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_swc2cmac_sop_cnt:CMAC_TX_SOP_CNT_FR_SWC_OFFSET_DBG_SWC2CMAC_SOP_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_TX_SOP_CNT_FR_SWC_ENTRY_SIZE];
} CMAC_TX_SOP_CNT_FR_SWC_t;

#define CMAC_TX_EOP_CNT_FR_SWC                                                                 ((jl_uint32)0x00140E2CU)
#define CMAC_TX_EOP_CNT_FR_SWC_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_TX_EOP_CNT_FR_SWC_ENTRY_SIZE                                                                           (1)
#define CMAC_TX_EOP_CNT_FR_SWC_OFFSET_DBG_SWC2CMAC_EOP_CNT                                                          (0)
#define CMAC_TX_EOP_CNT_FR_SWC_OFFSET_DBG_SWC2CMAC_EOP_CNT_WIDTH                                                   (32)
typedef union CMAC_TX_EOP_CNT_FR_SWC_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_swc2cmac_eop_cnt:CMAC_TX_EOP_CNT_FR_SWC_OFFSET_DBG_SWC2CMAC_EOP_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_TX_EOP_CNT_FR_SWC_ENTRY_SIZE];
} CMAC_TX_EOP_CNT_FR_SWC_t;

#define CMAC_TX_ERR_CNT_FR_SWC                                                                 ((jl_uint32)0x00140E30U)
#define CMAC_TX_ERR_CNT_FR_SWC_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_TX_ERR_CNT_FR_SWC_ENTRY_SIZE                                                                           (1)
#define CMAC_TX_ERR_CNT_FR_SWC_OFFSET_DBG_SWC2CMAC_ERR_CNT                                                          (0)
#define CMAC_TX_ERR_CNT_FR_SWC_OFFSET_DBG_SWC2CMAC_ERR_CNT_WIDTH                                                   (32)
typedef union CMAC_TX_ERR_CNT_FR_SWC_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_swc2cmac_err_cnt:CMAC_TX_ERR_CNT_FR_SWC_OFFSET_DBG_SWC2CMAC_ERR_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_TX_ERR_CNT_FR_SWC_ENTRY_SIZE];
} CMAC_TX_ERR_CNT_FR_SWC_t;

#define CMAC_TX_SOP_CNT_TO_DMA                                                                 ((jl_uint32)0x00140E34U)
#define CMAC_TX_SOP_CNT_TO_DMA_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_TX_SOP_CNT_TO_DMA_ENTRY_SIZE                                                                           (1)
#define CMAC_TX_SOP_CNT_TO_DMA_OFFSET_DBG_CMAC2DMA_SOP_CNT                                                          (0)
#define CMAC_TX_SOP_CNT_TO_DMA_OFFSET_DBG_CMAC2DMA_SOP_CNT_WIDTH                                                   (32)
typedef union CMAC_TX_SOP_CNT_TO_DMA_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_cmac2dma_sop_cnt:CMAC_TX_SOP_CNT_TO_DMA_OFFSET_DBG_CMAC2DMA_SOP_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_TX_SOP_CNT_TO_DMA_ENTRY_SIZE];
} CMAC_TX_SOP_CNT_TO_DMA_t;

#define CMAC_TX_EOP_CNT_TO_DMA                                                                 ((jl_uint32)0x00140E38U)
#define CMAC_TX_EOP_CNT_TO_DMA_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_TX_EOP_CNT_TO_DMA_ENTRY_SIZE                                                                           (1)
#define CMAC_TX_EOP_CNT_TO_DMA_OFFSET_DBG_CMAC2DMA_EOP_CNT                                                          (0)
#define CMAC_TX_EOP_CNT_TO_DMA_OFFSET_DBG_CMAC2DMA_EOP_CNT_WIDTH                                                   (32)
typedef union CMAC_TX_EOP_CNT_TO_DMA_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_cmac2dma_eop_cnt:CMAC_TX_EOP_CNT_TO_DMA_OFFSET_DBG_CMAC2DMA_EOP_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_TX_EOP_CNT_TO_DMA_ENTRY_SIZE];
} CMAC_TX_EOP_CNT_TO_DMA_t;

#define CMAC_RX_SOP_CNT_FR_DMA                                                                 ((jl_uint32)0x00140E3CU)
#define CMAC_RX_SOP_CNT_FR_DMA_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_RX_SOP_CNT_FR_DMA_ENTRY_SIZE                                                                           (1)
#define CMAC_RX_SOP_CNT_FR_DMA_OFFSET_DBG_DMA2CMAC_SOP_CNT                                                          (0)
#define CMAC_RX_SOP_CNT_FR_DMA_OFFSET_DBG_DMA2CMAC_SOP_CNT_WIDTH                                                   (32)
typedef union CMAC_RX_SOP_CNT_FR_DMA_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_dma2cmac_sop_cnt:CMAC_RX_SOP_CNT_FR_DMA_OFFSET_DBG_DMA2CMAC_SOP_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_RX_SOP_CNT_FR_DMA_ENTRY_SIZE];
} CMAC_RX_SOP_CNT_FR_DMA_t;

#define CMAC_RX_EOP_CNT_FR_DMA                                                                 ((jl_uint32)0x00140E40U)
#define CMAC_RX_EOP_CNT_FR_DMA_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_RX_EOP_CNT_FR_DMA_ENTRY_SIZE                                                                           (1)
#define CMAC_RX_EOP_CNT_FR_DMA_OFFSET_DBG_DMA2CMAC_EOP_CNT                                                          (0)
#define CMAC_RX_EOP_CNT_FR_DMA_OFFSET_DBG_DMA2CMAC_EOP_CNT_WIDTH                                                   (32)
typedef union CMAC_RX_EOP_CNT_FR_DMA_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_dma2cmac_eop_cnt:CMAC_RX_EOP_CNT_FR_DMA_OFFSET_DBG_DMA2CMAC_EOP_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_RX_EOP_CNT_FR_DMA_ENTRY_SIZE];
} CMAC_RX_EOP_CNT_FR_DMA_t;

#define CMAC_RX_SOP_CNT_TO_SWC                                                                 ((jl_uint32)0x00140E44U)
#define CMAC_RX_SOP_CNT_TO_SWC_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_RX_SOP_CNT_TO_SWC_ENTRY_SIZE                                                                           (1)
#define CMAC_RX_SOP_CNT_TO_SWC_OFFSET_DBG_CMAC2SWC_SOP_CNT                                                          (0)
#define CMAC_RX_SOP_CNT_TO_SWC_OFFSET_DBG_CMAC2SWC_SOP_CNT_WIDTH                                                   (32)
typedef union CMAC_RX_SOP_CNT_TO_SWC_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_cmac2swc_sop_cnt:CMAC_RX_SOP_CNT_TO_SWC_OFFSET_DBG_CMAC2SWC_SOP_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_RX_SOP_CNT_TO_SWC_ENTRY_SIZE];
} CMAC_RX_SOP_CNT_TO_SWC_t;

#define CMAC_RX_EOP_CNT_TO_SWC                                                                 ((jl_uint32)0x00140E48U)
#define CMAC_RX_EOP_CNT_TO_SWC_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_RX_EOP_CNT_TO_SWC_ENTRY_SIZE                                                                           (1)
#define CMAC_RX_EOP_CNT_TO_SWC_OFFSET_DBG_CMAC2SWC_EOP_CNT                                                          (0)
#define CMAC_RX_EOP_CNT_TO_SWC_OFFSET_DBG_CMAC2SWC_EOP_CNT_WIDTH                                                   (32)
typedef union CMAC_RX_EOP_CNT_TO_SWC_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_cmac2swc_eop_cnt:CMAC_RX_EOP_CNT_TO_SWC_OFFSET_DBG_CMAC2SWC_EOP_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_RX_EOP_CNT_TO_SWC_ENTRY_SIZE];
} CMAC_RX_EOP_CNT_TO_SWC_t;

#define CMAC_RX_ERR_CNT_TO_SWC                                                                 ((jl_uint32)0x00140E4CU)
#define CMAC_RX_ERR_CNT_TO_SWC_NUM                                                             ((jl_uint32)0x00000001U)
#define CMAC_RX_ERR_CNT_TO_SWC_ENTRY_SIZE                                                                           (1)
#define CMAC_RX_ERR_CNT_TO_SWC_OFFSET_DBG_CMAC2SWC_ERR_CNT                                                          (0)
#define CMAC_RX_ERR_CNT_TO_SWC_OFFSET_DBG_CMAC2SWC_ERR_CNT_WIDTH                                                   (32)
typedef union CMAC_RX_ERR_CNT_TO_SWC_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_cmac2swc_err_cnt:CMAC_RX_ERR_CNT_TO_SWC_OFFSET_DBG_CMAC2SWC_ERR_CNT_WIDTH;                          //[31: 0]
    } BF;
    jl_uint32 val[CMAC_RX_ERR_CNT_TO_SWC_ENTRY_SIZE];
} CMAC_RX_ERR_CNT_TO_SWC_t;

#define CMAC_TX_PKT_JUMBO_CNT                                                                  ((jl_uint32)0x00140E50U)
#define CMAC_TX_PKT_JUMBO_CNT_NUM                                                              ((jl_uint32)0x00000001U)
#define CMAC_TX_PKT_JUMBO_CNT_ENTRY_SIZE                                                                            (1)
#define CMAC_TX_PKT_JUMBO_CNT_OFFSET_DBG_TX_PKT_JUMBO_CNT                                                           (0)
#define CMAC_TX_PKT_JUMBO_CNT_OFFSET_DBG_TX_PKT_JUMBO_CNT_WIDTH                                                    (32)
typedef union CMAC_TX_PKT_JUMBO_CNT_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_tx_pkt_jumbo_cnt:CMAC_TX_PKT_JUMBO_CNT_OFFSET_DBG_TX_PKT_JUMBO_CNT_WIDTH;                           //[31: 0]
    } BF;
    jl_uint32 val[CMAC_TX_PKT_JUMBO_CNT_ENTRY_SIZE];
} CMAC_TX_PKT_JUMBO_CNT_t;

#define CMAC_TX_PKT_DROP_CNT                                                                   ((jl_uint32)0x00140E54U)
#define CMAC_TX_PKT_DROP_CNT_NUM                                                               ((jl_uint32)0x00000001U)
#define CMAC_TX_PKT_DROP_CNT_ENTRY_SIZE                                                                             (1)
#define CMAC_TX_PKT_DROP_CNT_OFFSET_DBG_TX_PKT_DROP_CNT                                                             (0)
#define CMAC_TX_PKT_DROP_CNT_OFFSET_DBG_TX_PKT_DROP_CNT_WIDTH                                                      (32)
typedef union CMAC_TX_PKT_DROP_CNT_u {
    struct __attribute__ ((packed)) {
        jl_uint32 dbg_tx_pkt_drop_cnt:CMAC_TX_PKT_DROP_CNT_OFFSET_DBG_TX_PKT_DROP_CNT_WIDTH;                              //[31: 0]
    } BF;
    jl_uint32 val[CMAC_TX_PKT_DROP_CNT_ENTRY_SIZE];
} CMAC_TX_PKT_DROP_CNT_t;

#endif /* __JL_REG_CMAC_H__ */
