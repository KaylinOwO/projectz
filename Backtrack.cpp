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

void CBacktracking::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	int iBestTarget = -1;
	float bestFov = 99999;

	if (!pLocal || !gCvars.misc_backtracking) return;
	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		CBaseEntity* pEntity = GetBaseEntity(i);

		if (i = me) continue;
		if (!pEntity || pEntity->IsDormant() || pEntity->GetLifeState() != LIFE_ALIVE) continue;
		if (gCvars.misc_backtracking_deathmatch && pEntity->GetTeamNum() != pLocal->GetTeamNum()) continue;
		if (pLocal->GetLifeState() != LIFE_ALIVE) continue;

		Vector hitboxpos = pEntity->GetHitboxPosition(0);
		if (GAME_CSS || GAME_DODS)
			hitboxpos = pEntity->GetHitboxPosition(12);
		else if (GAME_TF2)
			hitboxpos = pEntity->GetHitboxPosition(0);
		else if (gInts.Engine->GetAppId() != 440)
			hitboxpos = pEntity->GetHitboxPosition(4);

		headPositions[i][pCommand->command_number % 13] = BacktrackData{ pCommand->tick_count, hitboxpos };

		Vector ViewDir = angle_vector(pCommand->viewangles);
		float FOVDistance = distance_point_to_line(hitboxpos, pLocal->GetEyePosition(), ViewDir);

		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			iBestTarget = i;
		}

		if (iBestTarget != -1)
		{
			int bestTick = 0;
			float tempFOV = 9999;
			float bestFOV = 30;
			for (int t = 0; t < 13; t++)
			{
				Vector ViewDir = angle_vector(pCommand->viewangles);
				float tempFOV = distance_point_to_line(headPositions[iBestTarget][t].hitboxpos, pLocal->GetEyePosition(), ViewDir);
				if (bestFOV > tempFOV)
				{
					bestTick = t;
					bestFOV = tempFOV;
				}
			}
			if (pCommand->buttons & IN_ATTACK)
				pCommand->tick_count = headPositions[i][bestTick].tickcount;
		}
	}
}