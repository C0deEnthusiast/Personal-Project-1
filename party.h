// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Party Class

#include <iostream>
#include <vector>
#include "player.h"
#include "item.h"

#ifndef PARTY_H
#define PARTY_H

//Defines maximum count of players per game
#define playerCount 5

//Defines how many splices per line in item text file are needed for a complete item
#define data_count 10

//Standard delimiter for splitting data in required text files
#define std_delimiter ','

//Item Type Constants
#define isWeapon "weapon"
#define isTreasure "treasure"
#define isPotion "potion"
#define isArmor "armor"

#define New_isWeapon 'W'
#define New_isTreasure 'T'
#define New_isPotion 'P'
#define New_isArmor 'A'

//using namespace std;
using std::cout, std::endl;


/*Revise funcs with 'Modify' based on priority (Starts with '1st'); look
at other funcs only when these funcs have been revised sufficiently*/

class Party{
    private:
        //Maximums and constants
        //static const int player_size = playerCount; //Count for players[]
        static const int max_inventory_capacity = 30; //Max capacity of inventory[]

        //Game stats
        int alive_players_count = playerCount; //Count of players that are alive
        int money = 100; //In gold
        int danger_level = 100; //0 - 100
        int explored_rooms = 0; //Count of how many rooms have been explored
        int keys = 0; //Used to access keys
        int current_inventory_capacity = 0;
        int current_weapon_capacity = 0;
        int current_armor_capacity = 0;

        //Private Arrays
        Player players[playerCount]; //Player list
        Item inventory[max_inventory_capacity]; //Contains ALL player-owned items
        std::vector<Item> merchantList; //Used to allow players to access and add items through merchant
    public:
        Party(); //Default constructor; "items.txt" is default text file
        Party(std::string filename); //Parameterized Constructor

        //Getters (Maximums and Constants)
        int getMaxPlayerSize(){ return playerCount;}
        int getMaxInventoryCapacity(){ return max_inventory_capacity;}
        //Max number of equipped weapons will always be equal to the max number of players
        int getMaxEquippedWeaponCapacity(){ return playerCount;}
        //Max number of equipped armor will always be equal to the max number of players
        int getMaxEquippedArmorCapacity(){ return playerCount;}

        //Normal Getters

        int getLivingPlayerCount(){return alive_players_count;} //Modify
        int getMoney(){ return money;}
        int getDangerLevel(){ return danger_level;}
        int getExploredRooms(){ return explored_rooms;}
        int getKeys(){ return keys;}
        int getCurrentCapacity(){ return current_inventory_capacity;}
        //Retrieves count of currently EQUIPPED weapons
        int getCurrentWeaponCapacity(){ return current_weapon_capacity;}
        //Retrieves count of currently EQUIPPED armor
        int getCurrentArmorCapacity(){ return current_armor_capacity;}
        int getMerchantCapacity(){ return merchantList.size();}
        //int getLivePlayerCount(); //Modify 3rd
        bool isPlayerIndex(int index){ return (index >= 0 && index < playerCount);}
        std::vector<Item> copyMerchantList(); //For Battle Rewards
        Player getPlayer(int index);
        Item getEquippedWeapon(int index);
        Item getArmor(int index);

        //Setters
        void setMoney(int new_money);
        bool setDangerLevel(int new_level); //Remove Danger Level Implementation after Map modifications
        void setExploredRooms(int new_rooms);
        void setKeys(int new_keys);

        //Additional Modifiers

        void modifyPlayerHealth(int index, int health_change); //Modify 8th
        void modifyWeaponAttack(int index, int attack_change); //Modify 9th
        void addPlayer(std::string player_name); //Modify 7th
        bool addItems(Item item, int &item_count);
        bool removeItem(Item item, bool removeFromInventory = true);
        bool equipItem(Item item, int player_index, Item &retrieved_item);

        //Other functions

        Item returnItem(std::string item_name);
        void showInventory();
        int countItem(std::string item_name);
        void purchaseProcess(int amount, int unit_price, Item purchasedItem); //Modify 2nd
        void createMerchantList(std::string filename); //Modify 4th
        void presentItem(Item item, double tax, bool forMerchant = true); //Modify 5th
        void merchant(); //Modify 1st
        void showPartyStatus();
        void showPartyArsenal(bool enumerate = false);
        bool npcPuzzle(int riddle); //Modify 6th; consider removing though

        void customizeArsenal();

        //Monster functions
        //Modify this to account for Monster class change
        bool monsterOutcome(int outcome, int key_chance, int kill_index, int health_chance);

        //Ambitious functions
        //void rerollWeaponEffect()
};

inline void displayEffect(Effect T){
    cout << "     Ability: " << T.getEffectName() << " - ";

    //Weapon
    if (T.getEffectName() == effect_Burn){
        cout << "Inflicts " << T.getEffectValue() << " damage onto enemy every turn. ";
        cout << "Lasts for " << T.getEffectDuration() << " turn(s).";
    } else if (T.getEffectName() == effect_EnemyHP){
        cout << "Inclicts damage equal to " << T.getEffectValue() << "% of the enemy's ";
        cout << "current health (before base attack calculation).";
    } else if (T.getEffectName() == effect_Freeze){
        cout << "Immobilize enemy for " << T.getEffectDuration() << " turns.";
    } else if (T.getEffectName() == effect_Pierce){
        cout << "Weapon damage ignores enemy defenses, including armor.";
    }

    //Potion
    if (T.getEffectName() == effect_Heal){
        cout << "Heals Target player by " << T.getEffectValue() << ".";
    } else if (T.getEffectName() == effect_Rage){
        cout << "Target player gets to attack " << T.getEffectValue();
        cout << " times. Lasts " << T.getEffectDuration() <<" turns.";
    } else if (T.getEffectName() == effect_Undying){
        cout << "Target player takes no damage for " << T.getEffectDuration() << " turns.";
    } else if (T.getEffectName() == effect_Resurrection){
        cout << "Revives target player if they take lethal damage.";
    } else if (T.getEffectName() == effect_Godslayer){
        cout << "No god will stand against you. Not anymore.";
    }

    //Monster
    if (T.getEffectName() == effect_Wrath){
        cout << "Has " << T.getEffectChance();
        cout << "% chance to temporary boost its own attack by " << T.getEffectValue() << ".";
    } else if (T.getEffectName() == effect_Bleed){
        cout << "Inflicts " << T.getEffectValue();
        cout << " damage onto target player every turn. Lasts ";
        cout << T.getEffectDuration() << " turns.";
    } else if (T.getEffectName() == effect_Rampage){
        cout << "Has " << T.getEffectChance() << "% chance to strike ";
        cout << T.getEffectValue() << " times per turn.";
    } else if (T.getEffectName() == effect_Condemnation){
        cout << "Has " << T.getEffectChance();
        cout << "% chance to confuse target into attacking teammates. Lasts ";
        cout << T.getEffectDuration() << " turns.";
    } else if (T.getEffectName() == effect_Savage_Wrath){
        cout << "Permanently boosts attack by " << T.getEffectValue();
        cout << " every turn; has " << T.getEffectChance();
        cout << "% chance to lose " << T.getEffectValue() << " health.";
    } else if (T.getEffectName() == effect_Wicked___){ //Revise effect
        cout << "Complete effect";
    } else if (T.getEffectName() == effect_Unholy_Judgement){
        cout << "Affected player will be instantly killed after "; 
        cout << T.getEffectDuration() << " turns.";
    } else if (T.getEffectName() == effect_Final_Requiem){ //Revise effect
        cout << "Revise effect from monster text file";
    }
    cout << endl;

    return;
}

#endif