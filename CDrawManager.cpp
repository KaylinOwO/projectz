#include "CDrawManager.h"
//===================================================================================
CDrawManager gDrawManager;

#define ESP_HEIGHT 14
//===================================================================================
void CDrawManager::Initialize( )
{
	if ( gInts.Surface == NULL )
		return;

	gInts.Engine->GetScreenSize( gScreenSize.iScreenWidth, gScreenSize.iScreenHeight );
	m_Font = gInts.Surface->CreateFont( );	//This is better than Arial
	if (!gCvars.iMenu_Font)
		gInts.Surface->SetFontGlyphSet(m_Font, "Arial", ESP_HEIGHT, 500, 0, 0, 0x200);
	if (gCvars.iMenu_Font == 1)
		gInts.Surface->SetFontGlyphSet(m_Font, "Tahoma", ESP_HEIGHT, 500, 0, 0, 0x200);
	if (gCvars.iMenu_Font == 2)
		gInts.Surface->SetFontGlyphSet(m_Font, "Comic Sans MS", ESP_HEIGHT, 500, 0, 0, 0x200);

}
//===================================================================================

Color CDrawManager::GetPlayerColor(CBaseEntity* pPlayer)
{
	int redr = gCvars.color_r_red;
	int redg = gCvars.color_g_red;
	int redb = gCvars.color_b_red;

	int blur = gCvars.color_r_blu;
	int blug = gCvars.color_g_blu;
	int blub = gCvars.color_b_blu;

	int otherr = gCvars.color_r_other;
	int otherg = gCvars.color_g_other;
	int otherb = gCvars.color_b_other;

	if (!gCvars.PlayerMode[pPlayer->GetIndex()])
		return Color(0, 255, 255, 255);
	else if (gCvars.PlayerMode[pPlayer->GetIndex()] == 2)
		return Color::Yellow();
	else
	{

		switch (pPlayer->GetTeamNum())
		{
		case 2: //RED
			return Color(redr, redg, redb, 255);
		case 3: //BLU
			return Color(blur, blug, blub, 255);
		default:
			return Color(otherr, otherg, otherb, 255);
		}
	}

	return Color(0, 0, 0, 0); //no reason for this to be here, i just wanna look smart
}
//===================================================================================
void CDrawManager::DrawString( int x, int y, Color clrColor, const wchar_t *pszText)
{
	if( pszText == NULL )
		return;

	gInts.Surface->DrawSetTextPos( x, y );
	gInts.Surface->DrawSetTextFont( m_Font );
	gInts.Surface->DrawSetTextColor( clrColor.r(), clrColor.g(), clrColor.b(), clrColor.a() );
	gInts.Surface->DrawPrintText( pszText, wcslen( pszText ) );
}
//===================================================================================
void CDrawManager::DrawString( int x, int y, Color clrColor, const char *pszText, ... )
{
	if( pszText == NULL )
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start( va_alist, pszText );
	vsprintf_s( szBuffer, pszText, va_alist );
	va_end( va_alist );

	wsprintfW( szString, L"%S", szBuffer );

	gInts.Surface->DrawSetTextPos( x, y );
	gInts.Surface->DrawSetTextFont( m_Font );
	gInts.Surface->DrawSetTextColor( clrColor.r(), clrColor.g(), clrColor.b(), clrColor.a() );
	gInts.Surface->DrawPrintText( szString, wcslen( szString ) );
}

void CDrawManager::DrawStringA(int x, int y, Color color, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int Wid, Hit;
	
	gInts.Surface->GetTextSize(m_Font, wbuf, Wid, Hit);
	gInts.Surface->DrawSetTextFont(m_Font);
	gInts.Surface->DrawSetTextColor(color.r(), color.g(), color.b(), color.a());
	gInts.Surface->DrawSetTextPos(x - Wid / 2, y - Hit / 2);
	gInts.Surface->DrawPrintText(wbuf, wcslen(wbuf));
}


void CDrawManager::DrawStringB(int x, int y, Color color, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int Wid, Hit;

	gInts.Surface->GetTextSize(m_Font, wbuf, Wid, Hit);
	gInts.Surface->DrawSetTextFont(m_Font);
	gInts.Surface->DrawSetTextColor(color.r(), color.g(), color.b(), color.a());
	gInts.Surface->DrawSetTextPos(x, y - Hit / 2);
	gInts.Surface->DrawPrintText(wbuf, wcslen(wbuf));
}

void CDrawManager::DrawStringAG(int x, int y, Color color, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int Wid, Hit;

	gInts.SurfaceGmod->GetTextSize(m_Font, wbuf, Wid, Hit);
	gInts.Surface->DrawSetTextFont(m_Font);
	gInts.Surface->DrawSetTextColor(color.r(), color.g(), color.b(), color.a());
	gInts.Surface->DrawSetTextPos(x - Wid / 2, y - Hit / 2);
	gInts.Surface->DrawPrintText(wbuf, wcslen(wbuf));
}


void CDrawManager::DrawStringBG(int x, int y, Color color, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int Wid, Hit;

	gInts.SurfaceGmod->GetTextSize(m_Font, wbuf, Wid, Hit);
	gInts.Surface->DrawSetTextFont(m_Font);
	gInts.Surface->DrawSetTextColor(color.r(), color.g(), color.b(), color.a());
	gInts.Surface->DrawSetTextPos(x, y - Hit / 2);
	gInts.Surface->DrawPrintText(wbuf, wcslen(wbuf));
}

//===================================================================================
byte CDrawManager::GetESPHeight( )
{
	return ESP_HEIGHT;
}
//===================================================================================
void CDrawManager::DrawLine(int x, int y, int x1, int y1, Color clrColor)
{
	gInts.Surface->DrawSetColor(clrColor.r(), clrColor.g(), clrColor.b(), clrColor.a());
	gInts.Surface->DrawLine(x, y, x1, y1);
}
//===================================================================================
void CDrawManager::DrawLineEx(int x, int y, int w, int h, Color clrColor)
{
	gInts.Surface->DrawSetColor(clrColor.r(), clrColor.g(), clrColor.b(), clrColor.a());
	gInts.Surface->DrawLine(x, y, x + w, y + h);
}
//===================================================================================
void CDrawManager::DrawRect( int x, int y, int w, int h, Color clrColor)
{
	gInts.Surface->DrawSetColor( clrColor.r(), clrColor.g(), clrColor.b(), clrColor.a() );
	gInts.Surface->DrawFilledRect( x, y, x + w, y + h );
}
//===================================================================================
void CDrawManager::OutlineRect( int x, int y, int w, int h, Color clrColor)
{
	gInts.Surface->DrawSetColor( clrColor.r(), clrColor.g(), clrColor.b(), clrColor.a() );
	gInts.Surface->DrawOutlinedRect( x, y, x + w, y + h );
}
//===================================================================================
void CDrawManager::DrawBox( Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius )
{
	Vector vScreen;

	if( !WorldToScreen( vOrigin, vScreen ) )
		return;

	int radius2 = radius<<1;

	OutlineRect( vScreen.x - radius + box_width, vScreen.y - radius + box_width, radius2 - box_width, radius2 - box_width, Color::Black() );
	OutlineRect( vScreen.x - radius - 1, vScreen.y - radius - 1, radius2 + ( box_width + 2 ), radius2 + ( box_width + 2 ), Color::Black() );
	DrawRect( vScreen.x - radius + box_width, vScreen.y - radius, radius2 - box_width, box_width, Color( r, g, b, alpha ));
	DrawRect( vScreen.x - radius, vScreen.y + radius, radius2, box_width, Color( r, g, b, alpha ));
	DrawRect( vScreen.x - radius, vScreen.y - radius, box_width, radius2, Color( r, g, b, alpha ));
	DrawRect( vScreen.x + radius, vScreen.y - radius, box_width, radius2 + box_width, Color( r, g, b, alpha ) );
}
//===================================================================================
bool CDrawManager::WorldToScreen( Vector &vOrigin, Vector &vScreen )
{
	const matrix3x4& worldToScreen = gInts.Engine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	vScreen.z = 0; //Screen doesn't have a 3rd dimension.

	if( w > 0.001 ) //If the object is within view.
	{
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		vScreen.x = (gScreenSize.iScreenWidth / 2) + ( 0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * gScreenSize.iScreenWidth + 0.5); //Get the X dimension and push it in to the Vector.
		vScreen.y = (gScreenSize.iScreenHeight / 2) - ( 0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * gScreenSize.iScreenHeight + 0.5); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}