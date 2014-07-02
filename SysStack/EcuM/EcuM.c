/******************************************************************************
*  Filename: EcuM.c
*
*  Description:
*  ------------
*  
*  Author:  AndyZeng
*
*  Data: 2014.6.28
*=============================================================================
* $Log$
******************************************************************************/
/*========================[INCLUDE]===================================*/
#include "EcuM.h"

/*========================[INTERNAL DTAT TYPE]========================*/
typedef struct
{
    boolean           Inited;      /* is initialized */
    EcuM_StateType    State;       /* EcuM state */
    EcuM_ConfigType   *PbCfg; 

    EcuM_WakeupSourceType     ValidWks;
    EcuM_WakeupSourceType     PendingWks;
    EcuM_WakeupSourceType     ValidatedWks;
    EcuM_WakeupSourceType     ExpiredWks;

    EcuM_ShutdownTargetType   ShutdownTarget;

    boolean                   IsRun[ECUM_USER_MAX];
    uint8                     UserReqCount;

    boolean                   IsPostRun[ECUM_USER_MAX];
    uint8                     UserReqPostCount;

    uint8                     ComMChState[ECUM_COMM_CHANNEL_MAX];
    uint8                     ComMReqCount;
} EcuM_RtType;

/*========================[INTERNAL STATIC DTAT]========================*/
STATIC EcuM_RtType EcuM_Module = 
{
    .State  = ECUM_STATE_OFF;
    .Inited = FALSE;
    .PbCfg  = NULL_PTR;
    .UserReqCount = 0x0U;
};

/*========================[MACRO FUNCTION]============================*/
#define ECUM_CHECK_CFG_CONSISTENCY

/*========================[INTERNAL FUNCTION PROTYPE]=================*/

/*========================[FUNCTION IMPLEMENTION]=====================*/
/**********************************************************************
 * @brief        initializes EcuM and carries out the startup function 
 *               will never return
 * @ServiceId    0x01      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
FUNC(void, ECUM_CODE) EcuM_Init(void)
{
    EcuM_AppModeType app = ECUM_OSDEFAULTAPPMODE;
    EcuM_ResetType reset = MCU_RESET_UNDEFINED;

    /* 
     * 1. init run time data structure
     */
    EcuM_Module.State  = ECUM_STATE_STARTUP_ONE; 
    EcuM_Module.PbCfg  = NULL_PTR;
    EcuM_Module.inited = TRUE;

    /* 
     * 2. init sequence 1(ref to AutoSar Figure 4 - Init Sequence I)
     */
    EcuM_AL_DriverInitZero();                   /* Init Block 0 */

    EcuM_Module.PbCfg = EcuM_DeterminePbConfiguration();
    if ((NULL == EcuM_Module.PbCfg)
            || !ECUM_CHECK_CFG_CONSISTENCY(EcuM_Module.PbCfg)
       )
    {
       Dem_ReportErrorStatus(ECUM_E_CONFIGURATION_DATA_INCONSISENT,
               DEM_EVENT_STATUS_FAILED); 
       EcuM_ErrorHook(ECUM_E_INCONSISTENT_CONFIGURATION);
       return;
    }

    EcuM_AL_DriverInitOne(EcuM_Module.PbCfg);   /* Init Block I */

    reset = Mcu_GetResetReason();
    /* TODO: map reset reason to wakeup source */

    EcuM_SelectShutdownTarget(
            EcuM_Module.PbCfg->EcuMDefaultShutdown->Target);

    if (MCU_RESET_UNDEFINED == reset)
    {
        app = EcuM_SelectApplicationMode(
                EcuM_Module.PbCfg->EcuMDefaultAppMode);
    }

    /* 
     * 3. start os (not return)
     */
    StartOS(app);
}

/**********************************************************************
 * @brief        implements the STARTUP II state
 * @ServiceId    0x1a      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
FUNC(void, ECUM_CODE) EcuM_StartupTwo(void)
{
    #if(STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.inited)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_STARTUPTWO, ECUM_E_NOT_INITED);
        return;
    }
    #endif

    EcuM_Module.State = ECUM_STATE_STARTUP_TWO;

    #if (STD_ON == ECUM_INCLUDE_SCHM)
    SchM_Init();
    #endif

    #if (STD_ON == ECUM_INCLUDE_WDGM)
    (void)WdgM_SetMode(EcuM_Module.PbCfg->EcuMWdgMConfigData->EcuMWdgMStartupModeRef);
    #endif

    /* initializes drivers in two  */
    EcuM_AL_DriverInitTwo(EcuM_Module.PbCfg);  /* Init Block II */

    #if (STD_ON == ECUM_INCLUDE_NVRAM)
    /* read all configure data from NVM */
    NvM_Init();
    NvM_ReadAll();
    #endif

    #if (STD_ON == ECUM_INCLUDE_RTE)
    EcuM_OnRTEStartup();
    Rte_Start();
    Rte_Switch_currentMode_currentMode(ECUM_INSTANCE_ID, ECUM_STATE_STARTUP);
    #endif

    /* when no NVRAM, not need wait NvM_ReadAll finish, then switch to next state */
    #if (STD_ON != ECUM_INCLUDE_NVRAM)
    if (ECUM_WKSOURCE_NONE == EcuM_Module.ValidWks) /* no valid wakeup event */
    {
        /* enter Wakeup State */
        #if (STD_ON == ECUM_INCLUDE_RTE)
        Rte_Switch_currentMode_currentMode(RTE_MODE_EcuM_Mode_Wakeup);
        #endif

        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        EcuM_Module.State = ECUM_STATE_WAKEUP_VALIDATION
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }
    else
    {
        /* enter APP RUN State */
        #if (STD_ON == ECUM_INCLUDE_RTE)
        Rte_Switch_currentMode_currentMode(RTE_MODE_EcuM_Mode_RUN);
        #endif

        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        EcuM_Module.State = ECUM_STATE_APP_RUN;
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }
    #endif
}

/**********************************************************************
 * @brief        typically called from the shutdown hook, this function
 *               takes over execute and will carry out GO OFF II activities
 * @ServiceId    0x02      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
FUNC(void, ECUM_CODE) EcuM_Shutdown(void)
{
    #if(STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.inited)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SHUTDOWN, ECUM_E_NOT_INITED);
        return;
    }
    #endif

    /*
     * Figure 15 - DeInitialization Sequence III (GO OFF II)
     */
    EcuM_OnGoOffTwo();

    if (ECUM_STATE_RESET == EcuM_Module.ShutdownTarget.State)
    {
        Mcu_PerformReset();
    }
    else if (ECUM_STATE_OFF == EcuM_Module.ShutdownTarget.State)
    {
        EcuM_AL_SwitchOff();
    }
    else
    {
        /* do nothing */
    }
}

/**********************************************************************
 * @brief        get state
 * @ServiceId    0x07      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       E_OK: the out parameter was set successfully
 *               E_NOT_OK: the out parameter was not set
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetState
(
    P2VAR(EcuM_StateType, ECUM_APPL_DATA) state
)
{
    /* @req EcuM2421 EcuM2423 */
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETSTATE, ECUM_E_NOT_INITED);
        return E_NOT_OK;
    }
    if (NULL_PTR == state)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETSTATE, ECUM_E_NULL_POINTER);
        return E_NOT_OK;
    }
    #endif

    /* @req EcuM2823 */
    SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    *state = EcuM_Module.State;
    SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);

    return E_OK;
}

/**********************************************************************
 * @brief        places a request for the RUN state. Requests can be 
 *               placed by every user made known to the state manager 
 *               at configuration time
 * @ServiceId    0x03      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    user: ID of the entity requesting the RUN state 
 * @Param[out]   None
 * @Return       E_OK: the request was accepted by EcuM
 *               E_NOT_OK: the request was not accepted by EcuM
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_RequestRUN
(
    EcuM_UserType user
)
{
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_REQUEST_RUN, ECUM_E_NOT_INITED);
        return E_NOT_OK;
    }
    else if (user >= ECUM_USER_MAX)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_REQUEST_RUN, ECUM_E_INVALID_PAR);
        return E_NOT_OK;
    }
    #endif
    else
    {
        if (TRUE == EcuM_Module.IsKillRun)
        {
            Dem_ReportErrorStatus(ECUM_E_ALL_RUN_REQUESTS_KILLED, DEM_EVENT_STATUS_FAILED);
            return E_NOT_OK;
        }

        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        if (FALSE == EcuM_Module.IsRun[user])
        {
            EcuM_Module.IsRun[user] = TRUE;
            EcuM_Module.UserReqCount++;
        } 
        #if (STD_ON == ECUM_DEV_ERROR_DETECT)
        else
        {
            Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                    ECUM_SID_REQUEST_RUN, ECUM_E_MULTIPLE_RUN_REQUESTS);
            return E_NOT_OK;
        }
        #endif
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }

    return E_OK;
}

/**********************************************************************
 * @brief        releases a RUN state previously done with a called to 
 *               EcuM_RequestRUN. The service is intended for implementing
 *               AUTOSAR ports.
 * @ServiceId    0x04      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    user: ID of the entity requesting the RUN state 
 * @Param[out]   None
 * @Return       E_OK: release request was accepted by EcuM
 *               E_NOT_OK: release request was not accepted by EcuM
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ReleaseRUN
(
    EcuM_UserType user
)
{
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_RELEASE_RUN, ECUM_E_NOT_INITED);
        return E_NOT_OK;
    }
    else if (user >= ECUM_USER_MAX)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_RELEASE_RUN, ECUM_E_INVALID_PAR);
        return E_NOT_OK;
    }
    #endif
    else
    {
        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        if (TRUE == EcuM_Module.IsRun[user])
        {
            EcuM_Module.IsRun[user] = FALSE;
            EcuM_Module.UserReqCount--;
        } 
        #if (STD_ON == ECUM_DEV_ERROR_DETECT)
        else
        {
            Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                    ECUM_SID_RELEASE_RUN, ECUM_E_MISMATCHED_RUN_RELEASE);
            return E_NOT_OK;
        }
        #endif
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }

    return E_OK;
}

/**********************************************************************
 * @brief        the behavior is identical to EcuM_RequestRUN execept
 *               that the parameter user but a communication channel
 * @ServiceId    0x0e      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    channel: ID of the communication channel requesting 
 *                        the RUN state
 * @Param[out]   None
 * @Return       E_OK: request was accepted by EcuM
 *               E_NOT_OK: request was not accepted by EcuM
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ComM_RequestRUN
(
    NetworkHandleType channel
)
{
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_COMM_REQUEST_RUN, ECUM_E_NOT_INITED);
        return E_NOT_OK;
    }
    else if (channel >= ECUM_COMM_CHANNEL_MAX)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_COMM_REQUEST_RUN, ECUM_E_INVALID_PAR);
        return E_NOT_OK;
    }
    #endif
    else
    {
        if (TRUE == EcuM_Module.IsKillRun)
        {
            Dem_ReportErrorStatus(ECUM_E_ALL_RUN_REQUESTS_KILLED, DEM_EVENT_STATUS_FAILED);
            return E_NOT_OK;
        }

        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        if (ECUM_COMM_CH_STATE_INIT == EcuM_Module.ComMChState[channel])
        {
            EcuM_Module.ComMChState[channel] = ECUM_COMM_CH_STATE_REQ;
            EcuM_Module.ComMReqCount++;
        } 
        #if (STD_ON == ECUM_DEV_ERROR_DETECT)
        else
        {
            Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                    ECUM_SID_COMM_REQUEST_RUN, ECUM_E_MULTIPLE_RUN_REQUESTS);
            return E_NOT_OK;
        }
        #endif
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }

    return E_OK;

}

/**********************************************************************
 * @brief        releases a RUN request previously done with a call 
 *               EcuM_ComM_RequestRUN
 * @ServiceId    0x10      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    channel: ID of the communication channel requesting 
 *                        the RUN state
 * @Param[out]   None
 * @Return       E_OK: release was accepted by EcuM
 *               E_NOT_OK: release was not accepted by EcuM
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ComM_ReleaseRUN
(
    NetworkHandleType channel
)
{
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_COMM_RELEASE_RUN, ECUM_E_NOT_INITED);
        return E_NOT_OK;
    }
    else if (channel >= ECUM_COMM_CHANNEL_MAX)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_COMM_RELEASE_RUN, ECUM_E_INVALID_PAR);
        return E_NOT_OK;
    }
    #endif
    else
    {
        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        if (ECUM_COMM_CH_STATE_INIT != EcuM_Module.ComMChState[channel])
        {
            EcuM_Module.ComMChState[channel] = ECUM_COMM_CH_STATE_INIT;
            EcuM_Module.ComMReqCount--;
        } 
        #if (STD_ON == ECUM_DEV_ERROR_DETECT)
        else
        {
            Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                    ECUM_SID_COMM_RELEASE_RUN, ECUM_E_MISMATCHED_RUN_RELEASE);
            return E_NOT_OK;
        }
        #endif
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }

    return E_OK;
}

/**********************************************************************
 * @brief        return if a channel has requested RUN state
 * @ServiceId    0x1b      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    channel: ID of the communication channel requesting 
 *                        the RUN state
 * @Param[out]   None
 * @Return       TRUE: the channel has requested RUN state
 *               FALSE: the channel has not requested RUN state
 *********************************************************************/
FUNC(boolean, ECUM_CODE) EcuM_ComM_HasRequestedRUN
(
    NetworkHandleType channel
)
{
    boolean ret = FALSE;

    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_COMM_RELEASE_RUN, ECUM_E_NOT_INITED);
        return TRUE;
    }
    else if (channel >= ECUM_COMM_CHANNEL_MAX)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_COMM_RELEASE_RUN, ECUM_E_INVALID_PAR);
        return TRUE;
    }
    #endif
    else
    {
        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        if (ECUM_COMM_CH_STATE_INIT == EcuM_Module.ComMChState[channel])
        {
            ret = FALSE
        } 
        else
        {
            ret = TRUE; 
        }
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }

    return ret;
}

/**********************************************************************
 * @brief        places a request for the POST RUN state
 * @ServiceId    0x0a      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    user: ID of the entity requesting the RUN state 
 * @Param[out]   None
 * @Return       E_OK: request was accepted by EcuM
 *               E_NOT_OK: request was not accepted by EcuM
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_RequestPOST_RUN
(
    EcuM_UserType user
)
{
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_REQUESTPOST_RUN, ECUM_E_NOT_INITED);
        return E_NOT_OK;
    }
    else if (user >= ECUM_USER_MAX)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_REQUESTPOST_RUN, ECUM_E_INVALID_PAR);
        return E_NOT_OK;
    }
    #endif
    else
    {
        if (TRUE == EcuM_Module.IsKillRun)
        {
            Dem_ReportErrorStatus(ECUM_E_ALL_RUN_REQUESTS_KILLED, DEM_EVENT_STATUS_FAILED);
            return E_NOT_OK;
        }

        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        if (FALSE == EcuM_Module.IsPostRun[user])
        {
            EcuM_Module.IsPostRun[user] = TRUE;
            EcuM_Module.UserReqPostCount++;
        } 
        #if (STD_ON == ECUM_DEV_ERROR_DETECT)
        else
        {
            Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                    ECUM_SID_REQUESTPOST_RUN, ECUM_E_MULTIPLE_RUN_REQUESTS);
            return E_NOT_OK;
        }
        #endif
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }

    return E_OK;

}

/**********************************************************************
 * @brief        release a POST RUN request previously done with a called
 *               to EcuM_RequestPOST_RUN. the service is intended for 
 *               implements ports.
 * @ServiceId    0x0a      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    user: ID of the entity requesting the RUN state 
 * @Param[out]   None
 * @Return       E_OK: release was accepted by EcuM
 *               E_NOT_OK: release was not accepted by EcuM
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ReleasePOST_RUN
(
    EcuM_UserType user
)
{
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_RELEASEPOST_RUN, ECUM_E_NOT_INITED);
        return E_NOT_OK;
    }
    else if (user >= ECUM_USER_MAX)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_RELEASEPOST_RUN, ECUM_E_INVALID_PAR);
        return E_NOT_OK;
    }
    #endif
    else
    {
        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        if (TRUE == EcuM_Module.IsPostRun[user])
        {
            EcuM_Module.IsPostRun[user] = FALSE;
            EcuM_Module.UserReqPostCount--;
        } 
        #if (STD_ON == ECUM_DEV_ERROR_DETECT)
        else
        {
            Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                    ECUM_SID_RELEASEPOST_RUN, ECUM_E_MISMATCHED_RUN_RELEASE);
            return E_NOT_OK;
        }
        #endif
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }

    return E_OK;

}

/**********************************************************************
 * @brief        the benefit of this function over an ECU reset is that
 *               the shutdown sequence is executed
 * @ServiceId    0x05      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
FUNC(void, ECUM_CODE) EcuM_KillAllRUNRequests(void)
{
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_KILL_ALL_RUN_REQUESTS, ECUM_E_NOT_INITED);
        return;
    }
    #endif
    else
    {
        uint8 i = 0U;

        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        if (FALSE == EcuM_Module.IsKillRun)
        {
            EcuM_Module.IsKillRun = TRUE;
        }
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);

        #if (STD_ON == ECUM_INCLUDE_COMM)
        /* clear all ComM channel request */
        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        EcuM_Module.ComMReqCount = 0U;
        for (i = 0U; i < ECUM_COMM_CHANNEL_MAX; i++)
        {
            EcuM_Module.ComMChState = ECUM_COMM_CH_STATE_INIT;
        }
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        #endif

        /* clear all user request RUN / POST RUN */
        SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
        EcuM_Module.UserReqCount = 0U;
        EcuM_Module.UserReqPostCount = 0U;
        for (i = 0U; i < ECUM_USER_MAX; i++)
        {
            EcuM_Module.IsRun[i]     = FALSE;
            EcuM_Module.IsPostRun[i] = FALSE;
        }
        SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    }
}

/**********************************************************************
 * @brief        selects the shutdown target
 * @ServiceId    0x06      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    target: the selected shutdown target
 *               mode:   an index like value which can be dereferenced 
 *                       to a sleep mode
 * @Param[out]   None
 * @Return       E_OK: the new shutdown target was set
 *               E_NOT_OK: the new shutdown target was not set
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectShutdownTarget
(
    EcuM_StateType target,
    uint8 mode
)
{
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                ECUM_SID_SELECTSHUTDOWNTARGET, ECUM_E_NOT_INITED);
        return E_NOT_OK;
    }
    else if ((ECUM_STATE_SLEEP == target) && EcuM_IsValidSleepMode(mode))
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                ECUM_SID_KILL_ALL_RUN_REQUESTS, ECUM_E_INVALID_PAR);
        return E_NOT_OK;
    }
    #endif

    SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    EcuM_Module.ShutdownTarget.State       = target;
    EcuM_Module.ShutdownTarget.SleepModeId = mode;
    SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);

    return E_OK;
}

/**********************************************************************
 * @brief        returns the selected shutdown as set by 
 *               EcuM_SelectShutdownTarget
 * @ServiceId    0x09      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    None
 * @Param[out]   shutdownTarget: one of these values is returned:
 *                                - ECUM_STATE_SLEEP
 *                                - ECUM_STATE_RESET
 *                                - ECUM_STATE_OFF
 *               sleepmode: if the return parameter is ECUM_STATE_SLEEP,
 *                      this out parameter tells which of the configured
 *                      sleep modes was actually chosen
 * @Return       E_OK:     service has successded
 *               E_NOT_OK: service has failed, e.g. duo to NULL pointer
 *                         being passed
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetShutdownTarget
(
    P2VAR(EcuM_StateType, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(uint8, ECUM_APPL_DATA) sleepmode
)
{
    #if (STD_ON == ECUM_DEV_ERROR_DETECT)
    if (FALSE == EcuM_Module.Inited) 
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                ECUM_SID_GETSHUTDOWNTARGET, ECUM_E_NOT_INITED);
        return E_NOT_OK;
    }
    else if ((NULL_PTR == shutdownTarget) && (NULL_PTR == sleepmode))
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, 
                ECUM_SID_GETSHUTDOWNTARGET, ECUM_E_NULL_POINTER);
        return E_NOT_OK;
    }
    #endif

    SchM_Entry_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);
    *ShutdownTarget = EcuM_Module.ShutdownTarget.State;
    *mode = EcuM_Module.ShutdownTarget.SleepModeId;
    SchM_Exit_EcuM(ECUM_INSTANCE_ID, ECUM_AREA_CRITICAL);

    return E_OK;
}

/**********************************************************************
 * @brief        returns the shutdown target of the previous shutdown 
 *               process
 * @ServiceId    0x08      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    None
 * @Param[out]   shutdownTarget: one of these values is returned:
 *                                - ECUM_STATE_SLEEP
 *                                - ECUM_STATE_RESET
 *                                - ECUM_STATE_OFF
 *               sleepmode: if the return parameter is ECUM_STATE_SLEEP,
 *                          this out parameter tells which of the 
 *                          configured sleep modes was actually chosen
 * @Return       E_OK:     service has successded
 *               E_NOT_OK: service has failed, e.g. duo to NULL pointer
 *                         being passed
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetLastShutdownTarget
(
    P2VAR(EcuM_StateType, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(uint8, ECUM_APPL_DATA) sleepmode
)
{}

/**********************************************************************
 * @brief        gets pending wakeup events
 * @ServiceId    0x0d      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant, Non interruptible
 * @Param[in]    None
 * @Param[out]   None
 * @Return       EcuM_WakeupSourceType: all wakeup events
 *********************************************************************/
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetPendingWakeupEvents(void)
{}

/**********************************************************************
 * @brief        clears wakeup events
 * @ServiceId    0x18      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant, Non interruptible
 * @Param[in]    sources: events to be cleared
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
FUNC(void, ECUM_CODE) EcuM_ClearWakeupEvent
(
    EcuM_WakeupSourceType sources
)
{}

/**********************************************************************
 * @brief        gets validated wakeup events
 * @ServiceId    0x15      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant, Non interruptible
 * @Param[in]    None
 * @Param[out]   None
 * @Return       EcuM_WakeupSourceType
 *********************************************************************/
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetValidatedWakeupEvents(void)
{}

/**********************************************************************
 * @brief        gets expired wakeup events
 * @ServiceId    0x19      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant, Non interruptible
 * @Param[in]    None
 * @Param[out]   None
 * @Return       EcuM_WakeupSourceType
 *********************************************************************/
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetExpiredWakeupEvents(void)
{}

/**********************************************************************
 * @brief        sum status shall be computed according to the following
 *               algorithm
 * @ServiceId    0x17      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       EcuM_WakeupStatusType: sum status of all wakeup sources
 *               passed in the in parameter
 *********************************************************************/
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetStatusOfWakeupSource
(
    EcuM_WakeupSourceType sources
)
{}

/**********************************************************************
 * @brief        implementation should store the application mode 
 *               preferably in a non-initialized area of RAM
 * @ServiceId    0x0f      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    AppMode: application mode taken for next OS start
 * @Param[out]   None
 * @Return       E_OK: new application mode was accepted by EcuM
 *               E_NOT_OK: new application mode was not accepted by EcuM
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectApplicationMode
(
    AppModeType appMode
)
{}

/**********************************************************************
 * @brief        service is intended for implementing AUTOSAR ports
 * @ServiceId    0x11      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    None
 * @Param[out]   AppMode: currently selected application mode 
 * @Return       E_OK: service always successds.
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetApplicationMode
(
    P2VAR(AppModeType, ECUM_APPL_DATA) appMode
)
{}

/**********************************************************************
 * @brief        selects a boot target
 * @ServiceId    0x12      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    target: the selected boot target
 * @Param[out]   None
 * @Return       E_OK: the new boot target was accepted by EcuM
 *               E_NOT_OK: the new boot target was not accepted by EcuM
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectBootTarget
(
    EcuM_BootTargetType target
)
{}

/**********************************************************************
 * @brief        service is intended for implementing AUTOSAR ports
 * @ServiceId    0x13      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    None
 * @Param[out]   target: the currently selected boot target
 * @Return       E_OK: the service always successds
 *********************************************************************/
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetBootTarget
(
    EcuM_BootTargetType target
)
{}

/**********************************************************************
 * @brief        
 * @ServiceId    0x18      
 * @Timing:      VARIABLE_CYCLE
 *********************************************************************/
FUNC(void, ECUM_CODE) EcuM_MainFunction(void){}
