#include "SDK.h"
#include "Client.h"
#include "Util.h"
#include "Aimbot.h"
#include "Triggerbot.h"
#include "Misc.h"
#include "Backtrack.h"
#include "TimeShift.h"
#include "Glow.h"
#include "NPCAimbot.h"
#include "CDrawManager.h"
#include <intrin.h>
#include <cstdlib>
#include "AutoAirblast.h"
#include "Announcer.h"

Vector qLASTTICK;

//============================================================================================
bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand)
{
	VMTManager& hook = VMTManager::GetHook(ClientMode); //Get a pointer to the instance of your VMTManager with the function GetHook.
	bool bReturn = hook.GetMethod<bool(__thiscall*)(PVOID, float, CUserCmd*)>(gOffsets.iCreateMoveOffset)(ClientMode, input_sample_frametime, pCommand); //Call the original.

	CBaseEntity* pLocal = GetBaseEntity(me);

	try
	{
		if (!pCommand->command_number)
			return false;

		if (!pLocal)
			return bReturn;

		auto base = reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) - sizeof(uintptr_t);
		bool& bSendPacket = *(***reinterpret_cast<bool****>(base) - 1);

		if (pLocal->IsAlive())
		{
			if (gCvars.misc_fakelag)
			{
				auto netchan = gInts.Engine->GetNetChannelInfo();
				if (netchan->m_nChokedPackets < (int)gCvars.misc_fakelag_value)
					bSendPacket = false;
				else
					bSendPacket = true;
			}
		}

		gMisc.Run(pLocal, pCommand, bSendPacket); //Misc Features
		gAim.Run(pLocal, pCommand); //Aimbot
		gTrigger.Run(pLocal, pCommand); //Triggerbot
		gShift.Run(pLocal, pCommand); //Time Shift
		gBacktracking.Run( pCommand); //Backtrack (can cause crashes)
		if (GAME_TF2)
			gBlast.Run(pLocal, pCommand);
		CAnnouncer::Get().Run();
		//NPCAimbot, This is a slightly modified version of gir489's Black Mesa Darkstorm 
		if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2 || GAME_HLS || GAME_SYN)
		{

			if (gCvars.aimbot_active)
			{
				if (!Util->IsKeyPressed(gCvars.aimbot_key))
					return false;

				gNPCAimbot.FindTarget(pCommand);
				if (gNPCAimbot.bHasTarget())
				{
					if (gCvars.aimbot_autoshoot)
						pCommand->buttons |= IN_ATTACK;

					gInts.Engine->SetViewAngles(pCommand->viewangles);
					gNPCAimbot.SilentAimFix(pCommand, gNPCAimbot.GetAimbotAngles());
					bReturn = false;
				}
			}
		}

	}
	catch(...)
	{
		Log::Fatal("Failed Hooked_CreateMove");
	}
	qLASTTICK = pCommand->viewangles;
	return false/*bReturn*/;
}

void ForceMaterial(IMaterial* material, Color color)
{
	if (material != NULL)
	{
		gInts.RenderView->SetColorModulation(color.Base());
		gInts.MdlRender->ForcedMaterialOverride(material, OverrideType_t::OVERRIDE_NORMAL);
	}
}

void __stdcall Hooked_DrawModelExecute(void *state, ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	VMTManager& hook = VMTManager::GetHook(gInts.MdlRender);

	const char* pszModelName = gInts.ModelInfo->GetModelName(pInfo.pModel);


	if (GAME_TF2)
		if (gCvars.misc_nohats && strstr(pszModelName, "player/items"))
			return;

	hook.Unhook();

	if (GAME_TF2)
	{
		CBaseEntity* pModelEntity = (CBaseEntity*)gInts.EntList->GetClientEntity(pInfo.entity_index);
		CBaseEntity* pLocal = (CBaseEntity*)gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());


		if (strstr(pszModelName, "models/player") && gCvars.esp_chams)
		{

			if (pModelEntity) {

				if (gCvars.esp_enemyonly)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 && !pModelEntity->IsDormant() && pModelEntity->GetTeamNum() != pLocal->GetTeamNum()) {
						Color RGBA = gDrawManager.GetPlayerColor(pModelEntity);

						if (gCvars.esp_chams_mode == 1)
						{
							//Hidden UnLit
							gCvars.Hid_MatUnLit->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							gCvars.Hid_MatUnLit->ColorModulate(RGBA[0], RGBA[1], RGBA[2]);
							gCvars.Hid_MatUnLit->AddRef();

							ForceMaterial(gCvars.Hid_MatUnLit, RGBA);
							gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);

							//Visible UnLit
							gCvars.Vis_MatUnLit->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							gCvars.Vis_MatUnLit->ColorModulate(RGBA[0], RGBA[1], RGBA[2]);
							gCvars.Vis_MatUnLit->AddRef();

							ForceMaterial(gCvars.Vis_MatUnLit, RGBA);
							gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
						}
					}
					else
					{
						gInts.MdlRender->ForcedMaterialOverride(NULL, OverrideType_t::OVERRIDE_NORMAL);
					}
				}
				else if (!gCvars.esp_enemyonly)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 && !pModelEntity->IsDormant()) {
						Color RGBA = gDrawManager.GetPlayerColor(pModelEntity);


						if (gCvars.esp_chams_mode == 1)
						{
							//Hidden UnLit
							gCvars.Hid_MatUnLit->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							gCvars.Hid_MatUnLit->ColorModulate(RGBA[0], RGBA[1], RGBA[2]);
							gCvars.Hid_MatUnLit->AddRef();

							ForceMaterial(gCvars.Hid_MatUnLit, RGBA);
							gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);

							//Visible UnLit
							gCvars.Vis_MatUnLit->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							gCvars.Vis_MatUnLit->ColorModulate(RGBA[0], RGBA[1], RGBA[2]);
							gCvars.Vis_MatUnLit->AddRef();

							ForceMaterial(gCvars.Vis_MatUnLit, RGBA);
							gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
						}
						else if (gCvars.esp_chams_mode == 2)
						{
							//Hidden Lit
							gCvars.Hid_MatLit->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							gCvars.Hid_MatLit->ColorModulate(RGBA[0], RGBA[1], RGBA[2]);
							gCvars.Hid_MatLit->AddRef();

							ForceMaterial(gCvars.Hid_MatLit, RGBA);
							gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);

							//Visible Lit
							gCvars.Vis_MatLit->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							gCvars.Vis_MatLit->ColorModulate(RGBA[0], RGBA[1], RGBA[2]);
							gCvars.Vis_MatLit->AddRef();

							ForceMaterial(gCvars.Vis_MatLit, RGBA);
							gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
						}
					}
					else
					{
						gInts.MdlRender->ForcedMaterialOverride(NULL, OverrideType_t::OVERRIDE_NORMAL);
					}
				}
			}
		}


		if (gCvars.misc_hands)
		{
			if (gCvars.misc_hands_mode == 1 && strstr(pszModelName, "arms")) {
				gInts.RenderView->SetBlend(0);
				gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
			}
			else if (gCvars.misc_hands_mode == 2 && strstr(pszModelName, "arms")) {
				gInts.RenderView->SetBlend(0.5);
				gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
			}
			else if (gCvars.misc_hands_mode == 3 && strstr(pszModelName, "arms"))
			{
				CBaseEntity* pLocal = (CBaseEntity*)gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());

				if (pLocal->IsAlive() && pLocal->GetHealth() > 0 && !pLocal->IsDormant()) {
					Color RGBA = gDrawManager.GetPlayerColor(pLocal);

					gCvars.Vis_MatUnLit->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					gCvars.Vis_MatUnLit->ColorModulate(RGBA[0], RGBA[1], RGBA[2]);
					gCvars.Vis_MatUnLit->AddRef();

					ForceMaterial(gCvars.Vis_MatUnLit, RGBA);
					gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
				}
				else
				{
					gInts.MdlRender->ForcedMaterialOverride(NULL, OverrideType_t::OVERRIDE_NORMAL);
				}
			}
		}
	}



	gInts.MdlRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
	gInts.MdlRender->ForcedMaterialOverride(NULL, OverrideType_t::OVERRIDE_NORMAL);
	hook.Rehook();
}

void __fastcall FrameStageNotifyThink(PVOID CHLClient, void *_this, ClientFrameStage_t Stage)
{


	if (GAME_TF2 && Stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		CBaseEntity* pLocal = (CBaseEntity*)gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());

		if (!pLocal || pLocal->GetLifeState() != LIFE_ALIVE)
			return;

		for (auto i = 1; i <= gInts.Engine->GetMaxClients(); i++)
		{
			CBaseEntity *entity = nullptr;
			player_info_t temp;

			if (!(entity = gInts.EntList->GetClientEntity(i)))
				continue;

			if (entity->IsDormant())
				continue;

			if (!gInts.Engine->GetPlayerInfo(i, &temp))
				continue;

			if (!entity->GetLifeState() == LIFE_ALIVE)
				continue;

			Vector vX = entity->GetAngles();
			Vector vY = entity->GetAnglesHTC();
			auto *WritePitch = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]"));
			auto *WriteYaw = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[1]"));
			if (gCvars.aimbot_resolver)
			{
				if (vX.x == -89.0f)
					*WritePitch = 90.0f;

				if (vX.x == 89.0f)
					*WritePitch = -90.0f;
			}
		}
	}

	if (gInts.Engine->IsInGame() && Stage == FRAME_RENDER_START)
	{
		if (GAME_TF2)
			gGlow.fsn();

		CBaseEntity *oEntity = gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());

		int *Fov;
		int OldFov;

		int m_flFOVRate = 0xE5C;
		int &fovPtr = *(int*)(oEntity + gNetVars.get_offset("DT_BasePlayer", "m_iFOV")), defaultFov = *(int*)(oEntity + gNetVars.get_offset("DT_BasePlayer", "m_iDefaultFOV"));
		if (gCvars.misc_nozoom) //Code by cademtz
		{
			fovPtr = defaultFov;
			*(float*)(oEntity + m_flFOVRate) = 0;
		}

		if (gCvars.misc_thirdperson) //Code by Bux
		{
			CBaseEntity *oEntity = gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());

			auto *m_angEyeAngles = reinterpret_cast<float*>(reinterpret_cast<DWORD>(oEntity) + gNetVars.get_offset("DT_BasePlayer", "pl", "deadflag") + 8);

			auto *HTC = reinterpret_cast<float*>(reinterpret_cast<DWORD>(oEntity) + gNetVars.get_offset("DT_BasePlayer", "pl", "deadflag") + 4);

			*HTC = qLASTTICK.x;
			*m_angEyeAngles = qLASTTICK.y;

			ConVar* sv_cheats = gInts.cvar->FindVar("sv_cheats");
			if (sv_cheats->GetInt() == 0) sv_cheats->SetValue(1);
			ConVar* pThirdCamYaw = gInts.cvar->FindVar("cam_idealyaw");
			gInts.Engine->ClientCmd_Unrestricted("thirdperson");

			pThirdCamYaw->SetValue(0);
		}
		else if (!gCvars.misc_thirdperson)
		{
			gInts.Engine->ClientCmd_Unrestricted("firstperson");
		}
	}


	VMTManager &FSNBoiz = VMTManager::GetHook(CHLClient);
	return FSNBoiz.GetMethod<void(__fastcall *)(PVOID, void *, ClientFrameStage_t)>(35)(CHLClient, _this, Stage);
}

typedef void(__thiscall* FrameStageNotify)(void *, ClientFrameStage_t);

//============================================================================================
int __fastcall Hooked_KeyEvent(PVOID CHLClient, int edx, int eventcode, int keynum, const char *currentBinding)
{
	if (eventcode == 1)
	{
		//Nothing here, new menu, no need
	}

	VMTManager &hook = VMTManager::GetHook(CHLClient); // Get a pointer to the instance of your VMTManager with the function GetHook.
	return hook.GetMethod<int(__thiscall *)(PVOID, int, int, const char *)>(gOffsets.iKeyEventOffset)(CHLClient, eventcode, keynum, currentBinding); // Call the original.
}