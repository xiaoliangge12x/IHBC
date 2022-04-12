#include <pthread.h>
#include <sys/wait.h>
#include "ihbc/ihbc_hsm.h"
#ifdef _NEED_LOG
#include "common.h"
#endif
// -------------------------- global variable definition ------------------------
IHBCResult       ihbc_result;
WorkCondition    work_condition;
VehicleInfo      veh_info;
IHBC2VehicleInfo output;

void Worker()
{   
    InitUser();
    while (1) {
      RunUser(&ihbc_result, &work_condition, &veh_info, &output);
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
#ifdef _NEED_LOG
      LOG("main thread create failed!");
#endif
      exit(-1);
    } else {
#ifdef _NEED_LOG
      LOG("main thread create success!");
#endif
    }
    // 等待线程终止
    pthread_join(main_thread, NULL);

    return 0;
}