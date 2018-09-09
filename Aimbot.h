#pragma once
#include "SDK.h"



class CAimbot
{
public:

	void Run(CBaseEntity* pLocal, CUserCmd* pCommand);

	float GetFOV(Vector angle, Vector src, Vector dst);

	bool CanAmbassadorHeadshot(CBaseEntity* pLocal);

private:

	void w(bool silent, Vector vAngs, CUserCmd* pCommand);

	void MakeVector(Vector angle, Vector& vector);

	Vector calc_angle(Vector src, Vector dst);

	int GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCommand);

	int GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity);
};



extern CAimbot gAim; //dsajkhfdlksahf