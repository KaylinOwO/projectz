#pragma once

#include "SDK.h"

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(DWORD* dwRefEHandle) = 0;
	virtual DWORD GetRefEHandle(void) = 0;
	CBaseEntity* GetEntity(void)
	{
		return gInts.EntList->GetClientEntity(((*(PINT)(GetRefEHandle())) & ((1 << (11 + 1)) - 1)));
	}
};



class TraceFilter2 : public ITraceFilter2
{
public:
	virtual bool ShouldHitEntity(IHandleEntity *pEntityHandle, int contentsMask)
	{
		CBaseEntity *pBaseEntity = pEntityHandle->GetEntity();
		if (pBaseEntity->GetIndex() == gInts.Engine->GetLocalPlayer())
		{
			return false;
		}
		if (!strcmp(pBaseEntity->GetClientClass()->chName, "CBreakableSurface"))
		{
			return false;
		}
		return true;
	}
	virtual int	GetTraceType() const
	{
		return 0;
	}
};

class CNPCAimbot
{
public:
	CNPCAimbot();

	void	DropTarget(void);
	void	FindTarget(CUserCmd* pCommand);
	void	SetTarget(int iIndex);
	void	SilentAimFix(CUserCmd* pCmd, Vector& qaViewAngles);
	void	AimAtVector(Vector vecAimSpot);
	void	FindTargetCall(int iIndex, CUserCmd* pCommand);
	void	ClampAngle(Vector& qaAng);

	float	flGetFOV(Vector vOrigin);
	float	flGetDistance(Vector vOrigin);

	Vector	vecGetHitbox(CBaseEntity* pBaseEnt, int iHitbox);
	Vector	GetAimbotAngles();

	bool	bTriggerbotTrace(CUserCmd* pCmd);
	bool	bGetTeam(int iIndex);
	bool	bIsTargetSpot(int iIndex, int iHitbox);
	bool	bHasTarget(void);
	bool	bIsValidTarget(int iIndex, CUserCmd* pCommand);
	int		IsValidEntity(int iIndex);
	bool	bIsVisible(Vector& vecEnemy, CBaseEntity* pBaseEntity);

	int		iGetTarget() { return m_nTarget; }


private:

	int		m_nTarget;

	float	m_fDistanceToTarget;

	Vector	vTargetOrg;
	Vector	vMin, vMax;
	Vector	qAimAngles;
	Vector  qCurrentView;

	// Matrix
	matrix3x4 pBoneToWorld[128];

	// pointer to the hitbox
	mstudiobbox_t* pbox;
	void SinCos(float radians, float *sine, float *cosine);
	void AngleVector(Vector& Angles, Vector *in);
	void VectorAngles(Vector &forward, Vector &angles);
};

extern CNPCAimbot gNPCAimbot;