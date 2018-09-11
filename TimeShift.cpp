#include "TimeShift.h"

TimeShift gShift;

void TimeShift::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	if (!gCvars.timeshift_enabled)
		return;

	if (Util->IsKeyPressed2(gCvars.timeshift_key) //G
							  //  && pLocal->GetLifeState() == LIFE_ALIVE
		&& !gInts.Engine->Con_IsVisible()
		&& gInts.Engine->IsConnected())
		TimeShifts(pLocal, pCommand, (int)gCvars.timeshift_value);
}
void TimeShift::TimeShifts(CBaseEntity * local, CUserCmd * cmd, int value, bool disableattack)
{
	if (local == NULL) return;

	if (local->GetLifeState() != LIFE_ALIVE) return;

	INetChannel* ch = (INetChannel*)gInts.Engine->GetNetChannelInfo();
	int& m_nOutSequenceNr = *(int*)((unsigned)ch + 8);
	m_nOutSequenceNr += value;
}

void TimeShift::TimeShifts(CBaseEntity *local, int value)
{
	TimeShifts(local, new CUserCmd(), value);
}