/* ScriptData
SDName: Item_Teleport
SD%Complete: 100
SDComment: Used for Teleport Item Scripts
SDCategory: Items
EndScriptData */

#include "ScriptPCH.h"
#include <cstring>



class item_teleport : public ItemScript
{
public:
	item_teleport() : ItemScript("item_teleport") { }

	bool OnUse(Player *player, Item* _Item, SpellCastTargets const& targets) override
	{
		// Declare variables
		float x_pos, y_pos, z_pos, o_pos;
		uint32 mapid;
		uint32 itemId = _Item->GetEntry();
		extern DatabaseType WorldDatabase;

		// Fetch the coordinates
		QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `mapid`, `X_pos`, `Y_pos`, `Z_pos`, `orientation`  FROM `item_teleports` WHERE `entry` = '%i' LIMIT 1", itemId);
		if (result)
		{
			Field *fields = result->Fetch();
			// Read coords from the query result
			mapid = fields[0].GetInt32();
			x_pos = fields[1].GetFloat();
			y_pos = fields[2].GetFloat();
			z_pos = fields[3].GetFloat();
			o_pos = fields[4].GetFloat();
			// Teleport the player and show info in log
			player->TeleportTo(mapid, x_pos, y_pos, z_pos, o_pos);
			outstring_log("Teleport Activated: %i (m:%i x:%f y:%f z:%f o:%f)", itemId, mapid, x_pos, y_pos, z_pos, o_pos);
			return true;
		}
		else {
			// Teleport failed - show info in log
			outstring_log("Teleport Failed: %i (m:%i x:%f y:%f z:%f o:%f)", itemId, mapid, x_pos, y_pos, z_pos, o_pos);
		}
	}
};

void AddSC_item_teleport()
{
    new item_teleport();
}
