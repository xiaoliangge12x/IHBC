#include "ihbc/ihbc_hsm.h"

HsmRet dispatchForIHBCRunning(const uint8_t event)
{
    switch (event) {
        case IHBC_EVENT_DISABLE: {
            g_hsm.currentSt = IHBC_DISABLE;
            return HSM_TRAN;
        }

        case IHBC_EVENT_ERROR: {
            g_hsm.currentSt = IHBC_FAILURE;
            return HSM_TRAN;
        }

        case IHBC_EVENT_GLARE: {
            g_hsm.currentSt = IHBC_GLARE;
            return HSM_TRAN;
        }

        case IHBC_EVENT_ROADLIGHTING: {
            g_hsm.currentSt = IHBC_ROADLIGHTING;
            return HSM_TRAN;
        }

        case IHBC_EVENT_OVERTAKING: {
            g_hsm.currentSt = IHBC_OVERTAKING;
            return HSM_TRAN;
        }

        case IHBC_EVENT_DRIVEPAST: {
            g_hsm.currentSt = IHBC_DRIVEPAST;
            return HSM_TRAN;
        }

        case IHBC_EVENT_BRIGHTNESS: {
            g_hsm.currentSt = IHBC_BRIGHTNESS;
            return HSM_TRAN;
        }

        case IHBC_EVENT_ONCOMING: {
            g_hsm.currentSt = IHBC_ONCOMING;
            return HSM_TRAN;
        }

        case IHBC_EVENT_PRECEDING: {
            g_hsm.currentSt = IHBC_PRECEDING;
            return HSM_TRAN;
        }

        case IHBC_EVENT_BLINDNESS: {
            g_hsm.currentSt = IHBC_BLINDNESS;
            return HSM_TRAN;
        }

        case IHBC_EVENT_VELTOOLOW: {
            g_hsm.currentSt = IHBC_VELTOOLOW;
            return HSM_TRAN;
        }

        case IHBC_EVENT_NOTRAFFIC: {
            g_hsm.currentSt = IHBC_NOTRAFFIC;
            return HSM_TRAN;
        }

        default: {
            g_hsm.currentSt = ROOT;
            return HSM_SUPER;
        }
    }
}

// 状态分发函数
HsmRet dispatchForDisable(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInDisable();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForFailure(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInFailure();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForGlare(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInGlare();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForRoadlighting(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInRoadlighting();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForOvertaking(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInOvertaking();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForBrightness(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInBrightness();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForOncoming(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInOncoming();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForPreceding(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInPreceding();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForBlindness(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInBlindness();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForDrivePast(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInDrivePast();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForVehTooLow(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInVehTooLow();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}

HsmRet dispatchForNoTraffic(const uint8_t event)
{
    switch (event) {
        case IHBC_WORK: {
            workInNoTraffic();
            return HSM_HANDLED;
        }

        default: {
            g_hsm.currentSt = IHBC_RUNNING;
            return HSM_SUPER;
        }
    }
}