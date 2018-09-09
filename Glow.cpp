#include "Glow.h"
#include "Util.h"
#include "CDrawManager.h"
bool C_Glow::update;

C_Glow gGlow;

int C_Glow::registerGlowObject(CBaseEntity *ent, float r, float g, float b, float a, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot)
{
	if (!pGlow || !ent)
		return 0;

	typedef int(__thiscall *registerFn)(CGlowObjectManager *, CBaseEntity *, Vector &, float, bool, bool, int);
	static DWORD dwFn = gSignatures.GetClientSignature("55 8B EC 51 53 56 8B F1 57 8B 5E 14");
	static registerFn Register = (registerFn)dwFn;

	return Register(pGlow, ent, Vector{ r, g, b }, a, bRenderWhenOccluded, bRenderWhenUnoccluded, nSplitScreenSlot);
}

int AcquirePlayerCount_t()
{
	static int m_ipCountReturnFn = 0;

	for (int i = 0; i <= gInts.Engine->GetMaxClients(); i++)
	{
		CBaseEntity* cur = GetBaseEntity(1);

		if (!cur)
			continue;

		++m_ipCountReturnFn;
	}

	return m_ipCountReturnFn;
}

#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2

void C_Glow::fsn()
{
	static bool pRegsFn[33];
	static float pUpdateTimeFn = 0.0f;

	ConVar* glowvar = gInts.cvar->FindVar("glow_outline_effect_enable");

	if (gCvars.esp_glow && glowvar->GetInt() != 1)
		glowvar->SetValue(1);

	//static int oldcount = AcquirePlayerCount_t();

	//Remove and Re-register the glow objects every frame, because otherwise you gonna end up with doubled entries and the game will lag hard and likely crash. (test)dddd
	//Also, If you register once via bool, then not all entities are glowing. (g.g,, if new players join)

	pGlow->m_GlowObjectDefinitions.RemoveAll();

	for (int i = 0; i <= gInts.Engine->GetMaxClients(); i++)
	{
		CBaseEntity* cur = GetBaseEntity(i);
		CBaseEntity* pLocal = (CBaseEntity*)gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());

		if (pLocal->IsAlive() == false || gInts.Engine->IsInGame() == false)
			continue;

		if (!cur)
			continue;

		if (cur->GetLifeState() != LIFE_ALIVE)
			continue;

		if (cur->IsDormant() == true)
			continue;

		if (!gCvars.misc_thirdperson && cur->GetIndex() == me)
			continue;

		if (gCvars.esp_enemyonly && cur->GetTeamNum() == GetBaseEntity(me)->GetTeamNum())
			continue;

		if (!gCvars.esp_active)
			continue;

		if (!gCvars.esp_glow)
			continue;

		if (cur->GetTeamNum() == BLU_TEAM)
		{
			registerGlowObject(cur, 0.f, 153.f, 255.f, 255.f, true, true, 0);
		}
		else if (cur->GetTeamNum() == RED_TEAM)
		{
			registerGlowObject(cur, 255.f, 0.f, 0.f, 255.f, true, true, 0);
		}
	}
}

void C_Glow::init()
{
	DWORD dwGlowLoc = gSignatures.GetClientSignature("8B 0D ? ? ? ? A1 ? ? ? ? 56 8B 37") + 0x2;
	XASSERT(dwGlowLoc);
	pGlow = *(CGlowObjectManager**)dwGlowLoc;
	XASSERT(pGlow);
}