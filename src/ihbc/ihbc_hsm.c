
#include "ihbc/ihbc_hsm.h"

// ------------------------- global variable def ---------------------------------
static uint32 g_ihbc_signal_bitfields = 0U;
// ------------------------- func def  -------------------------------------------
void InitUser()
{
    memset(&simulink_data.ihbc_result, 0, sizeof(IHBCResult));
    memset(&simulink_data.work_condition, 0, sizeof(WorkCondition));
    memset(&simulink_data.veh_info, 0, sizeof(VehicleInfo));
    memset(&output, 0, sizeof(IHBC2VehicleInfo));
    // 刷新状态分发函数指针数组
    g_dispatchArray[IHBC_RUNNING]      = dispatchForIHBCRunning;
    g_dispatchArray[IHBC_DISABLE]      = dispatchForDisable;
    g_dispatchArray[IHBC_FAILURE]      = dispatchForFailure;
    g_dispatchArray[IHBC_GLARE]        = dispatchForGlare;
    g_dispatchArray[IHBC_ROADLIGHTING] = dispatchForRoadlighting;
    g_dispatchArray[IHBC_OVERTAKING]   = dispatchForOvertaking;
    g_dispatchArray[IHBC_BRIGHTNESS]   = dispatchForBrightness;
    g_dispatchArray[IHBC_ONCOMING]     = dispatchForOncoming;
    g_dispatchArray[IHBC_PRECEDING]    = dispatchForPreceding;
    g_dispatchArray[IHBC_BLINDNESS]    = dispatchForBlindness;
    g_dispatchArray[IHBC_DRIVEPAST]    = dispatchForDrivePast;
    g_dispatchArray[IHBC_VELTOOLOW]    = dispatchForVehTooLow;
    g_dispatchArray[IHBC_NOTRAFFIC]    = dispatchForNoTraffic;

    g_hsm.currentSt = IHBC_RUNNING;
    // DoInitialTransition(true);
}

void RunUser(const IHBCResult* ihbc_result, const WorkCondition* work_condition,
    const VehicleInfo* veh_info, IHBC2VehicleInfo* output)
{
    // 更新参数
    UpdateParams(ihbc_result, work_condition, veh_info);

    static uint8_t ihbc_event  = IHBC_USER_START;
    static bool    grave_error = false;
    
    // SIT 分析, 严重故障判断
    grave_error = ValidateSIT(ihbc_result, work_condition, veh_info);

    // 事件判断
    if (IsIHBCNotActive()) {
        ihbc_event = IHBC_EVENT_DISABLE;
    } else if (grave_error) {
        ihbc_event = IHBC_EVENT_ERROR;
    } else if (DetectBlindness()) {
        ihbc_event = IHBC_EVENT_BLINDNESS;
    // ------------------- 路灯  ---------------------------------
    } else if (RoadlightingCond()) {
        ihbc_event = IHBC_EVENT_ROADLIGHTING;
    // ------------------- 环境光亮度 -----------------------------
    } else if (BrightnessCond()) {
        ihbc_event = IHBC_EVENT_BRIGHTNESS;
    } else if (GlareCond()) {
        ihbc_event = IHBC_EVENT_GLARE;
    } else if (BadWeatherCond()) {
        ihbc_event = IHBC_EVENT_BADWEATHER;
    // ------------------ 车辆运动状态 ----------------------------
    } else if (VehSpdTooLow()) {
        ihbc_event = IHBC_EVENT_VELTOOLOW;
    } else if (OvertakingCond()) {
        ihbc_event = IHBC_EVENT_OVERTAKING;
    } else if (OnComingCond()) {
        ihbc_event = IHBC_EVENT_ONCOMING;
    } else if (PrecedingCond()) {
        ihbc_event = IHBC_EVENT_PRECEDING;
    } else if (DrivePastCond()) {
        ihbc_event = IHBC_EVENT_DRIVEPAST;
    } else {
        ihbc_event = IHBC_EVENT_NOTRAFFIC;
    }
    // LOG(COLOR_RED, "event_id: %d", ihbc_event);
    // 进行状态分发
    Dispatch(ihbc_event);
    
    // do work
    ihbc_event = IHBC_WORK;
    // 进行状态分发
    Dispatch(ihbc_event);
}

bool ValidateSIT(const IHBCResult* ihbc_result, const WorkCondition* work_condition,
    const VehicleInfo* veh_info)
{
    if (veh_info->IFC_SysCalibrationSt != CAM_SYS_CALI_SUCCESS) {
        return true;
    }

    g_ihbc_signal_bitfields = 0; 
    if (!veh_info->IFC_HMA_Enable || (!veh_info->BCM_LowBeamSt && !veh_info->BCM_HighBeamSt)) {
        SetSignalBitFields(&g_ihbc_signal_bitfields, IHBC_BITNO_DISABLE);
    }

    if (veh_info->IFC_CameraBlockageSt) {
        SetSignalBitFields(&g_ihbc_signal_bitfields, IHBC_BITNO_BLINDNESS);
    }

    if (ihbc_result->cal_lux_up >= K_EnvBrightnessThreshold) {
        SetSignalBitFields(&g_ihbc_signal_bitfields, IHBC_BITNO_BRIGHTNESS);
    }

    if (!veh_info->BCS_VehSpdVD || veh_info->BCS_VehSpd < K_VehSpdThreshold) {
        SetSignalBitFields(&g_ihbc_signal_bitfields, IHBC_BITNO_VELTOOLOW);
    }

    if (ihbc_params.is_bad_weather_cond) {
        SetSignalBitFields(&g_ihbc_signal_bitfields, IHBC_BITNO_BADWEATHER);
    }

    if (ihbc_params.oncoming_min_distance <= K_OncomingDistanceThreshold) {
        SetSignalBitFields(&g_ihbc_signal_bitfields, IHBC_BITNO_ONCOMING);
    }

    if (ihbc_params.preceding_min_distance <= K_PrecedingDistanceThreshold) {
        SetSignalBitFields(&g_ihbc_signal_bitfields, IHBC_BITNO_PRECEDING);
    }
    return false;
}

bool IsIHBCNotActive()
{
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_DISABLE);
}

bool DetectBlindness()
{
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_BLINDNESS);
}

// TODO: 判断路灯
bool RoadlightingCond()
{
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_ROADLIGHTING);
}

// TODO: 判断亮度
bool BrightnessCond()
{
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_BRIGHTNESS);
}

bool VehSpdTooLow()
{
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_VELTOOLOW);
}

bool OvertakingCond()
{
    // TODO:
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_OVERTAKING);
}

bool OnComingCond()
{
    // TODO:
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_ONCOMING);
}

bool PrecedingCond()
{
    // TODO:
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_PRECEDING);
}

bool GlareCond()
{
    // TODO:
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_GLARE);
}

bool BadWeatherCond()
{
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_BADWEATHER);
}

bool DrivePastCond()
{
    // TODO:
    return IsBitSet(g_ihbc_signal_bitfields, IHBC_BITNO_DRIVEPAST);
}