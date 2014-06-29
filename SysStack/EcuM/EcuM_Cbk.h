/******************************************************************************
*  Filename: EcuM_Cbk.h
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
#ifndef ECUM_CBK_H_
#define ECUM_CBK_H_
/*========================[INCLUDE]===================================*/

/*========================[DTAT TYPE]=================================*/

/*========================[API PROTYPE]===============================*/
/**********************************************************************
 * @brief        used to notify about the end of NVRAM jobs initated by
 *               EcuM The callback must be callable from normal and 
 *               interrupt execution contexts
 * @ServiceId    0x65      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    ServiceId: unique service ID of NVRAM manager service
 *               JobResult: covers the job result of the previous 
 *                          processed multi block job
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_CB_NfyNvMJobEnd
(
    uint8 ServiceId,
    NvM_RequestResultType JobResult
);

/**********************************************************************
 * @brief        this callout is called by the EcuM to poll a wakeup 
 *               source. it shall also be called by the ISR of a wakeup 
 *               source to set up the PLL and check other wakeup sources
 *               that may be connected to the same interrupt
 * @ServiceId    0x00      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant
 * @Param[in]    WakeupSource 
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_CheckWakeup
(
    EcuM_WakeupSourceType WakeupSource
);

/**********************************************************************
 * @brief        set the wakeup event
 * @ServiceId    0x0c      
 * @Async/Sync   Sync
 * @Reentrancy   Non Reentrant, Non-interruptible
 * @Param[in]    sources: value to be set
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_SetWakeupEvent
(
    EcuM_WakeupSourceType sources
);

/**********************************************************************
 * @brief        after wakeup, the ECU State Manager will stop the 
 *               process during the WAKEUP VALIDATION state to wait for
 *               validation of the wakeup event. the validation is carried
 *               out with a call to this API service
 * @ServiceId    0x14      
 * @Async/Sync   Sync
 * @Reentrancy   Reentrant
 * @Param[in]    sources: events to be validated
 * @Param[out]   None
 * @Return       None
 *********************************************************************/
EXTERN FUNC(void, ECUM_CODE) EcuM_ValidateWakeupEvent
(
    EcuM_WakeupSourceType sources
);

#endif /* #ifndef ECUM_CBK_H_ */
