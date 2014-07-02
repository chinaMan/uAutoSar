/******************************************************************************
*  Filename: EcuM_Callout_Stubs.c
*
*  Description:
*  ------------
*  
*  Author:  AndyZeng
*
*  Data: 2014.7.02
*=============================================================================
* $Log$
******************************************************************************/
/*========================[INCLUDE]===================================*/
#include "EcuM.h"

/*========================[INTERNAL DTAT TYPE]========================*/

/*========================[INTERNAL STATIC DTAT]========================*/

/*========================[INTERNAL FUNCTION PROTYPE]=================*/
STATIC FUNC(void, ECUM_CODE) EcuM_InitDriver(uint8 ModuleId);

/*========================[FUNCTION IMPLEMENTION]=====================*/
/**********************************************************************
 * @brief        in unrecoverable error situations, the ECU State Manager
 *               will call the error hook. it is up the system integrator
 *               to react accordingly
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    reason: reason for calling the error hook
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_ErrorHook
(
    Std_ReturnType reason
);

/**********************************************************************
 * @brief        this callout shall provide driver initialization and 
 *               other hardware-related startup activites for loading
 *               the post-build configuration data.
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
FUNC(void, ECUM_CODE) EcuM_AL_DriverInitZero(void)
{
    uint8 i = 0;

    /* initial the drivers in EcuMDriverInitListZero */
    for (i = 0; i < ECUM_INIT_LIST_ZERO_NUM; i++)
    {
        EcuM_InitDriver(EcuMDriverInitListZero[i]);
    }
}

/**********************************************************************
 * @brief        this callout shall provide driver initialization and 
 *               other hardware-related startup activites for loading
 *               the post-build configuration data.
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(P2VAR(EcuM_ConfigType, ECUM_APPL_DATA), ECUM_CODE) 
EcuM_DeterminePbConfiguration(void);

/**********************************************************************
 * @brief        this callout shall provide driver initialization and 
 *               other hardware-related startup activites in case of
 *               a power on reset
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    ConfigPtr: pointer to the EcuM post-build configuration
 *                          which contains pointers to all other BSW
 *                          module post-build configurations
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_AL_DriverInitOne
(
    P2CONST(EcuM_ConfigType, ECUM_APPL_DATA) ConfigPtr
);

/**********************************************************************
 * @brief        this callout shall provide driver initialization of 
 *               drivers which need OS and not need to wait for the 
 *               NvM_ReadAll job to finish
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    ConfigPtr: pointer to the EcuM post-build configuration
 *                          which contains pointers to all other BSW
 *                          module post-build configurations
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_AL_DriverInitTwo
(
    P2CONST(EcuM_ConfigType, ECUM_APPL_DATA) ConfigPtr
);

/**********************************************************************
 * @brief        this callout shall provide driver initialization of 
 *               drivers which need OS and not need to wait for the 
 *               NvM_ReadAll job to finish
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    ConfigPtr: pointer to the EcuM post-build configuration
 *                          which contains pointers to all other BSW
 *                          module post-build configurations
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_AL_DriverInitThree
(
    P2CONST(EcuM_ConfigType, ECUM_APPL_DATA) ConfigPtr
);

/**********************************************************************
 * @brief        just before calling RTE_Start
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnRTEStartup(void);

/**********************************************************************
 * @brief        on entry of RUN state is very similar to "just after 
 *               startup". this call allows the system designer to notify
 *               that RUN state has been reached.
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnEnterRun(void);

/**********************************************************************
 * @brief        this call allows the system designer to notify that the 
 *               APP RUN state is about to be left
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnExitRun(void);

/**********************************************************************
 * @brief        this call allows the system designer to notify that the 
 *               APP POST RUN state is about to be left
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnExitPostRun(void);

/**********************************************************************
 * @brief        this call allows the system designer to notify that the 
 *               PREP SHUTDOWN state is about to be entered
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnPrepShutdown(void);

/**********************************************************************
 * @brief        this call allows the system designer to notify that the 
 *               GOTO SLEEP state is about to be entered
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnGoSleep(void);

/**********************************************************************
 * @brief        this call allows the system designer to notify that the 
 *               GOTO OFF I state is about to be entered
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnGoOffOne(void);

/**********************************************************************
 * @brief        this call allows the system designer to notify that the 
 *               GOTO OFF II state is about to be entered
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnGoOffTwo(void);

/**********************************************************************
 * @brief        created to fix wakeup sequence
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    wakeupSource 
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_EnableWakeupSources
(
    EcuM_WakeupSourceType wakeupSource
);

/**********************************************************************
 * @brief        see EcuM_CheckRamHash
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_GenerateRamHash(void);

/**********************************************************************
 * @brief        this callout shall take the code for shutting off the 
 *               power supply of the ECU. if the ECU cannot unpower 
 *               itself, a reset may be an adequate reaction
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_AL_SwitchOff(void);

/**********************************************************************
 * @brief        
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       0: RAM integrity test failed
 *               else: RAM integrity test passed
 *********************************************************************/
EXTERN FUNC(uint8, ECUM_CODE) EcuM_CheckRamHash(void);

/**********************************************************************
 * @brief        the callout shall set up the given wakeup source(s) 
 *               so that they are not able to wakeup the ECU
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    wakeupSource
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_DisableWakeupSources
(
    EcuM_WakeupSourceType wakeupSource
);

/**********************************************************************
 * @brief        the callout shall provide driver initialization and 
 *               other hardware-related startup activities the wakeup 
 *               case.
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_DriverRestart(void);

/**********************************************************************
 * @brief        the callout shall start the given wakeup source(s)
 *               so that they are ready to perform wakeup validation
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    wakeupSource
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_StartWakeupSources
(
    EcuM_WakeupSourceType wakeupSource
);

/**********************************************************************
 * @brief        this callout is called by the Ecum to validate a wakeup 
 *               source. if a valid wakeup has been detected, it shall 
 *               be reported to EcuM via EcuM_ValidateWakeupEvent()
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    wakeupSource
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_CheckValidation
(
    EcuM_WakeupSourceType wakeupSource
);

/**********************************************************************
 * @brief        this callout shall stop the given wakeup source(s) 
 *               after unsuccessful wakeup validation
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    wakeupSource
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_StopWakeupSources
(
    EcuM_WakeupSourceType wakeupSource
);

/**********************************************************************
 * @brief        this callout gives the system designer the chance to 
 *               intercept the automatic boot behavior and to override 
 *               the wakeup reaction computed from wakeup source
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    wact: the wakeup reaction computed by ECU State Manager
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnWakeupReaction
(
    EcuM_WakeupReactionType wact
);

/**********************************************************************
 * @brief        this callout gives the system designer the chance to 
 *               intercept the automatic boot behavior and to override 
 *               the wakeup reaction computed from wakeup source
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    wact: the wakeup reaction computed by ECU State Manager
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_OnWakeupReaction
(
    EcuM_WakeupReactionType wact
);

/**********************************************************************
 * @brief        this callout is invoked periodicaly in all reduced clock
 *               sleep modes. it is explicitely allowed to poll wakeup
 *               sources from this callout and to call wakeup notification
 *               functions to indicate the end of the sleep state to the 
 *               ECU State Manager
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_SleepActivity(void);


STATIC FUNC(void, ECUM_CODE) EcuM_InitDriver(uint8 ModuleId)
{
    switch (ModuleId)
    {
#if (ECUM_INCLUDE_FIM)
        case FIM_MODULE_ID:
            Fim_Init();
            break;
#endif
#if (ECUM_INCLUDE_CAN)
        case CAN_MODULE_ID:
            Can_Init(EcuM_Module.PbCfg->InitCfg->Can_ConfigData);
            break;
#endif
    }
}
