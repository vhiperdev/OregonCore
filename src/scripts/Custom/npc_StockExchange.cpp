#include "ScriptPCH.h"
#include "Config/Config.h"
#include "ScriptedCreature.h"
#include "Language.h"
#include "time.h"

#define STOCKNUMBER 5
#define GOLDTOCOPPER 10000

class CS_StockExchange : public CreatureScript
{
public:
    CS_StockExchange() : CreatureScript("Creature_StockExchange") { }

    uint32 stock = 0;

    bool OnGossipHello(Player* plr, Creature* creature) override //MAIN MENU
    {
        //StockUpdate every 2 minutes
        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `UpdateTime`   FROM `stocks` WHERE iD = 0");
        if (!result)
            return false;
        Field* field = result->Fetch();
        uint32 stockPrevTime = field[0].GetUInt32();
        uint32 realCurrTime = getMSTime();
        uint32 stockDiff = getMSTimeDiff(stockPrevTime, realCurrTime);
        if (stockDiff >= 120000)
        {
            uint32 timeMult;
            timeMult = stockDiff / 120000;
            if (StockUpdate(timeMult))
                sLog.outError("Stocks have been updated (%u)", realCurrTime);
            else
            {
                sLog.outError("Error while updating stocks...");
            }
            stockPrevTime = realCurrTime;
            WorldDatabase.PExecute("UPDATE `stocks` SET `UpdateTime` = %u WHERE `iD` = 0", getMSTime());
        }
        WorldSession* session = plr->GetSession();
        QueryResult_AutoPtr licencetest = CharacterDatabase.PQuery("SELECT `1`, `2`, `3`, `4`, `5`  FROM `character_stockdata` WHERE iD = %u", plr->GetGUIDLow());
        plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Scroll_08:30:30:-18:0|tWhat are the current exchange rates?", GOSSIP_SENDER_MAIN, 2);
        plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|tHow many unsold shares are there left?", GOSSIP_SENDER_MAIN, 1);
        if (!licencetest)
        {
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_MISC_NOTE_02:30:30:-18:0|tI'd like to acquire a trade licence.", GOSSIP_SENDER_MAIN, 12, "This will cost you", 400000, false);
        }
        else //Player can invest if he has a licence
        {
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_02:30:30:-18:0|tI'd like to manage my portfolio.", GOSSIP_SENDER_MAIN, 3);
        }
        plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tIt was a pleasure to make business with you.", GOSSIP_SENDER_MAIN, 4);
        plr->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }
    bool OnGossipSelect(Player* plr, Creature* creature, uint32 sender, uint32 uiAction) override
    {

        int32 amount = 0;
        plr->PlayerTalkClass->ClearMenus();
        WorldSession* session = plr->GetSession();
        std::ostringstream message;
        
        float ownedratio[5];
        int worth[5];
        for (int iter = 1; iter < 6; iter++)
        {
            QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `OwnedRatio`,`Worth`   FROM `stocks` WHERE iD = %u", iter);
            if (!result)
                return false;
            Field* field = result->Fetch();
            ownedratio[iter - 1] = field[0].GetFloat();
            worth[iter - 1] = field[1].GetInt32();
        }
        switch (uiAction)
        {
        case 1: //REMAINING SHARES
            message << "REMAINING SHARES (in gold coins)$BRavenholdt Manor : " << worth[0] * (1 - ownedratio[0]) << "$BBloodsail : " << worth[1] * (1 - ownedratio[1]) << "$BSteamwheedle Cartel : " << worth[2] * (1 - ownedratio[2]) << "$BTheramore Silk Traders : " << worth[3] * (1 - ownedratio[3]) << "$BEldoar'norore : " << worth[4] * (1 - ownedratio[4]);
            creature->Whisper(message.str().c_str(), plr->GetGUID());
            plr->PlayerTalkClass->CloseGossip();
            OnGossipHello(plr, creature);
            break;
        case 2: //CURRENT EXCHANGE RATES
            message << "CURRENT EXCHANGE RATES (in gold coins)$BRavenholdt Manor : " << worth[0] << "$BBloodsail : " << worth[1] << "$BSteamwheedle Cartel : " << worth[2] << "$BTheramore Silk Traders : " << worth[3] << "$BEldoar'norore : " << worth[4];
            creature->Whisper(message.str().c_str(), plr->GetGUID());
            plr->PlayerTalkClass->CloseGossip();
            OnGossipHello(plr, creature);
            break;
        case 3: //MANAGE PORTFOLIO MENU (5-10)
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Helmet_30:30:30:-18:0|tRavenholdt Manor", GOSSIP_SENDER_MAIN, 5);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Helmet_66:30:30:-18:0|tBloodsail", GOSSIP_SENDER_MAIN, 6);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Bomb_03:30:30:-18:0|tSteamwheedle Cartel", GOSSIP_SENDER_MAIN, 7);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Chest_Cloth_56:30:30:-18:0|tTheramore Silk Traders", GOSSIP_SENDER_MAIN, 8);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_FireDancer_01:30:30:-18:0|tEldoar'norore", GOSSIP_SENDER_MAIN, 9);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack", GOSSIP_SENDER_MAIN, 10);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            break;
        case 4: //CLOSE GOSSIP
            plr->PlayerTalkClass->CloseGossip();
            break;
        case 5: //MANAGE RAVENHOLDT STOCK
            stock = 1;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|tBuy shares", stock, 0, "How much gold would you like to invest?", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|tSell all shares", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            return true;
            break;
        case 6: //MANAGE BLOODSAIL STOCK
            stock = 2;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|tBuy shares", stock, 0, "How much gold would you like to invest?", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|tSell all shares", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            break;
        case 7: //MANAGE GOBLIN STOCK
            stock = 3;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|tBuy shares", stock, 0, "How much gold would you like to invest?", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|tSell all shares", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            break;
        case 8: //MANAGE SILKTRADERS STOCK
            stock = 4;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|tBuy shares", stock, 0, "How much gold would you like to invest?", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|tSell all shares", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            break;
        case 9: //MANAGE ELDOAR'NORORE STOCK
            stock = 5;
            plr->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Letter_03:30:30:-18:0|tBuy shares", stock, 0, "How much gold would you like to invest?", 0, true);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/INV_Misc_Coin_16:30:30:-18:0|tSell all shares", stock, 11);
            plr->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack...", GOSSIP_SENDER_MAIN, 3);
            plr->SEND_GOSSIP_MENU(50002, creature->GetGUID());
            return true;
            break;
        case 11: //TRANSACTION = SELL
            if (!SellStock(plr, stock, creature))
                plr->GetSession()->SendNotification("Error while selling shares... Please contact the administrator.");
            else
            {

            }
            plr->PlayerTalkClass->CloseGossip();
            OnGossipHello(plr, creature);

            break;
        case 12: //CREATE PLAYER LICENCE ( = ROW IN CHARACTER_STOCKDATA )
            if (plr->GetMoney() >= 40 * GOLDTOCOPPER)
            {
                if (CreateLicence(plr))
                {
                    amount = 40 * GOLDTOCOPPER;
                    plr->ModifyMoney(-amount);
                    message << "Your licence has been acquired successfully, " << plr->GetName() << "!";
                    creature->Say(message.str().c_str(), LANG_UNIVERSAL, plr->GetGUID());
                }
                else
                    plr->GetSession()->SendNotification("Error while registering your licence. Please contact the administrator.");
            }
            else
            {
                message << "I am afraid you do not have these 40 gold coins, " << plr->GetName() << "...";
                creature->Say(message.str().c_str(), LANG_UNIVERSAL, plr->GetGUID());
            }
            plr->PlayerTalkClass->CloseGossip();
            OnGossipHello(plr, creature);
            break;
        default:
            OnGossipHello(plr, creature);
            break;
        }
        return true;
    }
    bool OnGossipSelectCode(Player* plr, Creature* creature, uint32 stock, uint32 uiAction, const char* code) override
    {
        plr->PlayerTalkClass->ClearMenus();
        std::ostringstream message;
        int32 amount = 0;
        std::string investment = code;
        static const char* allowedcharacters = "1234567890";
        if (!investment.length() || investment.find_first_not_of(allowedcharacters) != std::string::npos) //If investment was specified incorrectly
        {
            plr->GetSession()->SendNotification("Your investment bust be declared using figures.(0123456789)");
        }
        else //If investment was specified properly
        {
            //Statements
            uint32 investmentI = uint32(atol(code));
            if (investmentI*GOLDTOCOPPER <= plr->GetMoney())
            {
                QueryResult_AutoPtr result1 = WorldDatabase.PQuery("SELECT `OwnedRatio`,`Worth`  FROM `stocks` WHERE iD = %u", stock);
                if (!result1)
                    return false;
                Field* field = result1->Fetch();
                float ownedratio = field[0].GetFloat();
                int worth = field[1].GetInt32();
                QueryResult_AutoPtr result2 = CharacterDatabase.PQuery("SELECT `%u` FROM `character_stockdata` WHERE iD = %u", stock, plr->GetGUIDLow());
                if (!result2)
                    return false;
                field = result2->Fetch();
                float initialratio = field[0].GetFloat();
                float ratio = static_cast<float>(investmentI) / worth;
                if (ratio <= (1 - ownedratio)) //If enough stocks left
                {
                    int amount = investmentI * GOLDTOCOPPER;
                    plr->ModifyMoney(-amount); // substract it from player money
                    CharacterDatabase.PExecute("UPDATE `character_stockdata` SET `%u` = %f WHERE `iD` = %u", stock, initialratio + ratio, plr->GetGUIDLow());
                    WorldDatabase.PExecute("UPDATE `stocks` SET `OwnedRatio` = %f WHERE `iD` = %u", ownedratio + ratio, stock);
                }
                else
                {
                    plr->GetSession()->SendNotification("There are not so many shares for you to acquire, check the remaining shares before trying to invest.");
                }
            }
            else
            {
                message << "You do not have so much money, " << plr->GetName() << ".";
                creature->Say(message.str().c_str(), LANG_UNIVERSAL, plr->GetGUID());
            }
        }
        plr->PlayerTalkClass->CloseGossip();
        OnGossipHello(plr, creature);
        return true;
    }

private:
    bool SendMoneyByMail(Player* player, uint64 money, int32 stock)
    {
        MailSender sender(MAIL_CREATURE, 50038, MAIL_STATIONERY_NORMAL);
        std::string stockName;
        switch (stock)
        {
        case 1:
            stockName = "Ravenholdt Manor";
            break;
        case 2:
            stockName = "Bloodsail";
            break;
        case 3:
            stockName = "Steamwheedle Cartel";
            break;
        case 4:
            stockName = "Theramore Silk Traders";
            break;
        case 5:
            stockName = "Eldoar'norore";
            break;
        default:
            return false;
        }
        std::ostringstream text;
        text << "You have requested the General Exchange Hall to sell your shares of the " << stockName << " stock. This has yielded some " << money / GOLDTOCOPPER << " gold coins from which I have deducted " << money / 10 / GOLDTOCOPPER << " in the sake of various sale and accountancy costs. Enclosed to this letter are " << 9 * money / 10 / GOLDTOCOPPER << " gold coins which come down to you.";
        MailDraft draft("Share Sale", text.str().c_str());
        draft.AddMoney(money * 9 / 10);
        //SQLTransaction letter = CharacterDatabase.BeginTransaction();
        draft.SendMailTo(player, sender);
        //draft.SendMailTo(letter, MailReceiver(NULL, player.GetCounter()), sender);
        //CharacterDatabase.CommitTransaction(letter);
        return true;
    }
    bool SellStock(Player* plr, int32 stock, Creature* creature) /*SELL ALL THE SHARES OF THE SPECIFIED STOCK */
    {
        QueryResult_AutoPtr result2 = WorldDatabase.PQuery("SELECT `OwnedRatio`,`Worth`   FROM `stocks` WHERE iD = %u", stock);
        if (!result2)
            return false;
        QueryResult_AutoPtr result1 = CharacterDatabase.PQuery("SELECT `1`, `2`, `3`, `4`, `5`  FROM `character_stockdata` WHERE iD = %u", plr->GetGUIDLow());
        if (!result1)
            return false;
        Field* field = result1->Fetch();
        float ratio = field[stock - 1].GetFloat();
        if (ratio == 0)
        {
            plr->GetSession()->SendNotification("You don't own any share of this stock...");
        }
        else //ONLY IF PLAYER OWNS SOMETHING
        {
            Field* field = result2->Fetch();
            float newownedratio = field[0].GetFloat() - ratio;
            int worth = field[1].GetInt32();
            if (!SendMoneyByMail(plr, worth*GOLDTOCOPPER*ratio, stock))
                return false;
            std::ostringstream text;
            text << "I shall make the arragement for the sale, " << plr->GetName() << ", you will receive your money by mail soon enough.";
            creature->Say(text.str().c_str(),LANG_UNIVERSAL, plr->GetGUID());
            CharacterDatabase.PExecute("UPDATE `character_stockdata` SET `%u` = 0 WHERE `iD` = %u", stock, plr->GetGUIDLow());
            WorldDatabase.PExecute("UPDATE `stocks` SET `OwnedRatio` = %f WHERE `iD` = %u", newownedratio, stock);
        }
        return true;
    }
    bool CreateLicence(Player* plr)
    {
        CharacterDatabase.BeginTransaction();
        CharacterDatabase.PExecute("REPLACE INTO `character_stockdata` (`iD`, `1`, `2`, `3`, `4`, `5`) VALUES (%u, 0, 0, 0, 0, 0)", plr->GetGUIDLow());
        CharacterDatabase.CommitTransaction();
        return true;
    }
    bool StockUpdate(int timeMult)
    {
        int32 stockWorth[STOCKNUMBER], newStockWorth[STOCKNUMBER];
        int32 stockScale[STOCKNUMBER];
        float stockOwnedRatio[STOCKNUMBER];
        srand(time(NULL));
        // Load stock values from DB
        for (int itr = 0; itr < STOCKNUMBER; itr++)
        {
            QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `Scale`,`OwnedRatio`,`Worth`  FROM `stocks` WHERE iD = %u", itr + 1);
            if (!result)
                return false;
            Field* field = result->Fetch();
            stockScale[itr] = field[0].GetInt32();
            stockOwnedRatio[itr] = field[1].GetFloat();
            stockWorth[itr] = field[2].GetInt32();
        }
        // Dicethrows and Upload values to DB
        for (int itr = 0; itr < STOCKNUMBER; itr++)
        {
            char sign;
            if (stockWorth[itr] + stockScale[itr] * timeMult > 200000) //Don't go over 200.000 gold coins
                sign = -1;
            else if (stockWorth[itr] - stockScale[itr] * timeMult < 0) //Nor below 0
                sign = 1;
            else
            {
                int dice = (rand() % 101) + floor(10 * stockOwnedRatio[itr]) - 55;
                sign = dice / abs(dice);
            }
            newStockWorth[itr] = stockWorth[itr] + sign * stockScale[itr] * timeMult;
            WorldDatabase.PExecute("UPDATE `stocks` SET `worth` = %u WHERE `iD` = %u", newStockWorth[itr], itr + 1);
        }
        return true;
    }
};

void AddSC_StockExchange()
{
    new CS_StockExchange();
}
