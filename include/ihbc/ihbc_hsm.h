#ifndef IHBC_HSM_H_
#define IHBC_HSM_H_

#include "common.h"
#include "base/hsm_chart.h"
#include "ihbc_hsm_data.h"
#include "rte_type_data.h"

// ------------------------ global variable def --------------------------------
static float32 K_EnvBrightnessThreshold = 10.0;
static float32 K_VehSpdThreshold        = 30.0;
// ------------------------ global variable declaration ------------------------
extern IHBCResult       ihbc_result;
extern WorkCondition    work_condition;
extern VehicleInfo      veh_info;
extern IHBC2VehicleInfo output;
// ------------------------ function declaration -------------------------------
void InitUser();

void RunUser(const IHBCResult* ihbc_result, const WorkCondition* work_condition,
    const VehicleInfo* veh_info, IHBC2VehicleInfo* output);

// 状态分发函数
HsmRet dispatchForIHBCRunning(const uint8_t event);
HsmRet dispatchForDisable(const uint8_t event);
HsmRet dispatchForFailure(const uint8_t event);
HsmRet dispatchForGlare(const uint8_t event);
HsmRet dispatchForRoadlighting(const uint8_t event);
HsmRet dispatchForOvertaking(const uint8_t event);
HsmRet dispatchForBrightness(const uint8_t event);
HsmRet dispatchForOncoming(const uint8_t event);
HsmRet dispatchForPreceding(const uint8_t event);
HsmRet dispatchForBlindness(const uint8_t event);
HsmRet dispatchForDrivePast(const uint8_t event);
HsmRet dispatchForVehTooLow(const uint8_t event);
HsmRet dispatchForNoTraffic(const uint8_t event);

// 状态entry函数

// 状态exit函数

// 状态work函数
void workInDisable();
void workInFailure();
void workInGlare();
void workInRoadlighting();
void workInOvertaking();
void workInBrightness();
void workInOncoming();
void workInPreceding();
void workInBlindness();
void workInDrivePast();
void workInVehTooLow();
void workInNoTraffic();
 
bool IsIHBCNotActive(const uint8 hma_enable, const uint8 low_beam_st, const uint8 high_beam_st);
bool ValidateSIT(const uint8 cali_st);
bool DetectBlindness(const uint8 cam_blocked);
bool RoadlightingCond(const LightSource light_src);
bool BrightnessCond(const float32 env_lux);
bool VehSpdTooLow(const uint8 veh_spd_vd, const float32 veh_spd);
bool OvertakingCond();
bool OnComingCond();
bool PrecedingCond();
bool GlareCond();
bool BadWeatherCond(const uint8 property_type, const uint8 weather_type);
bool DrivePastCond();
#endif