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

class Party{
    private:
        //Maximums and constants
        //static const int player_size = playerCount; //Count for players[]
        static const int max_inventory_capacity = 30; //Max capacity of inventory[]
        static const int max_weapon_capacity = playerCount;
        static const int max_armor_capacity = playerCount;

        //Game stats
        int player_count = playerCount; //Count of players that are ALIVE
        int money = 100; //In gold
        int danger_level = 100; //0 - 100
        int explored_rooms = 0; //Count of how many rooms have been explored
        int keys = 0; //Used to access keys
        int current_inventory_capacity = 0;
        int current_weapon_capacity = 0;
        int current_armor_capacity = 0;
        int living_player_count = playerCount;

        //Private Arrays
        Player players[playerCount]; //Player list
        Item inventory[max_inventory_capacity]; //Contains ALL player-owned items
        std::vector<Item> merchantList; //Used to allow players to access and add items through merchant
        Item weapon_barracks[max_weapon_capacity]; //Quick access to weapons
        Item armorSets[max_armor_capacity];
    public:
        //Public Arrays
        //Player players[playerCount]; //Player list

        Party(); //Default constructor; "items.txt" is default text file
        Party(std::string filename); //Parameterized Constructor
        //Party(); //Dynamic Parameterized Constructor

        //Getters (Maximums and Constants)
        int getPlayerSize(){ return playerCount;}
        int getMaxInventoryCapacity(){ return max_inventory_capacity;}
        int getMaxWeaponCapacity(){ return max_weapon_capacity;}
        int getMaxArmorCapacity(){ return max_armor_capacity;}

        //Normal Getters
        int getMoney(){ return money;}
        int getDangerLevel(){ return danger_level;}
        int getExploredRooms(){ return explored_rooms;}
        int getKeys(){ return keys;}
        int getCurrentCapacity(){ return current_inventory_capacity;}
        int getCurrentWeaponCapacity(){ return current_weapon_capacity;}
        int getCurrentArmorCapacity(){ return current_armor_capacity;}
        int getMerchantCapacity(){ return merchantList.size();}
        int getLivePlayerCount();
        bool isPlayerIndex(int index){ return (index >= 0 && index < playerCount);}
        std::vector<Item> copyMerchantList(); //For Battle Rewards
        Player getPlayer(int index);
        Player new_getPlayer(int index, int &retrieved_status);
        Item getWeapon(int index);
        Item getArmor(int index);

        //Setters
        void setMoney(int new_money);
        //Remove Danger Level Implementation after Map modifications
        bool setDangerLevel(int new_level);
        void setExploredRooms(int new_rooms);
        void setKeys(int new_keys);

        //Other functions

        bool areAllPlayersDead(); //Remove this in place of getLivePlayerCount()
        Item returnItem(std::string item_name);
        
        void modifyPlayerHealth(int index, int health_change);
        void modifyWeaponAttack(int index, int attack_change);

        void addPlayer(std::string player_name);

        bool addItemProto(Item item);
        bool removeItemProto(Item item);

        bool addItem(Item item);
        bool removeItem(Item item);
        void showInventory();
        int countItem(std::string item_name);

        void purchaseProcess(int amount, int total_cost, Item purchasedItem);

        void createMerchantList(std::string filename);
        void presentMerchantItem(Item item, std::string target, double tax);
        void merchant();

        void showPartyStatus();

        bool npcPuzzle(int riddle);

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