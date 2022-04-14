#include "base/hsm_chart.h"

// 定义全局变量
Hsm g_hsm;
dispatch g_dispatchArray[MAX_HSM_STATES] = {dispatchForRoot};

void Dispatch(const uint8_t event)
{
    // 存储当前状态
    uint8_t currentSt = g_hsm.currentSt;
    // 存储目标状态
    uint8_t targetSt = g_hsm.currentSt;
    // 存储源状态
    uint8_t sourceSt = g_hsm.currentSt;
    // 返回当前状态句柄调用的结果
    HsmRet ret = HSM_SUPER;

    // 找其父状态
    do {
        sourceSt = g_hsm.currentSt;
        ret = g_dispatchArray[g_hsm.currentSt](event);
    } while (ret == HSM_SUPER);
    LOG(COLOR_NONE, "[Dispatch] sourceSt: %d, g_hsm.currentSt: %d", sourceSt, g_hsm.currentSt);
    // 需要跳转
    if (ret == HSM_TRAN) {
        // 保存跳转的目标状态
        targetSt = g_hsm.currentSt;

        // ??
        while (currentSt != sourceSt) {
            ret = ExitState(currentSt);
            if (ret == HSM_HANDLED) {
                GoSuperState(currentSt);
            }
            currentSt = g_hsm.currentSt;
        }

        // 保存目标状态的状态数组
        uint8_t routinePath[MAX_HSM_DEPTH];
        uint8_t routinePathIndex = 0;

        routinePath[routinePathIndex++] = targetSt;

        // 如果源状态 == 目标状态, 执行源状态的退出操作即可
        if (sourceSt == targetSt) {
            ExitState(sourceSt);
        } else {
            currentSt = targetSt;
            // 找到目标状态的父状态
            GoSuperState(currentSt);
            currentSt = g_hsm.currentSt;

            // 如果源状态 == 目标状态的父状态，不做任何事
            if (sourceSt != currentSt) {
                // 找到源状态的父状态
                GoSuperState(sourceSt);
                // 如果源状态的父状态 == 目标状态的父状态
                if (g_hsm.currentSt == currentSt) {
                    ExitState(sourceSt);
                } else {
                    // 如果 源状态的父状态 == 目标状态
                    if (g_hsm.currentSt == targetSt) {
                        ExitState(sourceSt);
                        routinePathIndex = 0;
                    } else { 
                        routinePath[routinePathIndex++] = currentSt;
                        // 新建跳转路径是否找到标志位
                        bool routinePathFound = false;

                        // 找目标状态的父状态
                        ret = GoSuperState(currentSt);
                        while (ret == HSM_SUPER) {
                            if (g_hsm.currentSt == sourceSt) {
                                routinePathFound = true;
                                ret = HSM_HANDLED;
                            } else {
                                routinePath[routinePathIndex++] = g_hsm.currentSt;
                                ret = GoSuperState(g_hsm.currentSt);
                            }
                        }

                        if (!routinePathFound) {
                            currentSt = sourceSt;
                            do {
                                // 源状态不断向父状态方向前进，做退出操作
                                ret = ExitState(currentSt);
                                if (ret == HSM_HANDLED) {
                                    GoSuperState(currentSt);
                                }
                                currentSt = g_hsm.currentSt;
                                do {
                                    // 如果找到和目标类一样的父类，那么直接退出，做entry操作
                                    if (currentSt == routinePath[routinePathIndex - 1]) {
                                        routinePathFound = true;
                                    } else {
                                        --routinePathIndex;
                                    }
                                } while ((!routinePathFound) && (routinePathIndex > 0));
                            } while (!routinePathFound);
                        }
                    }
                }
            }
        }

        for (int8_t i = (routinePathIndex - 1); i >= 0; --i) {
            currentSt = routinePath[i];
            EntryState(currentSt);
        }

        g_hsm.currentSt = targetSt;

        DoInitialTransition(false);

        targetSt = g_hsm.currentSt;
    }
    g_hsm.currentSt = targetSt;
}

void DoInitialTransition(const bool topmost)
{
    uint8_t currentSt = g_hsm.currentSt;
    
    if (topmost) {
        currentSt = ROOT;
    }

    uint8_t routinePath[MAX_HSM_DEPTH];
    uint8_t routinePathIndex = 0;

    uint8_t event = HSM_INITIAL;
    while (g_dispatchArray[g_hsm.currentSt](event) == HSM_TRAN) {
        routinePath[routinePathIndex++] = g_hsm.currentSt;

        GoSuperState(g_hsm.currentSt);  // father of target state
        while (g_hsm.currentSt != currentSt) {
            routinePath[routinePathIndex++] = g_hsm.currentSt;
            GoSuperState(g_hsm.currentSt);
        }

        for (int8_t i = (routinePathIndex - 1); i >= 0; --i) {
            currentSt = routinePath[i];
            EntryState(currentSt);
        }

        g_hsm.currentSt = currentSt;
    }

    g_hsm.currentSt = currentSt;
}

HsmRet ExitState(const uint8_t state)
{
    HsmRet ret = g_dispatchArray[state](HSM_EXIT);
    return ret;
}

HsmRet EntryState(const uint8_t state)
{
    HsmRet ret = g_dispatchArray[state](HSM_ENTRY);
    return ret;
}

HsmRet GoSuperState(const uint8_t state)
{
    HsmRet ret = g_dispatchArray[state](HSM_NO_SIG);
    return ret;
}

HsmRet dispatchForRoot(const uint8_t event)
{
    (void)event;
    return HSM_IGNORED;
}