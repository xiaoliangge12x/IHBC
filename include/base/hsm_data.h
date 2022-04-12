#ifndef HSM_DATA_H_
#define HSM_DATA_H_

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

static const uint8_t MAX_HSM_DEPTH = 16U;
enum {MAX_HSM_STATES = 20U};
// ------------------------------- HSM枚举 ----------------
typedef enum {
    HSM_NO_SIG,
    HSM_ENTRY,
    HSM_EXIT,
    HSM_INITIAL,
    IHBC_USER_START = 10,
} Event;

typedef enum {
    ROOT = 0,     // topmost state
} State;

typedef enum {
    HSM_IGNORED,
    HSM_HANDLED,
    HSM_TRAN,
    HSM_SUPER,
} HsmRet;

// ------------------------------- HSM结构体 ---------------
typedef struct {
    uint8_t currentSt;   // 当前状态机状态
} Hsm;

typedef HsmRet (*dispatch)(const uint8_t event);
#endif