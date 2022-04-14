#include "common.h"

void hb_TimeSync_GetTime(int64_t* value)
{
    struct timeval timeval_time;
    gettimeofday(&timeval_time, NULL);
    *value = (int64_t)(timeval_time.tv_sec - g_timeval.tv_sec) * S_TEN_NS_CONV_RATE + 
        (int64_t)(timeval_time.tv_usec - g_timeval.tv_usec) * US_TEN_NS_CONV_RATE;
}