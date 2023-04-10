// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Item Class

#include <iostream>
#include <cstdlib>
#include <fstream>

#ifndef ITEM_H
#define ITEM_H

//Defines how many players are in game
#define playerCount 5

//Defines default health of players
#define defaultPlayerHealth 100

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

using namespace std;

//General Functions to use
struct Functions{
    int createRand(int min, int max){
        srand(time(0));
        return (rand() % (max - min + 1)) + min;
    }
};

//Used by both items and monsters
class Effect{
    private:
        string name;
        int value;
        int chance;
        int duration;
    public:
        Effect(){
            name = invalid_effect;
            value = 0;
            chance = 0;
            duration = 0;
        };

        Effect(string effectName, int effectValue, int effectChance, int effectDuration){
            name = effectName;
            value = effectValue;
            chance = effectChance;
            duration = effectDuration;
        };

        string getEffectName(){ return name;}

        int getEffectValue(){ return value;}

        int getEffectChance(){ return chance;}

        int getEffectDuration(){ return duration;}

        void setEffectName(string new_name){
            name = new_name;

            return;
        }

        void setEffectValue(int new_value){
            value = new_value;

            return;
        }

        void setEffectChance(int new_chance){
            chance = new_chance;

            return;
        }

        void setEffectDuration(int new_duration){
            duration = new_duration;

            return;
        }
};


class Item{
    private:
        string item_name; //Universal attributes
        string item_type; //Type of Item (weapon, cookware, treasure)
        int item_cost; //For merchants only; also works for treasures
        int item_stats; //Attack (weapon), dmg reduction (armor), and potions got something else
        int crit_chance;
        int crit_boost;
    public:
        Effect weapon_effect;

        Item(); //Default constructor
        //Parameterized constructor
        /*Item(string name, string type, int cost, int stat, string effectName, 
        int effectValue, int effectChance, int effectDuration);*/
        Item(string name, string type, int cost, int stat, int critChance, int critBoost, 
        string effectName, int effectValue, int effectChance, int effectDuration);

        //Getters
        string getItemName();
        string getItemType();
        int getCost();
        int getStat();
        int getCritChance();
        int getCritBoost();

        //Setters
        void setItemName(string new_name);
        void setItemType(string new_type);
        void setCost(int new_cost);
        void setStat(int value);
        void setCritChance(int value);
        void setCritBoost(int value);
};

#endif