#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "Util.h"
#include "ESP.h"
#include "MenuZ.hpp"
#include <Lmcons.h>

CScreenSize gScreenSize;

bool Key(INT vKey)
{
	if (GetAsyncKeyState(vKey) & 1)
		return true;

	return false;
}


//===================================================================================

bool bMeme = false;

void __fastcall Hooked_PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	try
	{
		CBaseEntity* pLocal = gInts.EntList->GetClientEntity(me);
		auto crosshair = gInts.cvar->FindVar("crosshair");
		if (!strcmp("HudScope", gInts.Panels->GetName(vguiPanel)) && gCvars.misc_noscope)
		{
			if (gCvars.misc_noscope_disablecrosshair)
			{
				if (pLocal->GetCond() & tf_cond::TFCond_Zoomed)
					crosshair->SetValue(0);
				else
					crosshair->SetValue(1);
			}

			return;
		}
		VMTManager& hook = VMTManager::GetHook(pPanels); 
		hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		static unsigned int vguiFocusOverlayPanel;

		if (vguiFocusOverlayPanel == NULL)
		{											//FocusOverlayPanel
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'F' && szName[5] == 'O' &&  szName[12] == 'P' )
			{
				vguiFocusOverlayPanel = vguiPanel;
				Intro();
			}
		}
		
		if (vguiFocusOverlayPanel == vguiPanel)
		{
			int iWidth, iHeight; //Resolution fix, so this can work in Fullscreen
			gInts.Engine->GetScreenSize(iWidth, iHeight);
			if (gScreenSize.iScreenWidth != iWidth || gScreenSize.iScreenHeight != iHeight)
				gInts.Engine->GetScreenSize(gScreenSize.iScreenWidth, gScreenSize.iScreenHeight);

			if (gCvars.esp_debug)
			{
				for (int i = 1; i < gInts.EntList->GetHighestEntityIndex(); i++)
				{
					CBaseEntity* model = GetBaseEntity(i);

					if (model == NULL || model->IsDormant())
						continue;

					Vector vWorldPos, vScreenPos;

					model->GetWorldSpaceCenter(vWorldPos);

					if (gDrawManager.WorldToScreen(vWorldPos, vScreenPos))
						gDrawManager.DrawString(vScreenPos.x, vScreenPos.y, Color(255, 255, 255, 255), "%s", gInts.ModelInfo->GetModelName(model->GetModel()));
				}
			}
			
			char username[UNLEN + 1];
			DWORD username_len = UNLEN + 1;
			GetUserName(username, &username_len); //Username = Current Windows Username


			if (GAME_TF2)
			{
				if (gCvars.misc_purebypass || gCvars.esp_chams)
				{
					if (!gInts.Engine->IsInGame())
					{
						void *pure_orig = nullptr;
						void **pure_addr = nullptr;

						if (!pure_addr)
						{
							pure_addr = *reinterpret_cast<void ***>(gSignatures.GetEngineSignature("A1 ? ? ? ? 56 33 F6 85 C0") + 1); //Signature from F12017 github
						}
						if (*pure_addr)
							pure_orig = *pure_addr;
						*pure_addr = (void *)0;
					}
				}
			}

			if (GetAsyncKeyState(VK_INSERT) & 0x1)
			{
				bMenuOpen = !bMenuOpen;
			}

			if (bMenuOpen)
			{
				bMeme = true;
				HackMenu->Draw(800, 750, gScreenSize.iScreenWidth, gScreenSize.iScreenHeight);
				gInts.Panels->SetMouseInputEnabled(vguiPanel, true); 
			}
			else if (bMeme)
			{
				bMeme = false;
				gInts.Panels->SetMouseInputEnabled(vguiPanel, false);
				//gCvars.Save();
			}


			if( gInts.Engine->IsDrawingLoadingImage() || !gInts.Engine->IsInGame( ) || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible( ) )
				return; //No need to draw the rest.

			

			CBaseEntity* pLocal = gInts.EntList->GetClientEntity(me); 

			if (!pLocal)
				return;

			gESP.Run(pLocal);

			if (Key(VK_F9)) //Reinitalize CDrawManager on key press
				gDrawManager.Initialize();

		}
	}
	catch(...)
	{
		Log::Fatal("Failed PaintTraverse");
	}
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDrawManager.Initialize(); //Initalize the drawing class.

		gNetVars.Initialize();
	}
	catch(...)
	{
		Log::Fatal("Failed Intro");
	}
}