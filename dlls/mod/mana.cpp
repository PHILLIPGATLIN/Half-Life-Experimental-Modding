/*
	6 January 2024 - Phillip Gatlin aka The White Lion
	This file implements mana as pickup items for the player.
*/
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "effects.h"
#include "player.h"
#include "mod/mana.h"

LINK_ENTITY_TO_CLASS(item_mana, CItemMana);

bool CItemMana::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "manatype"))
	{
		m_eManaType = (t_Mana)atoi(pkvd->szValue);
		pkvd->fHandled = 1;
		return CBaseEntity::KeyValue(pkvd);
	}
	else if (FStrEq(pkvd->szKeyName, "mode"))
	{
		if (atoi(pkvd->szValue) == 1)
			SpawnerMode = true;
		else
			SpawnerMode = false;
		
		return CBaseEntity::KeyValue(pkvd);
	}
	else
	{
		return   CBaseEntity::KeyValue(pkvd);
	}

}

void CItemMana::Touch(CBaseEntity* pOther)
{
	// if it's not a player, ignore
	if (!pOther->IsPlayer())
	{
		return;
	}

	CBasePlayer* pPlayer = (CBasePlayer*)pOther;

	EMIT_SOUND(this->edict(), CHAN_ITEM, "fvox/blip.wav", 1, ATTN_NORM);
	pPlayer->AddPlayerMana(this);
	m_pManaSprite->TurnOff();
	UTIL_Remove(this);

	return;
}

void CItemMana::Spawn()
{
	Precache(); // this needs to be the first thing we do because non-precached resources will cause a terminating server error and disconnect the client
	if (SpawnerMode == true)
	{
		UTIL_SetOrigin(pev, pev->origin);
		pev->movetype = MOVETYPE_NONE;
		pev->solid = SOLID_NOT;
		SetTouch(NULL);
		return;
	}

	m_pManaSprite = CSprite::SpriteCreate("sprites/flare6.spr", pev->origin, true);

	// Admer456 has informed me these statements are not necessary because the Touch and Think functions
	// is already override in the CItemMana class declaration.
	//SetTouch(&CItemMana::Touch);
	//SetThink(&CItemMana::Think);
	
	UTIL_SetOrigin(pev, pev->origin);
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 16));
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_TRIGGER;

	m_pManaSprite->pev->rendermode = kRenderTransAdd;
	m_pManaSprite->pev->renderfx = kRenderFxNone;
	//pev->effects ^= EF_BRIGHTLIGHT; // use XOR for effects
	m_pManaSprite->SetBrightness(255);
	m_pManaSprite->pev->framerate = 8.0f;
	m_pManaSprite->SetScale(1.0f);

	if (m_eManaType == e_Mana_Red)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 255, 0, 0, 255, m_pManaSprite->pev->renderfx);
	else if (m_eManaType == e_Mana_Orange)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 255, 128, 0, 255, m_pManaSprite->pev->renderfx);
	else if (m_eManaType == e_Mana_Yellow)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 255, 255, 0, 255, m_pManaSprite->pev->renderfx);
	else if (m_eManaType == e_Mana_Green)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 0, 255, 0, 255, m_pManaSprite->pev->renderfx);
	else if (m_eManaType == e_Mana_Blue)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 0, 128, 255, 255, m_pManaSprite->pev->renderfx);
	else
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 255, 255, 255, 255, m_pManaSprite->pev->renderfx); // wrong color
	
	m_pManaSprite->TurnOn();

	return;
}

void CItemMana::Spawner()
{
	CItemMana* to_mana = (CItemMana*)CBaseEntity::Create("item_mana", this->pev->origin,this->pev->angles);
	to_mana->SetType(m_eManaType);
	return;
}

void CItemMana::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	this->Spawner();

	return;
}

void CItemMana::SetType(t_Mana manatype)
{
	m_eManaType = manatype;

	if (m_eManaType == e_Mana_Red)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 255, 0, 0, 255, m_pManaSprite->pev->renderfx);
	else if (m_eManaType == e_Mana_Orange)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 255, 128, 0, 255, m_pManaSprite->pev->renderfx);
	else if (m_eManaType == e_Mana_Yellow)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 255, 255, 0, 255, m_pManaSprite->pev->renderfx);
	else if (m_eManaType == e_Mana_Green)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 0, 255, 0, 255, m_pManaSprite->pev->renderfx);
	else if (m_eManaType == e_Mana_Blue)
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 0, 128, 255, 255, m_pManaSprite->pev->renderfx);
	else
		m_pManaSprite->SetTransparency(m_pManaSprite->pev->rendermode, 255, 255, 255, 255, m_pManaSprite->pev->renderfx); // wrong color

	return;
}

void CItemMana::Precache()
{
	PRECACHE_MODEL("sprites/flare6.spr");
	PRECACHE_SOUND("fvox/blip.wav");
}
