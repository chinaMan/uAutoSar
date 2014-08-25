/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <Fls.h>
 *  @brief      <Fls driver module header file>
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
#define FLS_H_AR_MAJOR_VERSION  2U
#define FLS_H_AR_MINOR_VERSION  2U
#define FLS_H_AR_PATCH_VERSION  3U

#define FLS_H_SW_MAJOR_VERSION  1U
#define FLS_H_SW_MINOR_VERSION  0U
#define FLS_H_SW_PATCH_VERSION  0U

/*=======[I N C L U D E S]====================================================*/

/*=======[M A C R O S]========================================================*/
/* @req <FLS004> */
/*Service (API) ID for DET reporting */
#define FLS_E_PARAM_CONFIG                 0x01U
#define FLS_E_PARAM_ADDRESS                0x02U
#define FLS_E_PARAM_LENGTH                 0x03U
#define FLS_E_PARAM_DATA                   0x04U
#define FLS_E_UNINIT                       0x05U
#define FLS_E_BUSY                         0x06U
#define FLS_E_VERIFY_ERASE_FAILED          0x07U
#define FLS_E_VERIFY_WRITE_FAILED          0x08U

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef Fls_AddressType  uint32;
typedef Fls_LengthType   uint32;

typedef struct
{
    Fls_AddressType      FlsSectorStartAddr;
    Fls_LengthType       FlsSectorSize;
    Fls_LengthType       FlsPageSize;
    Fls_LengthType       FlsNumberOfSectors;
    uint32               FlsStartBlock;
    uint8                FlsAddrSpace;
} Fls_SectorConfigType;

typedef struct
{
    P2FUNC(void, FLS_APPL_CODE, FlsAcErase)(void);
    P2FUNC(void, FLS_APPL_CODE, FlsAcWrite)(void);
    P2FUNC(void, FLS_APPL_CODE, FlsJobEndNotification)(void);
    P2FUNC(void, FLS_APPL_CODE, FlsJobErrorNotification)(void);
    uint32   FlsCallCycle;
    uint8    FlsMaxReadFastMode;
    uint8    FlsMaxReadNomralMode;
    uint8    FlsMaxWriteFastMode;
    uint8    FlsMaxWriteNomralMode;
    uint32   FlsProtection;
    uint32   FlsSectorListSize;
    P2CONST(Fls_SectorConfigType, FLS_CONST, FLS_CONST_PBCFG) FlsSectorList;
} Fls_ConfigType;

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"
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
extern FUNC(void, FLS_CODE) Fls_Init
(
    P2CONST(Fls_ConfigType, AUTOMATIC, FLS_APPL_DATA) ConfigPtr
);

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
extern FUNC(Std_ReturnType, FLS_CODE) Fls_Erase
(
    Fls_AddressType TargetAddress, 
    Fls_LengthType Length
);

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
extern FUNC(Std_ReturnType, FLS_CODE) Fls_Write
(
    Fls_AddressType TargetAddress, 
    P2CONST(uint8, AUTOMATIC, FLS_APPL_DATA) SourceAddressPtr, 
    Fls_LengthType Length
);

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
extern FUNC(void, FLS_CODE) Fls_Cancel(void);
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
#if (FLS_GET_STATUS_API == STD_ON)
extern FUNC(MemIf_StatusType, FLS_CODE) Fls_GetStatus(void);
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
#if (FLS_GET_JOB_RESULT_API == STD_ON)
extern FUNC(MemIf_JobResultType, FLS_CODE) Fls_GetJobResult(void);
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
extern FUNC(void, FLS_CODE) Fls_MainFunction(void);

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
extern FUNC(Std_ReturnType, FLS_CODE) Fls_Read
(
    Fls_AddressType SourceAddress,
    P2VAR(uint8, AUTOMATIC, FLS_APPL_DATA) TargetAddressPtr, 
    Fls_LengthType Length
);

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
extern FUNC(Std_ReturnType, FLS_CODE) Fls_Compare 
(
    Fls_AddressType SourceAddress, 
    P2CONST(uint8, AUTOMATIC, FLS_APPL_DATA) TargetAddressPtr, 
    Fls_LengthType Length 
);
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
extern FUNC(void,FLS_CODE) Fls_SetMode(MemIf_ModeType Mode);
#endif

#if (FLS_VERSION_INFO_API == STD_ON)
/******************************************************************************/
/*
 * Brief               <This function gets the version imformation.>
 * ServiceId           <0x0a>
 * Sync/Async          <Synchronous>
 * Reentrancy          <NON Reentrant>
 * Param-Name[in]      <None>      
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <VersioninfoPtr>
 * Return              <None>  
 * PreCondition        <None>  
 */
/******************************************************************************/
#if(STD_ON == FLS_DEV_ERROR_DETECT)
#define FLS_GetVersionInfo(versionInfo) \
    do{ \
        if (NULL_PTR == (versionInfo))\
        { \
            Det_ReportError(FLS_MODULE_ID, FLS_INSTANCE,  \
                FLS_GETVERSIONINFO_ID, FLS_E_PARAM_POINTER);\
        }\
        else\
        {\
            ((Std_VersionInfoType *)(versionInfo))->vendorID   = FLS_VENDOR_ID; \
            ((Std_VersionInfoType *)(versionInfo))->moduleID   = FLS_MODULE_ID; \
            ((Std_VersionInfoType *)(versionInfo))->instanceID = FLS_INSTANCE;  \
            ((Std_VersionInfoType *)(versionInfo))->sw_major_version = FLS_SW_MAJOR_VERSION; \
            ((Std_VersionInfoType *)(versionInfo))->sw_minor_version = FLS_SW_MINOR_VERSION; \
            ((Std_VersionInfoType *)(versionInfo))->sw_patch_version = FLS_SW_PATCH_VERSION; \
        }\
    }while(0)
#else
#define Fls_GetVersionInfo(versionInfo) \
    do{ \
        ((Std_VersionInfoType *)(versionInfo))->vendorID   = FLS_VENDOR_ID; \
        ((Std_VersionInfoType *)(versionInfo))->moduleID   = FLS_MODULE_ID; \
        ((Std_VersionInfoType *)(versionInfo))->instanceID = FLS_INSTANCE;  \
        ((Std_VersionInfoType *)(versionInfo))->sw_major_version = FLS_SW_MAJOR_VERSION; \
        ((Std_VersionInfoType *)(versionInfo))->sw_minor_version = FLS_SW_MINOR_VERSION; \
        ((Std_VersionInfoType *)(versionInfo))->sw_patch_version = FLS_SW_PATCH_VERSION; \
    }while(0)
#endif /* STD_ON == FLS_DEV_ERROR_DETECT */
#endif

#define Fls_STOP_SEC_CODE
#include "Fls_MemMap.h"

#endif  /* #ifndef FLS_H */

/*=======[E N D   O F   F I L E]==============================================*/

