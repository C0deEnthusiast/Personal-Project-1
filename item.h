// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Item Class

#include <iostream>
//#include <ctype.h>
//#include <vector>
//#include <fstream>
//#include <sstream>

#ifndef ITEM_H
#define ITEM_H

using namespace std;

//Default item name
#define default_item_name "Not an Item"

//Weapon Effect Constants
#define invalid_effect "None" //Literally does nothing
#define effect_Burn "Burn" //Inflicts certain amount of DMG every turn (based on duration)
#define effect_Freeze "Freeze" //Stops target from attacking over over given amount of turns
#define effect_Pierce "Pierce" //Weapon DMG ignores enemy armor
#define effect_EnemyHP "Enemy Current HP DMG" //Inflicts DMG based on percentage of enemy's current health

//Potion Effect Constants
#define effect_Heal "Heal" //Heals target
#define effect_Rage "Rage" //Target can attack multiple times
#define effect_Undying "Undying" //Target does not receive damage
#define effect_Resurrection "Resurrection"
//Add more potion effects here
#define effect_Muspelheim "Muspelheim"
#define effect_Godslayer "Godslayer"

//Monster Effect Constants
#define effect_Wrath "Wrath"
#define effect_Bleed "Bleed"
#define effect_Rampage "Rampage"
#define effect_Condemnation "Condemnation"
#define effect_Savage_Wrath "Savage Wrath"
#define effect_Wicked___ "Test Wicked Effect" //Finish this effect on monsters.txt
#define effect_Unholy_Judgement "Unholy Judgement"
#define effect_Final_Requiem "Final Requiem" //Finish this on monsters.txt
//Secret Enemy Effect
#define effect_Eternal_Rest "Eternal Rest"
//Special Enemy effect; for two monster combat: Enemy doesn't die before its ally does
#define effect_My_Brothers_And_Sisters "My Brothers And Sisters"
//#define effect_Eternal_Loyalty "Eternal Loyalty"


//Armor Effects
#define effect_Taunt "Taunt"
#define effect_Deflect "Deflect"



//Used by both items and monsters
class Effect{
    private:
        std::string effect_name; //Name of effect
        int effect_value; //Varies from effect (e.g. damage)
        int effect_procChance; //Chance of effect occurring
        int effect_duration; //How long effect occurs before expiring
    public:
        Effect();
        Effect(std::string name, int value, int chance, int duration);

        //Effect Getters
        std::string getEffectName();
        int getEffectValue();
        int getEffectChance();
        int getEffectDuration();

        //Effect Setters
        void setEffectName(std::string new_name);
        void setEffectValue(int new_value);
        void setEffectChance(int new_chance);
        void setEffectDuration(int new_duration);
};




class Item{
    private:
        std::string item_name; //Name of Item
        std::string item_type; //Type of Item
        int item_cost; //For merchants only; also works for treasures
        int item_stats; //Attack (weapon), DMG Reduction (armor), and potions get other special stats
        int crit_chance;
        int crit_boost;
    public:
        Effect weapon_effect;

        Item(); //Default constructor

        //Parameterized constructor
        Item(std::string name, string type, int cost, int stat, int critChance, int critBoost, 
        std::string effectName, int effectValue, int effectChance, int effectDuration);

        //Getters
        std::string getItemName(){ return item_name;};
        std::string getItemType(){ return item_type;};
        int getCost(){ return item_cost;};
        int getStat(){ return item_stats;}; //See details in items.txt file
        int getCritChance(){ return crit_chance;};
        int getCritBoost(){ return crit_boost;};

        //Setters
        void setItemName(std::string new_name);
        void setItemType(std::string new_type);
        void setCost(int new_cost);
        void setStat(int value);
        void setCritChance(int value);
        void setCritBoost(int value);
};

#endif