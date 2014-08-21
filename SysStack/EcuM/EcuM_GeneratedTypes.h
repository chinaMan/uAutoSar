/******************************************************************************
*  Filename: EcuM_GeneratedTypes.h
*
*  Description:
*  ------------
*  
*  Author:  AndyZeng
*
*  Data: 2014.7.04
*=============================================================================
* $Log$
******************************************************************************/
#ifndef ECUM_GENERATED_TYPES_H_
#define ECUM_GENERATED_TYPES_H_

/*========================[INCLUDE]===================================*/

/*========================[DTAT TYPE]=================================*/
/* 
 * EcuM state type 
 * --------------------------
 * | main state | sub state |
 * --------------------------
 * 7            3           0
 */
typedef uint8 EcuM_StateType;
/* main state */
#define ECUM_STATE_STARTUP            0x10U
#define ECUM_STATE_WAKEUP             0x20U
#define ECUM_STATE_RUN                0x30U
#define ECUM_STATE_SHUTDOWN           0x40U
#define ECUM_STATE_SLEEP              0x50U
#define ECUM_STATE_OFF                0x80U
#define ECUM_STATE_RESET              0x90U
/* mask of sub state */                   
#define ECUM_SUBSTATE_MASK            0x0fU
/* sub state of startup */                
#define ECUM_STATE_STARTUP_ONE        0x11U
#define ECUM_STATE_STARTUP_TWO        0x12U
/* sub state of wakeup */                 
#define ECUM_STATE_WAKEUP_ONE         0x21U
#define ECUM_STATE_WAKEUP_VALIDATION  0x22U
#define ECUM_STATE_WAKEUP_REACTION    0x23U
#define ECUM_STATE_WAKEUP_TWO         0x24U
#define ECUM_STATE_WAKEUPSLEEP        0x25U
/* sub state of run */                    
#define ECUM_STATE_APP_RUN            0x32U
#define ECUM_STATE_APP_POST_RUN       0x33U
/* sub state of shutdown */               
#define ECUM_STATE_PRE_SHUTDOWN       0x44U
#define ECUM_STATE_GO_SLEEP           0x49U
#define ECUM_STATE_GO_OFF_ONE         0x4dU
#define ECUM_STATE_GO_OFF_TWO         0x4eU

/* unique value for each user */
typedef uint8 EcuM_UserType;  

/* 
 * wakeup source type 
 * Note: 
 *   1. The bitfield provides one bit for each wakeup source
 *   2. The list can be extended by configuration
 */
typedef uint32 EcuM_WakeupSourceType;
#define ECUM_WKSOURCE_INTERNAL_RESET  0x0004U
#define ECUM_WKSOURCE_EXTERNAL_WDG    0x0010U
#define ECUM_WKSOURCE_INTERNAL_WDG    0x0008U
#define ECUM_WKSOURCE_POWER           0x0001U
#define ECUM_WKSOURCE_RESET           0x0002U

/* 
 * wakeup status type 
 */
typedef enum
{
    /* no pending wakeup event was detected */
    ECUM_WKSTATUS_NONE = 0x0U,       
    /* wakeup event was detected but not yet validated */
    ECUM_WKSTATUS_PENDING,
    /* wakeup event is valid */
    ECUM_WKSTATUS_VALIDATED,
    /* wakeup event has not been validated and has expired therefore */
    ECUM_WKSTATUS_EXPIRED
} EcuM_WakeupStatusType;

/*
 * wakeup reaction type 
 */
typedef enum
{
    /* initialization into RUN state */
    ECUM_WKACT_RUN      = 0x0U,
    /* execute time triggered increased inoperation protocol and shutdown */
    ECUM_WKACT_TTII     = 0x2U,
    /* immediate shutdown */
    ECUM_WKACT_SHUTDOWN = 0x3U
} EcuM_WakeupReactionType;

/*
 * boot target type 
 */
typedef enum
{
    /* ECU will boot into the application */
    ECUM_BOOT_TARGET_APP,
    /* ECU will boot into the bootloader */
    ECUM_BOOT_TARGET_BOOTLOADER
} EcuM_BootTargetType;

typedef struct
{
    EcuM_StateType   State;
    uint8            SleepMode;
} EcuM_ShutdownTargetType;

#endif /* #ifndef ECUM_GENERATED_TYPES_H_ */
