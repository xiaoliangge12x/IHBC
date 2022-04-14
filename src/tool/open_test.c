#include "tool/open_test.h"

// -------------------- driving table initilize ---------------------
static const Signal g_signal_table[] =
{
    {TYPE_INT32,   "object_num",           GetLightObjectNum},
    {TYPE_FLOAT32, "cal_lux_up",           GetCalLuxUp},
    {TYPE_UINT8,   "IFC_HMA_Enable",       GetIFC_HMA_Enable},
    {TYPE_UINT8,   "BCM_LowBeamSt",        GetBCM_LowBeamSt},
    {TYPE_UINT8,   "BCM_HighBeamSt",       GetBCM_HighBeamSt},
    {TYPE_UINT8,   "IFC_SysCalibrationSt", GetIFC_SysCalibrationSt},
    {TYPE_UINT8,   "IFC_CameraBlockageSt", GetIFC_CameraBlockageSt},
    {TYPE_UINT8,   "BCS_VehSpdVD",         GetBCS_VehSpdVD},
    {TYPE_FLOAT32, "BCS_VehSpd",           GetBCS_VehSpd},
};
// -------------------- function definition -----------------------------------------
// 从yaml中读取数据并设置
void ReadFromYamlAndSetData(const char* filename, SimulinkData* simulink_data)
{
    static char header_name[MAX_HEADER_NAME] = "";
    static char key_name[MAX_KEY_NAME] = "";
    static char value_name[MAX_VALUE_NAME] = "";
    static uint8 keyFlag = 0;

    FILE*  f = fopen(filename, "r");
    static yaml_parser_t parser;  
    static yaml_token_t  token;

    if (!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser.\n", stderr);
    }
    if (f == NULL) {
        fputs("Failed to open file!\n", stderr);
    }
    yaml_parser_set_input_file(&parser, f);   // 该工具是将文本句柄和解析器绑定的操作

    do {
        yaml_parser_scan(&parser, &token);
        switch(token.type){
            case YAML_KEY_TOKEN: {
                keyFlag = 1;
                break;
            }
            case YAML_VALUE_TOKEN: {
                keyFlag = 0;
                break;
            }
            
            case YAML_SCALAR_TOKEN: {
                if (keyFlag) {
                    if (strlen(key_name) != 0) {
                        // 说明无value， 刷新header name
                        memset(header_name, 0, MAX_HEADER_NAME);
                        strncpy(header_name, key_name, strlen(key_name));
                    }
                    memset(key_name, 0, MAX_KEY_NAME);
                    strncpy(key_name, token.data.scalar.value,
                            strlen(token.data.scalar.value));
                } else {
                    keyFlag = 0;
                    memset(value_name, 0, MAX_VALUE_NAME);
                    strncpy(value_name, token.data.scalar.value,
                            strlen(token.data.scalar.value));
                    SetSimulinkData(header_name, key_name, value_name, simulink_data);
                    memset(key_name, 0, MAX_KEY_NAME);
                }
            }
            /* Others */
            default: {
                int keyFlag = 0;
            }
        }
        if (token.type != YAML_STREAM_END_TOKEN) {
            yaml_token_delete(&token);
        }
    } while (token.type != YAML_STREAM_END_TOKEN);   // 令牌类型不到末尾
    
    yaml_token_delete(&token);
 
    yaml_parser_delete(&parser);
    fclose(f);
}

// 设置原始数据
void SetSimulinkData(const char header_str[], const char key_str[], const char value_str[], 
    SimulinkData* simulink_data) 
{
    static uint8 light_index      = 0;
    static uint8 category_index   = 0;
    static uint8 light_source_num = 0;
    static uint8 category_num     = 0;

    if (!strncmp(key_str, "light_source_num", strlen("light_source_num"))) {
        light_source_num = (uint8)atoi(value_str);
    } else if (!strncmp(key_str, "category_num", strlen("category_num"))) {
        category_num = (uint8)atoi(value_str);
    }

    if (!strncmp(header_str, "light_source", strlen("light_source"))) {
        if (!strncmp(key_str, "type", MAX_KEY_NAME)) {
            simulink_data->ihbc_result.light_source[light_index].type = 
                (uint8)atoi(value_str);
            return;
        } else if (!strncmp(key_str, "distance", MAX_KEY_NAME)) {
            simulink_data->ihbc_result.light_source[light_index].distance = 
                (float32)atof(value_str);
            ++light_index;
            return;
        } else {
            // do nothing;
        }
    } else if (!strncmp(header_str, "category", strlen("category"))) {
        if (!strncmp(key_str, "property_type", MAX_KEY_NAME)) {
            simulink_data->work_condition.category[category_index].property_type = 
                (uint8)atoi(value_str);
            return;
        } else if (!strncmp(key_str, "property", MAX_KEY_NAME)) {
            simulink_data->work_condition.category[category_index].property = 
                (int32)atoi(value_str);
            return;
        } else if (!strncmp(key_str, "property_conf", MAX_KEY_NAME)) {
            simulink_data->work_condition.category[category_index].property_conf = 
                (float32)atof(value_str);
            ++category_index;
            return;
        } else {

        }
    } else {
        // do nothing;
    }
    if (category_index == category_num) {
        category_index = 0;
    }
    if (light_index == light_source_num) {
        light_index = 0;
    }
    for (uint8 i = 0; i < (sizeof(g_signal_table) / sizeof(Signal)); ++i) {
        if (!strcmp(key_str, g_signal_table[i].signal_name)) {
            switch (g_signal_table[i].signal_type) {
                case TYPE_UINT8: {
                    *((uint8*)g_signal_table[i].signal_ptr_get(simulink_data)) =
                        (uint8)atoi(value_str);
                    break;
                }

                case TYPE_UINT16: {
                    *((uint16*)g_signal_table[i].signal_ptr_get(simulink_data)) =
                        (uint16)atoi(value_str);
                    break;
                }

                case TYPE_FLOAT32: {
                    *((float32*)g_signal_table[i].signal_ptr_get(simulink_data)) = 
                        (float32)atof(value_str);
                    break;
                }

                case TYPE_INT32: {
                    *((int32*)g_signal_table[i].signal_ptr_get(simulink_data)) = 
                        (int32)atoi(value_str);
                }

                default: {
                    // do nothing;
                }
            }
            return;
        }
    }
}

void* GetLightObjectNum(SimulinkData* simulink_data)
{
    return &(simulink_data->ihbc_result.object_num);
}

void* GetCalLuxUp(SimulinkData* simulink_data)
{
    return &(simulink_data->ihbc_result.cal_lux_up);
}

void* GetIFC_HMA_Enable(SimulinkData* simulink_data)
{
    return &(simulink_data->veh_info.IFC_HMA_Enable);
}

void* GetBCM_LowBeamSt(SimulinkData* simulink_data)
{
    return &(simulink_data->veh_info.BCM_LowBeamSt);
}

void* GetBCM_HighBeamSt(SimulinkData* simulink_data)
{
    return &(simulink_data->veh_info.BCM_HighBeamSt);
}

void* GetIFC_SysCalibrationSt(SimulinkData* simulink_data)
{
    return &(simulink_data->veh_info.IFC_SysCalibrationSt);
}

void* GetIFC_CameraBlockageSt(SimulinkData* simulink_data)
{
    return &(simulink_data->veh_info.IFC_CameraBlockageSt);
}

void* GetBCS_VehSpdVD(SimulinkData* simulink_data)
{
    return &(simulink_data->veh_info.BCS_VehSpdVD);
}

void* GetBCS_VehSpd(SimulinkData* simulink_data)
{
    return &(simulink_data->veh_info.BCS_VehSpd);
}
