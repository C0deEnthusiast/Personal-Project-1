// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Item Class

#include <iostream>
#include <ctype.h>

#ifndef ITEM_H
#define ITEM_H

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
//Secret Enemy Effect
#define effect_Eternal_Rest "Eternal Rest"
//Special Enemy effect; for two monster combat: Enemy doesn't die before its ally does
#define effect_My_Brothers_And_Sisters "My Brothers And Sisters"
//#define effect_Eternal_Loyalty "Eternal Loyalty"

using namespace std;

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

//General Functions to use
struct Functions{
    //Outputs pseudo-random value within a specified interval
    int createRand(int min, int max){
        srand(time(0));
        return (rand() % (max - min + 1)) + min;
    }
    
    //Checks if input is a number
    bool isNumber(string line){ //Checks for digits
        if (line.length() == 0){
            return false;
        }
        for (int i = 0; i < line.length(); i++){ //Checks if amount is a number
            if (!isdigit(line[i])){
                return false;
            }
        }

        return true;
    }

    void displayEffect(Effect effect){
        cout << "     Ability: " << effect.getEffectName() << " - ";

        //Weapon
        if (effect.getEffectName() == effect_Burn){
            cout << "Inflicts " << effect.getEffectValue() << " damage onto enemy every turn. ";
            cout << "Lasts for " << effect.getEffectDuration() << " turn(s).";
        } else if (effect.getEffectName() == effect_EnemyHP){
            cout << "Inclicts damage equal to " << effect.getEffectValue() << "% of the enemy's ";
            cout << "current health (before base attack calculation).";
        } else if (effect.getEffectName() == effect_Freeze){
            cout << "Immobilize enemy for " << effect.getEffectDuration() << " turns.";
        } else if (effect.getEffectName() == effect_Pierce){
            cout << "Weapon Damage ignores enemy defenses, including armor.";
        }

        //Potion
        if (effect.getEffectName() == effect_Heal){
            cout << "Heals chosen player by " << effect.getEffectValue() << ".";
        } else if (effect.getEffectName() == effect_Rage){
            cout << "Chosen player gets to attack " << effect.getEffectValue();
            cout << " times. Lasts " << effect.getEffectDuration() <<" turns.";
        } else if (effect.getEffectName() == effect_Undying){
            cout << "Chosen player takes no damage for " << effect.getEffectDuration() << " turns.";
        } else if (effect.getEffectName() == effect_Resurrection){
            cout << "Revives chosen dead player.";
        } else if (effect.getEffectName() == effect_Godslayer){
            cout << "No god will stand against you. Not anymore.";
        }

        //Monster
        if (effect.getEffectName() == effect_Wrath){
            cout << "Has " << effect.getEffectChance();
            cout << "% chance to temporary boost its own attack by " << effect.getEffectValue() << ".";
        } else if (effect.getEffectName() == effect_Bleed){
            cout << "Inflicts " << effect.getEffectValue();
            cout << " damage onto target player every turn. Lasts " << effect.getEffectDuration();
            cout << " turns.";
        } else if (effect.getEffectName() == effect_Rampage){
            cout << "Has " << effect.getEffectChance();
            cout << "% chance to strike " << effect.getEffectValue();
            cout << " times per turn.";
        } else if (effect.getEffectName() == effect_Condemnation){
            cout << "Has " << effect.getEffectChance();
            cout << "% chance to confuse target into attacking teammates. Lasts ";
            cout << effect.getEffectDuration() << " turns.";
        } else if (effect.getEffectName() == effect_Savage_Wrath){
            cout << "Permanently boosts attack by " << effect.getEffectValue();
            cout << " every turn; has " << effect.getEffectChance();
            cout << "% chance to lose " << effect.getEffectValue() << " health.";
        } else if (effect.getEffectName() == effect_Wicked___){
            cout << "Complete effect";
        } else if (effect.getEffectName() == effect_Unholy_Judgement){
            cout << "Targeted player of attack will be instantly killed after "; 
            cout << effect.getEffectDuration() << " turns.";
        } else if (effect.getEffectName() == effect_Final_Requiem){ //Revise effect
            cout << "Exclusive to Boss Monster; stuns ALL party members, then kills them";
        }
        cout << endl;

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
        string getItemName(){ return item_name;}
        string getItemType(){ return item_type;}
        int getCost(){ return item_cost;}
        int getItemStat(){ return item_stats;} //See details in items.txt file
        int getCritChance(){ return crit_chance;}
        int getCritBoost(){ return crit_boost;}

        //Setters
        void setItemName(string new_name);
        void setItemType(string new_type);
        void setCost(int new_cost);
        void setStat(int value);
        void setCritChance(int value);
        void setCritBoost(int value);
};

#endif