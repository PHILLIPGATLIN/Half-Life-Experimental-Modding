/*
	8 January 2024 by Phillip Gatlin aka The White Lion
	this file implements the CHudMana class so the player can see their mana on their HUD
*/
#include "stdio.h"
#include "stdlib.h"
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>

DECLARE_MESSAGE(m_Manas, ManaGet);

bool CHudMana::Init()
{
	/*
	 HOOK_MESSAGE is a macro for the engine to automatically call your
	 MsgFunc function when the corresponding message type is received
	 from the server, in this case it is the ManaGet message that is
	 registered as a message on the server side.
	*/
	HOOK_MESSAGE(ManaGet);

	m_iFlags |= HUD_ACTIVE; // like turning on a light switch, this HUD element turns on

	gHUD.AddHudElem(this);

	return true;
}

bool CHudMana::VidInit()
{
	return true;
}

bool CHudMana::Draw(float flTime)
{
	
	int ManaWidth, ManaHeight;
	int posx, posy;
	int r, g, b, a = 255;

	if ( (gHUD.m_iHideHUDDisplay & (HIDEHUD_ALL) ) != 0) return true;

	ManaWidth = gHUD.GetSpriteRect(gHUD.m_HUD_number_0).right - gHUD.GetSpriteRect(gHUD.m_HUD_number_0).left;
	posy = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;
	posx = (ScreenWidth * 3) / 8; // starting from 3/8 position of ScreenWidth, I hope this doesn't overlap with the armor indicator - PG
	
	gHUD.DrawHudNumber(posx, posy, DHN_2DIGITS | DHN_DRAWZERO, (int)ManaAmount_R, 255, 0, 0);
	gHUD.DrawHudNumber((posx + ((ManaWidth*2) * 1)) + 5, posy, DHN_2DIGITS | DHN_DRAWZERO, (int)ManaAmount_O, 255, 128, 0);
	gHUD.DrawHudNumber((posx + ((ManaWidth*2) * 2)) + 10, posy, DHN_2DIGITS | DHN_DRAWZERO, (int)ManaAmount_Y, 255, 255, 0);
	gHUD.DrawHudNumber((posx + ((ManaWidth*2) * 3)) + 15, posy, DHN_2DIGITS | DHN_DRAWZERO, (int)ManaAmount_G, 0, 255, 0);
	gHUD.DrawHudNumber((posx + ((ManaWidth*2) * 4)) + 20, posy, DHN_2DIGITS | DHN_DRAWZERO, (int)ManaAmount_B, 0, 128, 255);

	return true;
}

/*
	this function is important for custom HUD elements because this is the
	function that actually reads the data sent from the server to the client
	as a ManaGet type message, in this funcion is where you assign values
	to variables local to the client - PG
*/
bool CHudMana::MsgFunc_ManaGet(const char* pszName, int iSize, void* pbuf)
{
	// the data received from the server NEEDS to be read
	// in the order that it was sent!
	// in this case 1 byte size value first then 1 short sized value second
	// -PG
	BEGIN_READ(pbuf, iSize);
	ManaType = READ_BYTE();
	switch (ManaType)
	{
		case 0: // red
			ManaAmount_R = READ_SHORT();
			break;
		case 1: // orange
			ManaAmount_O = READ_SHORT();
			break;
		case 2: // yellow
			ManaAmount_Y = READ_SHORT();
			break;
		case 3: // green
			ManaAmount_G = READ_SHORT();
			break;
		case 4: // blue
			ManaAmount_B = READ_SHORT();
			break;
		default:
			return false;
			break;
	}

	return true;
}