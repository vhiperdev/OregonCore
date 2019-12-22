//CastleDEV
#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"

#define ARMOR_PART 60013
#define WEAPON_PART 60014
#define SOULS_COPON 60028      //illusion dust
#define UNHOLI_COPON 60034   //pheonix pheather
 
class gamble_npc : public CreatureScript
{
public:
    gamble_npc() : CreatureScript("gamble_npc") { }
    struct gamble_npcAI : public ScriptedAI {
        gamble_npcAI(Creature* creature) : ScriptedAI(creature) { }

        bool GiveItemAmount(Player * player, uint32 itemEntry, uint32 amount)
        {
            if (amount < 0)
            {
                player->DestroyItemCount(itemEntry, -amount, true, false);
                return true;
            }
            if (amount == 0)
                amount = 1;

            uint32 noSpaceForCount = 0;

            ItemPosCountVec dest;
            uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemEntry, amount, &noSpaceForCount);

            if (msg != EQUIP_ERR_OK)
                amount -= noSpaceForCount;

            if (amount == 0 || dest.empty())
                return false;

            Item* item = player->StoreNewItem(dest, itemEntry, true, itemEntry);
            return true;
        }
    };

	bool OnGossipHello(Player* player, Creature* creature) override
	{

		player->ADD_GOSSIP_ITEM(3, "|cffFF0000I want to play with |CFFFFCC99Armor Part!", GOSSIP_SENDER_MAIN, ARMOR_PART, "Insert amount of Armor Part", 0, true);
		player->ADD_GOSSIP_ITEM(3, "|cffFF0000I want to play with |CFFFFCC99Weapon Part!", GOSSIP_SENDER_MAIN, WEAPON_PART, "Insert amount of Weapon Part", 0, true);
		player->ADD_GOSSIP_ITEM(3, "|cffFF0000I want to play with |cff6699FFIllusion Dust!", GOSSIP_SENDER_MAIN, SOULS_COPON, "Insert amount of Illusion Dust", 0, true);
		player->ADD_GOSSIP_ITEM(3, "|cffFF0000I want to play with |cffFF0000Phoenix Feather!", GOSSIP_SENDER_MAIN, UNHOLI_COPON, "Insert amount of Phoenix Feather", 0, true);
		player->ADD_GOSSIP_ITEM(3, "The game works as following. you add an amount of the chosen currency and you can multiplay it!", GOSSIP_SENDER_MAIN, 0);

		player->PlayerTalkClass->SendGossipMenu(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override 
	{
		//uint32 uiAction = GetGossipActionFor(player, gossipListId);
		//uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
		player->PlayerTalkClass->ClearMenus();

		if (action == 0)
		{

			player->ADD_GOSSIP_ITEM(7, "Back..", GOSSIP_SENDER_MAIN, 1);
			player->PlayerTalkClass->SendGossipMenu(123432, player->GetGUID());
		}
		else if (action == 1)
		{
			OnGossipHello(player, creature);
		}
		else if (action == 2)
		{
			player->PlayerTalkClass->CloseGossip();
		}
		return true;
	}

	bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code) override
	{
		//uint32 uiAction = GetGossipActionFor(player, gossipListId);
		//uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
		player->PlayerTalkClass->ClearMenus();


		uint32 amount = 0;
		amount = uint32(atol(code));

		if (amount < 1 || !player->HasItemCount(action, amount, false))
		{
			player->GetSession()->SendNotification("Invalid amount inserted");
		}
		else
		{
			if (urand(1, 2) == 1)
			{

				player->AddItem(action, amount);
				player->GetSession()->SendNotification("You are lucky. You have won!");
			}
			else
			{

				player->DestroyItemCount(action, amount, true);
				player->GetSession()->SendNotification("You have lost.. Bet again and u might win!!");
			}
		}
		OnGossipSelect(player, creature, sender, action);
		return true;
	}

    CreatureAI* GetAI_gamble_npc(Creature* creature)
	{
        return new gamble_npcAI(creature);
    }
};
 
void AddSC_gamble_npc()
{
    new gamble_npc();
}
