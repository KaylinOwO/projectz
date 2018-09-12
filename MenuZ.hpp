#include "CDrawManager.h"
#include "SDK.h"
#include "WeaponList.h"
#include <string>

/*
    Menu by The m3mehook Development Team
	Ported Over to Darkstorm by Roguezilla
	"Cleaned up" by Kaylin
*/

///.-------------------------------------------------------------------------------------.
// |           Hexadecimal escape characters and their uses - Mally#0001                 |
///|-------------------------------------------------------------------------------------|
///|                                                                                     |
// |       Escape sequence        ASCII HEX value         Character represented          |
///|		\a and \007	           07	                   Alert (beep)                  |
///|		\b	                   08	                   Backspace                     |
///|		\f	                   0C	                   Formfeed                      |
///|		\n	                   0A	                   Newline                       |
///|		\r	                   0D	                   Carriage return               |
///|		\t	                   09	                   Horizontal tab                |
///|		\v	                   0B	                   Vertical tab                  |
///|		\\	                   5C	                   Backslash                     |
///|		\'	                   27	                   Single quotation mark         |
///|		\"	                   22	                   Double quotation mark         |
///|		\?	                   3F	                   Question mark                 |
///|		\e	                   1B	                   Escape character              |
///|                                                                                     |
///'-------------------------------------------------------------------------------------'

class CMenu
{
public:
	void Draw(int, int, int, int);


private:
	int iMouseX, iMouseY, iWidth, iHeight, iScreenWidth, iScreenHeight, iTab = 1;
	bool bClicked, bMouseD = false;
	char* title = "";
	int tick = 0;
	bool waiting = false;
	int CPWidth = 168, CPHeight = 179;
	void SaveButton(const char* strText, int iTabIndex, int iX, int iY);
	void LoadButton(const char* strText, int iTabIndex, int iX, int iY);
	void ResetColors(const char* strText, int iTabIndex, int iX, int iY);
	void GetMousePos();
	void GetClicked();
	bool Holding(int, int, int, int);
	bool Hovering(int, int, int, int);
	int MouseXPos(int, int, int, int);
	bool Clicked(int, int, int, int);
	void DrawRectM(int, int, int, int, Color);
	void DrawOutlinedRectM(int, int, int, int, Color);
	void DrawStringM(int, int, Color, const char*, ...);
	void DrawStringM2(int, int, Color, const char*);
	void DrawSlider(const char*, int, int, int, int, int, int &);
	void DrawButton(const char*, int, int, int, bool &);
	void DrawDropdown(const char*, int, int, int, int, const char**, int, int &);
	void DrawTab(const char*, int);
	void DrawKey(int, int, int, int);
	void DrawBackground();
	void DrawClose();
	void DrawButtons();

}; CMenu *HackMenu = new CMenu;

bool bMenuOpen = false;
const char* key[8] = { "Mouse 1", "Mouse 2", "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "F" };
const char* key2[6] = { "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "F" };
const char* aax[4] = { "FUp", "Up", "FDown", "Down" };
const char* aay[7] = { "FSideR", "FSideL", "FRight", "HBL", "HBR", "FLeft", "FManual" };
const char* hands[3] = { "None", "Transparent", "Chams" };
const char* weapons[2] = { "None", "Transparent" };
const char* health[3] = { "Text", "Bar", "Both" };
const char* method[2] = { "Plain", "Silent"};
const char* chams[2] = { "Material", "Flat"};
const char* box[3] = { "Normal", "Outlined", "Corner" };
const char* cfg[4] = { "Default", "Legit", "Rage" };
const char* tpriority[2] = { "FOV", "Distance" };

void CMenu::Draw(int iMenuWidth, int iMenuHeight, int iSWidth, int iSHeight)
{
	iWidth = iMenuWidth;
	iHeight = iMenuHeight;
	iScreenWidth = iSWidth;
	iScreenHeight = iSHeight;

	GetMousePos();
	GetClicked();
	DrawBackground();
	DrawButtons();
}

void CMenu::GetMousePos()
{
	gInts.Surface->GetCursorPosition(iMouseX, iMouseY);
}

void CMenu::GetClicked()
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		bClicked = false;
		bMouseD = true;
	}
	else
		if (bMouseD)
		{
			bClicked = true;
			bMouseD = false;
		}
		else
			bClicked = false;
}

bool CMenu::Holding(int iXStart, int iYStart, int iXEnd, int iYEnd)
{
	if (GetAsyncKeyState(VK_LBUTTON))
		if (Hovering(iXStart, iYStart, iXEnd, iYEnd))
			return true;

	return false;
}


bool CMenu::Hovering(int iXStart, int iYStart, int iXEnd, int iYEnd)
{
	int iRealXStart = ((iScreenWidth / 2) - (iWidth / 2)) + iXStart;
	int iRealYStart = ((iScreenHeight / 2) - (iHeight / 2)) + iYStart;
	int iRealXEnd = iRealXStart + iXEnd;
	int iRealYEnd = iRealYStart + iYEnd;

	if (iMouseX >= iRealXStart && iRealXEnd >= iMouseX)
		if (iMouseY >= iRealYStart && iRealYEnd >= iMouseY)
			return true;

	return false;
}

int CMenu::MouseXPos(int iXStart, int iYStart, int iXEnd, int iYEnd)
{
	int iRealXStart = ((iScreenWidth / 2) - (iWidth / 2)) + iXStart;
	return iMouseX - iRealXStart;
}

bool CMenu::Clicked(int iXStart, int iYStart, int iXEnd, int iYEnd)
{
	if (bClicked)
		if (Hovering(iXStart, iYStart, iXEnd, iYEnd))
			return true;

	return false;
}

void CMenu::DrawRectM(int iXStart, int iYStart, int iRWidth, int iRHeight, Color colDrawColor)
{
	int iRealXStart = ((iScreenWidth / 2) - (iWidth / 2)) + iXStart;
	int iRealYStart = ((iScreenHeight / 2) - (iHeight / 2)) + iYStart;
	
	gDrawManager.DrawRect(iRealXStart, iRealYStart, iRWidth, iRHeight, colDrawColor);
}

void CMenu::DrawOutlinedRectM(int iXStart, int iYStart, int iRWidth, int iRHeight, Color colDrawColor)
{
	int iRealXStart = ((iScreenWidth / 2) - (iWidth / 2)) + iXStart;
	int iRealYStart = ((iScreenHeight / 2) - (iHeight / 2)) + iYStart;

	gDrawManager.OutlineRect(iRealXStart, iRealYStart, iRWidth, iRHeight, colDrawColor);
}

void CMenu::DrawStringM(int iX, int iY, Color colDrawColor, const char* msg, ...)
{
	int iRealX = ((iScreenWidth / 2) - (iWidth / 2)) + iX;
	int iRealY = ((iScreenHeight / 2) - (iHeight / 2)) + iY;



	gDrawManager.DrawStringA(iRealX, iRealY, colDrawColor, msg);
}

void CMenu::DrawStringM2(int iX, int iY, Color colDrawColor, const char* msg)
{
	int iRealX = ((iScreenWidth / 2) - (iWidth / 2)) + iX;
	int iRealY = ((iScreenHeight / 2) - (iHeight / 2)) + iY;

	gDrawManager.DrawStringB(iRealX, iRealY, colDrawColor, msg);
}

void CMenu::DrawButton(const char* strText, int iTabIndex, int iX, int iY, bool &bVar)
{
	if (iTab == iTabIndex)
	{
		if (!bVar)
			DrawOutlinedRectM(iX, iY, 20, 20, Color(93, 0, 114, 255));
		else
			DrawRectM(iX, iY, 20, 20, Color(93, 0, 114, 255));

		DrawStringM2(iX + 28, iY + 10, Color(255, 255, 255, 255), strText);

		if (Clicked(iX, iY, 20, 20))
		{
			bVar = !bVar;
		}
	}
}

void CMenu::SaveButton(const char* strText, int iTabIndex, int iX, int iY)
{
	if (iTab == iTabIndex)
	{
		DrawOutlinedRectM(iX, iY, 125, 20, Color(93, 0, 114, 255));

		DrawStringM2(iX + 12, iY + 10, Color(255, 255, 255, 255), strText);

		if (Hovering(iX, iY, 125, 20))
			DrawRectM(iX, iY, 125, 20, Color(93, 0, 114, 125));

		if (Clicked(iX, iY, 125, 20))
			gCvars.Save();
	}
}

void CMenu::LoadButton(const char* strText, int iTabIndex, int iX, int iY)
{
	if (iTab == iTabIndex)
	{
		DrawOutlinedRectM(iX, iY, 125, 20, Color(93, 0, 114, 255));

		DrawStringM2(iX + 12, iY + 10, Color(255, 255, 255, 255), strText);

		if (Hovering(iX, iY, 125, 20))
			DrawRectM(iX, iY, 125, 20, Color(93, 0, 114, 125));

		if (Clicked(iX, iY, 125, 20))
			gCvars.Load();
	}
}

void CMenu::ResetColors(const char* strText, int iTabIndex, int iX, int iY)
{
	if (iTab == iTabIndex)
	{
		DrawOutlinedRectM(iX, iY, 125, 20, Color(93, 0, 114, 255));

		DrawStringM2(iX + 12, iY + 10, Color(255, 255, 255, 255), strText);

		if (Hovering(iX, iY, 125, 20))
			DrawRectM(iX, iY, 125, 20, Color(93, 0, 114, 125));

		if (Clicked(iX, iY, 125, 20))
		{
			gCvars.color_r_red = 255;
			gCvars.color_g_red = 20;
			gCvars.color_b_red = 20;

			gCvars.color_r_blu = 0;
			gCvars.color_g_blu = 153;
			gCvars.color_b_blu = 255;

			gCvars.color_r_other = 0;
			gCvars.color_g_other = 0;
			gCvars.color_b_other = 255;
		}
	}
}

int WhichKeyIsPressed()
{
	for (int i = 1; i < 255; ++i)
	{
		if (GetAsyncKeyState(i) & 0x8000)
			return i;
	}
}

void CMenu::DrawKey(int tabindex, int iX, int iY, int kek)
{
	if (iTab == tabindex)
	{
		auto bHover = Hovering(iX, iY, 60, 20);
		static auto bState = false;

		DrawOutlinedRectM(iX, iY, 60, 20, Color(93, 0, 114, 255));
		DrawRectM(iX + 1, iY + 1, 60 - 2, 20 - 2, Color(27, 27, 27));

		if (bHover && Clicked(iX, iY, 60, 20))
		{
			bState = !bState;
		}

		if (bState && GetAsyncKeyState(WhichKeyIsPressed()))
		{
			bState = false;
			kek = WhichKeyIsPressed();
		}

		char pszBuffer[0x20];
		sprintf_s(pszBuffer, "%.f", NULL);

		DrawStringM(iX + 40 / 2 + 5, iY + 20 / 2, Color(255, 255, 255), "%.f", std::string(pszBuffer));
		DrawStringM(iX + 40 / 2, iY + 20 / 2, Color(255, 255, 255), title);
	}
}

void CMenu::DrawSlider(const char* strText, int iTabIndex, int iX, int iY, int iWidth, int iMax, int &iVar)
{
	if (iTab == iTabIndex)
	{
		DrawOutlinedRectM(iX, iY, iWidth, 20, Color(93, 0, 114, 255));
		DrawRectM(iX, iY, iVar * ((float)iWidth / (float)(iMax)), 20, Color(93, 0, 114, 255));

		std::string s = std::to_string(iVar);
		char const *pchar = s.c_str();

		DrawStringM2(iX + 12, iY + 9, Color(255, 255, 255, 255), strText);
		DrawStringM2(iX + ((float)iWidth / (float)2) - 3, iY + 10, Color(255, 255, 255, 255), pchar);

		if (Holding(iX, iY, iWidth, 20))
		{
			iVar = MouseXPos(iX, iY, iWidth, 20) / ((float)iWidth / (float)(iMax));
		}
	}
}

void CMenu::DrawDropdown(const char* strText, int iTabIndex, int iX, int iY, int iAdd, const char** chTable, int iAmount, int &iVar)
{
	if (iTab == iTabIndex)
	{

		DrawOutlinedRectM(iX - 50, iY, 100, iAdd, Color(93, 0, 114, 255));

		DrawOutlinedRectM(iX - 50, iY, 100, iAdd + (iAmount * iAdd) + 1, Color(93, 0, 114, 255));

		DrawStringM(iX, iY + 10, Color(255, 255, 255, 255), strText);


		for (int iVarIndex = 1; iVarIndex < iAmount + 1; iVarIndex++)
		{
			const char* strMeme = chTable[iVarIndex - 1];

			if (Hovering(iX - 50, iY + iVarIndex * iAdd, 100, iAdd))
				DrawRectM(iX - 49, iY + iVarIndex * iAdd, 98, iAdd, iVar == iVarIndex ? Color(93, 0, 114, 125) : Color(255, 255, 255, 125));

			if (Holding(iX - 50, iY + iVarIndex * iAdd, 100, iAdd))
			{
				iVar = iVarIndex;
			}

			DrawStringM(iX, iY + (iAdd / 2) + (iVarIndex * iAdd), iVar == iVarIndex ? Color(93, 0, 114, 255) : Color(255, 255, 255, 255), strMeme);
		}
	}
}

void CMenu::DrawTab(const char* strText, int iTabIndex)
{
	int iBaseX = (iWidth / 4) * (iTabIndex - 1);
	int iBaseW = iWidth / 4;

	DrawRectM(iBaseX, 24, iBaseW, 28, iTab == iTabIndex ? Color(53, 0, 96, 255) : Color(93, 0, 114, 255));
	DrawStringM(iBaseX + (iBaseW / 2), 38, Color(255, 255, 255, 255), strText);

	if (Hovering(iBaseX, 24, iBaseW, 28))
		DrawRectM(iBaseX, 24, iBaseW, 28, Color(255, 255, 255, 50));

	if (Clicked(iBaseX, 24, iBaseW, 28))
		iTab = iTabIndex;
}

void CMenu::DrawBackground()
{
	DrawRectM(0, 0, iWidth, iHeight, Color(0, 0, 0, 125));
	DrawRectM(0, 0, iWidth, 24, Color(93, 0, 114, 255));
	DrawStringM(iWidth / 2, 12, Color(255, 255, 255, 255), "ProjectZ || Cheat Menu");
	DrawOutlinedRectM(0, 0, iWidth, iHeight, Color(93, 0, 114, 255));
}

void CMenu::DrawClose()
{
	DrawRectM(iWidth - 24, 0, 24, 24, Color(93, 0, 114, 255));
	DrawStringM(iWidth - 12, 12, Color(255, 255, 255, 255), "X");

	if (Clicked(iWidth - 24, 0, 24, 24))
		bMenuOpen = false;
}

void CMenu::DrawButtons()
{

	DrawClose();

	int y = 58;
	int add = 22;

	DrawTab("Aimbot", 1);
	DrawTab("Triggerbot", 2);
	DrawTab("Visuals", 3);
	DrawTab("Misc", 4);

	//
	// Aimbot
	//
	DrawButton("Aimbot", 1, 8, y, gCvars.aimbot_active);
	DrawDropdown("Target Priority", 1, iWidth - 160, 58, add, tpriority, 2, gCvars.aimbot_priority);
	DrawDropdown("Method", 1, iWidth - 58, 58, add, method, 2, gCvars.aimbot_mode);
	DrawButton("Key", 1, 8, y += add, gCvars.aimbot_key_enabled);
	DrawDropdown("Key", 1, iWidth - 160, 135, add, key, 8, gCvars.aimbot_key);
	//DrawDropdown("Hands", 3, iWidth - 58, 160, add, hands, 3, gCvars.misc_hands_mode);
	DrawButton("Hitscan", 1, 8, y += add, gCvars.aimbot_hitscan);
	DrawSlider("FOV", 1, 8, y += add, 180, 180, gCvars.aimbot_fov);
	DrawButton("Smoothing", 1, 8, y += add, gCvars.aimbot_smooth);
	if (gCvars.aimbot_smooth)
		DrawSlider("", 1, 8, y += add, 180, 180, gCvars.aimbot_smooth_amt);
	if (GAME_TF2)
	{
		DrawButton("Wait For Charge", 1, 8, y += add, gCvars.aimbot_waitforcharge);
		DrawButton("Zoomed Only", 1, 8, y += add, gCvars.aimbot_zoomedonly);
	}
	DrawButton("Autoshoot", 1, 8, y += add, gCvars.aimbot_autoshoot);
	DrawButton("Aim at Teammates", 1, 8, y += add, gCvars.aimbot_deathmatch);
	DrawButton("Aim at Backtrack", 1, 8, y += add, gCvars.aimbot_aimatbacktrack);
	if (GAME_TF2)
	{
		DrawButton("Ignore Cloaked", 1, 8, y += add, gCvars.aimbot_ignorecloaked);
		DrawButton("Resolver", 1, 8, y += add, gCvars.aimbot_resolver);
		DrawButton("Projectile Prediction", 1, 8, y += add, gCvars.aimbot_projectile);
	}

	if (GAME_HL2DM)
		DrawButton("Ignore Health > 100", 1, 8, y += add, gCvars.aimbot_ignoreabove100);

	//
	// Triggerbot
	//
	y = 58;
	DrawButton("Hitscan Triggerbot", 2, 8, y, gCvars.triggerbot_active);
	DrawButton("Head Only", 2, 12, y += add, gCvars.triggerbot_headonly);
	DrawDropdown("Key", 2, iWidth - 160, 58, add, key2, 6, gCvars.triggerbot_key);
	if (GAME_TF2)
	{
		DrawButton("Auto Backstab", 2, 8, y += add, gCvars.triggerbot_autobackstab);
		DrawButton("Auto Airblast", 2, 8, y += add, gCvars.autoairblast_enabled);
		DrawButton("Aim at Projectile", 2, 12, y += add, gCvars.autoairblast_rage);
	}

	//
	// Visuals
	//
	y = 58;
	DrawButton("Enabled", 3, 8, y, gCvars.esp_active);
	DrawButton("Enemy Only", 3, 8, y += add, gCvars.esp_enemyonly);
	if (GAME_TF2)
		DrawButton("Enemy Buildings", 3, 8, y += add, gCvars.esp_objects);
	DrawButton("Box", 3, 8, y += add, gCvars.esp_box);
	DrawButton("Bones", 3, 8, y += add, gCvars.esp_bones);
	DrawButton("Name", 3, 8, y += add, gCvars.esp_name);
	DrawButton("Health", 3, 8, y += add, gCvars.esp_health);
	if (GAME_TF2)
	{
		ConVar* mat_dxlevel = gInts.cvar->FindVar("mat_dxlevel");
		if (mat_dxlevel->GetInt() > 90)
			DrawButton("Glow", 3, 8, y += add, gCvars.esp_glow);
		DrawButton("Chams", 3, 8, y += add, gCvars.esp_chams);
		DrawDropdown("Chams", 3, iWidth - 58, 58, add, chams , 2, gCvars.esp_chams_mode);
		DrawButton("Hands", 3, 8, y += add, gCvars.misc_hands);
		DrawDropdown("Hands", 3, iWidth - 58, 160, add, hands, 3, gCvars.misc_hands_mode);
		DrawDropdown("Box", 3, iWidth - 160, 58, add, box, 3, gCvars.esp_box_mode);
		DrawDropdown("Health", 3, iWidth - 160, 160, add, health, 3, gCvars.esp_health_mode);

	}
	else
	{
		DrawDropdown("Box", 3, iWidth - 58, 58, add, box, 3, gCvars.esp_box_mode);
		DrawDropdown("Health", 3, iWidth - 160, 58, add, health, 3, gCvars.esp_health_mode);
	}
	DrawButton("Debug", 3, 8, y += add, gCvars.esp_debug);
	DrawButton("Thirdperson", 3, 8, y += add, gCvars.misc_thirdperson);
	if (GAME_TF2)
	{
		DrawButton("No Hats", 3, 8, y += add, gCvars.misc_nohats);
		DrawButton("No Scope", 3, 8, y += add, gCvars.misc_noscope);
	}
	DrawButton("No Zoom", 3, 8, y += add, gCvars.misc_nozoom);
	DrawButton("FOV Override", 3, 8, y += add, gCvars.misc_fovoverride);
	if (gCvars.misc_fovoverride)
		DrawSlider("", 3, 12, y += add, 180, 250, gCvars.misc_fov_value);
	DrawButton("Viewmodel FOV Override", 3, 8, y += add, gCvars.misc_viewmodelfovoverride);
	if (gCvars.misc_viewmodelfovoverride)
		DrawSlider("", 3, 12, y += add, 180, 250, gCvars.misc_viewmodelfov_value);


	DrawSlider("RED Red", 3, 8, y += add, 180, 255, gCvars.color_r_red);
	DrawSlider("RED Green", 3, 8, y += add, 180, 255, gCvars.color_g_red);
	DrawSlider("RED Blue", 3, 8, y += add, 180, 255, gCvars.color_b_red);

	DrawSlider("BLU Red", 3, 8, y += add, 180, 255, gCvars.color_r_blu);
	DrawSlider("BLU Green", 3, 8, y += add, 180, 255, gCvars.color_g_blu);
	DrawSlider("BLU Blue", 3, 8, y += add, 180, 255, gCvars.color_b_blu);

	DrawSlider("Other Red", 3, 8, y += add, 180, 255, gCvars.color_r_other);
	DrawSlider("Other Green", 3, 8, y += add, 180, 255, gCvars.color_g_other);
	DrawSlider("Other Blue", 3, 8, y += add, 180, 255, gCvars.color_b_other);
	ResetColors("Reset Colors", 3, 8, y += add);

	//
	// Misc
	//
	y = 58;
	DrawButton("Bunnyhop", 4, 8, y, gCvars.misc_bunnyhop);
	DrawButton("Auto Strafe", 4, 8, y += add, gCvars.misc_autostrafe);
	DrawButton("Time Shift", 4, 8, y += add, gCvars.timeshift_enabled);
		if (gCvars.timeshift_enabled)
			DrawSlider("", 4, 12, y += add, 180, 1000, gCvars.timeshift_value);
	DrawDropdown("TS Key", 4, iWidth - 160, 58, add, key2, 6, gCvars.timeshift_key);
	DrawButton("Server Lagger", 4, 8, y += add, gCvars.misc_serverlag);
		if (gCvars.misc_serverlag)
			DrawSlider("", 4, 12, y += add, 180, 500, gCvars.misc_serverlag_value);
	DrawDropdown("SL Key", 4, iWidth - 58, 58, add, key2, 6, gCvars.misc_serverlag_key);
	DrawButton("Anti-Aim", 4, 8, y += add, gCvars.misc_angles);
	DrawDropdown("Pitch", 4, iWidth - 160, 270, add, aax, 4, gCvars.misc_aax);
	DrawDropdown("Yaw", 4, iWidth - 58, 270, add, aay, 5, gCvars.misc_aay);
	DrawButton("Bypass Pure", 4, 8, y += add, gCvars.misc_purebypass);
	DrawButton("Announcer", 4, 8, y += add, gCvars.misc_announcer);
	DrawButton("Backtrack", 4, 8, y += add, gCvars.misc_backtracking);
	if (gCvars.misc_backtracking)
		DrawButton("Backtrack Teammates", 4, 12, y += add, gCvars.misc_backtracking_deathmatch);
	DrawButton("Fastcrouch", 4, 8, y += add, gCvars.misc_fastcrouch);
	DrawButton("Fakelag", 4, 8, y += add, gCvars.misc_fakelag);
	if (gCvars.misc_fakelag)
		DrawSlider("", 4, 12, y += add, 180, 15, gCvars.misc_fakelag_value);

	SaveButton("Save Configuration", 4, 20, y += 300);
	LoadButton("Load Configuration", 4, 20, y += 25);
}


