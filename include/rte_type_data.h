#ifndef RTE_TYPE_DATA_H_
#define RTE_TYPE_DATA_H_

#include "common.h"

#define TYPE_NAME_MAX_LEN   20U
#define LIGHTSOURCE_MAX_NUM 64U
#define CATEGORY_MAX_NUM    4U

// ------------------------------ typedef ----------------------------------------
typedef enum {
    LIGHT_SOURCE_HEADLIGHT = 0,    // 头灯
    LIGHT_SOURCE_TAILLIGHT,        // 尾灯
    LIGHT_SOURCE_STREETLIGHT,      // 路灯
} LightSourceType;

typedef enum {
    WORK_CONDITION_WEATHER = 0,   // 天气
    WORK_CONDITION_LIGHT,         // 光线条件
    WORK_CONDITION_SCENE,         // 场景
    WORK_CONDITION_TIME,          // 时间
} WorkConditionType;

typedef enum {
    WEATHER_NORMAL = 0,           // 晴天
    WEATHER_NORMAL2,              // 阴天
    WEATHER_RAINY,                // 小雨
    WEATHER_SNOWY,                // 雪天
    WEATHER_HEAVYRAIN,            // 大雨
    WEATHER_OTHER,                // 其他天气环境
} WeatherType;

typedef enum {
    LIGHT_NATURAL_LIGHT = 0,      // 自然光
    LIGHT_LAMP_LIGHT,             // 灯光
    LIGHT_HARD_LIGHT,             // 强光
    LIGHT_LOW_SUN,                // 夕阳光
    LIGHT_DARK,                   // 黑暗
    LIGHT_OTHER,                  // 其他
} LightType;

typedef enum {
    SCENE_HIGHWAY = 0,            // 高速公路
    SCENE_URBAN,                  // 城区道路
    SCENE_RURAL,                  // 乡村道路
    SCENE_TUNNEL,                 // 隧道
    SCENE_CHARGE_STATION,         // 收费站
    SCENE_OTHER,                  // 其他
} SceneType;

typedef enum {
    TIME_DAY = 0,                  // 白天
    TIME_NIGHT,                    // 夜晚
    TIME_OTHER,                    // 其他
} TimeType;

typedef enum {
    HMA_SWITCH_NO_CMD = 0,
    HMA_SWITCH_NO_ACTIVE,
    HMA_SWITCH_ACTIVE,
    HMA_SWITCH_NOT_USED,
} HMASwitchCmd;

typedef enum {
    CAM_SYS_CALI_SUCCESS = 0,
    CAM_SYS_CALI_IN_PROCESS,
    CAM_SYS_CALI_FAILED,
} CamSysCaliSt; 

// 参照J3 IHC输出
typedef struct {
    float32 left;
    float32 top;
    float32 right;
    float32 bottom;
} Rect;

typedef struct {
    Rect              rect;                          // 灯源大小
    LightSourceType   type;                          // 灯源类型
    char              type_name[TYPE_NAME_MAX_LEN];  // 灯源类型的字符串表示
    float32           distance;                      // 灯源的距离
} LightSource;

typedef struct {
    int32 cam_idx;    // 摄像头索引ID， 前视默认0
    int32 frame_id;   // 图像帧ID
    int64 time_stamp; // 时间戳
} Header;

typedef struct {
    WorkConditionType property_type;                     // 特征类型
    int32             property;                          // 特征值
    char              property_name[TYPE_NAME_MAX_LEN];  // 特征值的字符串表示
    float32           property_conf;                     // 特征的置信度
} Category;

typedef struct {
    Header      header;             // 帧头信息
    uint8       light_on;           // 大灯状态：1 开启， 0 关闭
    LightSource light_source[LIGHTSOURCE_MAX_NUM];       // 处理后灯源信息
    float32     cal_lux_up;         // 环境光亮度
    LightSource light_source_raw[LIGHTSOURCE_MAX_NUM];   // 原始灯源信息
    int32       object_num;         // 光源检测数量
} IHBCResult;

typedef struct {
    Header   header;                        // 帧头信息
    Category category[CATEGORY_MAX_NUM];    // 工况类型
} WorkCondition;

typedef struct {
    uint8   IFC_HMA_Enable;      // HMA 使能信号， 这个信号是我需要的
    // uint8 ACU_HMA_SwitchReq;   // HMA 开关信号， 这个可以理解为用户操作， 可能不需要这个信号
    uint8   BCM_LowBeamSt;       // 近光灯状态, 0 - 未激活， 1 - 激活
    uint8   BCM_HighBeamSt;      // 远光灯状态，0 - 未激活， 1 - 激活
    uint8   IFC_SysCalibrationSt; // 摄像头标定状态
    uint8   IFC_CameraBlockageSt; // 摄像头被遮挡, 1 - block, 0 - no block
    uint8   BCS_VehSpdVD;    // 车速有效位
    float32 BCS_VehSpd;    // 车速
} VehicleInfo;

typedef struct {
    uint8 IFC_HMA_St;              // HMA的状态
    uint8 IFC_HMA_HighBeamReq;     // HMA 远光灯请求， 1-请求远光灯  ，0-请求近光灯
} IHBC2VehicleInfo;

typedef struct {
    IHBCResult       ihbc_result;
    WorkCondition    work_condition;
    VehicleInfo      veh_info;
} SimulinkData;
#endif