/*
	8 January 2024 - Phillip gatlin aka The White Lion
	This file defines the mana class and the types of mana.
*/

#pragma once
#include "manatypes.h"
#include "effects.h" // for some reason the custom code doesn't compile without this header file: wtf - PG 8 January 2024

class CItemMana : public CBaseEntity
{
public:
	void Touch(CBaseEntity* pOther) override;
	void Spawn() override;
	void Precache() override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
	bool KeyValue(KeyValueData* pkvd) override;
	void Spawner();
	void SetType(t_Mana manatype); // this function makes the entity set its own mana type
	t_Mana m_eManaType;
private:
	CSprite* m_pManaSprite;
	bool SpawnerMode = false;
};