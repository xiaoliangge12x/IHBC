#ifndef HSM_CHART_H_
#define HSM_CHART_H_

#include "hsm_data.h"

void Dispatch(const uint8_t event);

HsmRet ExitState(const uint8_t state);
HsmRet EntryState(const uint8_t state);
HsmRet GoSuperState(const uint8_t state);
HsmRet dispatchForRoot(const uint8_t event);

#endif