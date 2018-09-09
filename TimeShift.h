#pragma once
#include "SDK.h"
#include "Util.h"
//a
class TimeShift
{
public:

	void Run(CBaseEntity * pLocal, CUserCmd * pCommand);

	void TimeShifts(CBaseEntity * local, int value = 900);

	void TimeShifts(CBaseEntity * local, CUserCmd * cmd, int value = 900, bool disableattack = true);
};

extern TimeShift gShift;