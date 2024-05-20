
//=========================================================
// mana_spawner creates a new mana entity in the game
// can be triggered by another source
//=========================================================
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "manatypes.h"

class CManaSpawner : public CBaseEntity
{
public:
	void Touch(CBaseEntity* pOther) override;
	void Spawn() override;
	bool KeyValue(KeyValueData* pkvd) override;
	t_Mana m_eManaType;
};