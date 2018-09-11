/*    ProjectZ 

     **BEWARE**
	 This source is 100% PASTE, messy, unoptimized and just pure garbage all around
*/


#include "SDK.h"
#include "Client.h"
#include "Panels.h"
#include "Glow.h"
#include "Announcer.h"

COffsets gOffsets;
CInterfaces gInts;

CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUIFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;
CreateInterface_t CvarFactory = NULL;
CreateInterfaceFn SteamFactory = NULL;
CreateInterface_t MatSysFactory = NULL;

ITFMatchGroupDescription* GetMatchGroupDescription(int& idx) //credits blackfire62
{
	typedef ITFMatchGroupDescription* (_cdecl* GetFn)(int&);
	static GetFn Get = (GetFn)(gSignatures.GetClientSignature("55 8B EC 8B 45 08 8B 00 83 F8 FF"));
	return Get(idx);
}

typedef void(__thiscall *OverrideViewFn) (void*, CViewSetup*);

void __fastcall Hooked_OverrideView(void* _this, void* _edx, CViewSetup* pSetup) // credits, ActualCheats and outi - plasma
{

	// epic code
	VMTManager& hook = VMTManager::GetHook(_this); //Get a pointer to the instance of your VMTManager with the function GetHook.
	hook.GetMethod<OverrideViewFn>(16)(_this, pSetup); //Call the original.
	if (!gInts.Engine->IsInGame() && !gInts.Engine->IsConnected())
		return;

	CBaseEntity* pLocal = GetBaseEntity(me);

	if (gCvars.misc_fovoverride)
		if (pSetup->m_fov != 20.0f && pSetup->m_fov != gCvars.misc_fov_value) // no delet this, important for when zoomed- plasma
			pSetup->m_fov = gCvars.misc_fov_value;
}

DWORD WINAPI dwMainThread( LPVOID lpArguments )
{
	if (gInts.Client == NULL)
	{
		VMTBaseManager* clientHook = new VMTBaseManager();
		VMTBaseManager* clientModeHook = new VMTBaseManager();
		VMTBaseManager* panelHook = new VMTBaseManager();
		VMTBaseManager* defRenderHook = new VMTBaseManager();

		ClientFactory = ( CreateInterfaceFn ) GetProcAddress( gSignatures.GetModuleHandleSafe( "client.dll" ), "CreateInterface" );
		EngineFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("engine.dll"), "CreateInterface");
		VGUIFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vguimatsurface.dll"), "CreateInterface");
		CvarFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vstdlib.dll"), "CreateInterface");
		SteamFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA("SteamClient.dll"), "CreateInterface");
		Sleep(2000);
		MatSysFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("materialsystem.dll"), "CreateInterface");

		gInts.Client = ( CHLClient* )ClientFactory( "VClient017", NULL);
		gInts.EntList = ( CEntList* ) ClientFactory( "VClientEntityList003", NULL );
		gInts.Engine = ( EngineClient* ) EngineFactory( "VEngineClient013", NULL );
		gInts.Surface = ( ISurface* ) VGUIFactory( "VGUI_Surface030", NULL );
		gInts.EngineTrace = ( IEngineTrace* ) EngineFactory( "EngineTraceClient003", NULL );
		gInts.ModelInfo = ( IVModelInfo* ) EngineFactory( "VModelInfoClient006", NULL );
		gInts.MdlRender = (CModelRender*)EngineFactory("VEngineModel016", NULL);
		if (GAME_TF2)
			gInts.MatSystem = (CMaterialSystem*)MatSysFactory("VMaterialSystem081", NULL);
		else
			gInts.MatSystem = (CMaterialSystem*)MatSysFactory("VMaterialSystem080", NULL);
		gInts.RenderView = (CRenderView*)EngineFactory("VEngineRenderView014", NULL);
		gInts.cvar = (ICvar*)CvarFactory("VEngineCvar004", NULL);
		gInts.steamclient = (ISteamClient017*)SteamFactory("SteamClient017", NULL);
		gInts.EventManager = (IGameEventManager2*)EngineFactory("GAMEEVENTSMANAGER002", NULL);


		if (GAME_TF2)
			gGlow.init();

		CAnnouncer::Get().Initialize();

		HSteamPipe hNewPipe = gInts.steamclient->CreateSteamPipe();
		HSteamUser hNewUser = gInts.steamclient->ConnectToGlobalUser(hNewPipe);

		gInts.steamfriends = reinterpret_cast<ISteamFriends002 *>(gInts.steamclient->GetISteamFriends(hNewUser, hNewPipe, STEAMFRIENDS_INTERFACE_VERSION_002));
		gInts.steamuser = reinterpret_cast<ISteamUser017 *>(gInts.steamclient->GetISteamUser(hNewUser, hNewPipe, STEAMUSER_INTERFACE_VERSION_017));

		//Textures list
		if (GAME_TF2)
		{
			std::ofstream("tf\\materials\\texture-shaded.vmt") << R"#("UnlitGeneric" {

				"$basetexture" "vgui/white_additive"

				"$ignorez" "0"

				"$envmap" ""

				"$normalmapalphaenvmapmask" "0"

				"$envmapcontrast" "0"

				"$nofog" "1"

				"$model" "1"

				"$nocull" "1"

				"$selfillum" "1"

				"$halflambert" "1"

				"$znearer" "0"

				"$flat" "1"

				}

				)#";

			std::ofstream("tf\\materials\\texture-unshaded.vmt") << R"#("VertexLitGeneric" {

				"$basetexture" "vgui/white_additive"

				"$ignorez" "0"

				"$envmap" ""

				"$normalmapalphaenvmapmask" "0"

				"$envmapcontrast" "0"

				"$nofog" "1"

				"$model" "1"

				"$nocull" "1"

				"$selfillum" "1"

				"$halflambert" "1"

				"$znearer" "0"

				"$flat" "1"

				}

				)#";

		}

		XASSERT(gInts.Client);
		XASSERT(gInts.EntList);
		XASSERT(gInts.Engine);
		XASSERT(gInts.Surface);
		XASSERT(gInts.EngineTrace);
		XASSERT(gInts.ModelInfo);
		XASSERT(gInts.MdlRender);
		XASSERT(gInts.MatSystem);
		XASSERT(gInts.cvar);
		XASSERT(gInts.steamclient);
		XASSERT(gInts.steamfriends);
		XASSERT(gInts.steamuser);

		if (GAME_TF2)
		{
			gCvars.Vis_MatLit = gInts.MatSystem->FindMaterial("texture-shaded", "Model textures");
			gCvars.Hid_MatLit = gInts.MatSystem->FindMaterial("texture-shaded", "Model textures");

			gCvars.Vis_MatUnLit = gInts.MatSystem->FindMaterial("texture-unshaded", "Model textures");
			gCvars.Hid_MatUnLit = gInts.MatSystem->FindMaterial("texture-unshaded", "Model textures");
		}

		gInts.Engine->ClientCmd_Unrestricted("unbind ins");
		gInts.Engine->ClientCmd_Unrestricted("unbind uparrow");
		gInts.Engine->ClientCmd_Unrestricted("unbind downarrow");
		gInts.Engine->ClientCmd_Unrestricted("unbind leftarrow");
		gInts.Engine->ClientCmd_Unrestricted("unbind rightarrow");
		gInts.Engine->ClientCmd_Unrestricted("toggleconsole");//Unbinds keys that the menu uses, hope no one actually uses these lmao

		gInts.cvar->ConsoleColorPrintf(Color(255, 105, 180, 255), "ProjectZ Injected\n");
		gInts.cvar->ConsoleColorPrintf(Color(255, 182, 193, 255), "Build Date: ");
		gInts.cvar->ConsoleColorPrintf(Color(197, 139, 231, 255), "%s\n", __TIMESTAMP__);
		gInts.cvar->ConsoleColorPrintf(Color(255, 182, 193, 255), "Current Username:  ");
		gInts.cvar->ConsoleColorPrintf(Color(197, 139, 231, 255), "%s\n", gInts.steamfriends->GetPersonaName());

		//Setup the Panel hook so we can draw.
		if( !gInts.Panels )
		{
			VGUI2Factory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vgui2.dll"), "CreateInterface");
			gInts.Panels = ( IPanel* ) VGUI2Factory( "VGUI_Panel009", NULL );
			XASSERT( gInts.Panels );

			if( gInts.Panels )
			{
				panelHook->Init(gInts.Panels);
				panelHook->HookMethod(&Hooked_PaintTraverse, gOffsets.iPaintTraverseOffset);
				panelHook->Rehook();
			}
		}

		if (GAME_TF2)
		{
			for (int i = 0; i < 12; i++) //Code by blackfire62 - Competitive Convar Bypass
			{
				ITFMatchGroupDescription* desc = GetMatchGroupDescription(i);

				if (!desc || desc->m_iID > 9) //ID's over 9 are invalid
					continue;

				if (desc->m_bForceCompetitiveSettings)
				{
					desc->m_bForceCompetitiveSettings = false;
				}
			}
		}

		gInts.globals = *reinterpret_cast<CGlobals **>(gSignatures.GetEngineSignature("A1 ? ? ? ? 8B 11 68") + 8); //Initalize Globals
		XASSERT(gInts.globals);

		DWORD dwClientModeAddress; 
		if (GAME_TF2)
			dwClientModeAddress = gSignatures.GetClientSignature("8B 0D ? ? ? ? 8B 02 D9 05"); //Team Fortress 2 ClientMode Signature
		else
			dwClientModeAddress = gSignatures.GetClientSignature("8B 0D ? ? ? ? 8B 01 5D FF 60 28 CC"); //Native Source SDK 2013 ClientMode Signature
		XASSERT(dwClientModeAddress);
		gInts.ClientMode = **(ClientModeShared***)(dwClientModeAddress + 2);
		LOGDEBUG("g_pClientModeShared_ptr client.dll+0x%X", (DWORD)gInts.ClientMode - dwClientBase);
				
		clientHook->Init(gInts.Client);
		clientHook->HookMethod(&Hooked_KeyEvent, gOffsets.iKeyEventOffset);
		clientHook->HookMethod(&FrameStageNotifyThink, 35);
		clientHook->Rehook();

		clientModeHook->Init(gInts.ClientMode);
		clientModeHook->HookMethod(&Hooked_CreateMove, gOffsets.iCreateMoveOffset); //ClientMode create move is called inside of CHLClient::CreateMove, and thus no need for hooking WriteUserCmdDelta.
		clientModeHook->HookMethod(&Hooked_OverrideView, 16);
		clientModeHook->Rehook();

		if (GAME_TF2)
		{
			defRenderHook->Init(gInts.MdlRender);
			defRenderHook->HookMethod(&Hooked_DrawModelExecute, 19);
			defRenderHook->Rehook();
		}

		if (GAME_CSS)
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_CSS.ini"));
		else if (GAME_DODS)
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_DODS.ini"));
		else if (GAME_TF2)
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_TF2.ini"));
		else if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2)
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_HL2.ini"));
		else if (GAME_HL2DM)
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_HL2DM.ini"));
		else if (GAME_HLS)
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_HLS.ini"));
		else if (GAME_SYN)
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_SYN.ini"));
		else if (GAME_GMOD)
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_GMOD.ini"));
		else if (GAME_CCURE)
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_CCURE.ini"));
		else
			gFileManager.Initialize(gBaseAPI.GetDirectoryFile("ProjectZ_OTHER.ini"));

		gCvars.Load();
	}
	return 0; //The thread has been completed, and we do not need to call anything once we're done. The call to Hooked_PaintTraverse is now our main thread.
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		gBaseAPI.BaseUponModule((HMODULE)hInstance);
		Log::Init(hInstance);
		CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)dwMainThread, NULL, 0, NULL ); 
	}
	return true;
}


/*
cccc
vvvv
yrros mi
*/