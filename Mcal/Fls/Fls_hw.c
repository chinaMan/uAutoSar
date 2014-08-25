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
#define FLS_HW_BLOCK_UNLOCK(addrSpace, blockIdx) \
    do{                                          \
        switch (addrSpace)                       \
        {                                        \
            case FLS_BANK_0:                     \
                Fls_CtrlRegs[0]->Lml =          ;\
                break;                           \
            case FLS_BANK_1:                     \
                Fls_CtrlRegs[1]->Hlr =          ;\
                break;                           \
            case FLS_BANK_2:                     \
                Fls_CtrlRegs[1]->Hlr =          ;\
                break;                           \
        }
    }while(0)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
STATIC FUNC(Fls_HwBankType, FLS_CODE) Fls_GetCntrlIdx(Fls_AddressType addr)
{
    Fls_HwBankType flsBank = FLS_BANK_0;

    if ((addr < 0x80000U)
            || ((addr >= 0xFFC000U) && (addr < 0x1000000U)))
    {
        flsBank = FLS_BANK_0;
    }
    else if (addr < 0x100000U)
    {
        flsBank = FLS_BANK_1;
    }
    else if (addr < 0x180000U)
    {
        flsBank = FLS_BANK_2;
    }
    else
    {
        /* do nothing */
    }

    return flsBank;
}

STATIC FUNC(Fls_HwAddrSpaceType, FLS_CODE) Fls_AddrSpace(Fls_AddressType addr)
{
    Fls_HwAddrSpaceType flsAddrSpace = FLS_SPACE_ADDR_LOW;

    if (addr < 0x40000U)
    {
        flsAddrSpace = FLS_SPACE_ADDR_LOW;
    }
    else if (addr < 0x800000U)
    {
        flsAddrSpace = FLS_SPACE_ADDR_MID;
    }
    else if (addr < 0xFFC000U)
    {
        flsAddrSpace = FLS_SPACE_ADDR_HIGH;
    }
    else if (addr < 0x1000000U)
    {
        flsAddrSpace = FLS_SPACE_ADDR_SHADOW;
    }
    else
    {
        /* do nothing */
    }

    return flsAddrSpace;
}

STATIC FUNC(uint8, FLS_CODE) Fls_GetBlockIdx(Fls_AddressType addr, 
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg)
{
    uint32 i = 0U;

    for (i = 0; i < sectorCfg->FlsNumberOfSectors; i++)
    {
        
    }

}

FUNC(boolean, FLS_CODE) Fls_HwInit(void)
{
    Fls_CtrlRegs[0]->Sll = FLS_SLL_PASSWORD; 
    Fls_CtrlRegs[0]->Lml = FLS_LML_PASSWORD; 
    Fls_CtrlRegs[1]->Hlr = FLS_HLR_PASSWORD; 
    Fls_CtrlRegs[2]->Hlr = FLS_HLR_PASSWORD; 

    Fls_CtrlRegs[0]->Sll = FLS_SLL_UNLOCK; 

    Fls_CtrlRegs[0]->Lml = FLS_LML_LOCK; 
    Fls_CtrlRegs[1]->Hlr = FLS_HLR_LOCK; 
    Fls_CtrlRegs[2]->Hlr = FLS_HLR_LOCK; 

    return TRUE;
}

FUNC(boolean, FLS_CODE) Fls_HwErase(Fls_AddressType addr, 
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg)
{
    uint8 flsIdx    = sectorCfg->FlsBank;
    uint8 blockIdx  = (addr - sectorCfg->FlsSectorStartAddr) / sectorCfg->FlsSectorSize;
    Fls_HwAddrSpaceType addrSpace = sectorCfg->FlsAddrSpace;
    boolean ret = FALSE;

    /* unlock this block */
    FLS_HW_BLOCK_UNLOCK(addrSpace, blockIdx);
    
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

    /* lock this block */
    FLS_HW_BLOCK_LOCK(flsIdx, blockIdx);

    return ret;
}

FUNC(boolean, FLS_CODE) Fls_HwWrite(Fls_AddressType addr, Fls_LengthType length)
{
    boolean ret = FALSE;

    /* write data */

    return ret;
}

FUNC(boolean, FLS_CODE) Fls_HwRead(Fls_AddressType addr, Fls_LengthType length)
{
    boolean ret = FALSE;

    /* read data */

    return ret;
}

FUNC(boolean, FLS_CODE) Fls_HwCompare(void)
{}

#define FLS_STOP_SEC_CODE
#include "Fls_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/

