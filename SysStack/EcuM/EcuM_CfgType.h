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
    boolean               Suspend;
    EcuM_WakeupSourceType WakeupSourceMask;
    Mcu_ModeType          McuMode;
    WdgM_ModeType         WdgMMode;
} EcuM_SleepModeConfigType;

typedef struct
{
    uint32  Wks;
    uint32  ValidationTimeout;
    uint8   ResetReason;
    boolean WksPolling;
    uint8   ComMChannel;
} EcuM_WakeupSourceConfigType;

typedef struct
{
    uint32  Divisor;
    uint8   Successor;
    uint8   SleepMode;
} EcuM_TTIIConfigType;

typedef struct
{
    WdgMSupervisedEntity  SupervisedEntityRef;
    WdgMMode              WakeupModeRef;
    WdgMMode              StartupModeRef;
    WdgMMode              RunModeRef;
    WdgMMode              PostMRunModeRef;
    WdgMMode              ShutdownModeRef;
} EcuM_WdgMConfigType;

typedef struct
{
    uint8     ConsistencyHash;
} EcuM_LTConfigType;

typedef struct
{
    uint32               SleepActivityPeriod;     
    uint32               NvramReadAllTimeout;
    uint32               RunMinimumDuration;
    uint32               NvramWriteAllTimeout;
    P2CONST(EcuM_InitCfgType, AUTOMATIC, ECUM_CONST_PBCFG) InitConfiguration;
    OsAppMode            DefaultAppMode;

    EcuM_ShutdownTargetType DefaultShutdownTarget;  /* EcuMDefaultShutdwonTarget */

    P2CONST(EcuM_WdgMConfigType, AUTOMATIC, ECUM_CONST_PBCFG) WdgMConfigData;
} EcuM_ConfigType;

#endif /* #ifndef ECUM_CFGTYPE_H_ */
