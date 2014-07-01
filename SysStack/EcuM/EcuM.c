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
    EcuM_ConfigType   *PreCfg; 
} EcuM_RtType;

/*========================[INTERNAL STATIC DTAT]========================*/
STATIC EcuM_RtType EcuM_Module = 
{
    .State  = ECUM_STATE_OFF;
    .Inited = FALSE;
    .PbCfg  = NULL_PTR;
    .PreCfg = NULL_PTR;
};

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
    EcuM_Module.PreCfg = &EcuM_ConfigData;

    /* 
     * 2. init sequence 1(ref to AutoSar Figure 4 - Init Sequence I)
     */
    EcuM_AL_DriverInitZero();
    EcuM_Module.PbCfg = EcuM_DeterminePbConfiguration();
    if ((NULL == EcuM_Module.PbCfg)
            || (EcuM_Module.PreCfg->EcuMConfigConsistencyHash 
                != EcuM_Module.PbCfg->EcuMConfigConsistencyHash))
    {
       Dem_ReportErrorStatus(ECUM_E_CONFIGURATION_DATA_INCONSISENT,
               DEM_EVENT_STATUS_FAILED); 
       EcuM_ErrorHook(ECUM_E_INCONSISTENT_CONFIGURATION);
       return;
    }
    EcuM_AL_DriverInitOne(EcuM_Module.PbCfg);
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
    /* TODO: check EcuM is inited */

    /* TODO: initializes SchM module */

    /* TODO: Set WdgM mode (option WdgM support) */

    /* initializes drivers in two  */
    EcuM_AL_DriverInitTwo(EcuM_Module.PbCfg);

    /* read all configure data from NVM */
    NvM_Init();
    NvM_ReadAll();

    EcuM_OnRTEStartup();
    Rte_Start();
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
{}

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
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetState
(
    P2VAR(EcuM_StateType, ECUM_APPL_DATA) state
)
{}
