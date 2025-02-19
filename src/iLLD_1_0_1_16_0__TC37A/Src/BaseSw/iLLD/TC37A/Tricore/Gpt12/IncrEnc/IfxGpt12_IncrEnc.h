/**
 * \file IfxGpt12_IncrEnc.h
 * \brief GPT12 INCRENC details
 * \ingroup IfxLld_Gpt12
 *
 * \version iLLD_1_0_1_16_0
 * \copyright Copyright (c) 2022 Infineon Technologies AG. All rights reserved.
 *
 *
 *
 *                                 IMPORTANT NOTICE
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees. If and as long as no such terms
 * of use are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *
 * \defgroup IfxLld_Gpt12_IncrEnc INCRENC
 * \ingroup IfxLld_Gpt12
 *
 * \defgroup IfxLld_Gpt12_IncrEnc_Usage How to use the GPT12 Incremental Encoder Driver
 * \ingroup IfxLld_Gpt12_IncrEnc
 *
 * The IncrEnc interface can be used in one of the following configurations T3 as core or T2 as core
 *
 *  Setup with T3 as core
 *   - T3 as incremental mode
 *      - Multiplication factor:
 *          - twoFold: use both edges of TxIn
 *          - fourFold: use both edges of TxIn and TxEUD
 *          - Reverse of direction supported by GPT12
 *          - T3 value diff is used for speed calculation at high speed (speed > speedModeThreshold)
 *   - T4 used for zero signal detection if  pinZ is not NULL
 *      - interrupt is generated to support turn calculation if zeroIsrPriority != 0
 *      - T4 clears T3 on zero signal event on TxIn input
 *   - T5 used for low speed calculation (speed < speedModeThreshold)
 *          - Rising edge of TxIn or TxEUD captures the T5 timer in CAPREL
 *
 *  Setup with T2 as core
 *   - T2 as incremental mode
 *      - Multiplication factor:
 *          - twoFold: sample on both edges of TxIn
 *          - fourFold: sample on both edges of TxIn and TxEUD
 *          - Reverse of direction supported by GPT12
 *          - T2 value diff is used for speed calculation at high speed
 *   - T4 used for zero signal detection if  pinZeroTxEUD is not NULL
 *   - T4 clears T2 on zero signal event on TxEUD input
 *
 * \section IfxLld_Gpt12_IncrEnc_Preparation Preparation
 * \subsection IfxLld_Gpt12_IncrEnc_Include Include Files
 *
 * Include following header file into your C code:
 *   \code
 *       #include <Gpt12/Gpt/IfxGpt12_IncrEnc.h>
 *   \endcode
 *
 * \subsection IfxLld_Gpt12_IncrEnc_Variables Variables
 *
 *   \code
 *   // used globally
 *   static IfxGpt12_IncrEnc gpt12;
 *   \endcode
 *
 * \subsection IfxLld_Gpt12_Gpt_Interrupt Interrupt Handler Installation
 *
 * See also \ref IfxLld_Cpu_Irq_Usage
 *
 * Define priorities for the Interrrupt handlers. This is normally done in the Ifx_IntPrioDef.h file:
 *   \code
 *       // priorities are normally defined in Ifx_IntPrioDef.h
 *       #define ISR_PRIORITY_INCRENC_ZERO 6
 *   \endcode
 *
 * Add the interrupt service routines to your C code. They have to call the GPT12 interrupt handlers by passing the gpt12 handle:
 *   \code
 *       IFX_INTERRUPT(ISR_IncrIncZero, 0, ISR_PRIORITY_INCRENC_ZERO)
 *       {
 *           IfxGpt12_IncrEnc_onZeroIrq(&gpt12);
 *       }
 *   \endcode
 *
 * Finally install the interrupt handlers in your initialisation function:
 *   \code
 *       // install interrupt handlers
 *       IfxCpu_Irq_installInterruptHandler(&ISR_IncrIncZero, ISR_PRIORITY_INCRENC_ZERO);
 *       IfxCpu_enableInterrupts();
 *   \endcode
 *
 * \subsection IfxLld_Gpt12_Gpt_Init Module Initialisation
 *
 *   The module initialisation can be done in the same function. Here an example:
 *   \code
 *       // Initialize global clocks
 *       IfxGpt12_enableModule(&MODULE_GPT120);
 *       IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_8);
 *       IfxGpt12_setGpt2BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt2BlockPrescaler_4);
 *
 *       // create module config
 *       IfxGpt12_IncrEnc_Config gpt12Config;
 *       IfxGpt12_IncrEnc_initConfig(&gpt12Config , &MODULE_GPT120);
 *
 *       // implementation with T3 as core
 *       gpt12Config.base.offset                    = 0;
 *       gpt12Config.base.reversed                  = FALSE;
 *       gpt12Config.base.resolution                = 2048;
 *       gpt12Config.base.periodPerRotation         = 1;
 *       gpt12Config.base.resolutionFactor          = IfxStdIf_Pos_ResolutionFactor_fourFold;
 *       gpt12Config.base.updatePeriod              = 100e-6;
 *       gpt12Config.base.speedModeThreshold        = 200;
 *       gpt12Config.base.minSpeed                  = 10;
 *       gpt12Config.base.maxSpeed                  = 500;
 *       gpt12Config.zeroIsrPriority                = ISR_PRIORITY(INTERRUPT_INCRINC_ZERO);
 *       gpt12Config.zeroIsrProvider                = ISR_PROVIDER(INTERRUPT_INCRINC_ZERO);
 *       gpt12Config.pinA                           = &IfxGpt120_T3INA_P02_6_IN;
 *       gpt12Config.pinB                           = &IfxGpt120_T3EUDA_P02_7_IN;
 *       gpt12Config.pinZ                           = &IfxGpt120_T4INA_P02_8_IN;
 *       gpt12Config.pinMode                        = IfxPort_InputMode_noPullDevice;
 *
 *       gpt12Config.base.speedFilterEnabled        = TRUE;
 *       gpt12Config.base.speedFilerCutOffFrequency = config.base.maxSpeed / 2 * IFX_PI * 2;
 *
 *
 *       // initialize module
 *       //IfxGpt12_IncrEnc gpt12; // defined globally
 *       IfxGpt12_IncrEnc_init(&gpt12, &gpt12Config);
 *   \endcode
 *
 * \section IfxLld_Gpt12_IncrEnc_Update Update
 *
 * speed, position and direction of the incremental encoder can be collected by the following
 *
 *   \code
 *       float32 speed, rawPosition;
 *       IfxStdIf_Pos_Dir direction;
 *
 *       Ifx_TickTime tickRefresh     = gpt12.updatePeriod * TimeConst_1s;
 *
 *       Ifx_TickTime refreshDeadLine = now();
 *
 *       if (isDeadLine(refreshDeadLine))
 *       {
 *           refreshDeadLine = addTTime(refreshDeadLine, tickRefresh);
 *           IfxGpt12_IncrEnc_update(&gpt12);
 *
 *           speed       = IfxGpt12_IncrEnc_getSpeed(&gpt12);
 *           rawPosition = IfxGpt12_IncrEnc_getRawPosition(&gpt12);
 *           direction   = IfxGpt12_IncrEnc_getDirection(&gpt12);
 *       }
 *   \endcode
 *
 * \defgroup IfxLld_Gpt12_IncrEnc INCRENC
 * \ingroup IfxLld_Gpt12
 * \defgroup IfxLld_Gpt12_IncrEnc_Datastructures Data structures
 * \ingroup IfxLld_Gpt12_IncrEnc
 * \defgroup IfxLld_Gpt12_IncrEnc_Functions Functions
 * \ingroup IfxLld_Gpt12_IncrEnc
 * \defgroup IfxLld_Gpt12_IncrEnc_StdIf_Functions StdIf Functions
 * \ingroup IfxLld_Gpt12_IncrEnc
 */

#ifndef IFXGPT12_INCRENC_H
#define IFXGPT12_INCRENC_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "StdIf/IfxStdIf_Pos.h"
#include "Gpt12/Std/IfxGpt12.h"
#include "string.h"

/******************************************************************************/
/*------------------------------Type Definitions------------------------------*/
/******************************************************************************/

typedef IfxStdIf_Pos_Update IfxGpt12_IncrEnc_Update;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Gpt12_IncrEnc_Datastructures
 * \{ */
/** \brief Incremental encoder object
 */
typedef struct
{
    sint32                  rawPosition;                  /**< \brief raw position in ticks. \note: the value already contains the offset */
    float32                 speed;                        /**< \brief mechanical speed in rad/s */
    sint32                  turn;                         /**< \brief number of mechanical turns */
    IfxStdIf_Pos_Dir        direction;                    /**< \brief rotation direction */
    IfxStdIf_Pos_Status     status;                       /**< \brief error code (0 = no error) */
    sint32                  offset;                       /**< \brief raw position offset */
    sint32                  resolution;                   /**< \brief resolution of this position sensor interface */
    float32                 updatePeriod;                 /**< \brief update period in seconds */
    float32                 speedConstPulseCount;         /**< \brief constant for calculating mechanical speed (in rad/s) from raw speed in pulse count mode */
    float32                 speedConstTimeDiff;           /**< \brief constant for calculating mechanical speed (in rad/s) from raw speed in time diff mode */
    float32                 positionConst;                /**< \brief constant for calculating mechanical position (in rad) from raw position */
    float32                 speedModeThreshold;           /**< \brief treshold used for speed calculation using pulse count mode or time diff mode in rad/s */
    sint32                  speedModeThresholdTick;       /**< \brief treshold used for speed calculation using pulse count mode or time diff mode in ticks */
    Ifx_GPT12              *module;                       /**< \brief Pointer to the GPT12 module */
    float32                 minSpeed;                     /**< \brief Absolute minimal allowed speed. below speed is recognized as 0rad/s */
    float32                 maxSpeed;                     /**< \brief Absolute maximal allowed speed. Above speed is recognized as error */
    IfxGpt12_IncrEnc_Update update;                       /**< \brief Update call back API */
} IfxGpt12_IncrEnc;

/** \brief Configuration structure for GPT12
 */
typedef struct
{
    IfxStdIf_Pos_Config base;                  /**< \brief Configuration data of \ref library_srvsw_stdif_posif */
    Ifx_GPT12          *module;                /**< \brief Pointer to module base address */
    IfxGpt12_TxIn_In   *pinA;                  /**< \brief Encoder A signal. Should be connecting to T2 or T3 TxIN. See \ref IfxLld_Gpt12_pinmap "GPT12 pin map data" */
    IfxGpt12_TxEud_In  *pinB;                  /**< \brief Encoder B signal. Should be connecting to T2 or T3  TxEUD. See \ref IfxLld_Gpt12_pinmap "GPT12 pin map data" */
    IfxGpt12_TxIn_In   *pinZ;                  /**< \brief Encoder Z signal. Should be connecting to T4 TxIn for use with T3 and TxEUD (Ignore compiler type conflict warning) for use with T4.  See \ref IfxLld_Gpt12_pinmap "GPT12 pin map data" */
    IfxPort_InputMode   pinMode;               /**< \brief Pin mode for A, B and Z inputs */
    Ifx_Priority        zeroIsrPriority;       /**< \brief Interrupt isrPriority of the zero interrupt, if 0 the interrupt is disable */
    IfxSrc_Tos          zeroIsrProvider;       /**< \brief Interrupt service provider for the zero interrupt */
    IfxPort_PadDriver   pinDriver;             /**< \brief Pad Driver */
    boolean             initPins;              /**< \brief TRUE: Initialize pins in driver
                                                * FALSE: Don't initialize pins. User handles separately. */
} IfxGpt12_IncrEnc_Config;

/** \} */

/** \addtogroup IfxLld_Gpt12_IncrEnc_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the Incremental interface
 * \param driver incremental encoder interface Handle
 * \param config Configuration structure for incremental encoder
 * \return TRUE on success else FALSE
 *
 * \code
 *     // create module config
 *     IfxGpt12_IncrEnc_Config gptConfig;
 *     IfxGpt12_IncrEnc_initConfig(&gptConfig , &MODULE_GPT120);
 *
 *     // initialize module
 *     //IfxGpt12_IncrEnc gpt12; // defined globally
 *
 *     IfxGpt12_IncrEnc_init(&gpt12, &gptConfig);
 * \endcode
 *
 */
IFX_EXTERN boolean IfxGpt12_IncrEnc_init(IfxGpt12_IncrEnc *driver, const IfxGpt12_IncrEnc_Config *config);

/** \brief Initializes the configuration structure to default
 * \param config Configuration structure for incremental encoder
 * \param gpt12 pointer to module base address
 * \return None
 *
 * see \ref IfxGpt12_IncrEnc_init
 *
 */
IFX_EXTERN void IfxGpt12_IncrEnc_initConfig(IfxGpt12_IncrEnc_Config *config, Ifx_GPT12 *gpt12);

/** \} */

/** \addtogroup IfxLld_Gpt12_IncrEnc_StdIf_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief \see IfxStdIf_Pos_GetAbsolutePosition
 * \param driver driver handle
 * \return absolute position
 */
IFX_EXTERN float32 IfxGpt12_IncrEnc_getAbsolutePosition(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetDirection
 * \param driver driver handle
 * \return direction
 */
IFX_EXTERN IfxStdIf_Pos_Dir IfxGpt12_IncrEnc_getDirection(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetFault
 * \param driver driver handle
 * \return Fault
 */
IFX_EXTERN IfxStdIf_Pos_Status IfxGpt12_IncrEnc_getFault(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetOffset
 * \param driver driver handle
 * \return offset address
 */
IFX_EXTERN sint32 IfxGpt12_IncrEnc_getOffset(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetPeriodPerRotation
 * \param driver driver handle
 * \return Period per rotation
 */
IFX_EXTERN uint16 IfxGpt12_IncrEnc_getPeriodPerRotation(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetPosition
 * \param driver driver handle
 * \return position
 */
IFX_EXTERN float32 IfxGpt12_IncrEnc_getPosition(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetRawPosition
 * \param driver driver handle
 * \return position in ticks
 */
IFX_EXTERN sint32 IfxGpt12_IncrEnc_getRawPosition(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetRefreshPeriod
 * \param driver driver handle
 * \return update period
 */
IFX_EXTERN float32 IfxGpt12_IncrEnc_getRefreshPeriod(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetResolution
 * \param driver driver handle
 * \return resolution
 */
IFX_EXTERN sint32 IfxGpt12_IncrEnc_getResolution(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetSensorType
 * \param driver driver handle
 * \return sensor type
 */
IFX_EXTERN IfxStdIf_Pos_SensorType IfxGpt12_IncrEnc_getSensorType(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetSpeed
 * \param driver driver handle
 * \return speed
 */
IFX_EXTERN float32 IfxGpt12_IncrEnc_getSpeed(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_GetTurn
 * \param driver driver handle
 * \return the number of turns
 */
IFX_EXTERN sint32 IfxGpt12_IncrEnc_getTurn(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_OnZeroIrq
 * \param driver driver handle
 * \return None
 */
IFX_EXTERN void IfxGpt12_IncrEnc_onZeroIrq(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_Reset
 * \param driver driver handle
 * \return None
 */
IFX_EXTERN void IfxGpt12_IncrEnc_reset(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_ResetFaults
 * \param driver driver handle
 * \return None
 */
IFX_EXTERN void IfxGpt12_IncrEnc_resetFaults(IfxGpt12_IncrEnc *driver);

/** \brief \see IfxStdIf_Pos_SetOffset
 * \param driver driver handle
 * \param offset offset address
 * \return None
 */
IFX_EXTERN void IfxGpt12_IncrEnc_setOffset(IfxGpt12_IncrEnc *driver, sint32 offset);

/** \brief \see IfxStdIf_Pos_SetRefreshPeriod
 * \param driver driver handle
 * \param updatePeriod update period
 * \return None
 */
IFX_EXTERN void IfxGpt12_IncrEnc_setRefreshPeriod(IfxGpt12_IncrEnc *driver, float32 updatePeriod);

/** \brief \see IfxStdIf_Pos_Update
 * \param driver driver handle
 * \return None
 */
IFX_EXTERN void IfxGpt12_IncrEnc_update(IfxGpt12_IncrEnc *driver);

/** \} */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initializes the standard interface "Pos"
 * \param stdif Standard interface position object
 * \param driver incremental encoder interface Handle
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGpt12_IncrEnc_stdIfPosInit(IfxStdIf_Pos *stdif, IfxGpt12_IncrEnc *driver);
#endif /* IFXGPT12_INCRENC_H */
