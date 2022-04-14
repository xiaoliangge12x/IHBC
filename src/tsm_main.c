#include <pthread.h>
#include <sys/wait.h>
#include "ihbc/ihbc_hsm.h"
#include "tool/open_test.h"
#include "common.h"
// -------------------------- global variable definition ------------------------
SimulinkData     simulink_data;
IHBC2VehicleInfo output;
struct timeval   g_timeval;

void Worker()
{   
    char* filename = "../../config/SImulinkDataInput.yaml";
    gettimeofday(&g_timeval, NULL);
    InitUser();
    while (1) {
        ReadFromYamlAndSetData(filename, &simulink_data);
        /* LOG(COLOR_YELLOW, "IFC_HMA_Enable: %d, BCM_LowBeamSt: %d,"
            "BCM_HighBeamSt: %d, IFC_SysCalibrationSt: %d, IFC_CameraBlockageSt: %d"
            "BCS_VehSpdVD: %d, BCS_VehSpd: %f", 
            simulink_data.veh_info.IFC_HMA_Enable,
            simulink_data.veh_info.BCM_LowBeamSt,
            simulink_data.veh_info.BCM_HighBeamSt,
            simulink_data.veh_info.IFC_SysCalibrationSt,
            simulink_data.veh_info.IFC_CameraBlockageSt,
            simulink_data.veh_info.BCS_VehSpdVD,
            simulink_data.veh_info.BCS_VehSpd); 
        LOG(COLOR_YELLOW, "category_0 property_type: %d, property: %d, property_conf: %f",
            simulink_data.work_condition.category[0].property_type,
            simulink_data.work_condition.category[0].property,
            simulink_data.work_condition.category[0].property_conf); */
        RunUser(&simulink_data.ihbc_result, &simulink_data.work_condition, 
            &simulink_data.veh_info, &output);
        usleep(20000);
    }
}

uint32_t main() 
{
    // 定义线程变量
    pthread_t main_thread;
    // 定义线程名称
    uint8_t *message = "main_thread";
    // 定义线程句柄
    uint32_t ret_thrd;

    ret_thrd = pthread_create(&main_thread, NULL, &Worker, (void*)message);

    // 线程创建失败
    if (ret_thrd != 0) {

        LOG(COLOR_RED, "main thread create failed!");
      exit(-1);
    } else {
        LOG(COLOR_GREEN, "main thread create success!");
    }
    // 等待线程终止
    pthread_join(main_thread, NULL);

    return 0;
}