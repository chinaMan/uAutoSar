/******************************************************************************
*  Filename: EcuM_Cfg.c
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

/*========================[INCLUDE]===================================*/

/*========================[DTAT DEFINE]=================================*/
STATIC CONST(uint8, ECUM_CONST) EcuM_DriverInitListZero[1] = 
{
    1
};

STATIC CONST(uint8, ECUM_CONST) EcuM_DriverInitListOne[1] = 
{
    1
};

STATIC CONST(uint8, ECUM_CONST) EcuM_DriverInitListTwo[1] = 
{
    1
};

STATIC CONST(uint8, ECUM_CONST) EcuM_DriverInitListThree[1] = 
{
    1
};

STATIC CONST(uint8, ECUM_CONST) EcuM_DriverRestartList[1] = 
{
    1
};

STATIC CONST(EcuM_WakeupSourceConfigType, ECUM_CONST) EcuM_WkeupSourceConfigData[1] = 
{
    0,         /* EcuMWakeupSourceId */
    0x1000,    /* EcuMValidationTimeout */
    0,         /* EcuMResetReason */
    FALSE,     /* EcuMWakeupSourcePolling */
    0,         /* EcuMComMChannelRef */
};

STATIC CONST(uint8, ECUM_CONST) EcuM_UserConfigData[1] = 
{
    0
};

STATIC CONST(EcuM_TTIIConfigType, ECUM_CONST) EcuM_TTIIConfigData[1] = 
{
    0x1,       /* EcuMDivisor */
    0x1,       /* EcuMSuccessorRef */
    0x1,       /* EcuMSleepModeRef */
};

STATIC CONST(EcuM_SleepModeConfigType, ECUM_CONST) EcuM_SleepModeConfigData[1] = 
{
    {
        TRUE,
        0,
        0,
        0
    }
};
