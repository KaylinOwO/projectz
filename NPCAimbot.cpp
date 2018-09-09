#include "SDK.h"
#include "NPCAimbot.h"
#include "CDrawManager.h"
#include "COffsets.h"
#include "Client.h"
#include "Util.h"

/*This code is a slightly modified version of gir489's Black Mesa Darkstorm Aimbot*/

//====================================================================================
CNPCAimbot gNPCAimbot;
//====================================================================================

Vector calc_angle(Vector src, Vector dst)
{
	Vector AimAngles, delta;
	float hyp;
	delta = src - dst;
	hyp = sqrtf((delta.x * delta.x) + (delta.y * delta.y)); //SUPER SECRET IMPROVEMENT CODE NAME DONUT STEEL
	AimAngles.x = atanf(delta.z / hyp) * RADPI;
	AimAngles.y = atanf(delta.y / delta.x) * RADPI;
	AimAngles.z = 0.0f;
	if (delta.x >= 0.0)
		AimAngles.y += 180.0f;
	return AimAngles;
}
void MakeVector(Vector angle, Vector& vector)
{
	float pitch, yaw, tmp;
	pitch = float(angle[0] * PI / 180);
	yaw = float(angle[1] * PI / 180);
	tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw)*tmp);
	vector[2] = float(-sin(pitch));
}
float GetFOV(Vector angle, Vector src, Vector dst)
{
	Vector ang, aim;
	float mag, u_dot_v;
	ang = calc_angle(src, dst);


	MakeVector(angle, aim);
	MakeVector(ang, ang);

	mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
	u_dot_v = aim.Dot(ang);

	return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
}
void CNPCAimbot::SinCos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}
//====================================================================================
void CNPCAimbot::AngleVector(Vector& Angles, Vector *in)
{
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(Angles[1]), &sy, &cy);
	SinCos(DEG2RAD(Angles[0]), &sp, &cp);

	in->x = cp * cy;
	in->y = cp * sy;
	in->z = -sp;
}
//====================================================================================
void CNPCAimbot::VectorAngles(Vector &forward, Vector &angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt((forward[0] * forward[0] + forward[1] * forward[1]));
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
//===================================================================================
float CNPCAimbot::flGetDistance(Vector vOrigin)
{
	if (vOrigin.Length() == 0)
		return 0;

	CBaseEntity *pLocalEnt = gInts.EntList->GetClientEntity(me);

	if (pLocalEnt == NULL)
		return 1.0f;

	Vector vDistance = vOrigin - pLocalEnt->GetAbsOrigin();

	float flDistance = sqrt(vDistance.Length());

	if (flDistance < 1.0f)
		return 1.0f;

	return flDistance;
}
//===================================================================================
float CNPCAimbot::flGetFOV(Vector vOrigin)
{
	try
	{
		CBaseEntity* pBaseEnt = GetBaseEntity(me);
		Vector qAim = pBaseEnt->GetAbsAngles();
		Vector vecAim;
		AngleVector(qAim, &vecAim);

		Vector vecEyeDiff = vOrigin - pBaseEnt->GetEyePosition();
		vecEyeDiff.NormalizeInPlace();

		float flDotProduct = vecAim.Dot(vecEyeDiff);
		float flFoV = RAD2DEG(acos(flDotProduct));

		if (flFoV > gCvars.aimbot_fov)
		{
			return flFoV;
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed flGetFOV");
	}
}
//===================================================================================
CNPCAimbot::CNPCAimbot(void)
{
	DropTarget();
}
//===================================================================================
void CNPCAimbot::DropTarget(void)
{
	m_nTarget = -1;
	m_fDistanceToTarget = 4096.0f;
}
//===================================================================================
void CNPCAimbot::SetTarget(int iIndex)
{
	m_nTarget = iIndex;
}
//===================================================================================
bool CNPCAimbot::bHasTarget(void)
{
	return m_nTarget != -1;
}
//===================================================================================
bool	CNPCAimbot::bGetTeam(int iIndex)
{
	//Add team shit here later for when they add MP.
	return true;
}
//===================================================================================
bool	CNPCAimbot::bIsVisible(Vector& vecEnemy, CBaseEntity* pBaseEntity)
{
	try
	{
		CBaseEntity* pBaseEnt = GetBaseEntity(me);
		trace_t2 pTrace;
		Ray_t2 pRay;
		TraceFilter2 filter;

		pRay.Init(pBaseEnt->GetEyePosition(), vecEnemy);
		gInts.EngineTrace->TraceRay2(pRay, MASK_SHOT, &filter, &pTrace);

		if (pTrace.m_pEnt != NULL)
		{
			return (pTrace.m_pEnt == pBaseEntity);
		}

		return true;
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed bIsVisible");
	}
	return false;
}
//===================================================================================
mstudiobbox_t* pGetHitbox(int iHitbox, DWORD* pHeader)
{
	int HitboxSetIndex = *(int*)((DWORD)pHeader + 0xB0);
	mstudiohitboxset_t* pSet = (mstudiohitboxset_t*)(((PBYTE)pHeader) + HitboxSetIndex);

	return (mstudiobbox_t*)pSet->pHitbox(iHitbox);
}
//===================================================================================
void VectorTransform(Vector& in1, const matrix3x4 &in2, Vector &out)
{
	out[0] = (in1[0] * in2[0][0] + in1[1] * in2[0][1] + in1[2] * in2[0][2]) + in2[0][3];
	out[1] = (in1[0] * in2[1][0] + in1[1] * in2[1][1] + in1[2] * in2[1][2]) + in2[1][3];
	out[2] = (in1[0] * in2[2][0] + in1[1] * in2[2][1] + in1[2] * in2[2][2]) + in2[2][3];
}
//===================================================================================
Vector CNPCAimbot::vecGetHitbox(CBaseEntity* pBaseEntity, int iHitbox)
{
	Vector vHitbox;
	try
	{
		if (pBaseEntity == NULL || pBaseEntity->GetIndex() == 0)
			return vHitbox;

		DWORD *pStudioHdr = (PDWORD)gInts.ModelInfo->GetStudiomodel(pBaseEntity->GetModel());

		if (!pBaseEntity->SetupBones(pBoneToWorld, 128, 0x100, 0))
			return vHitbox;

		pbox = pGetHitbox(iHitbox, pStudioHdr);

		VectorTransform(pbox->bbmin, pBoneToWorld[pbox->bone], vMin);
		VectorTransform(pbox->bbmax, pBoneToWorld[pbox->bone], vMax);

		vHitbox = (vMin + vMax) / 2;
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed to get hitbox: %i for %s (%s)", iHitbox, pBaseEntity->GetClientClass()->chName, gInts.ModelInfo->GetModelName(pBaseEntity->GetModel()));
	}
	return vHitbox;
}
//===================================================================================
bool	CNPCAimbot::bIsTargetSpot(int iIndex, int iHitbox)
{
	try
	{
		CBaseEntity* pBaseEntity = GetBaseEntity(iIndex);

		if (pBaseEntity == NULL)
			return false;

		return bIsVisible(vecGetHitbox(pBaseEntity, iHitbox), pBaseEntity);
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed bIsTargetSpot");
	}
	return false;
}
//===================================================================================
int CNPCAimbot::IsValidEntity(int iIndex)
{
	try
	{
		if (iIndex == gInts.Engine->GetLocalPlayer())
			return -1;

		CBaseEntity* pBaseEntity = GetBaseEntity(iIndex);

		if (pBaseEntity == NULL)
			return -1;

		const char* chName = pBaseEntity->GetClientClass()->chName;
		if (!strcmp(chName, "CAI_BaseNPC"))
		{
			if (pBaseEntity->IsAlive())
			{
				const char* chModelName = gInts.ModelInfo->GetModelName(pBaseEntity->GetModel());
				if (strstr(chModelName, "scientist"))
				{
					return -1;
				}

				if (strstr(chModelName, "zombie"))
				{
					return 5;
				}

				if (strstr(chModelName, "guard"))
				{
					return -1;
				}

				if (strstr(chModelName, "combine"))
				{
					return 5;
				}

				if (strstr(chModelName, "police"))
				{
					return 5;
				}

				if (strstr(chModelName, "sentry"))
				{
					return 1;
				}

				if (strstr(chModelName, "bullsquid"))
				{
					return 2;
				}

				if (strstr(chModelName, "controller"))
				{
					return 0;
				}

				if (strstr(chModelName, "garg"))
				{
					return -1;
				}

				if (strstr(chModelName, "houndeye"))
				{
					return 1;
				}

				if (strstr(chModelName, "agrunt"))
				{
					return 0;
				}

				if (strstr(chModelName, "headcrab"))
				{
					return 0;
				}

				if (strstr(chModelName, "ichthyosaur"))
				{
					return 0;
				}

				if (strstr(chModelName, "snark"))
				{
					return 0;
				}

				if (strstr(chModelName, "apache"))
				{
					return 0;
				}
			}
		}

		return -1;
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed IsValidEntity");
	}
	return false;
}
//===================================================================================
bool CNPCAimbot::bIsValidTarget(int iIndex, CUserCmd* pCommand)
{
	try
	{
		int iReturn = IsValidEntity(iIndex);

		if (iReturn == -1)
			return false;

		if (bIsTargetSpot(iIndex, iReturn) == false)
			return false;

		Vector vecAimSpot = vecGetHitbox(GetBaseEntity(iIndex), iReturn);
		CBaseEntity *pLocalEnt = gInts.EntList->GetClientEntity(me);
		Vector vLocal = pLocalEnt->GetEyePosition();

		float flFOV = GetFOV(pCommand->viewangles, vLocal, vecAimSpot);
		float distance = Util->flGetDistance(vecAimSpot, pLocalEnt->GetEyePosition());
		double minimalDistance = 99999.0;

		if (distance < minimalDistance)
		{
			if (flFOV < m_fDistanceToTarget && flFOV < gCvars.aimbot_fov)
			{
				m_fDistanceToTarget = flFOV;
				AimAtVector(vecAimSpot);
				return true;
			}
		}
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed bIsValidTarget");
	}
	return false;
}
//===================================================================================
void CNPCAimbot::FindTarget(CUserCmd* pCommand)
{
	try
	{
		CBaseEntity *pBaseEnt = GetBaseEntity(me);

		DropTarget();

		if (pBaseEnt == NULL || pBaseEnt->IsAlive() == false)
			return;

		for (int iIndex = 1; iIndex <= gInts.EntList->GetHighestEntityIndex(); iIndex++)
		{
			if (bIsValidTarget(iIndex, pCommand))
			{
				m_nTarget = 1;
			}
		}
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed FindTarget");
	}
}
//===================================================================================
void	CNPCAimbot::AimAtVector(Vector vecAimSpot)
{
	try
	{
		CBaseEntity *pBaseEnt = GetBaseEntity(me);
		if (GetBaseEntity(me) == NULL)
			return;

		VectorAngles(vecAimSpot - pBaseEnt->GetEyePosition(), qAimAngles);

		ClampAngle(qAimAngles);
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed Aim At Target");
	}
}
//=================================================================================
bool CNPCAimbot::bTriggerbotTrace(CUserCmd* pCmd)
{
	try
	{
		CBaseEntity* pBaseEntity = GetBaseEntity(me);
		Vector vDirection;
		trace_t2 pTrace;
		Ray_t2 ray;
		TraceFilter2 filter;
		AngleVector(pCmd->viewangles, &vDirection);
		vDirection = vDirection * 8192 + pBaseEntity->GetEyePosition();
		Vector vLocalPosition = pBaseEntity->GetEyePosition();
		ray.Init(vLocalPosition, vDirection);
		gInts.EngineTrace->TraceRay2(ray, MASK_SHOT, &filter, &pTrace);
		if (pTrace.m_pEnt != NULL)
		{
			//CBaseEntity* pBaseWeapon = gOffsets.pGetBaseCombatActiveWeapon(GetBaseEntity(me));
			//if (pBaseWeapon != NULL && (pBaseWeapon->GetWeaponSlot() == WEAPON_TYPE_MELEE) && (flGetDistance(pTrace.end) > 9))
			//	return false;
			return (IsValidEntity(pTrace.m_pEnt->GetIndex()) != -1);
		}
		return false;
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed Triggerbot");
	}
	return false;
}
//===================================================================================
void CNPCAimbot::SilentAimFix(CUserCmd* pCmd, Vector& qaViewAngles)
{
	Vector vecSilent(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
	float flSpeed = sqrt(vecSilent.x*vecSilent.x + vecSilent.y*vecSilent.y);
	Vector angMove;
	VectorAngles(vecSilent, angMove);
	float flYaw = DEG2RAD(qaViewAngles.y - pCmd->viewangles.y + angMove.y);
	pCmd->forwardmove = cos(flYaw) * flSpeed;
	pCmd->sidemove = sin(flYaw) * flSpeed;
	pCmd->viewangles = qaViewAngles;
}
//===================================================================================
void CNPCAimbot::ClampAngle(Vector& qaAng)
{
	while (qaAng[0] > 89)
		qaAng[0] -= 180;

	while (qaAng[0] < -89)
		qaAng[0] += 180;

	while (qaAng[1] > 180)
		qaAng[1] -= 360;

	while (qaAng[1] < -180)
		qaAng[1] += 360;

	qaAng.z = 0;
}
//===================================================================================
void CNPCAimbot::FindTargetCall(int iIndex, CUserCmd* pCommand)
{
	try
	{
		if (bIsValidTarget(iIndex, pCommand))
		{
			if (m_nTarget == -1)
			{
				if (GetBaseEntity(iIndex) == NULL)
				{
					return;
				}

				m_nTarget = iIndex;
				return;
			}
		}
		return;
	}
	catch (...)
	{
		gBaseAPI.LogToFile("Failed FindTargetCall");
	}
}
//===================================================================================
Vector CNPCAimbot::GetAimbotAngles()
{
	return qAimAngles;
}