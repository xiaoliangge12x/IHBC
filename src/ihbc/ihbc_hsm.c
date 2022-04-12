
#include "ihbc/ihbc_hsm.h"
#include "ihbc/ihbc_params.h"

//
void InitUser()
{
    memset(&ihbc_result, 0, sizeof(IHBCResult));
    memset(&work_condition, 0, sizeof(WorkCondition));
    memset(&veh_info, 0, sizeof(VehicleInfo));
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

    static uint8_t ihbc_event = IHBC_USER_START;
    static bool    grave_error = false;
    
    // SIT 分析, 严重故障
    grave_error = ValidateSIT(veh_info->IFC_SysCalibrationSt);

    // 事件判断
    if (IsIHBCNotActive(veh_info->IFC_HMA_Enable, veh_info->BCM_LowBeamSt, 
        veh_info->BCM_HighBeamSt)) {
        ihbc_event = IHBC_EVENT_DISABLE;
    } else if (grave_error) {
        ihbc_event = IHBC_EVENT_ERROR;
    } else if (DetectBlindness(veh_info->IFC_CameraBlockageSt)) {
        ihbc_event = IHBC_EVENT_BLINDNESS;
    // ------------------- 路灯  ---------------------------------
    } else if (RoadlightingCond(ihbc_result->light_source)) {
        ihbc_event = IHBC_EVENT_ROADLIGHTING;
    // ------------------- 环境光亮度 -----------------------------
    } else if (BrightnessCond(ihbc_result->cal_lux_up)) {
        ihbc_event = IHBC_EVENT_BRIGHTNESS;
    } else if (GlareCond()) {
        ihbc_event = IHBC_EVENT_GLARE;
    } else if (BadWeatherCond(work_condition->category.property_type, work_condition->category.property)) {
        ihbc_event = IHBC_EVENT_BADWEATHER;
    // ------------------ 车辆运动状态 ----------------------------
    } else if (VehSpdTooLow(veh_info->BCS_VehSpdVD, veh_info->BCS_VehSpd)) {
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

    // 进行状态分发
    Dispatch(ihbc_event);
    
    // do work
    ihbc_event = IHBC_WORK;
    // 进行状态分发
    Dispatch(ihbc_event);
}

bool IsIHBCNotActive(const uint8 hma_enable, const uint8 low_beam_st, const uint8 high_beam_st)
{
    return ((!hma_enable) || (!low_beam_st && !high_beam_st));
}

bool ValidateSIT(const uint8 cali_st)
{
    return (cali_st == CAM_SYS_CALI_SUCCESS);
}

bool DetectBlindness(const uint8 cam_blocked)
{
    return cam_blocked;
}

bool RoadlightingCond(const LightSource light_src)
{
    return (light_src.type == LIGHT_SOURCE_STREETLIGHT);
}

bool BrightnessCond(const float32 env_lux)
{
    return (env_lux >= K_EnvBrightnessThreshold);
}

bool VehSpdTooLow(const uint8 veh_spd_vd, const float32 veh_spd)
{
    return (!veh_spd_vd || veh_spd < K_VehSpdThreshold);
}

bool OvertakingCond()
{
    // TODO:
    return false;
}

bool OnComingCond()
{
    // TODO:
    return false;   
}

bool PrecedingCond()
{
    // TODO:
    return false;
}

bool GlareCond()
{
    // TODO:
    return false;
}

bool BadWeatherCond(const uint8 property_type, const uint8 weather_type)
{
    if (property_type == WORK_CONDITION_WEATHER) {
        if ((weather_type == WEATHER_HEAVYRAIN) || (weather_type == WEATHER_RAINY) ||
            (weather_type == WEATHER_SNOWY) || (weather_type == WEATHER_OTHER)) {
            return true;
        }
    }
    return false;
}

bool DrivePastCond()
{
    // TODO:
    return false;
}