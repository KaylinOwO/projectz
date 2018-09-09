#pragma once
#include "SDK.h"

typedef struct CScreenSize_t
{
	int iScreenHeight;
	int iScreenWidth;

} CScreenSize;

void __fastcall Hooked_PaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void Intro();
void Reinit();

extern CScreenSize gScreenSize;
