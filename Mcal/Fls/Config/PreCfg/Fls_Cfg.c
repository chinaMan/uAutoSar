/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Fls_Cfg.c>
 *  @brief      <Pre-Compiler Flash Configure>
 *  
 * <Compiler: CodeWarrior V2.7    MCU:MPC5634>
 *  
 *  @author     <bo.zeng>
 *  @date       <25-08-2014>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>      <DATE>     <AUTHOR>      <REVISION LOG>
 *   V1.0.0       20140825     bo.zeng      Initial version
 */
/*============================================================================*/

/*=======[I N C L U D E S]====================================================*/
#include "Fls.h"

/*=======[V E R S I O N   I N F O R M A T I O N]===============================*/
#define FLS_CFG_C_AR_MAJOR_VERSION     2U 
#define FLS_CFG_C_AR_MINOR_VERSION     4U 
#define FLS_CFG_C_AR_PATCH_VERSION     0U 
#define FLS_CFG_C_SW_MAJOR_VERSION     1U
#define FLS_CFG_C_SW_MINOR_VERSION     0U
#define FLS_CFG_C_SW_PATCH_VERSION     0U 

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (FLS_CFG_C_AR_MAJOR_VERSION != FLS_CFG_H_AR_MAJOR_VERSION)
    #error "Fls_Cfg.c:Mismatch in Specification Major Version"
#endif 
#if (FLS_CFG_C_AR_MINOR_VERSION != FLS_CFG_H_AR_MINOR_VERSION)
    #error "Fls_Cfg.c:Mismatch in Specification Minor Version"
#endif
#if (FLS_CFG_C_AR_PATCH_VERSION != FLS_CFG_H_AR_PATCH_VERSION)
    #error "Fls_Cfg.c:Mismatch in Specification Patch Version"
#endif
#if (FLS_CFG_C_SW_MAJOR_VERSION != FLS_CFG_H_SW_MAJOR_VERSION)
    #error "Fls_Cfg.c:Mismatch in Specification Major Version"
#endif
#if (FLS_CFG_C_SW_MINOR_VERSION != FLS_CFG_H_SW_MINOR_VERSION)
    #error "Fls_Cfg.c:Mismatch in Specification Minor Version"
#endif

/*=======[E X T E R N A L   D A T A]==========================================*/
#define FLS_START_CONST_PBCFG
#include "Fls_MemMap.h"

STATIC CONST(Fls_SectorConfigType, FLS_CONST_PBCFG) Fls_SectorConfigData[7] = 
{
    {
        0x00000000U,
        16*1024U,
        8U,
        2,
        FLS_SPACE_ADDR_LOW,
        FLS_BANK_0
    },
    {
        0x00008000U,
        32*1024U,
        8U,
        2,
        FLS_SPACE_ADDR_LOW,
        FLS_BANK_0
    },
    {
        0x00018000U,
        16*1024U,
        8U,
        2,
        FLS_SPACE_ADDR_LOW,
        FLS_BANK_0
    },
    {
        0x00020000U,
        64*1024U,
        8U,
        2,
        FLS_SPACE_ADDR_LOW,
        FLS_BANK_0
    },
    {
        0x00040000U,
        128*1024U,
        8U,
        2,
        FLS_SPACE_ADDR_MID,
        FLS_BANK_0
    },
    {
        0x00080000U,
        128*1024U,
        8U,
        4
        FLS_SPACE_ADDR_HIGH,
        FLS_BANK_1
    },
    {
        0x00100000U,
        128*1024U,
        8U,
        4,
        FLS_SPACE_ADDR_HIGH,
        FLS_BANK_2
    },
    {
        0x00FFC000U,
        16*1024U,
        8U,
        1,
        FLS_SPACE_ADDR_SHADOW,
        FLS_BANK_0
    }
};
#define FLS_STOP_CONST_PBCFG
#include "Fls_MemMap.h"

#define FLS_START_SEC_CONST_UNSPECIFIED
#include "Fls_MemMap.h"
CONST(Fls_ConfigType, FLS_CONST) Fls_ConfigData = 
{
    NULL_PTR,                 /* FlsAcErase */
    NULL_PTR,                 /* FlsAcWrite */
    NULL_PTR,                 /* FlsJobEndNotification */
    NULL_PTR,                 /* FlsJobErrorNotification */
    100,                      /* FlsCallCycle */
    10,                       /* FlsMaxReadFastMode */
    1,                        /* FlsMaxReadNomralMode */
    10,                       /* FlsMaxWriteFastMode */
    1,                        /* FlsMaxWriteNomralMode */
    0,                        /* FlsProtection */
    7,                        /* FlsSectorListSize */
    Fls_SectorConfigData      /* FlsSectorList */
};
#define FLS_STOP_SEC_CONST_UNSPECIFIED
#include "Fls_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/

