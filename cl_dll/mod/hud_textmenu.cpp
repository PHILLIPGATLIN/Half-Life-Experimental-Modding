/*
	21 January 2024 by Phillip Gatlin aka The White Lion
	this file implements a new HUD element intended to open a VGUI text menu for the player
*/

#include "stdio.h"
#include "stdlib.h"
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>
#include "vgui_TeamFortressViewport.h"
#include "filesystem_utils.h"

DECLARE_MESSAGE(m_TextMenu, ShowText);

extern TeamFortressViewport* gViewPort;

bool CHudTextMenu::Init()
{
	// we're going to cache some data here
	// I am making the client cache this data because I don't want to transmit the contents of these files via client messages from the server since they could
	// be quite large in size, so I think this implementation is more efficient than sending client messages.
	// the HUD is doing it because the HUD needs this text ready, stored inside a HUD-local data structure, to show to the player when it receives a ShowText message.
	// I am doing some preloading before that kind of event happens to prevent a pause in the flow of gameplay by loading a file extemporaneously when it receives
	// a ShowText message.
	// -- PG
	FileFindHandle_t handle = FILESYSTEM_INVALID_FIND_HANDLE;
	std::vector<std::string> Texts;

	HOOK_MESSAGE(ShowText);

	m_iFlags |= HUD_ACTIVE;

	gHUD.AddHudElem(this);

	// build a list of text file names that need to be cached

	const char* fileName = g_pFileSystem->FindFirst("text/*.txt",&handle); // FindFirst starts in the game directory which is Half-Life/halflife_updated in this case

	if (fileName != nullptr)
	{
		do
		{
			std::string textName = fileName;
			textName.resize(textName.size() - 4);	// truncate textName to get rid of the file extension ".txt"

			// this code is compressed which makes it a little cryptic, which is why I dislike C++, but 
			// basically it means find textName inside Texts, if it's there then return else put textName into Texts
			if (std::find_if(Texts.begin(), Texts.end(), [=](const auto& candidate)
					{ return 0 == stricmp(candidate.c_str(), textName.c_str()); }) == Texts.end())
			{
				Texts.push_back(std::move(textName));
			}
		} while ((fileName = g_pFileSystem->FindNext(handle)) != nullptr);


	}

	// Now we are going to process std::vector Texts and load each file into memory and store a pointer to the text
	for (std::string textName : Texts)
	{
		std::string file = "text/" + textName + ".txt";

		char* LoadedFile = (char*)gEngfuncs.COM_LoadFile(file.c_str(), 5, NULL); // I'm honestly not sure where COM_LoadFile is going to search for file...actually it searches inside Game Directory
		
		CachedTexts.insert({textName, LoadedFile});
	}

	Texts.clear();

	return true;
}

bool CHudTextMenu::VidInit()
{
	return true;
}

bool CHudTextMenu::Draw(float flTime)
{
	return true;
}

void CHudTextMenu::CopyTextTitle(char* pArray)
{
	strncpy(pArray, &TextTitle[0], 64);

	return;
}

// new 18 January 2024 -- PG
bool CHudTextMenu::MsgFunc_ShowText(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	strncpy(&TextTitle[0], READ_STRING(), (size_t)MAX_TEXTTITLE_LENGTH);
	gEngfuncs.Con_Printf("TeamFortressViewport::MsgFunc_ShowText, TextTitle is %s", &TextTitle[0]);
	
	if (gViewPort)
	{
		return gViewPort->MsgFunc_ShowText(pszName, iSize, pbuf);
	}

	return false;
}

char* CHudTextMenu::GetText(char* titleName)
{
	if (titleName == nullptr)
		return (char*)nullptr;

	std::string textTitle = std::string(titleName);

	auto mapElement = CachedTexts.find(textTitle);

	if (mapElement == CachedTexts.end())
		return titleName;

	return mapElement->second;
}