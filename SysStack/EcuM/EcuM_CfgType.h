/******************************************************************************
*  Filename: EcuM_CfgType.h
*
*  Description:
*  ------------
*  
*  Author:  AndyZeng
*
*  Data: 2014.7.01
*=============================================================================
* $Log$
******************************************************************************/
#ifndef ECUM_CFGTYPE_H_
#define ECUM_CFGTYPE_H_

/*========================[INCLUDE]===================================*/

/*========================[DTAT TYPE]=================================*/
typedef struct
{
    uint32  EcuMValidationTimeout;
    uint8   EcuMResetReason;
    boolean EcuMWakeupSourcePolling;
    uint8   EcuMComMChannelRef;
} EcuM_WakeupSourceConfigType;

typedef struct
{
    uint32  EcuMDivisor;
    uint8   EcuMSuccessorRef;
    uint8   EcuMSleepModeRef;
} EcuM_TTIIConfigType;

typedef struct
{
    WdgMSupervisedEntity  EcuMSupervisedEntityRef;
    WdgMMode              EcuMWdgMWakeupModeRef;
    WdgMMode              EcuMWdgMStartupModeRef;
    WdgMMode              EcuMWdgMRunModeRef;
    WdgMMode              EcuMWdgPostMRunModeRef;
    WdgMMode              EcuMWdgShutdownModeRef;
} EcuM_WdgMConfigType;

typedef struct
{
    uint8     EcuMConfigConsistencyHash;
} EcuM_LTConfigType;

typedef struct
{
    uint32               EcuMSleepActivityPeriod;     
    uint32               EcuMNvramReadAllTimeout;
    uint32               EcuMRunMinimumDuration;
    uint32               EcuMNvramWriteAllTimeout;
    P2CONST(EcuM_InitCfgType, AUTOMATIC, ECUM_CONST_PBCFG) EcuMInitConfiguration;
    OsAppMode            EcuMDefaultAppMode;

    /* EcuMDefaultShutdwonTarget */
    EcuM_StateType       EcuMDefaultState;    
    uint8                EcuMSleepModeId;

    P2CONST(EcuM_WdgMConfigType, AUTOMATIC, ECUM_CONST_PBCFG) EcuMWdgMConfigData;
} EcuM_ConfigType;


#endif /* #ifndef ECUM_CFGTYPE_H_ */
