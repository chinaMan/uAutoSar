/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <Fls.h>
 *  @brief      <Flash hardware module header file>
 *  
 *  <Compiler: CodeWarrior V2.7    MCU:MPC5634>
 *  
 *  @author     <bo.zeng>
 *  @date       <21-08-2014>
 */
/*============================================================================*/
#ifndef  FLS_H
#define  FLS_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20140821  bo.zeng     Initial version
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define FLS_HW_H_AR_MAJOR_VERSION  2U
#define FLS_HW_H_AR_MINOR_VERSION  2U
#define FLS_HW_H_AR_PATCH_VERSION  3U

#define FLS_HW_H_SW_MAJOR_VERSION  1U
#define FLS_HW_H_SW_MINOR_VERSION  0U
#define FLS_HW_H_SW_PATCH_VERSION  0U

/*=======[I N C L U D E S]====================================================*/

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"

extern FUNC(boolean, FLS_CODE) Fls_HwInit(void);
extern FUNC(boolean, FLS_CODE) Fls_HwErase
(
    Fls_AddressType addr, 
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg
);
extern FUNC(boolean, FLS_CODE) Fls_HwWrite 
(
    Fls_AddressType targetAddress, 
    Fls_AddressType sourceAddress, 
    Fls_LengthType length,
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg
);
extern FUNC(boolean, FLS_CODE) Fls_HwRead 
(
    Fls_AddressType targetAddress, 
    Fls_AddressType sourceAddress, 
    Fls_LengthType length,
);
extern FUNC(boolean, FLS_CODE) Fls_HwCompare 
(
    Fls_AddressType targetAddress, 
    Fls_AddressType sourceAddress, 
    Fls_LengthType length,
);

#define Fls_STOP_SEC_CODE
#include "Fls_MemMap.h"

#endif  /* #ifndef FLS_HW_H */

/*=======[E N D   O F   F I L E]==============================================*/

