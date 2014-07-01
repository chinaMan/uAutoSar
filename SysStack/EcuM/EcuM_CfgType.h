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



} EcuM_PBConfigType;


#endif /* #ifndef ECUM_CFGTYPE_H_ */
