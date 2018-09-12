#pragma once
#include "SDK.h"
#include "Aimbot.h"

struct BacktrackData
{
	int tickcount;
	Vector hitboxpos;
	matrix3x4 matrix[128];
};

class CBacktracking
{
public:
	void Run(CBaseEntity* pLocal, CUserCmd* pCommand);

private:

};

extern BacktrackData headPositions[24][12];
extern CBacktracking gBacktracking;