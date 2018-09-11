#include "SDK.h"
#include "CGlobalVars.hpp"
#include <Windows.h>
#include "CFileManager.h"

CGlobalVariables gCvars;

void CGlobalVariables::Save(void)
{
	//
	// Save Aimbot
	//
	if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2 || GAME_HLS || GAME_SYN)
	{
		gFileManager.WriteFloat("Aimbot", "Enabled", aimbot_active);
		gFileManager.WriteFloat("Aimbot", "FOV", aimbot_fov);
		gFileManager.WriteFloat("Aimbot", "Autoshoot", aimbot_autoshoot);
	}
	else
	{
		gFileManager.WriteFloat("Aimbot", "Enabled", aimbot_active);
		gFileManager.WriteFloat("Aimbot", "Key", aimbot_key_enabled);
		gFileManager.WriteFloat("Aimbot", "Hitscan", aimbot_hitscan);
		gFileManager.WriteFloat("Aimbot", "FOV", aimbot_fov);
		gFileManager.WriteFloat("Aimbot", "Smooth", aimbot_smooth);
		gFileManager.WriteFloat("Aimbot", "Smooth Amt", aimbot_smooth_amt);
		gFileManager.WriteFloat("Aimbot", "Method", aimbot_mode);
		if (GAME_TF2)
		{
			gFileManager.WriteFloat("Aimbot", "Projectile Aim", aimbot_projectile);
			gFileManager.WriteFloat("Aimbot", "Zoomed Only", aimbot_zoomedonly);
			gFileManager.WriteFloat("Aimbot", "Wait For Charge", aimbot_waitforcharge);
		}
		gFileManager.WriteFloat("Aimbot", "Resolver", aimbot_resolver);
		gFileManager.WriteFloat("Aimbot", "Autoshoot", aimbot_autoshoot);
		gFileManager.WriteFloat("Aimbot", "Aim at Teammates", aimbot_deathmatch);
		gFileManager.WriteFloat("Aimbot", "Aim at Backtrack", aimbot_aimatbacktrack);
		gFileManager.WriteFloat("Aimbot", "Ignore Health > 100", aimbot_ignoreabove100);
		gFileManager.WriteFloat("Aimbot", "Ignore Cloaked", aimbot_ignorecloaked);
	}

	//
	// Save Triggerbot
	//
	if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2 || GAME_HLS || GAME_SYN)
	{
		//planning on adding Triggerbot support soon
	}
	else
	{
		gFileManager.WriteFloat("Triggerbot", "Hitscan", triggerbot_active);
		gFileManager.WriteFloat("Triggerbot", "Head Only", triggerbot_headonly);
		gFileManager.WriteFloat("Triggerbot", "Ignore Cloaked", triggerbot_ignorecloaked);
		gFileManager.WriteFloat("Triggerbot", "Auto Airblast", autoairblast_enabled);
		gFileManager.WriteFloat("Triggerbot", "Rage Airblast", autoairblast_rage);
		gFileManager.WriteFloat("Triggerbot", "Silent Airblast", autoairblast_silent);
	}
	
	//
	// Save Visuals
	//
	if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2 || GAME_HLS || GAME_SYN)
	{
		gFileManager.WriteFloat("ESP", "Enabled", esp_active);
		gFileManager.WriteFloat("ESP", "Name", esp_name);
	}
	else
	{
		gFileManager.WriteFloat("ESP", "Enabled", esp_active);
		gFileManager.WriteFloat("ESP", "Enemies Only", esp_enemyonly);
		gFileManager.WriteFloat("ESP", "Box", esp_box);
		gFileManager.WriteFloat("ESP", "Box Modes", esp_box_mode);
		gFileManager.WriteFloat("ESP", "Name", esp_name);
		if (GAME_TF2)
		{
			gFileManager.WriteFloat("ESP", "Class", esp_class);
			gFileManager.WriteFloat("ESP", "Glow", esp_glow);
			gFileManager.WriteFloat("ESP", "Chams", esp_chams);
			gFileManager.WriteFloat("ESP", "Chams Mode", esp_chams_mode);
		}
		gFileManager.WriteFloat("ESP", "Health", esp_health);
		gFileManager.WriteFloat("ESP", "Health Modes", esp_health_mode);
		gFileManager.WriteFloat("ESP", "Position", esp_pos);
		gFileManager.WriteFloat("ESP", "T1 Red", color_r_red);
		gFileManager.WriteFloat("ESP", "T1 Blue", color_g_red);
		gFileManager.WriteFloat("ESP", "T1 Green", color_b_red);
		gFileManager.WriteFloat("ESP", "T2 Red", color_r_blu);
		gFileManager.WriteFloat("ESP", "T2 Blue", color_g_blu);
		gFileManager.WriteFloat("ESP", "T2 Green", color_b_blu);
		gFileManager.WriteFloat("ESP", "TO Red", color_r_other);
		gFileManager.WriteFloat("ESP", "TO Blue", color_g_other);
		gFileManager.WriteFloat("ESP", "TO Green", color_b_other);
	}

	//
	// Save Time Shift
	//
	gFileManager.WriteFloat("Time Shift", "Enabled", timeshift_enabled);
		gFileManager.WriteFloat("Time Shift", "Value", timeshift_value);

	//
	// Save Misc
	//
	gFileManager.WriteFloat("Misc", "Bunnyhop", misc_bunnyhop);
	gFileManager.WriteFloat("Misc", "Autostrafe", misc_autostrafe);
	if (GAME_TF2)
	{
		gFileManager.WriteFloat("Misc", "Hands", gCvars.misc_hands);
		gFileManager.WriteFloat("Misc", "Hands Mode", gCvars.misc_hands_mode);
		gFileManager.WriteFloat("Misc", "No Hats", gCvars.misc_nohats);
		gFileManager.WriteFloat("Misc", "Crouch Exploit", gCvars.misc_fastcrouch);
		gFileManager.WriteFloat("Misc", "No Scope", gCvars.misc_noscope);
		gFileManager.WriteFloat("Misc", "Disable Crosshair", gCvars.misc_noscope_disablecrosshair);
	}
	gFileManager.WriteFloat("Misc", "Announcer", misc_announcer);
	gFileManager.WriteFloat("Misc", "Server Lagger", misc_serverlag);
	gFileManager.WriteFloat("Misc", "Server Lagger Value", misc_serverlag_value);
	gFileManager.WriteFloat("Misc", "No Zoom", misc_nozoom);
	gFileManager.WriteFloat("Misc", "FOV Override", misc_fov_value);
	gFileManager.WriteFloat("Misc", "Viewmodel FOV Override", misc_viewmodelfov_value);
	if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2 || GAME_HLS || GAME_SYN)
	{
		//maybe smth here eventually
	}
	else
	{
		gFileManager.WriteFloat("Misc", "Pure Bypass", misc_purebypass);
		gFileManager.WriteFloat("Misc", "Thirdperson", misc_thirdperson);
		gFileManager.WriteFloat("Misc", "Angles", misc_angles);
		gFileManager.WriteFloat("Misc", "Pitch Angles", misc_aax);
		gFileManager.WriteFloat("Misc", "Yaw Angles", misc_aay);
		gFileManager.WriteFloat("Misc", "Backtracking", misc_backtracking);
		gFileManager.WriteFloat("Misc", "Backtrack Teammates", misc_backtracking_deathmatch);
	}

	gFileManager.WriteFloat("Settings", "Position X", iMenu_Pos_X);
	gFileManager.WriteFloat("Settings", "Position Y", iMenu_Pos_Y);
	gFileManager.WriteFloat("Settings", "Color Scheme", iMenu_Color_Scheme);
	gFileManager.WriteFloat("Settings", "Font", iMenu_Font);

	//
	// Save Hotkeys
	//
	gFileManager.WriteFloat("Hotkeys", "Aimbot", aimbot_key);
	gFileManager.WriteFloat("Hotkeys", "Triggerbot", triggerbot_key);
	gFileManager.WriteFloat("Hotkeys", "Time Shift", timeshift_key);
	gFileManager.WriteFloat("Hotkeys", "Server Lagger", misc_serverlag_key);
}
//===================================================================================
void CGlobalVariables::Load(void)
{
	//
	// Load Aimbot
	//
	if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2 || GAME_HLS || GAME_SYN)
	{
		aimbot_active = gFileManager.ReadFloat("Aimbot", "Enabled", aimbot_active);
		aimbot_fov = gFileManager.ReadFloat("Aimbot", "FOV", aimbot_fov);
		aimbot_autoshoot = gFileManager.ReadFloat("Aimbot", "Autoshoot", aimbot_autoshoot);
	}
	else
	{
		aimbot_active = gFileManager.ReadFloat("Aimbot", "Enabled", aimbot_active);
		aimbot_key_enabled = gFileManager.ReadFloat("Aimbot", "Key", aimbot_key_enabled);
		aimbot_hitscan = gFileManager.ReadFloat("Aimbot", "Hitscan", aimbot_hitscan);
		aimbot_fov = gFileManager.ReadFloat("Aimbot", "FOV", aimbot_fov);
		aimbot_smooth = gFileManager.ReadFloat("Aimbot", "Smooth", aimbot_smooth);
		aimbot_smooth_amt = gFileManager.ReadFloat("Aimbot", "Smooth Amt", aimbot_smooth_amt);
		aimbot_mode = gFileManager.ReadFloat("Aimbot", "Method", aimbot_mode);
		if (GAME_TF2)
		{
			aimbot_projectile = gFileManager.ReadFloat("Aimbot", "Projectile Aim", aimbot_projectile);
			aimbot_zoomedonly = gFileManager.ReadFloat("Aimbot", "Zoomed Only", aimbot_zoomedonly);
			aimbot_waitforcharge = gFileManager.ReadFloat("Aimbot", "Wait For Charge", aimbot_waitforcharge);
		}
		aimbot_deathmatch = gFileManager.ReadFloat("Aimbot", "deathmatch", aimbot_deathmatch);
		aimbot_resolver = gFileManager.ReadFloat("Aimbot", "Resolver", aimbot_resolver);
		aimbot_autoshoot = gFileManager.ReadFloat("Aimbot", "Autoshoot", aimbot_autoshoot);
		aimbot_deathmatch = gFileManager.ReadFloat("Aimbot", "Aim at Backtrack", aimbot_deathmatch);
		aimbot_aimatbacktrack = gFileManager.ReadFloat("Aimbot", "Aim at Backtrack", aimbot_aimatbacktrack);
		aimbot_ignorecloaked = gFileManager.ReadFloat("Aimbot", "Ignore Cloaked", aimbot_ignorecloaked);
	}

	//
	// Load Triggerbot
	//
	if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2 || GAME_HLS || GAME_SYN)
	{
		//planning on adding Triggerbot support soon
	}
	else
	{
		triggerbot_active = gFileManager.ReadFloat("Triggerbot", "Hitscan", triggerbot_active);
		triggerbot_headonly = gFileManager.ReadFloat("Triggerbot", "Head Only", triggerbot_headonly);
		autoairblast_enabled = gFileManager.ReadFloat("Triggerbot", "Auto Airblast", autoairblast_enabled);
		autoairblast_rage = gFileManager.ReadFloat("Triggerbot", "Rage Airblast", autoairblast_rage);
		autoairblast_silent = gFileManager.ReadFloat("Triggerbot", "Silent Airblast", autoairblast_silent);
	}



	//
	// Load Visuals
	//
	if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2 || GAME_HLS || GAME_SYN)
	{
		esp_active = gFileManager.ReadFloat("ESP", "Enabled", esp_active);
		esp_name = gFileManager.ReadFloat("ESP", "Name", esp_name);
	}
	else
	{
		esp_active = gFileManager.ReadFloat("ESP", "Enabled", esp_active);
		esp_enemyonly = gFileManager.ReadFloat("ESP", "Enemies Only", esp_enemyonly);
		esp_box = gFileManager.ReadFloat("ESP", "Box", esp_box);
		esp_box_mode = gFileManager.ReadFloat("ESP", "Box Modes", esp_box_mode);
		esp_name = gFileManager.ReadFloat("ESP", "Name", esp_name);
		if (GAME_TF2)
		{
			esp_class = gFileManager.ReadFloat("ESP", "Class", esp_class);
			esp_glow = gFileManager.ReadFloat("ESP", "Glow", esp_glow);
			esp_chams = gFileManager.ReadFloat("ESP", "Chams", esp_chams);
			esp_chams_mode = gFileManager.ReadFloat("ESP", "Chams Mode", esp_chams_mode);
		}
		esp_health = gFileManager.ReadFloat("ESP", "Health", esp_health);
		esp_health_mode = gFileManager.ReadFloat("ESP", "Health Modes", esp_health_mode);
		esp_pos = gFileManager.ReadFloat("ESP", "Position", esp_pos);
		color_r_red = gFileManager.ReadFloat("ESP", "T1 Red", color_r_red);
		color_g_red = gFileManager.ReadFloat("ESP", "T1 Blue", color_g_red);
		color_b_red = gFileManager.ReadFloat("ESP", "T1 Green", color_b_red);
		color_r_blu = gFileManager.ReadFloat("ESP", "T2 Red", color_r_blu);
		color_g_blu = gFileManager.ReadFloat("ESP", "T2 Blue", color_g_blu);
		color_b_blu = gFileManager.ReadFloat("ESP", "T2 Green", color_b_blu);
		color_r_other = gFileManager.ReadFloat("ESP", "TO Red", color_r_other);
		color_g_other = gFileManager.ReadFloat("ESP", "TO Blue", color_g_other);
		color_b_other = gFileManager.ReadFloat("ESP", "TO Green", color_b_other);
	}

	//
	// Load Time Shift
	//
	timeshift_enabled = gFileManager.ReadFloat("Time Shift", "Enabled", timeshift_enabled);
	timeshift_value = gFileManager.ReadFloat("Time Shift", "Value", timeshift_value);

	//
	// Load Misc
	//
	misc_bunnyhop = gFileManager.ReadFloat("Misc", "Bunnyhop", misc_bunnyhop);
	misc_autostrafe = gFileManager.ReadFloat("Misc", "Autostrafe", misc_autostrafe);
	if (GAME_TF2)
	{
		misc_hands = gFileManager.ReadFloat("Misc", "Hands", gCvars.misc_hands);
		misc_hands_mode = gFileManager.ReadFloat("Misc", "Hands Mode", gCvars.misc_hands_mode);
		misc_nohats = gFileManager.ReadFloat("Misc", "No Hats", gCvars.misc_nohats);
		misc_fastcrouch = gFileManager.ReadFloat("Misc", "Crouch Exploit", gCvars.misc_fastcrouch);
		misc_noscope = gFileManager.ReadFloat("Misc", "No Scope", gCvars.misc_noscope);
		misc_noscope_disablecrosshair = gFileManager.ReadFloat("Misc", "Disable Crosshair", gCvars.misc_noscope_disablecrosshair);
	}
	misc_announcer = gFileManager.ReadFloat("Misc", "Announcer", misc_announcer);
	misc_serverlag = gFileManager.ReadFloat("Misc", "Server Lagger", misc_serverlag);
	misc_serverlag_value = gFileManager.ReadFloat("Misc", "Server Lagger Value", misc_serverlag_value);
	misc_nozoom = gFileManager.ReadFloat("Misc", "No Zoom", misc_nozoom);
	misc_fov_value = gFileManager.ReadFloat("Misc", "FOV Override", misc_fov_value);
	misc_viewmodelfov_value = gFileManager.ReadFloat("Misc", "Viewmodel FOV Override", misc_viewmodelfov_value);
	if (GAME_HL2 || GAME_HL2_LC || GAME_HL2_EP1 || GAME_HL2_EP2 || GAME_HLS || GAME_SYN)
	{
		//maybe something here eventually
	}
	else
	{
		misc_purebypass = gFileManager.ReadFloat("Misc", "Pure Bypass", misc_purebypass);
		misc_thirdperson = gFileManager.ReadFloat("Misc", "Thirdperson", misc_thirdperson);
		misc_angles = gFileManager.ReadFloat("Misc", "Angles", misc_angles);
		misc_aax = gFileManager.ReadFloat("Misc", "Pitch Angles", misc_aax);
		misc_aay = gFileManager.ReadFloat("Misc", "Yaw Angles", misc_aay);
		misc_backtracking = gFileManager.ReadFloat("Misc", "Backtracking", misc_backtracking);
		misc_backtracking_deathmatch = gFileManager.ReadFloat("Misc", "Backtrack Teammates", misc_backtracking_deathmatch);
	}

	iMenu_Pos_X = gFileManager.ReadFloat("Settings", "Position X", iMenu_Pos_X);
	iMenu_Pos_Y = gFileManager.ReadFloat("Settings", "Position Y", iMenu_Pos_Y);
	iMenu_Color_Scheme = gFileManager.ReadFloat("Settings", "Color Scheme", iMenu_Color_Scheme);
	iMenu_Font = gFileManager.ReadFloat("Settings", "Font", iMenu_Font);

	//
	// Load Hotkeys
	//
	aimbot_key = gFileManager.ReadFloat("Hotkeys", "Aimbot", aimbot_key);
	triggerbot_key = gFileManager.ReadFloat("Hotkeys", "Triggerbot", triggerbot_key);
	timeshift_key = gFileManager.ReadFloat("Hotkeys", "Time Shift", timeshift_key);
	misc_serverlag_key = gFileManager.ReadFloat("Hotkeys", "Server Lagger", misc_serverlag_key);
}

// 
// Clear settings
//
void CGlobalVariables::Clear(void)
{
	gCvars.aimbot_switch = false;
	gCvars.aimbot_active = false;
	gCvars.aimbot_key_enabled = false;
	gCvars.aimbot_key = false;
	gCvars.aimbot_hitscan = false;
	gCvars.aimbot_fov = false;
	gCvars.aimbot_smooth = false;
	gCvars.aimbot_smooth_amt = false;
	gCvars.aimbot_mode = false;
	gCvars.aimbot_autoshoot = false;
	gCvars.aimbot_waitforcharge = false;
	gCvars.aimbot_zoomedonly = false;
	gCvars.aimbot_ignorecloaked = false;
	gCvars.aimbot_ignoreabove100 = false;
	gCvars.aimbot_resolver = false;
	gCvars.aimbot_projectile = false;
	gCvars.aimbot_aimatbacktrack = false;

	gCvars.triggerbot_switch = false;
	gCvars.triggerbot_active = false;
	gCvars.triggerbot_key = false;
	gCvars.triggerbot_headonly = false;
	gCvars.triggerbot_ignorecloaked = false;
	gCvars.autoairblast_enabled = false;
	gCvars.autoairblast_rage = false;
	gCvars.autoairblast_silent = false;
	gCvars.triggerbot_autobackstab = false;

	gCvars.esp_switch = false;
	gCvars.esp_active = false;
	gCvars.esp_pos = false;
	gCvars.esp_enemyonly = false;
	gCvars.esp_box = false;
	gCvars.esp_objects = false;
	gCvars.esp_box_mode = false;
	gCvars.esp_name = false;
	gCvars.esp_class = false;
	gCvars.esp_bones = false;
	gCvars.esp_health = false;
	gCvars.esp_health_mode = false;
	gCvars.esp_glow = false;
	gCvars.esp_dontdrawteammates = false;
	gCvars.esp_debug = false;
	gCvars.esp_chams = false;
	gCvars.esp_chams_mode  = false;

	gCvars.misc_switch = false;
	gCvars.misc_bunnyhop = false;
	gCvars.misc_autostrafe = false;
	gCvars.misc_angles = false;
	gCvars.misc_aax = false;
	gCvars.misc_aay = false;
	gCvars.misc_angles2 = false;
	gCvars.misc_aaxa = false;
	gCvars.misc_aaya = false;
	gCvars.misc_thirdperson = false;
	gCvars.misc_purebypass = false;
	gCvars.misc_nozoom = false;
	gCvars.misc_noscope = false;
	gCvars.misc_noscope_disablecrosshair = false;
	gCvars.misc_serverlag = false; 
	gCvars.misc_serverlag_key = false; 
	gCvars.misc_serverlag_value = false; 
	gCvars.misc_nohats = false;
	gCvars.misc_hands = false;
	gCvars.misc_hands_mode = false;
	gCvars.misc_weapons = false;
	gCvars.misc_weapons_mode = false;
	gCvars.misc_backtracking = false;
	gCvars.misc_fastcrouch = false;
	gCvars.misc_zangle = false;
	gCvars.misc_fovoverride = false; 
	gCvars.misc_fov_value = false; 
	gCvars.misc_viewmodelfovoverride = false; 
	gCvars.misc_viewmodelfov_value = false; 
	gCvars.misc_tauntslide = false; 
	gCvars.misc_fakelag = false; 
	gCvars.misc_fakelag_value = false; 
	gCvars.misc_announcer = false; 
	gCvars.misc_configselection = false; 
	gCvars.misc_novisrecoil = false; 
}