#ifndef IHBC_HSM_H_
#define IHBC_HSM_H_

#include "common.h"
#include "base/hsm_chart.h"
#include "ihbc/ihbc_params.h"
#include "ihbc_hsm_data.h"
#include "rte_type_data.h"

// ------------------------ global variable declaration ------------------------
extern SimulinkData     simulink_data;
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
void exitForOncoming();
void exitForPreceding();

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

bool ValidateSIT(const IHBCResult* ihbc_result, const WorkCondition* work_condition,
    const VehicleInfo* veh_info);
bool IsIHBCNotActive();
bool DetectBlindness();
bool RoadlightingCond();
bool BrightnessCond();
bool VehSpdTooLow();
bool OvertakingCond();
bool OnComingCond();
bool PrecedingCond();
bool GlareCond();
bool BadWeatherCond();
bool DrivePastCond();
#endif