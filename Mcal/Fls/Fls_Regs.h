/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <Fls_Regs.h>
 *  @brief      <Flash registers header file>
 *  
 *  <Compiler: CodeWarrior V2.7    MCU:MPC5634>
 *  
 *  @author     <bo.zeng>
 *  @date       <21-08-2014>
 */
/*============================================================================*/
#ifndef  FLS_REGS_H
#define  FLS_REGS_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20140821  bo.zeng     Initial version
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define FLS_H_AR_MAJOR_VERSION  2U
#define FLS_H_AR_MINOR_VERSION  2U
#define FLS_H_AR_PATCH_VERSION  3U

#define FLS_H_SW_MAJOR_VERSION  1U
#define FLS_H_SW_MINOR_VERSION  0U
#define FLS_H_SW_PATCH_VERSION  0U

/*=======[I N C L U D E S]====================================================*/

/*=======[M A C R O S]========================================================*/
/* 
 * Registers bit mask
 */
#define   FLS_PFCR1_GCE         0x10000000U
#define   FLS_PFCR1_M3PFE       0x10000000U
#define   FLS_PFCR1_M2PFE       0x10000000U
#define   FLS_PFCR1_M1PFE       0x10000000U
#define   FLS_PFCR1_M0PFE       0x10000000U
#define   FLS_PFCR1_APC         0x10000000U
#define   FLS_PFCR1_WWSC        0x10000000U
#define   FLS_PFCR1_RWSC        0x10000000U
#define   FLS_PFCR1_DPFE        0x10000000U
#define   FLS_PFCR1_IPFE        0x10000000U
#define   FLS_PFCR1_PFLIM       0x10000000U
#define   FLS_PFCR1_BFE         0x10000000U


#define   FLS_SLL_SLE           0x0000000FU
/* 
 * Registers value define 
 */
#define   FLS_PFCR1_VALUE
#define   FLS_PFAPR_VALUE
#define   FLS_PFCR2_VALUE
#define   FLS_PFCR3_VALUE

#define   FLS_SLL_PASSWORD      0xC3C33333U
#define   FLS_LML_PASSWORD      0xA1A11111U
#define   FLS_HLR_PASSWORD      0xB2B22222U

#define   FLS_SLL_UNLOCK        0x00U
#define   FLS_LML_UNLOCK        0x00U
#define   FLS_HLR_UNLOCK        0x00U

#define   FLS_LML_LOCK          0x00U
#define   FLS_HLR_LOCK          0x0FU

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef struct
{
    uint32   Mcr;
    uint32   Lml;
    uint32   Hbl;
    uint32   Sll;
    uint32   Lms;
    uint32   Hbs;
    uint32   Adr;

    uint32   Pfcr1;
    uint32   Pfapr;
    uint32   Pfcr2;
    uint32   Pfcr3;

    uint32   Ut0;
    uint32   Ut1;
    uint32   Ut2;
    uint32   Umisr0;
    uint32   Umisr1;
    uint32   Umisr2;
    uint32   Umisr3;
    uint32   Umisr4;
} Fls_BankRegsType;

#endif  /* #ifndef FLS_REGS_H */

/*=======[E N D   O F   F I L E]==============================================*/

