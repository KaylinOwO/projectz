#include "Backtrack.h"

BacktrackData headPositions[24][12];

CBacktracking gBacktracking;

//=======================================================================
inline float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

inline Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}
//=======================================================================

void CBacktracking::Run(CUserCmd* cmd)
{
	int iBestTarget = -1;
	float bestFov = 999999;
	float backtrack_tick = 0;
	CBaseEntity* pLocal = gInts.EntList->GetClientEntity(me);

	if (!pLocal)
		return;

	if (gCvars.misc_backtracking)

	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		CBaseEntity* pEntity = GetBaseEntity(i);
		CBaseCombatWeapon* pWep = pLocal->GetActiveWeapon();

		if (!pEntity || pEntity->IsDormant() || pEntity->GetLifeState() != LIFE_ALIVE) continue;
		if (gInts.Engine->GetAppId() == 320 && pEntity->GetTeamNum() == pLocal->GetTeamNum()) continue;
		if (pLocal->GetLifeState() != LIFE_ALIVE) continue;
		if (GAME_TF2) { if (pWep->GetSlot() == 2) continue; }

		Vector hitboxpos = pEntity->GetHitboxPosition(0);

		if (GAME_CSS || GAME_DODS)
			hitboxpos = pEntity->GetHitboxPosition(12);
		else if (GAME_TF2)
			hitboxpos = pEntity->GetHitboxPosition(0);
		else
			hitboxpos = pEntity->GetHitboxPosition(4);

		headPositions[i][cmd->command_number % 13] = BacktrackData{ cmd->tick_count, hitboxpos };

		Vector ViewDir = angle_vector(cmd->viewangles);
		float FOVDistance = distance_point_to_line(hitboxpos, pLocal->GetEyePosition(), ViewDir);

		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			iBestTarget = i;
		}

		if (iBestTarget != -1)
		{
			int bestTick = 0;
			float tempFOV = 999999;
			float bestFOV = 30;
			Vector lowestDistTicks(180, 180, 0);
			bestTick = 12;
			backtrack_tick = 12;

			if (cmd->buttons & IN_ATTACK)
				cmd->tick_count = headPositions[i][bestTick].tickcount;

			pEntity->SetupBones(headPositions[i][bestTick].matrix, 128, 256, gInts.globals->curtime);
		}

	}

}