/*
	Reading Text -- a game entity that shows an interactive menu containing text to the player for reading. PG 18 January 2024
*/
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "UserMessages.h"
#include "player.h"

class CReadingText : public CBaseEntity
{
public:
	void Spawn() override;
	void Precache() override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
	void Touch(CBaseEntity* pOther) override;
	bool KeyValue(KeyValueData* pkvd) override;

private:
	char  m_aTextFile[64];
};

LINK_ENTITY_TO_CLASS(reading_text, CReadingText);

/*
	This implementation works in singleplayer only because the design assumes there is only one client
	20 January 2024, PG: this implementation DOESN'T work...
*/
void CReadingText::Spawn()
{
	Precache();

	return;
}

void CReadingText::Precache()
{	
	// sending a client message during Precache causes the server to crash, don't do it. - PG

	/*
	ALERT(at_console, "CReadingText::Precache(), m_aTextFile[] is %s", &m_aTextFile[0]);
	if (m_aTextFile[0] == 0)
		return;

	MESSAGE_BEGIN(MSG_ONE, gmsgCacheText, NULL, this->edict());
		WRITE_STRING(&m_aTextFile[0]);
	MESSAGE_END();
	return;
	*/
}

void CReadingText::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	char TitleName[64];

	ALERT(at_console, "CReadingText::Use(), m_aTextFile[] is %s", &m_aTextFile[0]);
	// the server will tell the client what text file to show them, the text file has already been cached by the client HUD system

	if (!pActivator->IsPlayer())
		return;

	if (strcmp(m_aTextFile, "\0") == 0)
		return;
	strncpy(TitleName, m_aTextFile, (size_t)64);

	// THIS WORKS! for some reason the server message needs to be transmitted from the player entity and not the CReadingText entity...I dunno, just the way goldsrc functions - PG
	CBasePlayer* player = (CBasePlayer*)pActivator;
	player->SendTitleName(&TitleName[0]);

	return;
}

void CReadingText::Touch(CBaseEntity* pOther)
{

	return;
}

bool CReadingText::KeyValue(KeyValueData* pkvd)
{
	// evaluate the filepath
	if (FStrEq(pkvd->szKeyName, "textfile"))
	{
		strncpy(m_aTextFile, pkvd->szValue, (size_t)64);
		pkvd->fHandled = 1;
		ALERT(at_console, "pkvd->szValue is %s", pkvd->szValue);
		return CBaseEntity::KeyValue(pkvd);
	}
	else
		return CBaseEntity::KeyValue(pkvd);

	return false;
}