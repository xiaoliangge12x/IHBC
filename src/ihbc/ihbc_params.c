#include "ihbc/ihbc_params.h"

// ----------------------------- cal variable def --------------------
float32 K_EnvBrightnessThreshold     = 10.0;
float32 K_VehSpdThreshold            = 30.0;
float32 K_OncomingDistanceThreshold  = 50.0;
float32 K_PrecedingDistanceThreshold = 40.0;

float32 K_OncommingDisappearThreshold = 5.0;
float32 K_PrecedingDisappearThreshold = 6.0;
// ----------------------------- global variable def -----------------
Ihbc_Params         ihbc_params;
static TimingParams g_timing_params_table[MAX_TIMING_NUM];

void UpdateParams(const IHBCResult* ihbc_result, const WorkCondition* work_condition,
    const VehicleInfo* veh_info)
{
    ihbc_params.system_initialize_flag = true;

    ihbc_params.oncoming_min_distance  = FLT_MAX;
    ihbc_params.preceding_min_distance = FLT_MAX;
    for (uint8 i = 0; i < ihbc_result->object_num; ++i) {
        if (ihbc_result->light_source[i].type == LIGHT_SOURCE_HEADLIGHT) {
            if (ihbc_params.oncoming_min_distance > ihbc_result->light_source[i].distance) {
                ihbc_params.oncoming_min_distance = ihbc_result->light_source[i].distance;
            }
        } else if (ihbc_result->light_source[i].type == LIGHT_SOURCE_TAILLIGHT) {
            if (ihbc_params.preceding_min_distance > ihbc_result->light_source[i].distance) {
                ihbc_params.preceding_min_distance = ihbc_result->light_source[i].distance;
            }
        } else {
            // temprary do nothing;
        }
    }

    ihbc_params.is_bad_weather_cond = false;
    for (uint8 i = 0; i < CATEGORY_MAX_NUM; ++i) {
        if (work_condition->category[i].property_type == WORK_CONDITION_WEATHER) {
            if ((work_condition->category[i].property == WEATHER_HEAVYRAIN) ||
                (work_condition->category[i].property == WEATHER_RAINY) ||
                (work_condition->category[i].property == WEATHER_SNOWY) ||
                (work_condition->category[i].property == WEATHER_OTHER)) {
                ihbc_params.is_bad_weather_cond = true;
            }
        }
    }
}

bool IsBitSet(const uint32 event_bitfields, const uint8 bit_no)
{
    if ((event_bitfields & ((uint32)1U << bit_no)) != 0) {
        return true;
    } else {
        return false;
    }
}

void SetSignalBitFields(uint32* event_bitfields, const uint8 bit_no)
{
    *event_bitfields |= (uint32)1U << bit_no;
}

void ResetSignalBitFields(uint32* event_bitfields, const uint8 bit_no)
{
    *event_bitfields &= ~((uint32)1U << bit_no);
}

void StartTiming(int64* cur_time, uint8* flag)
{
    hb_TimeSync_GetTime(cur_time);
    *flag = 1;
}

void StopTiming(uint8* flag)
{
    *flag = 0;
}

float32 GetTimeGapInSec(const int64 start_time, const uint8 flag)
{
    static const float32 S_TEN_NS_CONV_RATE_FLOAT_TYPE = 100000000.0;

    if (flag == 1) {
        int64 cur_time = 0;
        hb_TimeSync_GetTime(&cur_time);
        return (float32)(cur_time - start_time) / S_TEN_NS_CONV_RATE_FLOAT_TYPE;
    } else {
        return 0.0;
    }
}

void SetExpiredTime(TimingMark timing_mark, float32 expired_time)
{
    g_timing_params_table[timing_mark].expired_time = expired_time;
    hb_TimeSync_GetTime(&g_timing_params_table[timing_mark].start_time);
    g_timing_params_table[timing_mark].is_running = true;
}

bool IsExpired(TimingMark timing_mark) 
{
    static const float32 S_TEN_NS_CONV_RATE_FLOAT_TYPE = 100000000.0;

    if (g_timing_params_table[timing_mark].is_running) {
        int64 cur_time   = 0;
        int64 start_time = g_timing_params_table[timing_mark].start_time;

        hb_TimeSync_GetTime(&cur_time);
        LOG(COLOR_RED, "timing_mark: %d, time pass: %f, expired time: %f", 
            timing_mark, (float32)(cur_time - start_time) / S_TEN_NS_CONV_RATE_FLOAT_TYPE,
        g_timing_params_table[timing_mark].expired_time);
        if ((float32)(cur_time - start_time) / S_TEN_NS_CONV_RATE_FLOAT_TYPE >
            g_timing_params_table[timing_mark].expired_time) {  
            g_timing_params_table[timing_mark].is_running = false;
        }
    }
    return !g_timing_params_table[timing_mark].is_running; 
}