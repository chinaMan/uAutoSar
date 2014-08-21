/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <Fls_Cfg.h>
 *  @brief      <Flash module configuration generation header file>
 *  
 *  <Compiler: CodeWarrior V2.7    MCU:MPC5634>
 *  
 *  @author     <bo.zeng>
 *  @date       <21-08-2014>
 */
/*============================================================================*/
#ifndef FLS_CFG_H
#define FLS_CFG_H

/*=======[F I L E  V E R S I O N   I N F O R M A T I O N]===============================*/
#define FLS_CFG_H_AR_MAJOR_VERSION     2U 
#define FLS_CFG_H_AR_MINOR_VERSION     2U 
#define FLS_CFG_H_AR_PATCH_VERSION     3U 
#define FLS_CFG_H_SW_MAJOR_VERSION     1U
#define FLS_CFG_H_SW_MINOR_VERSION     0U
#define FLS_CFG_H_SW_PATCH_VERSION     0U 
 
/*=======[R E L E A S E   V E R S I O N   I N F O R M A T I O N]===============================*/
#define FLS_VENDOR_ID                 62U         /* Replace with a symbolic name once available */
#define FLS_MODULE_ID                 80U         /* from AUTOSAR_BasicSoftwareModules.pdf */
#define FLS_INSTANCE                  0U
#define FLS_AR_MAJOR_VERSION          2U 
#define FLS_AR_MINOR_VERSION          2U 
#define FLS_AR_PATCH_VERSION          3U 
#define FLS_SW_MAJOR_VERSION          1U
#define FLS_SW_MINOR_VERSION          0U
#define FLS_SW_PATCH_VERSION          0U 

/*=======[M A C R O S]========================================================*/
/*
 * Variants Class (PC, PB)
 */
#define FLS_VARIANT_PC   0
#define FLS_VARIANT_PB   1
#define FLS_VARIANT_CFG  FLS_VARIANT_PC

/*
 * FlsGeneral
 */
#define FLS_DEV_ERROR_DETECT            STD_OFF    /* development error detection */
#define FLS_DEM_ERROR_DETECT            STD_OFF    /* public error detection */

#define FLS_AC_LOAD_ON_JOB_START        STD_ON     
#define FLS_CANCEL_API                  STD_OFF
#define FLS_COMPARE_API                 STD_OFF
#define FLS_GET_JOB_RESULT_API          STD_OFF
#define FLS_GET_STATUS_API              STD_OFF
#define FLS_SET_MODE_API                STD_OFF
#define FLS_VERSION_INFO_API            STD_OFF

#define FLS_INSTANCE                    0x0U 

#define FLS_BASE_ADDRESS                0x00000000U
#define FLS_TOTAL_SIZE                  0x4000U

#define FLS_USE_INTERRUPTS              STD_ON

#endif /* #define FLS_CFG_H */

/*=======[E N D   O F   F I L E]==============================================*/
