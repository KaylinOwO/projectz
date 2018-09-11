#pragma once
#include "SDK.h"

class CESP
{
public:

	void Run(CBaseEntity* pLocal);

private:

	void Player_ESP(CBaseEntity* pLocal, CBaseEntity* pEntity);
	//Other funcs

	void DrawBone(CBaseEntity* pEntity, int* iBones, int count, Color clrCol);

	void DrawFace(CBaseEntity* pEntity, Color clr);
};

extern CESP gESP;