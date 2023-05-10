// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Party Class

#include <iostream>
#include <vector>
#include "player.h"
#include "item.h"

#ifndef PARTY_H
#define PARTY_H

//Defines how many players are in game
#define playerCount 5

//Item Type Constants
#define isWeapon "weapon"
#define isTreasure "treasure"
#define isPotion "potion"
#define isArmor "armor"

using namespace std;

class Party{
    private:
        /*Relates to how data sections per line in item text file
        are needed for a complete item*/
        static const int data_count = 10;

        //Maximums and constants
        static const int player_size = playerCount; //Count for players[]
        static const int max_inventory_capacity = 30; //Max capacity of inventory[]
        static const int max_weapon_capacity = player_size;
        static const int max_armor_capacity = player_size;
        char separator = ','; //Standard delimiter for splitting data in text files

        //Game stats
        int player_count = player_size; //Count of players that are ALIVE
        int money = 100; //In gold
        int danger_level = 100; //0 - 100
        int explored_rooms = 0; //Count of how many rooms have been explored
        int keys = 0; //Used to access keys
        int current_inventory_capacity = 0;
        int current_weapon_capacity = 0;
        int current_armor_capacity = 0;

        //Arrays
        Player players[player_size]; //Player list
        Item inventory[max_inventory_capacity]; //Contains ALL player-owned items
        vector<Item> merchantList; //Used to allow players to access and add items through merchant
        Item weapon_barracks[max_weapon_capacity]; //Quick access to weapons
        Item armorSets[max_armor_capacity];

        bool addItemHelper(Item itemList[], int list_max_capacity, int &list_current_capacity, Item item){
            if (list_current_capacity >= list_max_capacity){
                return false;
            }

            for (int i = 0; i < list_max_capacity; i++){
                if (itemList[i].getItemName() == default_item_name){
                    //Creates new item in inventory
                    itemList[i] = item;
                    list_current_capacity++;//Adds to capacity
                    return true;
                }
            }

            return false;
        }

        bool removeItemHelper(Item itemList[], int list_max_capacity, int &list_current_capacity, Item item){
            if (list_current_capacity == 0){
                return false; //Quickly returns false to give efficient output rather than wait for loop
            }

            for (int i = 0; i < list_max_capacity; i++){
                if (itemList[i].getItemName() == item.getItemName()){
                    //Default constructor is used to override current item; effectively removing it
                    itemList[i] = Item();
                    list_current_capacity--;//Reduces capacity
                    return true;
                }
            }

            return false; //This occurs only if item is not found for deletion
        }
    public:
        Party(); //Default constructor
        Party(string filename); //Parameterized Constructor
        //Party(); //Dynamic Parameterized Constructor

        //Getters (Maximums and Constants)
        int getPlayerSize(){ return player_size;}
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
        bool isPlayer(int index){ return (index >= 0 && index < player_size);}
        vector<Item> copyMerchantList(); //For Battle Rewards
        Player getPlayer(int index);
        Item getWeapon(int index);
        Item getArmor(int index);

        //Setters
        void setMoney(int new_money);
        //Remove Danger Level Implementation after Map modifications
        bool setDangerLevel(int new_level);
        void setExploredRooms(int new_rooms);
        void setKeys(int new_keys);
        void setCurrentCapacity(int new_current_capacity);
        void setCurrentArmorCapacity(int new_armor);

        //Other functions

        bool areAllPlayersDead(); //Remove this in place of getLivePlayerCount()
        Item returnItem(string item_name);
        
        //Remove this; cookware and food will be replaced by potions
        void cook(int probability);

        void modifyPlayerHealth(int index, int health_change);
        void modifyWeaponAttack(int index, int attack_change);

        void addPlayer(string player_name);

        bool addItem(Item item);
        bool removeItem(Item item);
        void showInventory();
        int countItem(string item_name);

        void purchaseProcess(int amount, int total_cost, Item purchasedItem);

        void createMerchantList(string filename);
        void presentMerchantItem(Item item, string target, double tax);
        void merchant();

        void showPartyStatus();

        bool npcPuzzle(int riddle);

        //Monster functions
        //Modify this to account for Monster class change
        bool monsterOutcome(int outcome, int key_chance, int kill_index, int health_chance);

        //Ambitious functions
        //void rerollWeaponEffect()
};

#endif