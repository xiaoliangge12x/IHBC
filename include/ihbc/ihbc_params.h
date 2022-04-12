#ifndef IHBC_PARAMS_H_
#define IHBC_PARAMS_H_

#include <stdbool.h>
#include "rte_type_data.h"

typedef struct {
    // 该处放一些需要转换的参数
    bool system_initialize_flag;    // 系统初始化状态
} Ihbc_Params;

extern Ihbc_Params ihbc_params;
// 更新输入参数
void UpdateParams();

#endif