#ifndef _CONTROL_VARIABLES_H_
#define _CONTROL_VARIABLES_H_
#include "SDK.h"

class IMaterial;
class CGlobalVariables
{
public:
    void Load(void);
	void Save(void);

	int iAimbotIndex = -1;

	//int key = NULL;
	
	float settings_switch;
	float iMenu_Pos_X = 300;
	float iMenu_Pos_Y = 75;
	float iMenu_Font;
	float iMenu_Color_Scheme;

	int color_r_red = 255;
	int color_g_red = 20;
	int color_b_red = 20;

	int color_r_blu = 0;
	int color_g_blu = 153;
	int color_b_blu = 255;

	int color_r_other = 0;
	int color_g_other = 0;
	int color_b_other = 255;
	int color_r_menu = 255;
	int color_g_menu = 255;
	int color_b_menu = 255;

	float hotkeys_switch;

	bool aimbot_switch;
	bool aimbot_active;
	bool aimbot_key_enabled;
	int aimbot_key = 1;
	bool aimbot_hitscan;
	int aimbot_fov;
	bool aimbot_smooth;
	int aimbot_smooth_amt;
	int aimbot_mode = 1; 
	bool aimbot_autoshoot;
	bool aimbot_waitforcharge;
	bool aimbot_deathmatch;
	bool aimbot_zoomedonly;
	bool aimbot_ignorecloaked;
	bool aimbot_ignoreabove100;
	bool aimbot_resolver;
	bool aimbot_projectile;
	bool aimbot_aimatbacktrack;
	int aimbot_priority = 1;

	bool triggerbot_switch;
	bool triggerbot_active;
	int triggerbot_key = 1;
	bool triggerbot_headonly;
	bool triggerbot_ignorecloaked;
	bool autoairblast_enabled;
	bool autoairblast_rage;
	bool autoairblast_silent;
	bool triggerbot_autobackstab;

	bool esp_switch;
	bool esp_active;
	int esp_pos;
	bool esp_enemyonly;
	bool esp_box;
	bool esp_objects;
	int esp_box_mode = 1;
	bool esp_name;
	bool esp_class;
	bool esp_bones;
	bool esp_health;
	int esp_health_mode = 1;
	bool esp_glow;
	bool esp_dontdrawteammates;
	bool esp_debug;
	bool esp_chams;
	int esp_chams_mode = 1;

	bool misc_switch;
	bool misc_bunnyhop;
	bool misc_autostrafe;
	bool misc_angles;
	int misc_aax;
	int misc_aay;
	float misc_angles2;
	float misc_aaxa;
	float misc_aaya;
	bool misc_thirdperson;
	bool misc_purebypass;
	bool misc_nozoom;
	bool misc_noscope;
	bool misc_noscope_disablecrosshair;
	bool misc_serverlag;
	int misc_serverlag_key = 1;
	int misc_serverlag_value = 100;
	bool misc_nohats;
	bool misc_hands;
	int misc_hands_mode;
	bool misc_weapons;
	int misc_weapons_mode;
	bool misc_backtracking;
	bool misc_backtracking_deathmatch;
	bool misc_fastcrouch;
	int misc_zangle;
	bool misc_fovoverride;
	int misc_fov_value = 75;
	bool misc_viewmodelfovoverride;
	int misc_viewmodelfov_value = 54;
	bool misc_tauntslide;
	bool misc_fakelag;
	int misc_fakelag_value = 1;
	bool misc_announcer;
	int misc_configselection = 1;
	bool misc_novisrecoil;

	bool timeshift_switch;
	bool timeshift_enabled;
	int timeshift_key = 1;
	int timeshift_value = 800;

	float PlayerMode[64] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};


	IMaterial* Vis_MatLit;
	IMaterial* Hid_MatLit;

	IMaterial* Vis_MatUnLit;
	IMaterial* Hid_MatUnLit;

	IMaterial* Vis_MatShine;
	IMaterial* Hid_MatShine;
};
extern CGlobalVariables gCvars;
//===================================================================================
#endif


