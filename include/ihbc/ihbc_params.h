#ifndef IHBC_PARAMS_H_
#define IHBC_PARAMS_H_

#include <stdbool.h>
#include <float.h>
#include "rte_type_data.h"

// ----------------------- macro  -----------------------------
#define MAX_TIMING_NUM 10U
// ----------------------- typedef ----------------------------
typedef struct {
    // 该处放一些需要转换的参数
    bool         system_initialize_flag;   // 系统初始化状态
    float32      oncoming_min_distance;    // 对向来车光源的最近距离
    float32      preceding_min_distance;   // 同向车辆光源的最近距离
    bool         is_bad_weather_cond;      // 是否是坏天气
} Ihbc_Params;

typedef enum 
{
    ONCOMING_TIMING  = 0,
    PRECEDING_TIMING,
} TimingMark;

typedef struct 
{
    int64       start_time;
    float32     expired_time;
    bool        is_running;
} TimingParams;

// ---------------------------- extern ---------------------------
extern float32 K_EnvBrightnessThreshold;
extern float32 K_VehSpdThreshold;
extern float32 K_OncomingDistanceThreshold;
extern float32 K_PrecedingDistanceThreshold;

extern float32 K_OncommingDisappearThreshold;
extern float32 K_PrecedingDisappearThreshold;
extern Ihbc_Params ihbc_params;
// 更新输入参数
void UpdateParams(const IHBCResult* ihbc_result, const WorkCondition* work_condition,
    const VehicleInfo* veh_info);
bool IsBitSet(const uint32 event_bitfields, const uint8 bit_no);
void SetSignalBitFields(uint32* event_bitfields, const uint8 bit_no);
void ResetSignalBitFields(uint32* event_bitfields, const uint8 bit_no);
void StartTiming(int64* cur_time, uint8* flag);   // 开始计时
void StopTiming(uint8* flag);    // 停止计时
float32 GetTimeGapInSec(const int64 start_time, const uint8 flag); // 计算持续时间
void SetExpiredTime(TimingMark timing_mark, float32 expired_time);
bool IsExpired(TimingMark timing_mark);
#endif