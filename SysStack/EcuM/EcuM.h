/******************************************************************************
*  Filename: EcuM.h
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
#ifndef ECUM_H_
#define ECUM_H_
/*========================[INCLUDE]===================================*/

/*========================[DTAT TYPE]=================================*/
/*========================[API PROTYPE]===============================*/
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
EXTERN FUNC(void, ECUM_CODE) EcuM_Init(void);

/**********************************************************************
 * @brief        implements the STARTUP II state
 * @ServiceId    0x1a      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_StartupTwo(void);

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
EXTERN FUNC(void, ECUM_CODE) EcuM_Shutdown(void);

/**********************************************************************
 * @brief        get state
 * @ServiceId    0x07      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    None
 * @Param[out]   None
 * @Return       E_OK: the out parameter was set successfully
 *               E_NOT_OK: the out parameter was not set
 *********************************************************************/
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetState
(
    P2VAR(EcuM_StateType, ECUM_APPL_DATA) state
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_RequestRUN
(
    EcuM_UserType user
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_ReleaseRUN
(
    EcuM_UserType user
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_ComM_RequestRUN
(
    NetworkHandleType channel
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_ComM_ReleaseRUN
(
    NetworkHandleType channel
);

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
EXTERN FUNC(boolean, ECUM_CODE) EcuM_ComM_HasRequestedRUN
(
    NetworkHandleType channel
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_RequestPOST_RUN
(
    EcuM_UserType user
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_ReleasePOST_RUN
(
    EcuM_UserType user
);

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
EXTERN FUNC(void, ECUM_CODE) EcuM_KillAllRUNRequests(void);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectShutdownTarget
(
    EcuM_StateType target,
    uint8 mode
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetShutdownTarget
(
    P2VAR(EcuM_StateType, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(uint8, ECUM_APPL_DATA) sleepmode
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetLastShutdownTarget
(
    P2VAR(EcuM_StateType, ECUM_APPL_DATA) shutdownTarget,
    P2VAR(uint8, ECUM_APPL_DATA) sleepmode
);

/**********************************************************************
 * @brief        gets pending wakeup events
 * @ServiceId    0x0d      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant, Non interruptible
 * @Param[in]    None
 * @Param[out]   None
 * @Return       EcuM_WakeupSourceType: all wakeup events
 *********************************************************************/
EXTERN FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetPendingWakeupEvents(void);

/**********************************************************************
 * @brief        clears wakeup events
 * @ServiceId    0x18      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant, Non interruptible
 * @Param[in]    sources: events to be cleared
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_ClearWakeupEvent
(
    EcuM_WakeupSourceType sources
);

/**********************************************************************
 * @brief        gets validated wakeup events
 * @ServiceId    0x15      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant, Non interruptible
 * @Param[in]    None
 * @Param[out]   None
 * @Return       EcuM_WakeupSourceType
 *********************************************************************/
EXTERN FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetValidatedWakeupEvents(void);

/**********************************************************************
 * @brief        gets expired wakeup events
 * @ServiceId    0x19      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant, Non interruptible
 * @Param[in]    None
 * @Param[out]   None
 * @Return       EcuM_WakeupSourceType
 *********************************************************************/
EXTERN FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetExpiredWakeupEvents(void);

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
EXTERN FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetStatusOfWakeupSource
(
    EcuM_WakeupSourceType sources
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectApplicationMode
(
    AppModeType appMode
);

/**********************************************************************
 * @brief        service is intended for implementing AUTOSAR ports
 * @ServiceId    0x11      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    None
 * @Param[out]   AppMode: currently selected application mode 
 * @Return       E_OK: service always successds.
 *********************************************************************/
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetApplicationMode
(
    P2VAR(AppModeType, ECUM_APPL_DATA) appMode
);

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
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectBootTarget
(
    EcuM_BootTargetType target
);

/**********************************************************************
 * @brief        service is intended for implementing AUTOSAR ports
 * @ServiceId    0x13      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    None
 * @Param[out]   target: the currently selected boot target
 * @Return       E_OK: the service always successds
 *********************************************************************/
EXTERN FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetBootTarget
(
    EcuM_BootTargetType target
);

/**********************************************************************
 * @brief        
 * @ServiceId    0x18      
 * @Timing:      VARIABLE_CYCLE
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_MainFunction(void);
#endif /* #ifndef ECUM_H_ */
