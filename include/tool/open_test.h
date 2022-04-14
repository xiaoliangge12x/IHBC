#ifndef OPEN_TEST_H_
#define OPEN_TEST_H_

#include <yaml.h>
#include <stdlib.h>
#include <string.h>
#include "rte_type_data.h"
// -------------------------  macro         ---------------------------------
#define MAX_HEADER_NAME        50U
#define MAX_KEY_NAME           50U
#define MAX_VALUE_NAME         20U
#define SIGNAL_NAME_MAX_LEN    50U

// -------------------------  typedef       ----------------------------------
typedef void* (*GetSignalPtr) (SimulinkData* simulink_data);

typedef enum {
    TYPE_UINT8 = 0,
    TYPE_UINT16,
    TYPE_FLOAT32,
    TYPE_INT32,
} SignalType;

typedef struct {
    SignalType   signal_type;  
    char         signal_name[SIGNAL_NAME_MAX_LEN];
    GetSignalPtr signal_ptr_get;
} Signal;

// -------------------------  driving table -----------------------------------
// static const Signal g_signal_table[];  // 此处非声明，而是定义

// -------------------------  function declaration ----------------------------
void  ReadFromYamlAndSetData(const char* filename, SimulinkData* simulink_data);
void SetSimulinkData(const char header_str[], const char key_str[], const char value_str[], 
    SimulinkData* simulink_data);
void* GetLightObjectNum(SimulinkData* simulink_data);
void* GetCalLuxUp(SimulinkData* simulink_data);
void* GetIFC_HMA_Enable(SimulinkData* simulink_data);
void* GetBCM_LowBeamSt(SimulinkData* simulink_data);
void* GetBCM_HighBeamSt(SimulinkData* simulink_data);
void* GetIFC_SysCalibrationSt(SimulinkData* simulink_data);
void* GetIFC_CameraBlockageSt(SimulinkData* simulink_data);
void* GetBCS_VehSpdVD(SimulinkData* simulink_data);
void* GetBCS_VehSpd(SimulinkData* simulink_data);
#endif