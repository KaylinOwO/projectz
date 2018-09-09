#pragma once
#include "SDK.h"

class CUtil
{
public:
	void VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut);

	void FixMove(CUserCmd* pCmd, Vector m_vOldAngles, float m_fOldForward, float m_fOldSidemove);

	bool IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd);

	float flGetDistance(Vector vOrigin, Vector vLocalOrigin);

	DWORD FindPattern(DWORD dwAddress, DWORD dwLength, const char* szPattern);

	DWORD FindSignature(const char* szModuleName, char* szPattern);

	Vector EstimateAbsVelocity(CBaseEntity *ent);

	bool IsKeyPressed(int i);

	bool IsKeyPressed2(int i);

	bool IsHeadshotWeapon(CBaseEntity* pLocal, CBaseCombatWeapon* pWep);

	float RandFloatRange(float min, float max);

	void NormaliseViewAngle(Vector &angle);

	PVOID InitKeyValue();
};

extern CUtil* Util;