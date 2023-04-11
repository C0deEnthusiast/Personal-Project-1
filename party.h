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
        static const int food_recover = 5; //Healing amount for when food is consumed
        static const int max_capacity = 30; //Max capacity of inventory[]
        static const int max_weapon = player_size; //Maximum Weapon Count (proportional to player_size)
        static const int max_armor = player_size; //Max armor count (proportional to player_size)
        char separator = ','; //Standard delimiter for splitting data in text files

        //Game stats
        int money; //In gold
        int danger_level; //0 - 100
        int explored_rooms; //Count of how many rooms have been explored
        int keys; //Room keys
        int current_capacity; //Accounts for current count inventory[];
        int current_weapon; //Accounts for current WEAPON capacity
        int current_armor; //How much armor

        //Arrays
        Player players[player_size]; //Player list
        Item inventory[max_capacity]; //Contains ALL player-owned items
        vector<Item> merchantList; //Used to allow players to access and add items through merchant
        Item weapon_barracks[max_weapon]; //Quick access to weapons
        Item armorSets[max_armor];

        bool addItemHelper(Item itemList[], int list_max_capacity, int &list_current_capacity, Item item){
            if (list_current_capacity >= max_capacity){
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

        //Getters (Maximums and Constants)
        int getPlayerSize();
        int getHealthRecover();
        int getMaxCapacity();
        int getMaxWeaponCapacity();
        int getMaxArmorCapacity();

        //Normal Getters
        Player getPlayer(int index){
            if (index < 0 || index > player_size){
                return Player();
            } else {
                return players[index];
            }
        }
        Item getWeapon(int index){
            if (index < 0 || index > max_weapon){
                return Item();
            } else {
                return weapon_barracks[index];
            }
        }
        Item getArmor(int index){
            if (index < 0 || index > max_armor){
                return Item();
            } else {
                return armorSets[index];
            }
        }
        int getMoney();
        int getDangerLevel();
        int getExploredRooms();
        int getKeys();
        int getCurrentCapacity();
        int getCurrentWeaponCapacity();
        int getCurrentArmorCapacity();
        int getPlayerCount();
        int getMerchantCapacity();

        //Setters
        void setMoney(int new_money);
        bool setDangerLevel(int new_level);
        void setExploredRooms(int new_rooms);
        void setKeys(int new_keys);
        void setCurrentCapacity(int new_current_capacity);
        void setCurrentArmorCapacity(int new_armor);

        //Other functions

        bool areAllPlayersDead();
        Item returnItem(string item_name);
        
        //Remove this; cookware and food will be replaced by potions
        void cook(int probability);

        //Modify this so that it becomes a void function
        //IMPORTANT: Remove the boolean status from PLAYER class
        bool changePlayerHealth(int index, int health_change);
        void modifyPlayerHealth(int index, int health_change);
        void modifyWeaponAttack(int index, int attack_change);

        void addPlayer(string player_name);

        bool addItem(Item item);

        //Similar to addItem() above, replace string-based removeItem() with commented Item-based removeItem()
        bool removeItemOld(string item_name);

        //Keep this
        bool removeItem(Item item);
        void showInventory();
        int countItem(string item_name);

        void purchaseProcess(int amount, int total_cost, Item purchasedItem);

        void createMerchantList(string filename);
        void presentMerchantItem(Item item, string target, double tax);
        void merchant();

        void displayEffect(Effect show);

        void showPartyStatus();

        bool npcPuzzle(int riddle);

        //Monster functions
        //Modify this to account for Monster class change
        bool monsterOutcome(int outcome, int key_chance, int kill_index, int health_chance);
};

#endif