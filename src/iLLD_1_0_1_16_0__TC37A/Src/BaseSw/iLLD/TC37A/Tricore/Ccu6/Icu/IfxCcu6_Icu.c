/**
 * \file IfxCcu6_Icu.c
 * \brief CCU6 ICU details
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
 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxCcu6_Icu.h"

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

uint32 IfxCcu6_Icu_getTimeStamp(IfxCcu6_Icu_Channel *channel)
{
    uint32 timeStamp = 0;

    timeStamp = IfxCcu6_getCaptureShadowRegisterValue(channel->ccu6, channel->channelId);

    return timeStamp;
}


void IfxCcu6_Icu_initChannel(IfxCcu6_Icu_Channel *channel, const IfxCcu6_Icu_ChannelConfig *channelConfig)
{
    channel->ccu6 = channelConfig->ccu6;

    /* -- capture input initialisation -- */

    const IfxCcu6_Icu_Pins *pins = channelConfig->pins;

    if (pins != NULL_PTR)
    {
        IfxCcu6_Cc60in_In *cc60In = pins->cc60In;

        if (cc60In != NULL_PTR)
        {
            IfxCcu6_initCc60InPinWithPadLevel(cc60In, pins->cc6xInputMode, pins->pinDriver);
        }

        IfxCcu6_Cc61in_In *cc61In = pins->cc61In;

        if (cc61In != NULL_PTR)
        {
            IfxCcu6_initCc61InPinWithPadLevel(cc61In, pins->cc6xInputMode, pins->pinDriver);
        }

        IfxCcu6_Cc62in_In *cc62In = pins->cc62In;

        if (cc62In != NULL_PTR)
        {
            IfxCcu6_initCc62InPinWithPadLevel(cc62In, pins->cc6xInputMode, pins->pinDriver);
        }

        if (channelConfig->multiInputCaptureEnabled)
        {
            IfxCcu6_Ccpos0_In *ccpos0 = pins->ccpos0;

            if (ccpos0 != NULL_PTR)
            {
                IfxCcu6_initCcpos0PinWithPadLevel(ccpos0, pins->ccposxInputMode, pins->pinDriver);
            }

            IfxCcu6_Ccpos1_In *ccpos1 = pins->ccpos1;

            if (ccpos1 != NULL_PTR)
            {
                IfxCcu6_initCcpos1PinWithPadLevel(ccpos1, pins->ccposxInputMode, pins->pinDriver);
            }

            IfxCcu6_Ccpos2_In *ccpos2 = pins->ccpos2;

            if (ccpos2 != NULL_PTR)
            {
                IfxCcu6_initCcpos2PinWithPadLevel(ccpos2, pins->ccposxInputMode, pins->pinDriver);
            }
        }

        IfxCcu6_T12hr_In *t12hr = pins->t12hr;

        if (t12hr != NULL_PTR)
        {
            IfxCcu6_initT12hrPinWithPadLevel(t12hr, pins->t1xhrInputMode, pins->pinDriver);
        }

        IfxCcu6_T13hr_In *t13hr = pins->t13hr;

        if (t13hr != NULL_PTR)
        {
            IfxCcu6_initT13hrPinWithPadLevel(t13hr, pins->t1xhrInputMode, pins->pinDriver);
        }
    }

    /* -- interrupt initialisation -- */

    if (channelConfig->interrupt1.priority > 0)
    {
        IfxCcu6_enableInterrupt(channelConfig->ccu6, channelConfig->interrupt1.source);
        IfxCcu6_routeInterruptNode(channelConfig->ccu6, channelConfig->interrupt1.source, channelConfig->interrupt1.serviceRequest);

        volatile Ifx_SRC_SRCR *src;
        src = IfxCcu6_getSrcAddress(channelConfig->ccu6, channelConfig->interrupt1.serviceRequest);
        IfxSrc_init(src, channelConfig->interrupt1.typeOfService, channelConfig->interrupt1.priority);
        IfxSrc_enable(src);
    }

    if (channelConfig->interrupt2.priority > 0)
    {
        IfxCcu6_enableInterrupt(channelConfig->ccu6, channelConfig->interrupt2.source);
        IfxCcu6_routeInterruptNode(channelConfig->ccu6, channelConfig->interrupt2.source, channelConfig->interrupt2.serviceRequest);

        volatile Ifx_SRC_SRCR *src;
        src = IfxCcu6_getSrcAddress(channelConfig->ccu6, channelConfig->interrupt2.serviceRequest);
        IfxSrc_init(src, channelConfig->interrupt2.typeOfService, channelConfig->interrupt2.priority);
        IfxSrc_enable(src);
    }

    if (channelConfig->interrupt3.priority > 0)
    {
        IfxCcu6_enableInterrupt(channelConfig->ccu6, channelConfig->interrupt3.source);
        IfxCcu6_routeInterruptNode(channelConfig->ccu6, channelConfig->interrupt3.source, channelConfig->interrupt3.serviceRequest);

        volatile Ifx_SRC_SRCR *src;
        src = IfxCcu6_getSrcAddress(channelConfig->ccu6, channelConfig->interrupt3.serviceRequest);
        IfxSrc_init(src, channelConfig->interrupt3.typeOfService, channelConfig->interrupt3.priority);
        IfxSrc_enable(src);
    }

    if (channelConfig->interrupt4.priority > 0)
    {
        IfxCcu6_enableInterrupt(channelConfig->ccu6, channelConfig->interrupt4.source);
        IfxCcu6_routeInterruptNode(channelConfig->ccu6, channelConfig->interrupt4.source, channelConfig->interrupt4.serviceRequest);

        volatile Ifx_SRC_SRCR *src;
        src = IfxCcu6_getSrcAddress(channelConfig->ccu6, channelConfig->interrupt4.serviceRequest);
        IfxSrc_init(src, channelConfig->interrupt4.typeOfService, channelConfig->interrupt4.priority);
        IfxSrc_enable(src);
    }

    channel->trigger     = channelConfig->trigger;
    channel->channelId   = channelConfig->channelId;
    channel->channelMode = channelConfig->channelMode;

#if IFX_CFG_USE_STANDARD_INTERFACE
    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, (uint32)icu == ((uint32)&icu->base));
    icu->base.functions.startCapture = (Icu_StartCapture) & IfxCcu6_Icu_startCapture;
    icu->base.functions.stopCapture  = (Icu_StopCapture) & IfxCcu6_Icu_stopCapture;
    icu->base.functions.getTimeStamp = (Icu_GetTimeStamp) & IfxCcu6_Icu_getTimeStamp;
#endif
}


void IfxCcu6_Icu_initChannelConfig(IfxCcu6_Icu_ChannelConfig *channelConfig, Ifx_CCU6 *ccu6)
{
    channelConfig->ccu6                        = ccu6;

    channelConfig->channelId                   = IfxCcu6_T12Channel_0;                               // default channel 0
    channelConfig->channelMode                 = IfxCcu6_T12ChannelMode_doubleRegisterCaptureRising; // default CC6xIN rising edge

    channelConfig->interrupt1.source           = IfxCcu6_InterruptSource_cc60RisingEdge;
    channelConfig->interrupt1.serviceRequest   = IfxCcu6_ServiceRequest_0;
    channelConfig->interrupt1.priority         = 0;                // interrupt priority 0
    channelConfig->interrupt1.typeOfService    = IfxSrc_Tos_cpu0;  // type of service CPU0

    channelConfig->interrupt2.source           = IfxCcu6_InterruptSource_cc60FallingEdge;
    channelConfig->interrupt2.serviceRequest   = IfxCcu6_ServiceRequest_1;
    channelConfig->interrupt2.priority         = 0;                // interrupt priority 0
    channelConfig->interrupt2.typeOfService    = IfxSrc_Tos_cpu0;  // type of service CPU0

    channelConfig->interrupt3.source           = IfxCcu6_InterruptSource_cc61RisingEdge;
    channelConfig->interrupt3.serviceRequest   = IfxCcu6_ServiceRequest_2;
    channelConfig->interrupt3.priority         = 0;                // interrupt priority 0
    channelConfig->interrupt3.typeOfService    = IfxSrc_Tos_cpu0;  // type of service CPU0

    channelConfig->interrupt4.source           = IfxCcu6_InterruptSource_trap;
    channelConfig->interrupt4.serviceRequest   = IfxCcu6_ServiceRequest_3;
    channelConfig->interrupt4.priority         = 0;                // interrupt priority 0
    channelConfig->interrupt4.typeOfService    = IfxSrc_Tos_cpu0;  // type of service CPU0

    channelConfig->trigger.extInputTrigger     = channelConfig->timer.trigger.t12ExtInputTrigger;
    channelConfig->trigger.extInputTriggerMode = channelConfig->timer.trigger.extInputTriggerMode;

    channelConfig->pins                        = NULL_PTR;

    channelConfig->multiInputCaptureEnabled    = FALSE;
}


void IfxCcu6_Icu_initModule(IfxCcu6_Icu *icu, const IfxCcu6_Icu_Config *config)
{
    Ifx_CCU6 *ccu6SFR = config->ccu6; // pointer to CCU6 registers
    icu->ccu6 = ccu6SFR;              // adding register pointer to module handler

    /* -- hardware module initialisation -- */

    // enable module if it hasn't been enabled by any other interface //
    if (IfxCcu6_isModuleEnabled(ccu6SFR) == FALSE)
    {
        IfxCcu6_enableModule(ccu6SFR);
    }

    /* -- timer initialisation -- */

    // enable Timer12 if it hasn't been enabled by any other interface //
    if (IfxCcu6_getTimerAvailabilityStatus(ccu6SFR, IfxCcu6_TimerId_t12) == FALSE)
    {
        IfxCcu6_enableTimer(ccu6SFR, IfxCcu6_TimerId_t12);
    }

    // clock initialization //

    Ifx_TimerValue period = config->period;

    if (config->clock.extClockEnabled)       // if external source is selected as reference
    {
        // input signal selection
        if (config->clock.extClockInput != NULL_PTR)
        {
            IfxCcu6_setT12InputSignal(config->ccu6, config->clock.extClockInput);
        }

        // counting input mode selection
        IfxCcu6_setCountingInputMode(ccu6SFR, IfxCcu6_TimerId_t12, config->clock.countingInputMode);

        // countining mode selection
        IfxCcu6_setT12CountMode(ccu6SFR, config->countMode);

        // period selection for center aligned mode
        if (config->countMode == IfxCcu6_T12CountMode_centerAligned)
        {
            period = (period / 2) - 1;
        }

        // period setting
        IfxCcu6_setT12PeriodValue(ccu6SFR, (uint16)period);

        // frequency is meaningless here : set to 0
        icu->captureFrequency = 0;
    }
    else        // if internal clock is selected as reference
    {
        // in case of in sync with T13, T12 runs at same frequency as T13
        icu->captureFrequency = IfxCcu6_setT12Frequency(ccu6SFR, config->frequency, period, config->countMode);
    }

    // counter initialisation //

    IfxCcu6_setT12CounterValue(ccu6SFR, config->counterValue);
}


void IfxCcu6_Icu_initModuleConfig(IfxCcu6_Icu_Config *config, Ifx_CCU6 *ccu6)
{
    config->ccu6                    = ccu6;

    config->frequency               = 400000;
    config->period                  = 0xFFFE; /* max(uint16) - 1 */

    config->clock.extClockEnabled   = FALSE;
    config->clock.extClockInput     = NULL_PTR;
    config->clock.countingInputMode = IfxCcu6_CountingInputMode_internal;

    config->countMode               = IfxCcu6_T12CountMode_edgeAligned;
    config->counterValue            = 0;
}


void IfxCcu6_Icu_startCapture(IfxCcu6_Icu_Channel *channel)
{
    if (IfxCcu6_getTimerRunStatus(channel->ccu6, IfxCcu6_TimerId_t12) != IfxCcu6_TimerRunStatus_running)
    {
        // enable shadow transfer
        IfxCcu6_enableShadowTransfer(channel->ccu6, TRUE, FALSE);

        // start the timer
        if (channel->trigger.extInputTrigger != NULL_PTR)
        {
            // external input setting
            IfxCcu6_setExternalRunMode(channel->ccu6, IfxCcu6_TimerId_t12, channel->trigger.extInputTriggerMode);
            IfxCcu6_setT12InputSignal(channel->ccu6, channel->trigger.extInputTrigger);
        }
        else
        {
            // internal start
            IfxCcu6_startTimer(channel->ccu6, TRUE, FALSE);
        }
    }

    // enable capture mode
    IfxCcu6_setT12ChannelMode(channel->ccu6, channel->channelId, channel->channelMode);
}


void IfxCcu6_Icu_stopCapture(IfxCcu6_Icu_Channel *channel)
{
    // disable shadow transfer
    IfxCcu6_disableShadowTransfer(channel->ccu6, TRUE, FALSE);

    // disable capture mode
    IfxCcu6_setT12ChannelMode(channel->ccu6, channel->channelId, IfxCcu6_T12ChannelMode_off);

    // remove the external input
    IfxCcu6_setExternalRunMode(channel->ccu6, IfxCcu6_TimerId_t12, IfxCcu6_ExternalTriggerMode_disable);

    // stop the timer
    IfxCcu6_stopTimer(channel->ccu6, TRUE, FALSE);
}
