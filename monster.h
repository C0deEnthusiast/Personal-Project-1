// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Monster Class

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "party.h"
#include "player.h"
#include "item.h"

#ifndef MONSTER_H
#define MONSTER_H

using namespace std;

//Defines boss rating
#define bossRating 7
#define bossFinalRating 8

//Defines boss' health after Resurrection (2nd phase)
#define bossFinalHealth 1500
//Defines boss' attack after Resurrection (2nd phase)
#define bossFinalAttack 30

class Monster{
    private:
        static const int min_rating = 1; //Minimum Difficulty Rating of Monsters
        static const int max_rating = bossRating; //Maximum Difficulty Rating of Monsters
    public:
        //Basic Monster Data
        string monster_name; //Monster Name
        int difficultyRating; //Used for calculating rewards and battle frequency
        int attack_power; //Monster's attack value
        int monster_health;
        int crit_chance; //Likelihood to land a critical hit
        int crit_boost; //Boost of critical hit
        
        //Advanced Monster Data
        Effect power;
    
        Monster(); //Default Constructor
        /*Monster(string name, int rating, int attkPwr, int health, string effect, int effectValue,
        int effectChance, int effectDuration); //Parameterized Constructor*/
        Monster(string name, int rating, int attkPwr, int health, int critChance, int critBoost,
        string effect, int effectValue, int effectChance, int effectDuration); //Parameterized Constructor

        //Getters
        int getMinRating();
        int getMaxRating();
};

//Manages construction and set up of monster fights through Battle() class
class Combat{
    private:
        const char separator = ','; //Standard delimiter for monster text files
        static const int monster_info_count = 10; //Accounts all data sections per monster
        /*After first monsterRush(), every consecutive use of fxn would increase rating of all monsters,
        thereby raising difficulty*/
        int raise_rating;
        vector<Monster> refillList; //This is to skip most of monsterRush() function
        vector<Monster> monsterList;
        string monsterFilename;
    public:
        Combat(string filename);

        int getMonsterSize();


        void setRaiseRating(int new_raise);

        void monsterRush(string filename, bool new_rush = true);
        void removeMonster(int index);
        int countMonsterRating(int rating);

        bool battleVictory(Party& party, string monster_name, int monster_rating, int key_chance);
        bool battleDefeat(Party& party, int kill_index, int health_loss_chance, bool surrender = false);
        
        //bool monsterAmbush() //Will randomly trigger encounters

        //Sets up combat()
        int encounter(Party& party, bool room = false);
        Monster returnMonster(int rating, int &monster_index);
        bool bossIsOnlyOneAlive();
        int ratingReconfiguration(int &og_rating);

};

struct Status{
    int effect_target; //-1 is monster; non-negative values for player index
    /*Used to track whether certain effect should activate again
    Example:
    Bleed if false = Activates and false->true
    Bleed if true = Activates again

    Savage Wrath if false = Activates and false->true
    Savage Wrath if true = Activates?

    Wrath if false = Activates and false->true
    Wrath if true (if duration != 0) = Does not activate again
    Wrath if true (if duration == 0) = Activates (removes boost)
    */
    Effect power;
    int max_duration;
    Status(){
        effect_target = -2;
        max_duration = 0;
    }
    Status(int new_target, Effect new_power){
        effect_target = new_target;
        power = new_power;
        max_duration = new_power.getEffectDuration();
    }
};


class Battle{
    private:
        //For the sake of brevity, parties and monsters will be passed by reference
        Party *curParty;
        Monster *curMonster;

        //Effect Tracker
        vector<Status> monster_0; //Monster statuses
        vector<Status> playerStatuses[playerCount]; //Player statuses

        int player_actions_counter = 0;
        static const int player_actions_max = 7; //Includes taking potions, attacking, and blocking

        //Array; Determines whether a given player can attack/take action
        bool player_active[playerCount];

        //Array; Determines whether a given player is charmed into attacking allies
        bool player_charmed[playerCount];

        //Array; determines whether a given player is immune to damage
        bool player_immuneToDMG[playerCount];

        //Array; determines whether damage taken accounts for defenses like armor
        bool player_defensesUp[playerCount];

        //Determines whether player is blocking
        bool player_blocking[playerCount];

        //Array; determines how many times a given player can attack per turn
        int player_attack_max[playerCount];

        //Monster modifiers
        
        bool monster_active = true; //Determines whether monster can take action or not
        bool monster_defensesUp = true; //Determines whether damage taken accounts for defenses
        int monster_innate_armor;
        int monster_attack_max = 1; //Determines how many times monster can attack per turn
        //If possible, make another bool for if two monster combat can be implemented

        //DMG % reduced if a player chooses to block rather than attack; stacks multiplicately with armor
        double block_reduction = 0.25;

        //Determines who attacks in current turn; player always starts first
        bool player_turn = true;
        bool monster_turn = false;
        
    public:
        Battle(Party *party, Monster *monster){ //Pass memory addresses
            curParty = party;
            curMonster = monster;

            for (int i = 0; i < playerCount; i++){
                player_active[i] = true;
                player_charmed[i] = false;
                player_immuneToDMG[i] = false;
                player_defensesUp[i] = true;
                player_blocking[i] = false;
                player_attack_max[i] = 1;
            }

            monster_innate_armor = curMonster->difficultyRating * 10;
        }

        ~Battle(){
            curParty = nullptr;
            curMonster = nullptr;
        }

        //target_index: -1 for monster, otherwise [0,5] for player
        void addStatus(Effect new_effect, int target_index){
            Status temp = Status(target_index, new_effect);
            if (target_index == -1){ //Monster
                monster_0.push_back(temp);
            } else if (target_index >= 0 && target_index <= curParty->getPlayerSize()){ //Player
                playerStatuses[target_index].push_back(temp);
            }
            return;
        }

        void removeStatusesHelper(vector<Status> &vect, int target_duration){
            for (int i = 0; i < vect.size(); i++){
                if (vect[i].power.getEffectDuration() == target_duration){
                    vect.erase(vect.begin() + i);
                    i--;
                }
            }

            return;
        }

        void removeStatuses(int target_index, int target_duration){
            if (target_index >= 0 && target_index <= curParty->getPlayerSize()){
                removeStatusesHelper(playerStatuses[target_index],target_duration);
            } else if (target_index == -1) {
                removeStatusesHelper(monster_0,target_duration);
            }
        }

        int test();

        void displayEffects(string name, int health, vector<Status> vect){
            cout << "| " << name << " | " << health << " | ";
            int i = 0;
            for (auto& x: vect){
                cout << x.power.getEffectName() << "(" << x.power.getEffectDuration() << ") ";
                if (x.power.getEffectName() == effect_Burn || x.power.getEffectName() == effect_Bleed){
                    cout << ">> DMG(" << x.power.getEffectValue() << ") ";
                }

                (i != vect.size() - 1) ? cout << "| ": cout << "";
                i++;
            }
            cout << endl;
        }

        //void activateZeroDurationEffect(Effect effect, int target_index);

        bool willActivate(int effect_chance);

        void activateEffect(Status effectData);
        //Fix node configuration

        //Update this and monsterCombat
        int playerCombat();

        //Calculates how monster attacks
        int monsterCombat();

        int adjustAttack(int base_attack, int target); //Manually change defensesUp boolean to true here


        //void activateEffect(Party &party, Monster &monster, Effect effect, int target, bool in_effect);
        void battleActivate(Party &party, Monster &monster, Effect effect, int target);
        void postBattleActivate(Party &party, Monster &monster);
};

//Ideas for effects to implement
/*
 *Effect that any damage taken from player heals monster instead
 *Experiment: Replace "Crit Multiplier" effect with another; make crit innate to weapons and monsters
 *Implement <Stats> vector into Player Class Implementation; if so, modify any functions related if necessary
 *Implement <Item> weapon into Player Class Implementation (if possible)
*/

//Outline on Effect Implementation
/*
    - At beginning of given turn:
        - Activate all effects of respective turn, then remove all effects with 0 duration
    - For player's turn:
        * Set up integer counter; increment if player took action (i.e. party member attacked)
            **Set up a condition such that if counter reaches max;
            ends player turn and moves to monster turn
        * With a while loop, player can choose between taking eligible potions, attacking, or blocking
        * Block has innate 25% damage reduction (compounded by armor)
        * /
    - For monster's turn:
        * /
    - At end of given turn, reduce duration of all effects
        * Caveat: Certain effects from weapons and effects from potions will activate DURING turn
        * For 0 duration effects (i.e. Heal), negate duration reduction (ensures effect removal by next turn)
    -


Implementation Details (to work on):
    - Set up three activation functions:
        * activateEffect(): parameters are Party(), Monster(), and Node*
            ** Actually activates effect

    - Don't implement these; activateALLEffects() will take care of this
        * battleActivate(): Party(), Monster(), Effect() and int target
            ** During turn, battle-centric effects are added to list and are activated
        * postBattleActivate(): parameters are Party(), Monster(), and Node*
            ** Activates effects that are NOT battle-centric such as Crit, Heal, Rage, etc
    - Set up a refresh() function
        * Purpose: For effects like Freeze and Condemnation, where having a duplicate stacked on top
        wouldn't make a difference; it is better to have current version to recover MAX duration
        rather than use more memory for redundant effect
*/
#endif