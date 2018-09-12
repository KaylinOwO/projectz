#include "Aimbot.h"
#include "Util.h"
#include "Backtrack.h"
#include "SDK.h"
CAimbot gAim;
#define TICK_INTERVAL			(gInts.globals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define tick_interval gInts.globals->interval_per_tick
#define tick_intervalsqr tick_interval * tick_interval
bool CAimbot::CanAmbassadorHeadshot(CBaseEntity* pLocal)
{
	return gInts.globals->curtime - pLocal->GetActiveWeapon()->m_flLastFireTime() >= 0.930;//1.0;
}
void DoNoRecoil(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	if (pLocal)
	{
		Vector AimPunch = pLocal->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCommand->viewangles -= AimPunch * 2;
			Util->NormaliseViewAngle(pCommand->viewangles);
		}
	}
}
Vector CAimbot::calc_angle(Vector src, Vector dst)
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
void CAimbot::MakeVector(Vector angle, Vector& vector)
{
	float pitch, yaw, tmp;
	pitch = float(angle[0] * PI / 180);
	yaw = float(angle[1] * PI / 180);
	tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw)*tmp);
	vector[2] = float(-sin(pitch));
}
float CAimbot::GetFOV(Vector angle, Vector src, Vector dst)
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
void FixMove(CUserCmd* pCmd, Vector m_vOldAngles, float m_fOldForward, float m_fOldSidemove)
{
	float deltaView = pCmd->viewangles.y - m_vOldAngles.y;
	float f1;
	float f2;
	if (m_vOldAngles.y < 0.f)
		f1 = 360.0f + m_vOldAngles.y;
	else
		f1 = m_vOldAngles.y;
	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;
	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;
	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * m_fOldForward + cos(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * m_fOldForward + sin(DEG2RAD(deltaView + 90.f)) * m_fOldSidemove;
}
/*bool BulletTime(CBaseEntity* pLocal)
{
if (!pLocal) return false;
 auto tick_base = pLocal->iTickBase();
if (!tick_base) return false;
 auto local_weapon = pLocal->GetActiveWeapon();
if (local_weapon == nullptr) return false;
 auto next_attack = local_weapon->get_next_attack();
auto can_tick_base = next_attack <= TICKS_TO_TIME(tick_base);
 return can_tick_base;
}*/
bool IsVisiblePoint(CBaseEntity* pLocal, Vector Point)
{
	trace_t tr; Ray_t ray; CTraceFilter filter;
	ray.Init(pLocal->GetEyePosition(), Point);
	gInts.EngineTrace->TraceRay(ray, MASK_SOLID, &filter, &tr);
	if (tr.fraction > 0.9f) return true;
	return false;
}
void CAimbot::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	Vector m_vOldViewAngle = pCommand->viewangles;
	float m_fOldSideMove = pCommand->sidemove;
	float m_fOldForwardMove = pCommand->forwardmove;
	if (pLocal->IsAlive() == false)
		return;
	gCvars.iAimbotIndex = -1;
	//if (gCvars.aimbot_mode == 2)
	//BulletTime(pLocal);
	if (!gCvars.aimbot_active)
		return;
	if (gCvars.aimbot_key_enabled && !Util->IsKeyPressed(gCvars.aimbot_key))
		return;
	if (!pLocal->GetActiveWeapon())
		return;
	if (!gCvars.aimbot_projectile && GAME_TF2)
	{
		auto id = pLocal->GetActiveWeapon()->GetItemDefinitionIndex(); //This ignores all projectile weapons, doesn't work for strange/killstreak/etc
		if (
#pragma region soldier
			id == (int)soldierweapons::WPN_RocketLauncher
			|| id == (int)soldierweapons::WPN_NewRocketLauncher
			|| id == (int)soldierweapons::WPN_DirectHit
			|| id == (int)soldierweapons::WPN_BlackBox
			|| id == (int)soldierweapons::WPN_RocketJumper
			|| id == (int)soldierweapons::WPN_LibertyLauncher
			|| id == (int)soldierweapons::WPN_CowMangler
			|| id == (int)soldierweapons::WPN_Original
			|| id == (int)soldierweapons::WPN_FestiveRocketLauncher
			|| id == (int)soldierweapons::WPN_BeggersBazooka
			|| id == (int)soldierweapons::WPN_BotRocketlauncherS
			|| id == (int)soldierweapons::WPN_BotRocketlauncherG
			|| id == (int)soldierweapons::WPN_BotRocketlauncherR
			|| id == (int)soldierweapons::WPN_BotRocketlauncherB
			|| id == (int)soldierweapons::WPN_BotRocketlauncherC
			|| id == (int)soldierweapons::WPN_BotRocketlauncherD
			|| id == (int)soldierweapons::WPN_BotRocketlauncherES
			|| id == (int)soldierweapons::WPN_BotRocketlauncherEG
			|| id == (int)soldierweapons::WPN_FestiveBlackbox
			|| id == (int)soldierweapons::WPN_Airstrike
#pragma endregion
#pragma region scout
			|| id == (int)scoutweapons::WPN_Sandman
			|| id == (int)scoutweapons::WPN_CritCola
			|| id == (int)scoutweapons::WPN_CritCola
			|| id == (int)scoutweapons::WPN_FlyingGuillotine1
			|| id == (int)scoutweapons::WPN_FlyingGuillotine2
			|| id == (int)scoutweapons::WPN_Milk
			|| id == (int)scoutweapons::WPN_MutatedMilk
			|| id == (int)scoutweapons::WPN_WrapAssassin
#pragma endregion
#pragma region pyro
			|| id == (int)pyroweapons::WPN_Detonator
			|| id == (int)pyroweapons::WPN_FestiveFlaregun
			|| id == (int)pyroweapons::WPN_Flaregun
			|| id == (int)pyroweapons::WPN_ScorchShot
#pragma endregion
#pragma region demo
			|| id == (int)demomanweapons::WPN_FestiveGrenadeLauncher
			|| id == (int)demomanweapons::WPN_FestiveStickyLauncher
			|| id == (int)demomanweapons::WPN_GrenadeLauncher
			|| id == (int)demomanweapons::WPN_IronBomber
			|| id == (int)demomanweapons::WPN_LochNLoad
			|| id == (int)demomanweapons::WPN_LoooseCannon
			|| id == (int)demomanweapons::WPN_QuickieBombLauncher
			|| id == (int)demomanweapons::WPN_StickyLauncher
			|| id == (int)demomanweapons::WPN_ScottsSkullctter
#pragma endregion
#pragma region heavy
			|| id == (int)heavyweapons::WPN_FestiveSandvich
			|| id == (int)heavyweapons::WPN_RobotSandvich
			|| id == (int)heavyweapons::WPN_Sandvich
			|| id == (int)heavyweapons::WPN_Steak
			|| id == (int)heavyweapons::WPN_CandyBar
			|| id == (int)heavyweapons::WPN_Fishcake
#pragma endregion
#pragma region engineer
			|| id == (int)engineerweapons::WPN_Builder
			|| id == (int)engineerweapons::WPN_Builder2
			|| id == (int)engineerweapons::WPN_Destructor
			|| id == (int)engineerweapons::WPN_FestiveWrangler
			|| id == (int)engineerweapons::WPN_Pomson
			|| id == (int)engineerweapons::WPN_RescueRanger
			|| id == (int)engineerweapons::WPN_ShortCircut
			|| id == (int)engineerweapons::WPN_Toolbox
			|| id == (int)engineerweapons::WPN_Wrangler
#pragma endregion
#pragma region medic
			|| id == (int)medicweapons::WPN_Blutsauger
			|| id == (int)medicweapons::WPN_FestiveCrossbow
			|| id == (int)medicweapons::WPN_FestiveMedigun
			|| id == (int)medicweapons::WPN_Kritzkrieg
			|| id == (int)medicweapons::WPN_Medigun
			|| id == (int)medicweapons::WPN_Overdose
			|| id == (int)medicweapons::WPN_QuickFix
			|| id == (int)medicweapons::WPN_SyringeGun
			|| id == (int)medicweapons::WPN_Vaccinator
#pragma endregion
#pragma region sniper
			|| id == (int)sniperweapons::WPN_CompoundBow
			|| id == (int)sniperweapons::WPN_FestiveJarate
			|| id == (int)sniperweapons::WPN_FestiveHuntsman
			|| id == (int)sniperweapons::WPN_Huntsman
			|| id == (int)sniperweapons::WPN_Jarate
#pragma endregion
#pragma region spy
			|| id == (int)spyweapons::WPN_ApSap
			|| id == (int)spyweapons::WPN_DisguiseKit
			|| id == (int)spyweapons::WPN_RedTape1
			|| id == (int)spyweapons::WPN_RedTape2
			|| id == (int)spyweapons::WPN_Sapper
			|| id == (int)spyweapons::WPN_Sapper2
			|| id == (int)spyweapons::WPN_FestiveSapper
#pragma endregion
			)
			return;
	}
	CBaseEntity* pEntity = GetBaseEntity(GetBestTarget(pLocal, pCommand));
	if (!pEntity)
		return;
	int iBestHitbox = GetBestHitbox(pLocal, pEntity);
	if (iBestHitbox == -1)
		return;
	if (GAME_TF2)
	{
		auto pWep = pLocal->GetActiveWeapon();
		if (gCvars.aimbot_waitforcharge) //Advanced Ambassador Wait For Charge!
			if (pWep->GetItemDefinitionIndex() == spyweapons::WPN_Ambassador || pWep->GetItemDefinitionIndex() == spyweapons::WPN_FestiveAmbassador)
				if (!CanAmbassadorHeadshot(pLocal)) return;
	}
	Vector vEntity;
	if (gCvars.aimbot_aimatbacktrack && gCvars.misc_backtracking) //Aims at backtrack ticks 8 and below, not really useful, will just improve accuracy slightly so not worth including in patch notes.
		for (int i = 0; i < 8; i++)
			vEntity = headPositions[pEntity->GetIndex()][i].hitboxpos;
	else
		vEntity = pEntity->GetHitboxPosition(iBestHitbox);
	Vector vLocal = pLocal->GetEyePosition();
	Vector vAngs;
	if (gCvars.aimbot_projectile && GAME_TF2)
	{
		CBaseCombatWeapon* pWeapon = pLocal->GetActiveWeapon();
		if (!pWeapon) return;
		int item_index = pWeapon->GetItemDefinitionIndex();
		int weapon_slot;
		weapon_slot = pWeapon->GetSlot();
		auto get_speed = [&pLocal, &pWeapon, &pEntity, &item_index]() -> float
		{
			auto weapon_speed = 0.0f;
			switch (item_index)
			{
			case WPN_DirectHit:
				weapon_speed = 1980.0f; break;
			case WPN_BotRocketlauncherB:
			case WPN_BotRocketlauncherC:
			case WPN_BotRocketlauncherD:
			case WPN_BotRocketlauncherEG:
			case WPN_BotRocketlauncherES:
			case WPN_BotRocketlauncherG:
			case WPN_BotRocketlauncherR:
			case WPN_BotRocketlauncherS:
			case WPN_FestiveRocketLauncher:
			case WPN_NewRocketLauncher:
			case WPN_RocketLauncher:
			case WPN_Original:
			case WPN_Airstrike:
				weapon_speed = 1100.0f; break;
			case WPN_FestiveFlaregun:
			case WPN_Flaregun:
				weapon_speed = 2000.0f; break;
			case WPN_SyringeGun:
			case WPN_NewSyringeGun:
			case WPN_Blutsauger:
			case WPN_Overdose:
				weapon_speed = 1000.0f; break;
			case WPN_RescueRanger:
			case WPN_Crossbow:
			case WPN_FestiveCrossbow:
				weapon_speed = 2400.0f; break;
			case WPN_GrenadeLauncher:
			case WPN_NewGrenadeLauncher:
			case WPN_FestiveGrenadeLauncher:
				weapon_speed = 1216.6f; break;
			case WPN_LochNLoad:
				weapon_speed = 1513.3f; break;
			case WPN_LoooseCannon:
				weapon_speed = 1453.9f; break;
			case WPN_IronBomber:
				weapon_speed = 1216.6f; break;
			case WPN_CompoundBow:
			case WPN_Huntsman:
			case WPN_FestiveHuntsman:
			{
				float chargetime =
					gInts.globals->curtime * tick_interval -
					pWeapon->GetChargeTime() *
					tick_interval;
				weapon_speed = (fminf(fmaxf(chargetime, 0.0f), 1.0f) * 800.0f) + 1800.0f;
			} break;
			default: weapon_speed = 0.0f; break;
			}
			return weapon_speed;
		};
		auto get_gravity = [&pLocal, &pWeapon, &pEntity, &item_index]() -> float
		{
			auto weapon_gravity = 0.0f;
			switch (item_index)
			{
			case WPN_RescueRanger:
			case WPN_Crossbow:
			case WPN_FestiveCrossbow:
				weapon_gravity = 0.2f; break;
			case WPN_GrenadeLauncher:
			case WPN_NewGrenadeLauncher:
			case WPN_FestiveGrenadeLauncher:
			case WPN_LoooseCannon:
			case WPN_LochNLoad:
			case WPN_IronBomber:
				weapon_gravity = 0.4f; break;
			case WPN_CompoundBow:
			case WPN_Huntsman:
			case WPN_FestiveHuntsman:
			{
				float chargetime =
					gInts.globals->curtime * tick_interval -
					pWeapon->GetChargeTime() *
					tick_interval;
				weapon_gravity = (fminf(fmaxf(chargetime, 0.0f), 1.0f) * -0.40000001f) + 0.5f;
			} break;
			default: weapon_gravity = 0.0f; break;
			}
			return weapon_gravity;
		};
		auto is_projectile_weapon = [&pLocal, &pWeapon, &pEntity, &item_index]() -> bool
		{
			auto local_class = pLocal->GetClassNum(), weapon_slot = pWeapon->GetSlot();
			if (local_class == TF2_Soldier ||
				local_class == TF2_Medic ||
				local_class == TF2_Demoman ||
				local_class == TF2_Engineer && item_index == WPN_RescueRanger ||
				local_class == TF2_Sniper && item_index == WPN_FestiveHuntsman ||
				local_class == TF2_Sniper && item_index == WPN_Huntsman ||
				local_class == TF2_Sniper && item_index == WPN_CompoundBow ||
				local_class == TF2_Pyro && item_index == WPN_Flaregun ||
				local_class == TF2_Pyro && item_index == WPN_FestiveFlaregun)
				if (weapon_slot == 0)
					return true;
			return false;
		};
		auto distance_to_ground = [&pEntity]() -> float
		{
			if (pEntity->GetFlags() & FL_ONGROUND)
				return 0.0f;
			auto distance_to_ground = [&pEntity](Vector origin) -> float
			{
				trace_t ground_trace; Ray_t ray;
				CTraceFilter filter; filter.pSkip = pEntity;
				Vector endpos = origin;
				endpos.z -= 8192;
				ray.Init(origin, endpos);
				gInts.EngineTrace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &ground_trace);
				return 8192.0f * ground_trace.fraction;
			};
			Vector origin = pEntity->GetAbsOrigin();
			float v1 = distance_to_ground(origin + Vector(10.0f, 10.0f, 0.0f));
			float v2 = distance_to_ground(origin + Vector(-10.0f, 10.0f, 0.0f));
			float v3 = distance_to_ground(origin + Vector(10.0f, -10.0f, 0.0f));
			float v4 = distance_to_ground(origin + Vector(-10.0f, -10.0f, 0.0f));
			return min(v1, min(v2, min(v3, v4)));
		};
		if (is_projectile_weapon())
		{
			bool isonGround = pEntity->GetFlags() & FL_ONGROUND;
			if (pLocal->GetClassNum() == TF2_Medic || pLocal->GetClassNum() == TF2_Engineer)
				vEntity = pEntity->GetHitboxPosition(4);
			else if (pLocal->GetClassNum() == TF2_Demoman || pLocal->GetClassNum() == TF2_Soldier) {
				vEntity = pEntity->GetAbsOrigin();
				vEntity[2] += 10.0f;
			}
			auto hitbox_pred = [&pLocal, &pEntity, &isonGround](Vector hitbox, float speed, float gravity, float distance_to_ground) -> Vector
			{
				auto best_hitbox = hitbox;
				auto predicted_time = ((pLocal->GetEyePosition().DistTo(hitbox) / speed) + tick_interval);
				auto server_gravity = gInts.cvar->FindVar("sv_gravity")->GetFloat();
				auto vec_velocity = Util->EstimateAbsVelocity(pEntity);
				vec_velocity[2] += (-server_gravity) * tick_intervalsqr + (gravity * tick_intervalsqr);
				best_hitbox[0] += (vec_velocity[0] * predicted_time) + tick_interval;
				best_hitbox[1] += (vec_velocity[1] * predicted_time) + tick_interval;
				best_hitbox[2] += (isonGround ? (vec_velocity[2] * predicted_time) + tick_interval :
					(0.5 * (-server_gravity + gravity)* pow(predicted_time, 2) + vec_velocity[2] * predicted_time)) + tick_interval;
				if (distance_to_ground > 0.0f)
					if (best_hitbox[2] < hitbox[2] - distance_to_ground)
						best_hitbox[2] = hitbox[2] - distance_to_ground;
				return best_hitbox;
			};
			vEntity = hitbox_pred(vEntity, get_speed(), get_gravity(), distance_to_ground());
		}
	}
	VectorAngles((vEntity - vLocal), vAngs);
	//ClampAngle(vAngs);
	//DoNoRecoil(pLocal, pCommand); //Large crashy bois
	gCvars.iAimbotIndex = pEntity->GetIndex();
	if (gCvars.aimbot_autoshoot)
		pCommand->buttons |= IN_ATTACK;
	if (gCvars.aimbot_smooth && gCvars.aimbot_mode == 1)
	{
		Vector vDelta(pCommand->viewangles - vAngs);
		AngleNormalize(vDelta);
		vAngs = pCommand->viewangles - vDelta / gCvars.aimbot_smooth_amt;
	}
	if (GAME_TF2)
	{
		auto pWep = pLocal->GetActiveWeapon();
		auto pItem = pWep->GetClientClass()->iClassID;
		auto isNotBow = (pItem != 199);
		if (isNotBow)
		{
			Vector m_vOldViewAngle = pCommand->viewangles;
			float m_fOldSideMove = pCommand->sidemove;
			float m_fOldForwardMove = pCommand->forwardmove;
			if (gCvars.aimbot_mode == 2)
			{
				ClampAngle(vAngs);
				Util->FixMove(pCommand, m_vOldViewAngle, m_fOldForwardMove, m_fOldSideMove);
				pCommand->viewangles = vAngs;
			}
			else if (gCvars.aimbot_mode == 1)
			{
				pCommand->viewangles = vAngs;
				gInts.Engine->SetViewAngles(pCommand->viewangles);
			}
		}
		else
		{
			pCommand->viewangles = vAngs;
			gInts.Engine->SetViewAngles(pCommand->viewangles);
		}
	}
	else
	{
		Vector m_vOldViewAngle = pCommand->viewangles;
		float m_fOldSideMove = pCommand->sidemove;
		float m_fOldForwardMove = pCommand->forwardmove;
		if (gCvars.aimbot_mode == 2)
		{
			ClampAngle(vAngs);
			Util->FixMove(pCommand, m_vOldViewAngle, m_fOldForwardMove, m_fOldSideMove);
			pCommand->viewangles = vAngs;
		}
		else if (gCvars.aimbot_mode == 1)
		{
			pCommand->viewangles = vAngs;
			gInts.Engine->SetViewAngles(pCommand->viewangles);
		}
	}
	Util->FixMove(pCommand, m_vOldViewAngle, m_fOldForwardMove, m_fOldSideMove);
}
int CAimbot::GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	int iBestTarget = -1;
	//this num could be smaller 
	float flDistToBest = 99999.f;
	double minimalDistance = 99999.0;
	Vector vLocal = pLocal->GetEyePosition();
	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;
		CBaseEntity* pEntity = GetBaseEntity(i);
		if (!pEntity)
			continue;
		if (pEntity->IsDormant())
			continue;
		auto pWeap = pLocal->GetActiveWeapon(); if (!pWeap) continue;
		if (GAME_TF2)
		{
			auto isMedic = (pLocal->GetClassNum() == TF2_Medic && pWeap->GetSlot() == 1);
			if (pEntity->GetLifeState() != LIFE_ALIVE || !isMedic && !gCvars.aimbot_deathmatch && pEntity->GetTeamNum() == pLocal->GetTeamNum())
				continue;
			else if (pEntity->GetLifeState() != LIFE_ALIVE || isMedic && pEntity->GetTeamNum() != pLocal->GetTeamNum())
				continue;
		}
		else
		{
			if (pEntity->GetLifeState() != LIFE_ALIVE || !gCvars.aimbot_deathmatch && pEntity->GetTeamNum() == pLocal->GetTeamNum())
				continue;
		}
		if (GAME_TF2)
		{
			if (pEntity->GetCond() & TFCond_Ubercharged ||
				pEntity->GetCond() & TFCond_UberchargeFading ||
				pEntity->GetCond() & TFCond_Bonked)
				continue;
		}
		if (GAME_TF2)
		{
			if (gCvars.aimbot_ignorecloaked && pEntity->GetCond() & TFCond_Cloaked)
				continue;
		}
		if (GAME_TF2)
		{
			auto isMedic = (pLocal->GetClassNum() == TF2_Medic && pWeap->GetSlot() == 1);
			if (isMedic)
				minimalDistance = 21.8f;
		}
		if (GAME_TF2)
		{
			auto pWep = pLocal->GetActiveWeapon();
			auto pClass = pWep->GetItemDefinitionIndex();
			auto urmomgay = pClass == demomanweapons::WPN_Sword || pClass == demomanweapons::WPN_FestiveEyelander || pClass == demomanweapons::WPN_Golfclub || pClass == demomanweapons::WPN_ScottsSkullctter || pClass == demomanweapons::WPN_Headless;
			if (pWep->GetSlot() == 2 && !urmomgay)
				minimalDistance = 8.4;
			if (pClass == demomanweapons::WPN_Sword || pClass == demomanweapons::WPN_FestiveEyelander || pClass == demomanweapons::WPN_Golfclub || pClass == demomanweapons::WPN_ScottsSkullctter || pClass == demomanweapons::WPN_Headless)
				minimalDistance = 12.0;
			if (pLocal->szGetClass() == "Pyro" && (pClass == pyroweapons::WPN_Backburner || pClass == pyroweapons::WPN_Degreaser || pClass == pyroweapons::WPN_FestiveBackburner || pClass == pyroweapons::WPN_FestiveFlamethrower || pClass == pyroweapons::WPN_Flamethrower || pClass == pyroweapons::WPN_Phlogistinator || pClass == pyroweapons::WPN_Rainblower))
				minimalDistance = 17.0;
		}

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);
		if (iBestHitbox == -1)
			continue;
		Vector vEntity;
		if (gCvars.aimbot_aimatbacktrack && gCvars.misc_backtracking)
			for (int i = 0; i < 8; i++)
				vEntity = headPositions[pEntity->GetIndex()][i].hitboxpos;
		else
			vEntity = pEntity->GetHitboxPosition(iBestHitbox);
		if (!gCvars.PlayerMode[i])
			continue;
		if (GAME_TF2)
		{
			auto pWep = pLocal->GetActiveWeapon();
			auto pItem = pWep->GetClientClass()->iClassID;
			auto isSniperRifle = (pItem != 199);
			if (pLocal->szGetClass() == "Sniper" && pLocal->GetActiveWeapon()->GetSlot() == 0 && isSniperRifle)
			{
				float damage = pLocal->GetActiveWeapon()->GetChargeDamage();
				if (gCvars.aimbot_zoomedonly)// || gCvars.aimbot.waitforcharge)
				{
					if (!(pLocal->GetCond() & tf_cond::TFCond_Zoomed))
						return -1;
					if (damage < 10.f)
						return -1;
				}
				if (gCvars.aimbot_waitforcharge) //This isn't the best code but it works for raging.
				{
					if (gCvars.aimbot_waitforcharge && ZOOM_BASE_DAMAGE + damage < pEntity->GetHealth())//<
						return -1;
				}
			}
		}
		float flFOV = GetFOV(pCommand->viewangles, vLocal, vEntity);
		float distance = Util->flGetDistance(vEntity, pLocal->GetEyePosition());
		if (distance < minimalDistance)
		{
			if (flFOV < flDistToBest && flFOV < gCvars.aimbot_fov)
			{
				if (gCvars.PlayerMode[i] == 2)
					return i;
				flDistToBest = flFOV;
				gCvars.iAimbotIndex = i;
				iBestTarget = i;
			}
		}
		if (gCvars.PlayerMode[i] == 2) //always aim at rage targets first
			return i;
	}
	return iBestTarget;
}

int CAimbot::GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	int iBestHitbox = -1;
	iBestHitbox = 4;
	if (GAME_CSS || GAME_DODS)
	{
		if (gCvars.aimbot_hitscan && !gCvars.aimbot_aimatbacktrack)
		{
			for (int i = 12; i < 17; i++)
				if (Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(i)))
					return i;
		}
		else
			iBestHitbox = 12;
	}
	else if (GAME_TF2)
	{
		if (Util->IsHeadshotWeapon(pLocal, pLocal->GetActiveWeapon()))
			iBestHitbox = 0;
		else
			iBestHitbox = 4;
		if (gCvars.aimbot_hitscan && !gCvars.aimbot_aimatbacktrack)
			for (int i = iBestHitbox; i < 17; i++)
				if (Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(i)))
					return i;
	}
	else
	{
		if (gCvars.aimbot_hitscan && !gCvars.aimbot_aimatbacktrack)
		{
			for (int i = 4; i < 17; i++)
				if (Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(i)))
					return i;
		}
		else
			iBestHitbox = 4;
	}
	if (pEntity->GetHitboxPosition(iBestHitbox).IsZero())
		return -1;
	if (!Util->IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(iBestHitbox)))
		return -1;
	return iBestHitbox;
}