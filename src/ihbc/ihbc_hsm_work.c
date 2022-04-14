#include "ihbc/ihbc_hsm.h"

void workInDisable()
{
    LOG(COLOR_NONE, "Disable St.");
}

void workInFailure()
{
    LOG(COLOR_NONE, "Failure St.");
}

void workInGlare()
{
    LOG(COLOR_NONE, "Glare St.");
}

void workInRoadlighting()
{
    LOG(COLOR_NONE, "RoadLighting St.");
}

void workInOvertaking()
{
    LOG(COLOR_NONE, "OverTaking St.");
}

void workInBrightness()
{
    LOG(COLOR_NONE, "Brightnes St.");
}

void workInOncoming()
{
    LOG(COLOR_NONE, "OnComing St.");
}

void workInPreceding()
{
    LOG(COLOR_NONE, "Preceding St.");
}

void workInBlindness()
{
    LOG(COLOR_NONE, "Blindness St.");
}

void workInDrivePast()
{
    LOG(COLOR_NONE, "DrivePast St.");
}

void workInVehTooLow()
{
    LOG(COLOR_NONE, "Veh Too Low St.");
}

void workInNoTraffic()
{
    LOG(COLOR_NONE, "No Traffic st.");
}

// 状态exit函数
void exitForOncoming()
{
    SetExpiredTime(ONCOMING_TIMING, K_OncommingDisappearThreshold);
}

void exitForPreceding()
{
    SetExpiredTime(PRECEDING_TIMING, K_PrecedingDisappearThreshold);
}