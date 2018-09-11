#include "ESP.h"
#include "CDrawManager.h"
#include "Backtrack.h"
#include "Client.h"

CESP gESP;

void CESP::Run(CBaseEntity* pLocal)
{
	if (!gCvars.esp_active)
		return;

	CBaseEntity* pBaseLocalEnt = GetBaseEntity(me);


	//Object ESP
	if (GAME_TF2)
	{
		for (int i = 1; i <= gInts.EntList->GetHighestEntityIndex(); i++)
		{
			CBaseEntity *pEnt = gInts.EntList->GetClientEntity(i);

			if (pEnt == NULL)
				continue;

			if (pEnt->IsDormant())
				continue;

			auto getcolor = [&pEnt]() -> Color
			{
				return Color(gCvars.color_r_other, gCvars.color_g_other, gCvars.color_b_other, 255);
			};

			Color clrTeam = getcolor();

			if (pLocal->GetIndex() == pEnt->GetIndex())
				continue;

			if (pEnt->GetTeamNum() == pLocal->GetTeamNum())
				continue;

			if (i == me)
				continue;

			CBaseEntity* pEntity = (CBaseEntity*)pEnt;

			int id = pEnt->GetClientClass()->iClassID;

			char* name = pEnt->GetClientClass()->chName;

			Vector center;

			pEnt->GetWorldSpaceCenter(center);

			Vector a;

			if (gDrawManager.WorldToScreen(center, a))
			{
				if (gCvars.esp_objects)
				{
					if (id == 88)
					{
						CObjectSentryGun* pSentryGun = reinterpret_cast<CObjectSentryGun*>(pEnt);

						if (pSentryGun == NULL)
							return;

						if (!pSentryGun->GetLevel())
							return;

						gDrawManager.DrawString(a.x, a.y, clrTeam, "Sentry: %i", pSentryGun->GetLevel());
						a.y += 11;

						if (pSentryGun->IsSapped())
						{
							gDrawManager.DrawString(a.x, a.y, clrTeam, "**SAPPED**");
							a.y += 11;
						}
						gDrawManager.DrawString(a.x, a.y, Color::Green(), "HP: %i", pSentryGun->GetHealth());
						a.y += 11;
					}

					if (id == 86)
					{
						CObjectDispenser* pDispenser = reinterpret_cast<CObjectDispenser*>(pEnt);

						if (pDispenser == NULL)
							return;

						if (!pDispenser->GetLevel())
							return;

						gDrawManager.DrawString(a.x, a.y, clrTeam, "Dispenser: %i", pDispenser->GetLevel());
						a.y += 11;

						if (pDispenser->IsSapped())
						{
							gDrawManager.DrawString(a.x, a.y, clrTeam, "**SAPPED**");
							a.y += 11;
						}
						gDrawManager.DrawString(a.x, a.y, Color::Green(), "HP: %i", pDispenser->GetHealth());
						a.y += 11;
					}

					if (id == 89)
					{
						CObjectTeleporter* pTeleporter = reinterpret_cast<CObjectTeleporter*>(pEnt);

						if (pTeleporter == NULL)
							return;

						if (!pTeleporter->GetLevel())
							return;

						gDrawManager.DrawString(a.x, a.y, clrTeam, "Teleporter: %i", pTeleporter->GetLevel());
						a.y += 11;

						if (pTeleporter->IsSapped())
						{
							gDrawManager.DrawString(a.x, a.y, clrTeam, "**SAPPED**");
							a.y += 11;
						}
						gDrawManager.DrawString(a.x, a.y, Color::Green(), "HP: %i", pTeleporter->GetHealth());
						a.y += 11;
					}
				}
			}
		}

	}

	//NPC ESP
	for (int iIndex = 0; iIndex <= gInts.EntList->GetHighestEntityIndex(); iIndex++)
	{
		Vector vScreen, vWorldPos;

		if (pBaseLocalEnt == NULL)
			return;

		CBaseEntity* pBaseEntity = GetBaseEntity(iIndex);

		if (pBaseEntity == NULL)
			continue;

		const char* chName = pBaseEntity->GetClientClass()->chName;

		if (gCvars.esp_active && gCvars.esp_name)
		{


			if (!strcmp(chName, "CAI_BaseNPC"))
			{
				if (pBaseEntity->IsAlive())
				{
					const char* chModelName = gInts.ModelInfo->GetModelName(pBaseEntity->GetModel());

					if (strstr(chModelName, "citizen"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(2);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Citizen");
						continue;
					}

					if (strstr(chModelName, "police"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(2);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Police");
						continue;
					}

					if (strstr(chModelName, "combine_soldier"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(2);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Soldier");
						continue;
					}

					if (strstr(chModelName, "combine_soldier_prisonguard"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(2);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Prison Guard");
						continue;
					}

					if (strstr(chModelName, "combine_super_soldier"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(2);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Super Soldier");
						continue;
					}

					if (strstr(chModelName, "sentry"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(1);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Sentry");
						continue;
					}

					if (strstr(chModelName, "bullsquid"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(2);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Bullsquid");
						continue;
					}

					if (strstr(chModelName, "controller"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(0);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Controller");
						continue;
					}

					if (strstr(chModelName, "garg"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(2);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Garg");
						continue;
					}

					if (strstr(chModelName, "houndeye"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(1);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Houndeye");
						continue;
					}

					if (strstr(chModelName, "agrunt"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(3);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "AGrunt");
						continue;
					}

					if (strstr(chModelName, "ichthyosaur"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(1);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Ichthyosaur");
						continue;
					}

					if (strstr(chModelName, "snark"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(0);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Snark");
						continue;
					}

					if (strstr(chModelName, "apache"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(0);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Apache");
						continue;
					}

					if (strstr(chModelName, "abrams") || strstr(chModelName, "lav.mdl"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(0);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Tank");
						continue;
					}


					if (strstr(chModelName, "headcrab"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(0);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Headcrab");
						continue;
					}

					if (strstr(chModelName, "zombie"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(2);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Red(), "Zombie");
						continue;
					}


					if (strstr(chModelName, "gman") || strstr(chModelName, "gman_high"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(2);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color::White(), "GMan");
						continue;
					}

					if (strstr(chModelName, "fisherman"))
					{
						vWorldPos = vWorldPos = pBaseEntity->GetHitboxPosition(16);
						if (!gDrawManager.WorldToScreen(vWorldPos, vScreen))
							continue;
						gDrawManager.DrawString(vScreen.x, vScreen.y, Color(255, 255, 255, 255), "My Naughty Uncle"); //My proudest achievement
						continue;
					}
				}
				continue;
			}
		}
	}

	//Player ESP
	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (!gCvars.misc_thirdperson && i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (pEntity->GetLifeState() != LIFE_ALIVE)
			continue;

		if (gCvars.esp_enemyonly && pEntity->GetTeamNum() == pLocal->GetTeamNum())
			continue;

		Player_ESP(pLocal, pEntity);
	}
}

void GetWorldSpaceCenter(CBaseEntity* pBaseEnt, Vector& vWorldSpaceCenter)
{
	if (pBaseEnt)
	{
		Vector vMin, vMax;
		pBaseEnt->GetRenderBounds(vMin, vMax);
		vWorldSpaceCenter = pBaseEnt->GetAbsOrigin();
		vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2.0f;
	}
}

void CESP::Player_ESP(CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	player_info_t pInfo;
	if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
		return;

	const matrix3x4& vMatrix = pEntity->GetRgflCoordinateFrame();

	Vector vMin = pEntity->GetCollideableMins();
	Vector vMax = pEntity->GetCollideableMaxs();
	Color clrPlayerCol = gDrawManager.GetPlayerColor(pEntity);
	Color clrBoneCol = gCvars.esp_bones == 1 ? Color::White() : gCvars.esp_bones == 2 ? Color::Green() : clrPlayerCol;

	Vector vPointList[] = {
		Vector(vMin.x, vMin.y, vMin.z),
		Vector(vMin.x, vMax.y, vMin.z),
		Vector(vMax.x, vMax.y, vMin.z),
		Vector(vMax.x, vMin.y, vMin.z),
		Vector(vMax.x, vMax.y, vMax.z),
		Vector(vMin.x, vMax.y, vMax.z),
		Vector(vMin.x, vMin.y, vMax.z),
		Vector(vMax.x, vMin.y, vMax.z)
	};

	Vector vTransformed[8];

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
			vTransformed[i][j] = vPointList[i].Dot((Vector&)vMatrix[j]) + vMatrix[j][3];

	Vector flb, brt, blb, frt, frb, brb, blt, flt;

	if (!gDrawManager.WorldToScreen(vTransformed[3], flb) ||
		!gDrawManager.WorldToScreen(vTransformed[0], blb) ||
		!gDrawManager.WorldToScreen(vTransformed[2], frb) ||
		!gDrawManager.WorldToScreen(vTransformed[6], blt) ||
		!gDrawManager.WorldToScreen(vTransformed[5], brt) ||
		!gDrawManager.WorldToScreen(vTransformed[4], frt) ||
		!gDrawManager.WorldToScreen(vTransformed[1], brb) ||
		!gDrawManager.WorldToScreen(vTransformed[7], flt))
		return;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 0; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top < arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom > arr[i].y)
			bottom = arr[i].y;
	}

	float x = left;
	float y = bottom;
	float w = right - left;
	float h = top - bottom;

	x += ((right - left) / 8);
	w -= ((right - left) / 8) * 2;

	//	Color clrBoneCol = gCvars.esp_bones == 1 ? Color::White() : gCvars.esp_bones == 2 ? Color::Green() : clrPlayerCol;
	int iY = 0;
	int iHp = pEntity->GetHealth(), iMaxHp = pEntity->GetMaxHealth();
	if (iHp > iMaxHp)
		iHp = iMaxHp;

	Vector vScreen, vWorldPos;

	if (gCvars.esp_pos == 1)
	{
		GetWorldSpaceCenter(pEntity, vWorldPos);
	}

	if (gCvars.esp_pos == 1 && gDrawManager.WorldToScreen(vWorldPos, vScreen) == false)
		return;

	if (gCvars.esp_box)
	{
		if (gCvars.esp_box_mode == 1)
		{
			gDrawManager.OutlineRect(x, y, w, h, clrPlayerCol);
			gDrawManager.OutlineRect(x + 1, y + 1, w - 2, h - 2, clrPlayerCol);
		}
		else if (gCvars.esp_box_mode == 2)
		{
			gDrawManager.OutlineRect(x - 1, y - 1, w + 2, h + 2, Color::Black());
			gDrawManager.OutlineRect(x, y, w, h, clrPlayerCol);
			gDrawManager.OutlineRect(x + 1, y + 1, w - 2, h - 2, Color::Black());
		}
		else if (gCvars.esp_box_mode == 3)
		{
			int VertLine;
			int HorzLine;
			VertLine = (((float)w) * (0.2f));
			HorzLine = (((float)h) * (0.2f));

			gDrawManager.OutlineRect(x, y - 1, VertLine, 1, Color(10, 10, 10, 150));
			gDrawManager.OutlineRect(x + w - VertLine, y - 1, VertLine, 1, Color(10, 10, 10, 150));
			gDrawManager.OutlineRect(x, y + h - 1, VertLine, 1, Color(10, 10, 10, 150));
			gDrawManager.OutlineRect(x + w - VertLine, y + h - 1, VertLine, 1, Color(10, 10, 10, 150));

			gDrawManager.OutlineRect(x - 1, y, 1, HorzLine, Color(10, 10, 10, 150));
			gDrawManager.OutlineRect(x - 1, y + h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));
			gDrawManager.OutlineRect(x + w - 1, y, 1, HorzLine, Color(10, 10, 10, 150));
			gDrawManager.OutlineRect(x + w - 1, y + h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));

			gDrawManager.OutlineRect(x, y, VertLine, 1, clrPlayerCol);
			gDrawManager.OutlineRect(x + w - VertLine, y, VertLine, 1, clrPlayerCol);
			gDrawManager.OutlineRect(x, y + h, VertLine, 1, clrPlayerCol);
			gDrawManager.OutlineRect(x + w - VertLine, y + h, VertLine, 1, clrPlayerCol);

			gDrawManager.OutlineRect(x, y, 1, HorzLine, clrPlayerCol);
			gDrawManager.OutlineRect(x, y + h - HorzLine, 1, HorzLine, clrPlayerCol);
			gDrawManager.OutlineRect(x + w, y, 1, HorzLine, clrPlayerCol);
			gDrawManager.OutlineRect(x + w, y + h - HorzLine, 1, HorzLine, clrPlayerCol);
		}
	}

	if (gCvars.esp_health)
	{
		if (gCvars.esp_health_mode == 2 || gCvars.esp_health_mode == 3)
		{
			gDrawManager.OutlineRect(x - 6, y - 1, 5, h, Color::Black());
			gDrawManager.DrawRect(x - 5, y + (h - (h / iMaxHp * iHp)) - 1, 3, h / iMaxHp * iHp, Color::Green());
		}
	}

	if (gCvars.esp_name)
	{
		if (gCvars.esp_pos == 0)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, clrPlayerCol, pInfo.name);
			iY += gDrawManager.GetESPHeight();
		}
		else if (gCvars.esp_pos == 1)
		{
			gDrawManager.DrawString(vScreen.x, vScreen.y, clrPlayerCol, "%s", pInfo.name);
			vScreen.y += gDrawManager.GetESPHeight();
		}
	}


	if (GAME_TF2) //TF2 Specific ESP functions
	{
		if (gCvars.esp_class)
		{
			if (gCvars.esp_pos == 0)
			{
				gDrawManager.DrawString(x + w + 2, y + iY, clrPlayerCol, "%s", pEntity->szGetClass());
				iY += gDrawManager.GetESPHeight();
			}
			else if (gCvars.esp_pos == 1)
			{
				gDrawManager.DrawString(vScreen.x, vScreen.y, clrPlayerCol, "%s", pEntity->szGetClass());
				vScreen.y += gDrawManager.GetESPHeight();
			}
		}
	}

	if (gCvars.esp_health)
	{
		if (gCvars.esp_health_mode == 1 || gCvars.esp_health_mode == 3)
		{
			if (gCvars.esp_pos == 0)
			{
				gDrawManager.DrawString(x + w + 2, y + iY, Color::Green(), "%d HP", pEntity->GetHealth());
				iY += gDrawManager.GetESPHeight();
			}
			else if (gCvars.esp_pos == 1)
			{
				gDrawManager.DrawString(vScreen.x, vScreen.y, Color::Green(), "%d HP", pEntity->GetHealth());
				vScreen.y += gDrawManager.GetESPHeight();
			}
		}
	}

	if (gCvars.esp_bones) //bones
	{
		static int iLeftArmBones[] = { 8, 7, 6, 4 };
		static int iRightArmBones[] = { 11, 10, 9, 4 };
		static int iHeadBones[] = { 0, 4, 1 };
		static int iLeftLegBones[] = { 14, 13, 1 };
		static int iRightLegBones[] = { 17, 16, 1 };

		DrawBone(pEntity, iLeftArmBones, 4, clrBoneCol);
		DrawBone(pEntity, iRightArmBones, 4, clrBoneCol);

		DrawBone(pEntity, iHeadBones, 3, clrBoneCol);

		DrawBone(pEntity, iLeftLegBones, 3, clrBoneCol);
		DrawBone(pEntity, iRightLegBones, 3, clrBoneCol);
	}

	Vector b = pLocal->GetAbsAngles();
}



//My code, but creds to f1ssion for giving me the idea
void CESP::DrawBone(CBaseEntity* pEntity, int* iBones, int count, Color clrCol)
{
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1)
			continue;

		Vector vBone1 = pEntity->GetHitboxPosition(iBones[i]);
		Vector vBone2 = pEntity->GetHitboxPosition(iBones[i + 1]);

		Vector vScr1, vScr2;

		if (!gDrawManager.WorldToScreen(vBone1, vScr1) || !gDrawManager.WorldToScreen(vBone2, vScr2))
			continue;

		gDrawManager.DrawLine(vScr1.x, vScr1.y, vScr2.x, vScr2.y, clrCol);
	}
}