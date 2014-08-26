/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file       <Fls.c>
 *  @brief      <Flash hardware Module source file>
 *
 *  <Compiler: CodeWarrior V2.7    MCU:MPC5634>
 *  
 *  @author     <bo.zeng>
 *  @date       <21-08-2014>
 */
/*============================================================================*/

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20140821  bo.zeng     Initial version
 */
/*============================================================================*/
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define FLS_HW_C_AR_MAJOR_VERSION      2
#define FLS_HW_C_AR_MINOR_VERSION      2
#define FLS_HW_C_AR_PATCH_VERSION      3

#define FLS_HW_C_SW_MAJOR_VERSION      1
#define FLS_HW_C_SW_MINOR_VERSION      0
#define FLS_HW_C_SW_PATCH_VERSION      0

/*=======[I N C L U D E S]====================================================*/
#include "Fls.h"
#include "Fls_hw.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (FLS_HW_C_AR_MAJOR_VERSION != FLS_HW_H_AR_MAJOR_VERSION)
    #error "Fls.c:Mismatch in Specification Major Version"
#endif /* FLS_C_AR_MAJOR_VERSION != FLS_H_AR_MAJOR_VERSION */
#if (FLS_HW_C_AR_MINOR_VERSION != FLS_HW_H_AR_MINOR_VERSION)
    #error "Fls.c:Mismatch in Specification Minor Version"
#endif /* FLS_C_AR_MINOR_VERSION != FLS_H_AR_MINOR_VERSION */
#if (FLS_HW_C_AR_PATCH_VERSION != FLS_HW_H_AR_PATCH_VERSION)
    #error "Fls.c:Mismatch in Specification Patch Version"
#endif /* FLS_C_AR_PATCH_VERSION != FLS_H_AR_PATCH_VERSION */
#if (FLS_HW_C_SW_MAJOR_VERSION != FLS_HW_H_SW_MAJOR_VERSION)
    #error "Fls.c:Mismatch in Specification Major Version"
#endif /* FLS_C_SW_MAJOR_VERSION != FLS_H_SW_MAJOR_VERSION */
#if (FLS_HW_C_SW_MINOR_VERSION != FLS_HW_H_SW_MINOR_VERSION)
    #error "Fls.c:Mismatch in Specification Minor Version"
#endif /* FLS_C_SW_MINOR_VERSION != FLS_H_SW_MINOR_VERSION */

/*=======[M A C R O S]========================================================*/
#define FLS_HW_ADDRSPACE_UNLOCK(addrSpace)       \
    do{                                          \
        switch ((addrSpace))                     \
        {                                        \
            case FLS_BANK0ARRAY0_LOW:            \
                Fls_CtrlRegs[0]->Lml &= ~FLS_LML_LLK; \
                break;                           \
            case FLS_BANK0ARRAY0_MID:            \
                Fls_CtrlRegs[1]->Lml &= ~FLS_LML_MLK; \
                break;                           \
            case FLS_BANK1ARRAY1_HIHG:           \
                Fls_CtrlRegs[1]->Hlr &= ~FLS_HLR_HLK;\
                break;                           \
            case FLS_BANK1ARRAY2_HIHG:           \
                Fls_CtrlRegs[2]->Hlr &= ~FLS_HLR_HLK;\
                break;                           \
            case FLS_BANK0ARRAY0_SHADOW:         \
                Fls_CtrlRegs[0]->Lml &= ~FLS_LML_SLK;\
                break;                           \
        }                                        \
    }while(0)

#define FLS_HW_ADDRSPACE_LOCK(addrSpace)             \
    do{                                          \
        switch ((addrSpace))                     \
        {                                        \
            case FLS_BANK0ARRAY0_LOW:            \
                Fls_CtrlRegs[0]->Lml |= FLS_LML_LLK; \
                break;                           \
            case FLS_BANK0ARRAY0_MID:            \
                Fls_CtrlRegs[1]->Lml |= FLS_LML_MLK; \
                break;                           \
            case FLS_BANK1ARRAY1_HIGH:           \
                Fls_CtrlRegs[1]->Hlr |= FLS_HLR_HLK;\
                break;                           \
            case FLS_BANK1ARRAY2_HIGH:           \
                Fls_CtrlRegs[2]->Hlr |= FLS_HLR_HLK;\
                break;                           \
            case FLS_BANK0ARRAY0_SHADOW:         \
                Fls_CtrlRegs[0]->Lml |= FLS_LML_SLK;\
                break;                           \
        }                                        \
    }while(0)

#define FLS_HW_BLOCK_SELECT(addrSpace, blockIdx)  \
    do{                                           \
        switch ((addrSpace))                      \
        {                                         \
            case FLS_BANK0ARRAY0_LOW:             \
                Fls_CtrlRegs[0]->Lms &= ~FLS_LMS_LSL;\
                Fls_CtrlRegs[0]->Lms |= (1U << (blockIdx));\
                break;                            \
            case FLS_BANK0ARRAY0_MID:             \
                Fls_CtrlRegs[0]->Lms &= ~FLS_LMS_MSL;\
                Fls_CtrlRegs[0]->Lms |= (1U << (blockIdx)) << 16;\
                break;                            \
            case FLS_BANK1ARRAY1_HIGH:            \
                Fls_CtrlRegs[1]->Hbs = (1U<<(blockIdx));\
                break;                            \
            case FLS_BANK1ARRAY2_HIGH:            \
                Fls_CtrlRegs[1]->Hbs = (1U<<(blockIdx));\
                break;                            \
            case FLS_BANK0ARRAY0_SHADOW:          \
                break;                            \
        }                                         \
    }while(0)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
FUNC(boolean, FLS_CODE) Fls_HwInit 
(
    P2CONST(Fls_ConfigType, AUTOMATIC, FLS_APPL_DATA) ConfigPtr
)
{
    Fls_CtrlRegs[0]->Sll = FLS_SLL_PASSWORD; 
    Fls_CtrlRegs[0]->Lml = FLS_LML_PASSWORD; 
    Fls_CtrlRegs[1]->Hlr = FLS_HLR_PASSWORD; 
    Fls_CtrlRegs[2]->Hlr = FLS_HLR_PASSWORD; 

    Fls_CtrlRegs[0]->Sll = FLS_SLL_UNLOCK; 

    Fls_CtrlRegs[0]->Lml = FLS_LML_UNLOCK; 
    Fls_CtrlRegs[1]->Hlr = FLS_HLR_UNLOCK; 
    Fls_CtrlRegs[2]->Hlr = FLS_HLR_UNLOCK; 

    return TRUE;
}

FUNC(boolean, FLS_CODE) Fls_HwErase(Fls_AddressType addr, 
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg)
{
    uint8 blockIdx  = (addr-sectorCfg->FlsSectorStartAddr)/sectorCfg->FlsSectorSize 
                      + sectorCfg->FlsStartBlock;
    Fls_HwAddrSpaceType addrSpace = sectorCfg->FlsAddrSpace;
    uint8 flsIdx    = addrSpace >> 4U;
    boolean ret = FALSE;

    /* 
     * erase block step 
     */
    Fls_CtrlRegs[flsIdx]->MCR |= FLS_MCR_ERS;

    FLS_HW_BLOCK_SELECT(flsIdx, blockIdx);

    *sectorCfg->FlsSectorStartAddr = 0xDEADBEEF;

    Fls_CtrlRegs[flsIdx]->MCR |= FLS_MCR_EHV;

    while (FLS_MCR_DONE != (Fls_CtrlRegs[flsIdx]->MCR & FLS_MCR_DONE));

    if (FLS_MCR_PEG != (Fls_CtrlRegs[flsIdx]->MCR & FLS_MCR_PEG))
    {
        ret = FALSE;
    }

    Fls_CtrlRegs[flsIdx]->MCR &= ~FLS_MCR_EHV;

    Fls_CtrlRegs[flsIdx]->MCR &= ~FLS_MCR_ERS;

    return ret;
}

FUNC(boolean, FLS_CODE) Fls_HwWrite(
    Fls_AddressType targetAddress, 
    Fls_AddressType sourceAddress, 
    Fls_LengthType length,
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg
)
{
    uint8 flsIdx = sectorCfg->FlsAddrSpace >> 4U;
    boolean ret = FALSE;
    P2CONST(uint32, AUTOMATIC, FLS_APPL_DATA) dest 
        = P2CONST(uint32, AUTOMATIC, FLS_APPL_DATA) targetAddress;
    P2CONST(uint32, AUTOMATIC, FLS_APPL_DATA) src 
        = P2CONST(uint32, AUTOMATIC, FLS_APPL_DATA) sourceAddress;

    /* write data */
    Fls_CtrlRegs[flsIdx]->MCR |= FLS_MCR_PEG;

    while (length > 0x0U)
    {
        *dest++=*src++;        
        *dest++=*src++;        
        length -= 8U;
        Fls_CtrlRegs[flsIdx]->MCR |= FLS_MCR_EHV;

        while (FLS_MCR_DONE != (Fls_CtrlRegs[flsIdx]->MCR & FLS_MCR_DONE));
        if (FLS_MCR_PEG != (Fls_CtrlRegs[flsIdx]->MCR & FLS_MCR_PEG))
        {
            ret = FALSE;
        }
        Fls_CtrlRegs[flsIdx]->MCR &= ~FLS_MCR_EHV;
    }

    Fls_CtrlRegs[flsIdx]->MCR &= ~FLS_MCR_PEG;

    return ret;
}

FUNC(boolean, FLS_CODE) Fls_HwRead(
    Fls_AddressType targetAddress, 
    Fls_AddressType sourceAddress, 
    Fls_LengthType length,
)
{
    P2CONST(uint8, AUTOMATIC, FLS_APPL_DATA) dest 
        = P2CONST(uint32, AUTOMATIC, FLS_APPL_DATA) targetAddress;
    P2CONST(uint8, AUTOMATIC, FLS_APPL_DATA) src 
        = P2CONST(uint32, AUTOMATIC, FLS_APPL_DATA) sourceAddress;

    /* read data */
    while (length > 0U)
    {
        *dest++ = *src++;
        length--;
    }

    return TRUE;
}

FUNC(boolean, FLS_CODE) Fls_HwCompare(
    Fls_AddressType targetAddress, 
    Fls_AddressType sourceAddress, 
    Fls_LengthType length,
)
{
    boolean ret = TRUE;
    P2CONST(uint8, AUTOMATIC, FLS_APPL_DATA) dest 
        = P2CONST(uint32, AUTOMATIC, FLS_APPL_DATA) targetAddress;
    P2CONST(uint8, AUTOMATIC, FLS_APPL_DATA) src 
        = P2CONST(uint32, AUTOMATIC, FLS_APPL_DATA) sourceAddress;

    /* compare data */
    while (length > 0U)
    {
        if (*dest++ != *src++)
        {
            ret = FALSE;
            break;
        }
        length--;
    }

    return ret;
}

#define FLS_STOP_SEC_CODE
#include "Fls_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/

