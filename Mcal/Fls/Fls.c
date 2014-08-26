/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file       <Fls.c>
 *  @brief      <Flash driver Module source file>
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
#define FLS_C_AR_MAJOR_VERSION      2
#define FLS_C_AR_MINOR_VERSION      2
#define FLS_C_AR_PATCH_VERSION      3

#define FLS_C_SW_MAJOR_VERSION      1
#define FLS_C_SW_MINOR_VERSION      0
#define FLS_C_SW_PATCH_VERSION      0

/*=======[I N C L U D E S]====================================================*/
#include "Fls.h"
#if (STD_ON == FLS_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* STD_ON == FLS_DEV_ERROR_DETECT */
#if (STD_ON == FLS_DEM_ERROR_DETECT)
#include "Dem.h"
#endif /* STD_ON == FLS_DEM_ERROR_DETECT */

/*=======[V E R S I O N  C H E C K]===========================================*/
#if (FLS_C_AR_MAJOR_VERSION != FLS_H_AR_MAJOR_VERSION)
    #error "Fls.c:Mismatch in Specification Major Version"
#endif /* FLS_C_AR_MAJOR_VERSION != FLS_H_AR_MAJOR_VERSION */
#if (FLS_C_AR_MINOR_VERSION != FLS_H_AR_MINOR_VERSION)
    #error "Fls.c:Mismatch in Specification Minor Version"
#endif /* FLS_C_AR_MINOR_VERSION != FLS_H_AR_MINOR_VERSION */
#if (FLS_C_AR_PATCH_VERSION != FLS_H_AR_PATCH_VERSION)
    #error "Fls.c:Mismatch in Specification Patch Version"
#endif /* FLS_C_AR_PATCH_VERSION != FLS_H_AR_PATCH_VERSION */
#if (FLS_C_SW_MAJOR_VERSION != FLS_H_SW_MAJOR_VERSION)
    #error "Fls.c:Mismatch in Specification Major Version"
#endif /* FLS_C_SW_MAJOR_VERSION != FLS_H_SW_MAJOR_VERSION */
#if (FLS_C_SW_MINOR_VERSION != FLS_H_SW_MINOR_VERSION)
    #error "Fls.c:Mismatch in Specification Minor Version"
#endif /* FLS_C_SW_MINOR_VERSION != FLS_H_SW_MINOR_VERSION */

#if (STD_ON == FLS_DEV_ERROR_DETECT)
#if (2 != DET_H_AR_MAJOR_VERSION)
    #error "Fls.c:Mismatch in Specification Major Version"
#endif
#if (2 != DET_H_AR_MINOR_VERSION)
    #error "Fls.c:Mismatch in Specification Minor Version"
#endif
#endif /* STD_ON == FLS_DEV_ERROR_DETECT */

#if (STD_ON == FLS_DEM_ERROR_DETECT)
#if (3 != DEM_H_AR_MAJOR_VERSION)
    #error "Fls.c:Mismatch in Specification Major Version"
#endif
#if (1 != DEM_H_AR_MINOR_VERSION)
    #error "Fls.c:Mismatch in Specification Minor Version"
#endif
#endif /* STD_ON == FLS_DEM_ERROR_DETECT */

/*=======[M A C R O S]========================================================*/
#if (FLS_VARIANT_PB == FLS_VARIANT_CFG)   
#define FLS_CONFIG Fls_RtData.ConfigPtr
#else
#define FLS_CONFIG &Fls_ConfigData
#endif /* #if (FLS_VARIANT_PB == FLS_VARIANT_CFG) */

#define FLS_JOB_ERROR_NOTIFICATION()   \
    do {                               \
        if (NULL_PTR != FLS_CONFIG->FlsJobEndNotification) \
        {                              \
            FLS_CONFIG->FlsJobEndNotification(); \
        }                              \
    } while(0)

#define FLS_JOB_END_NOTIFICATION()     \
    do {                               \
        if (NULL_PTR != FLS_CONFIG->FlsJobEndNotification) \
        {                              \
            FLS_CONFIG->FlsJobEndNotification(); \
        }                              \
    } while(0)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* job type */
typedef enum
{
    FLS_JOB_NONE,
    FLS_JOB_COMPARE,
    FLS_JOB_ERASE,
    FLS_JOB_READ,
    FLS_JOB_WRITE
} Fls_JobType;

/* the runtimg data structure of flash */
typedef struct
{
    #if (FLS_VARIANT_PB == FLS_VARIANT_CFG)   
    P2CONST(Can_ConfigType, CAN_CONST, CAN_CONST_PBCFG) ConfigPtr;
    #endif
    MemIf_StatusType      Status;
    MemIf_ModeType        Mode;

    Fls_JobType           Job;
    MemIf_JobResultType   JobResult;
    Fls_AddressType       SourceAddr;
    Fls_AddressType       TargetAddr;
    Fls_LengthType        Length;
} Fls_RtType;

/*=======[I N T E R N A L   D A T A]==========================================*/
/* Controller Runtime structure */
#define FLS_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Fls_MemMap.h"
STATIC VAR(Fls_RtType, FLS_VAR) Fls_RtData;
#define FLS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Fls_MemMap.h"

#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
STATIC FUNC(boolean, FLS_CODE) Fls_CheckPageStartAlign(Fls_AddressType addr);
STATIC FUNC(boolean, FLS_CODE) Fls_CheckPageEndAlign(Fls_AddressType addr);
STATIC FUNC(boolean, FLS_CODE) Fls_CheckSectorStartAlign(Fls_AddressType addr);
STATIC FUNC(boolean, FLS_CODE) Fls_CheckSectorEndAlign(Fls_AddressType addr);

STATIC FUNC(void, FLS_CODE) Fls_JobErase(void)
STATIC FUNC(void, FLS_CODE) Fls_JobRead(void)
STATIC FUNC(void, FLS_CODE) Fls_JobWrite(void)
STATIC FUNC(void, FLS_CODE) Fls_JobCompare(void)

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/******************************************************************************/
/*
 * Brief               <This function shall initialize Flash>
 * ServiceId           <0x00>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <ConfigPtr>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>  
 * PreCondition        <None>  
 */
/******************************************************************************/
FUNC(void, FLS_CODE) Fls_Init
(
    P2CONST(Fls_ConfigType, AUTOMATIC, FLS_APPL_DATA) ConfigPtr
)
{
    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_BUSY == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_INIT_ID, FLS_E_BUSY);
    }
    #if (FLS_VARIANT_PB == FLS_VARIANT_CFG)   
    else if (NULL_PTR == ConfigPtr)                                          
    {                                                                   
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_INIT_ID, FLS_E_PARAM_CONFIG);
    }
    #endif  /* #if (FLS_VARIANT_PB == FLS_VARIANT_CFG) */
    else
    #endif  /* #if (FLS_DEV_ERROR_DETECT == STD_ON) */
    {
        /* backup config pointer */
        #if (FLS_VARIANT_PB == FLS_VARIANT_CFG)   
        Fls_RtData.ConfigPtr = Config;
        #endif /* #if (FLS_VARIANT_PB == FLS_VARIANT_CFG) */

        /* init runtime data structure */
        Fls_RtData.Status      = MEMIF_UNINIT;
        Fls_RtData.Mode        = MEMIF_MODE_SLOW;
        Fls_RtData.Job         = FLS_JOB_NONE;
        Fls_RtData.SourceAddr  = 0x0U;
        Fls_RtData.TargetAddr  = 0x0U;
        Fls_RtData.Length      = 0x0U;

        /* @req <FLS014> */
        /* init flash hardware */
        (void)Fls_HwInit(FLS_CONFIG);

        /* @req <FLS016> */
        Fls_RtData.Status    = MEMIF_IDLE;
        Fls_RtData.JobResult = MEMIF_JOB_OK;
    }

    return;
}

/******************************************************************************/
/*
 * Brief               <This function shall service for erasing FLS.>
 * ServiceId           <0x04>
 * Sync/Async          <Asynchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <TargetAddress,Length>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>  
 * PreCondition        <None>  
 */
/******************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Erase
(
    Fls_AddressType TargetAddress, 
    Fls_LengthType Length
)
{
    Std_ReturnType ret = E_NOT_OK;

    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_ERASE_ID, FLS_E_UNINIT);
    }
    else if (MEMIF_BUSY == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_ERASE_ID, FLS_E_BUSY);
    }
    else if (TRUE == Fls_CheckSectorStartAlign(TargetAddress))                                          
    {                                                                   
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_ERASE_ID, FLS_E_PARAM_ADDRESS);
    }
    else if ((Length <= 0U) && (TRUE == Fls_CheckSectorEndAlign(TargetAddress+Length)))
    {                                                                   
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_ERASE_ID, FLS_E_PARAM_LENGTH);
    }
    else
    #endif  /* #if (FLS_DEV_ERROR_DETECT == STD_ON) */
    {
        /* @req <FLS219> */
        Fls_RtData.Status     = MEMIF_BUSY;
        Fls_RtData.JobResult  = MEMIF_JOB_PENDING;
        Fls_RtData.Job        = FLS_JOB_ERASE;
        Fls_RtData.SourceAddr = TargetAddress;
        Fls_RtData.Length     = Length;

        /* @req <FLS145> */
        #if (STD_ON == FLS_USE_INTERRUPTS)              
        Fls_JobErase();
        #endif

        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               <This function shall writes to FLS.>
 * ServiceId           <0x03>
 * Sync/Async          <Asynchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <TargetAddress,SourceAddressPtr,Length>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>  
 * PreCondition        <None>  
 */
/******************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Write
(
    Fls_AddressType TargetAddress, 
    P2CONST(uint8, AUTOMATIC, FLS_APPL_DATA) SourceAddressPtr, 
    Fls_LengthType Length
)
{
    Std_ReturnType ret = E_NOT_OK;

    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_WRITE_ID, FLS_E_UNINIT);
    }
    else if (MEMIF_BUSY == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_WRITE_ID, FLS_E_BUSY);
    }
    else if (TRUE == Fls_CheckPageStartAlign(TargetAddress))                                          
    {                                                                   
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_WRITE_ID, FLS_E_PARAM_ADDRESS);
    }
    else if ((Length <= 0U) && ((FALSE == Fls_CheckPageEndAlign(TargetAddress+Length))))
    {                                                                   
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_WRITE_ID, FLS_E_PARAM_LENGTH);
    }
    else if (NULL_PTR == SourceAddressPtr)
    {                                                                   
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_WRITE_ID, FLS_E_PARAM_POINTER);
    }
    else
    #endif  /* #if (FLS_DEV_ERROR_DETECT == STD_ON) */
    {
        /* @req <FLS224> */
        Fls_RtData.Status     = MEMIF_BUSY;
        Fls_RtData.JobResult  = MEMIF_JOB_PENDING;
        Fls_RtData.Job        = FLS_JOB_WRITE;
        Fls_RtData.SourceAddr = TargetAddress;
        Fls_RtData.TargetAddr = SourceAddressPtr;
        Fls_RtData.Length     = Length;

        /* @req <FLS146> */
        #if (STD_ON == FLS_USE_INTERRUPTS)              
        Fls_JobWrite();
        #endif

        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               <This function cancels a running job.>
 * ServiceId           <0x06>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>  
 * PreCondition        <None>  
 */
/******************************************************************************/
#if (FLS_CANCEL_API == STD_ON)
FUNC(void, FLS_CODE) Fls_Cancel(void)
{
    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_CANCEL_ID, FLS_E_UNINIT);
    }
    else
    #endif  /* #if (FLS_DEV_ERROR_DETECT == STD_ON) */
    {
        /* @req <FLS230> */
        /* @req <FLS032> */
        Fls_RtData.SourceAddr = 0x0U;
        Fls_RtData.TargetAddr = 0x0U;
        Fls_RtData.Length     = 0x0U;
        Fls_RtData.Status     = MEMIF_IDLE;
        Fls_RtData.Job        = FLS_JOB_NONE;

        /* @req <FLS033> */
        if (MEMIF_JOB_PENDING == Fls_RtData.JobResult)
        {
            Fls_RtData.JobResult = MEMIF_JOB_CANCELED;
        }

        /* @req <FLS147> */
        FLS_JOB_ERROR_NOTIFICATION();
    }
}
#endif

/******************************************************************************/
/*
 * Brief               <This function returns the FLS status.>
 * ServiceId           <0x07>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <MemIf_StatusType>  
 * PreCondition        <None>  
 */
/******************************************************************************/
#if (STD_ON == FLS_GET_STATUS_API)
FUNC(MemIf_StatusType, FLS_CODE) Fls_GetStatus(void)
{
    MemIf_StatusType status;

    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_GETSTATUS_ID, FLS_E_UNINIT);
        status = MEMIF_UNINIT;
    }
    else
    #endif  /* #if (FLS_DEV_ERROR_DETECT == STD_ON) */
    {
        status = Fls_RtData.Status;
    }

    return status;
}
#endif

/******************************************************************************/
/*
 * Brief               <This function returns the result of the last job.>
 * ServiceId           <0x05>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <MemIf_JobResultType>  
 * PreCondition        <None>  
 */
/******************************************************************************/
#if (STD_ON == FLS_GET_JOB_RESULT_API)
FUNC(MemIf_JobResultType, FLS_CODE) Fls_GetJobResult(void)
{
    MemIf_JobResultType jobResult;

    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_GETSTATUS_ID, FLS_E_UNINIT);
        jobResult = MEMIF_JOB_FAIL;
    }
    else
    #endif  /* #if (FLS_DEV_ERROR_DETECT == STD_ON) */
    {
        jobResult = Fls_RtData.JobResult;
    }

    return jobResult;
}
#endif

/******************************************************************************/
/*
 * Brief               <This function shall perform the processing of the FLS jobs.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <NON Reentrant>
 * Param-Name[in]      <None>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>  
 * PreCondition        <None>  
 */
/******************************************************************************/
FUNC(void, FLS_CODE) Fls_MainFunction(void)
{
    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_MAINFUNCTION_ID, FLS_E_UNINIT);
    }
    else
    #endif  /* #if (FLS_DEV_ERROR_DETECT == STD_ON) */
    {
        if (MEMIF_JOB_PENDING == Fls_RtData.JobResult)
        {
            switch (Fls_RtData.Job)
            {
                case FLS_JOB_ERASE:
                    Fls_JobErase();
                    break;
                case FLS_JOB_WRITE:
                    Fls_JobWrite();
                    break;
                case FLS_JOB_COMPARE:
                    Fls_JobCompare();
                    break;
                case FLS_JOB_READ:
                    Fls_JobRead();
                    break;
                defult:
                    break;
            } /* switch (Fls_RtData.Job) */
        } /* if (MEMIF_JOB_PENDING == Fls_RtData.JobResult) */
    }  
}

/******************************************************************************/
/*
 * Brief               <This function shall reads from Flash.>
 * ServiceId           <0x03>
 * Sync/Async          <Asynchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <SourceAddress,Length>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <TargetAddressPtr>
 * Return              <Std_ReturnType>  
 * PreCondition        <None>  
 */
/******************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Read
(
    Fls_AddressType SourceAddress,
    P2VAR(uint8, AUTOMATIC, FLS_APPL_DATA) TargetAddressPtr, 
    Fls_LengthType Length
)
{
    Std_ReturnType ret = E_NOT_OK;

    #if (FLS_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_READ_ID, FLS_E_UNINIT);
    }
    else if (MEMIF_BUSY == Fls_RtData.Status)                                  
    {                                                           
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_READ_ID, FLS_E_BUSY);
    }
    else if (TRUE == Fls_CheckPageStartAlign(SourceAddress))                                          
    {                                                                   
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_READ_ID, FLS_E_PARAM_ADDRESS);
    }
    else if (TRUE == Fls_CheckPageEndAlign(SourceAddress+Length))
    {                                                                   
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_READ_ID, FLS_E_PARAM_LENGTH);
    }
    else if (NULL_PTR == TargetAddressPtr)
    {                                                                   
        Det_ReportError(FLS_MODULE_ID, CAN_INSTANCE, CAN_READ_ID, FLS_E_PARAM_POINTER);
    }
    else
    #endif  /* #if (FLS_DEV_ERROR_DETECT == STD_ON) */
    {
        /* @req <FLS237> */
        Fls_RtData.Status    = MEMIF_BUSY;
        Fls_RtData.JobResult = MEMIF_JOB_PENDING;
        Fls_RtData.Job       = FLS_JOB_READ;
        Fls_RtData.SourceAddress = TargetAddress;
        Fls_RtData.Length    = Length;

        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               <This function shall compares a data block in FLS 
 *                      with an FLS block in the memory.>
 * ServiceId           <0x05>
 * Sync/Async          <Asynchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <SourceAddress,TargetAddressPtr,Length>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType>  
 * PreCondition        <None>  
 */
/******************************************************************************/
#if (FLS_COMPARE_API == STD_ON)
FUNC(Std_ReturnType, FLS_CODE) Fls_Compare 
(
    Fls_AddressType SourceAddress, 
    P2CONST(uint8, AUTOMATIC, FLS_APPL_DATA) TargetAddressPtr, 
    Fls_LengthType Length 
)
{}
#endif

/******************************************************************************/
/*
 * Brief               <This function shall set the mode of Flash>
 * ServiceId           <0x09>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <Mode>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>  
 * PreCondition        <None>  
 */
/******************************************************************************/
#if (FLS_SET_MODE_API == STD_ON)
FUNC(void,FLS_CODE) Fls_SetMode(MemIf_ModeType Mode)
{}
#endif

STATIC FUNC(P2VAR(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA), FLS_CODE) 
Fls_FindSectorConfig(Fls_AddressType addr) 
{
    uint32 i = 0x0U;
    Fls_AddressType sectorStartAddr = 0x0U;
    Fls_LengthType sectorSize = 0x0U;
    Fls_LengthType num = 0x0U;
    P2VAR(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorConfig = NULL_PTR;

    for (i = 0; i < FLS_CONFIG->FlsSectorListSize; i++)
    {
        sectorStartAddr = FLS_CONFIG->FlsSectorList[i]->FlsSectorStartAddr;
        sectorSize = FLS_CONFIG->FlsSectorList[i]->FlsSectorSize;
        num = FLS_CONFIG->FlsSectorList[i]->FlsNumberOfSectors;

        if ((addr >= sectorStartAddr) 
            && (addr < (sectorStartAddr+num * sectorSize)))
        {
            sectorConfig = FLS_CONFIG->FlsSectorList[i];
            break;
        }
    }

    return sectorConfig;
}

STATIC FUNC(boolean, FLS_CODE) Fls_CheckPageStartAlign(Fls_AddressType addr)
{
    boolean ret = FALSE;
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg;

    sectorCfg = Fls_FindSectorConfig(addr);

    if ((NULL_PTR != sectorCfg) 
            && (0U == (addr - sectorCfg->FlsSectorStartAddr) % sectorCfg->FlsPageSize))
    {
        ret = TRUE;
    }

    return ret;
}

STATIC FUNC(boolean, FLS_CODE) Fls_CheckPageEndAlign(Fls_AddressType addr)
{
    boolean ret = FALSE;
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg;

    sectorCfg = Fls_FindSectorConfig(addr);

    if ((NULL_PTR != sectorCfg) 
            && ((pageSize - 1U) == (addr - sectorCfg->FlsSectorStartAddr) % sectorCfg->FlsPageSize))
    {
        ret = TRUE;
    }

    return ret;
}

STATIC FUNC(boolean, FLS_CODE) Fls_CheckSectorStartAlign(Fls_AddressType addr)
{
    boolean ret = FALSE;
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg;

    sectorCfg = Fls_FindSectorConfig(addr);

    if ((NULL_PTR != sectorCfg) 
            && (0U == (addr - sectorCfg->FlsSectorStartAddr) % sectorCfg->FlsSectorSize))
    {
        ret = TRUE;
    }

    return ret;
}

STATIC FUNC(boolean, FLS_CODE) Fls_CheckSectorEndAlign(Fls_AddressType addr)
{
    boolean ret = FALSE;
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg;

    sectorCfg = Fls_FindSectorConfig(addr);

    if ((NULL_PTR != sectorCfg) 
            && ((sectorSize - 1U) == (addr - sectorCfg->FlsSectorStartAddr) % sectorCfg->FlsSectorSize))
    {
        ret = TRUE;
    }

    return ret;
}

STATIC FUNC(boolean, FLS_CODE) Fls_Check(Fls_AddressType addr)
STATIC FUNC(void, FLS_CODE) Fls_JobErase(void)
{
    boolean jobRet = FALSE;
    Fls_LengthType sectorSize = 0x0U;
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg;

    sectorCfg = Fls_FindSectorConfig(Fls_RtData.SourceAddress);

    sectorSize = sectorCfg->FlsSectorSize;

    jobRet = Fls_HwErase(Fls_RtData.SourceAddress, sectorCfg);

    if (FALSE == jobRet) /* job fail */
    {
        Fls_RtData.Job       = FLS_JOB_NONE;
        Fls_RtData.Status    = MEMIF_IDLE;
        Fls_RtData.JobResult = MEMIF_JOB_FAIL;
        FLS_JOB_ERROR_NOTIFICATION();

        /* @req <FLS104> */
        Dem_ReportErrorStatus(CAN_E_ERASE_FAILED, DEM_EVENT_STATUS_FAILED);
    }  
    else
    {
        Fls_RtData.SourceAddressPtr += sectorSize;
        Fls_RtData.Length -= sectorSize;

        if (Fls_RtData.Length <= 0x0U) /* job success */
        {
            /* @req <FLS052> */
            Fls_RtData.Job       = FLS_JOB_NONE;
            Fls_RtData.Status    = MEMIF_IDLE;
            Fls_RtData.JobResult = MEMIF_JOB_OK;

            FLS_JOB_END_NOTIFICATION();
        }
    }

    return;
}

STATIC FUNC(void, FLS_CODE) Fls_JobWrite(void)
{
    Fls_LengthType length = 0x0U;
    boolean jobRet = FALSE;
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg;

    /* calc length of write once */
    sectorCfg = Fls_FindSectorConfig(Fls_RtData.SourceAddress);
    if (MEMIF_MODE_SLOW == Fls_RtData.Mode)
    {
        length = FLS_CONFIG->FlsMaxWriteNomralMode * sectorCfg->FlsPageSize;
    }
    else
    {
        length = FLS_CONFIG->FlsMaxWriteFastMode * sectorCfg->FlsPageSize;
    }
    if (Fls_RtData.Length < length)
    {
        length = Fls_RtData.Length;
    }

    jobRet = Fls_HwWrite(Fls_RtData.SourceAddress, 
        Fls_RtData.TargetAddress, 
        length,
        sectorCfg);

    if (FALSE == jobRet) /* job fail */
    {
        Fls_RtData.Job       = FLS_JOB_NONE;
        Fls_RtData.Status    = MEMIF_IDLE;
        Fls_RtData.JobResult = MEMIF_JOB_FAIL;

        FLS_JOB_ERROR_NOTIFICATION();

        /* @req <FLS105> */
        Dem_ReportErrorStatus(CAN_E_WRITE_FAILED, DEM_EVENT_STATUS_FAILED);
    }  
    else
    {
        Fls_RtData.SourceAddressPtr += length;
        Fls_RtData.TargetAddress += length;
        Fls_RtData.Length -= length;

        if (Fls_RtData.Length <= 0x0U) /* job success */
        {
            /* @req <FLS052> */
            Fls_RtData.Job       = FLS_JOB_NONE;
            Fls_RtData.Status    = MEMIF_IDLE;
            Fls_RtData.JobResult = MEMIF_JOB_OK;

            FLS_JOB_END_NOTIFICATION();
        }
    }

    return;
}

STATIC FUNC(void, FLS_CODE) Fls_JobRead(void)
{
    Fls_LengthType length = 0x0U;
    boolean jobRet = FALSE;
    P2CONST(Fls_SectorConfigType, AUTOMATIC, FLS_APPL_DATA) sectorCfg;

    /* calc length of write once */
    sectorCfg = Fls_FindSectorConfig(Fls_RtData.SourceAddress);
    if (MEMIF_MODE_SLOW == Fls_RtData.Mode)
    {
        length = FLS_CONFIG->FlsMaxReadNomralMode * sectorCfg->FlsPageSize;
    }
    else
    {
        length = FLS_CONFIG->FlsMaxReadFastMode * sectorCfg->FlsPageSize;
    }
    if (Fls_RtData.Length < length)
    {
        length = Fls_RtData.Length;
    }

    jobRet = Fls_HwRead(Fls_RtData.SourceAddress, Fls_RtData.TargetAddress, length);

    if (FALSE == jobRet) /* job fail */
    {
        Fls_RtData.Job       = FLS_JOB_NONE;
        Fls_RtData.Status    = MEMIF_IDLE;
        Fls_RtData.JobResult = MEMIF_JOB_FAIL;

        FLS_JOB_ERROR_NOTIFICATION();

        /* @req <FLS106> */
        Dem_ReportErrorStatus(CAN_E_READ_FAILED, DEM_EVENT_STATUS_FAILED);
    }  
    else
    {
        Fls_RtData.SourceAddressPtr += length;
        Fls_RtData.TargetAddress += length;
        Fls_RtData.Length -= length;

        if (Fls_RtData.Length <= 0x0U) /* job success */
        {
            /* @req <FLS052> */
            Fls_RtData.Job       = FLS_JOB_NONE;
            Fls_RtData.Status    = MEMIF_IDLE;
            Fls_RtData.JobResult = MEMIF_JOB_OK;

            FLS_JOB_END_NOTIFICATION();
        }
    }

    return;
}

STATIC FUNC(void, FLS_CODE) Fls_JobCompare(void)
{
    Fls_LengthType length = 0x0U;
    boolean jobRet = FALSE;

    jobRet = Fls_HwCompare(Fls_RtData.SourceAddress, Fls_RtData.TargetAddress, length);

    if (FALSE == jobRet) /* job fail */
    {
        /* @req <FLS200> */
        Fls_RtData.Job       = FLS_JOB_NONE;
        Fls_RtData.Status    = MEMIF_IDLE;
        Fls_RtData.JobResult = MEMIF_BLOCK_INCONSISENT;

        FLS_JOB_ERROR_NOTIFICATION();

        /* @req <FLS154> */
        Dem_ReportErrorStatus(CAN_E_COMPARE_FAILED, DEM_EVENT_STATUS_FAILED);
    }  
    else
    {
        Fls_RtData.SourceAddressPtr += length;
        Fls_RtData.TargetAddress += length;
        Fls_RtData.Length -= length;

        if (Fls_RtData.Length <= 0x0U) /* job success */
        {
            /* @req <FLS052> */
            Fls_RtData.Job       = FLS_JOB_NONE;
            Fls_RtData.Status    = MEMIF_IDLE;
            Fls_RtData.JobResult = MEMIF_JOB_OK;

            FLS_JOB_END_NOTIFICATION();
        }
    }

    return;
}

#define FLS_STOP_SEC_CODE
#include "Fls_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/

