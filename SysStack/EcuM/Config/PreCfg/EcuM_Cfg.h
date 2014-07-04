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
#ifndef ECUM_CFG_H
#define ECUM_CFG_H

/*========================[FILE VERSION INFORMATION]===================================*/
#define ECUM_CFG_H_AR_MAJOR_VERSION     3U 
#define ECUM_CFG_H_AR_MINOR_VERSION     1U 
#define ECUM_CFG_H_AR_PATCH_VERSION     5U 
#define ECUM_CFG_H_SW_MAJOR_VERSION     1U
#define ECUM_CFG_H_SW_MINOR_VERSION     0U
#define ECUM_CFG_H_SW_PATCH_VERSION     0U 
 
/*========================[RELEASE VERSION INFORMATION]===================================*/
#define ECUM_VENDOR_ID                 62U         /* Replace with a symbolic name once available */
#define ECUM_MODULE_ID                 80U         /* from AUTOSAR_BasicSoftwareModules.pdf */
#define ECUM_INSTANCE                  0U
#define ECUM_AR_MAJOR_VERSION          2U 
#define ECUM_AR_MINOR_VERSION          4U 
#define ECUM_AR_PATCH_VERSION          0U 
#define ECUM_SW_MAJOR_VERSION          1U
#define ECUM_SW_MINOR_VERSION          0U
#define ECUM_SW_PATCH_VERSION          0U 

/*========================[INCLUDE]===================================*/


/*========================[MACROS]=================================*/
/*
 * Variants Class (PC, PB)
 */
#define ECUM_VARIANT_PC                VARIANT_PRE_COMPILE
#define ECUM_VARIANT_CFG               ECUM_VARIANT_PC

/*
 * EcuMGeneral
 */
#define ECUM_TTII_ENABLED                STD_ON    /* enable/disable TTII */
#define ECUM_DEV_ERROR_DETECT            STD_ON    /* development error detection */
#define ECUM_VERSIONINFO_API             STD_OFF   /* switches the version info API on or off */

#define ECUM_INCLUDE_DEM                 STD_OFF   
#define ECUM_INCLUDE_NVRAM_MGR           STD_OFF   
#define ECUM_INCLUDE_WDGM                STD_OFF   
#define ECUM_INCLUDE_DET                 STD_OFF   

#define ECUM_MAIN_FUNCTION_PERIOD        1000U

#define ECUM_TTII_WKSOURCE_ID            2U

#define ECUM_ALL_WKSOURCES               0xFFFF

#endif /* #define ECUM_CFG_H */

/*=======[END OF FILE]==============================================*/
