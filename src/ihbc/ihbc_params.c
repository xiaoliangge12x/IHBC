#include "ihbc/ihbc_params.h"

Ihbc_Params ihbc_params;

void UpdateParams(const IHBCResult* ihbc_result, const WorkCondition* work_condition,
    const VehicleInfo* veh_info)
{
    ihbc_params.system_initialize_flag = true;
}