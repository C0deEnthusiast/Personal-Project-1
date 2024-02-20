// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Monster Class

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "party.h"
#include "player.h"
#include "item.h"
#include "functions.h"

#ifndef MONSTER_H
#define MONSTER_H

using namespace std;

//Defines minimum monster difficulty rating
#define minimumRating 1

//Defines maximum damage reduction allowed for monster (in percentage form)
#define maxArmorValue 90

//Defines boss rating
#define bossRating 7 //Note: This also behaves the maximum rating a monster can normally have
#define bossFinalRating 8

//Defines boss' health after Resurrection (2nd phase)
#define bossFinalHealth 1500
//Defines boss' attack after Resurrection (2nd phase)
#define bossFinalAttack 30

//Status buffs and debuffs
#define isBuff 1
#define isDebuff -1
#define isNotBuffOrDebuff 0


struct Monster{
    std::string monster_name; //Monster Name
    int difficulty_rating; //Determines likelihood of encounter, rewards, and monster's damage reduction
    int attack_power; //Monster's attack value
    int monster_health; //Monster's hitpoints before death
    int crit_chance; //Probability to land a critical hit
    int crit_boost; //Boost damage percentage on critical hit
    Effect power; //Monster's skill/effect
    //vector<Effect> powers;

    Monster(); //Default Constructor
    Monster(std::string name, int rating, int attkPwr, int health, int critChance, int critBoost,
    Effect new_effect); //Parameterized Constructor

    //Setter
    void setRating(int new_rating);
};

struct Status{
    Status* next;
    Effect power;
    int effect_target; //-1 is monster; non-negative values for player index
    int max_duration;
    int buffStatus;
    bool toBeRemoved;
    Status();
    Status(int new_target, Effect new_power);
};

/*struct pStatus{
    int effect_target; //-1 is monster; non-negative values for player index
    Effect power;
    int max_duration;
    pStatus* next;
    pStatus(){
        effect_target = -2;
        max_duration = 0;
    }
    pStatus(int new_target, Effect new_power){
        effect_target = new_target;
        power = new_power;
        max_duration = new_power.getEffectDuration();
    }
};*/


//Manages Battle
class Battle{
    private:
        //Parties and monsters will be passed by reference
        Party *curParty;
        Monster *curMonster;

        //Effect Tracker
        vector<Status> monster_0; //Monster statuses
        vector<Status> playerStatuses[playerCount]; //Player statuses

        Status** playersStatuses_proto;
        Status* monsterStatuses;

        //Effect Tracker (updated)
        //pStatus** playerStatusArray;
        //pStatus* first_monster_head;

        //Attack adjustments for weapons and monster will be ONLY in battle (unless in 'updates')
        /*Alternative choice: Let monster stats be updated after battle so as to give
        player/user an easier/harder time dealing with prior monster after retreat*/
        Monster temp_monster;

        Item* altered_weapons; //Copies weapons currently equipped; used for damage calculation instead

        int player_actions_counter = 0;
        static const int player_actions_max = 7; //Includes taking potions, attacking, and blocking

        //Player Modifiers

        //Arrays (Default Value)
        bool* player_active; //Determines which player can attack/take action (True)
        bool* player_charmed; //Determines which player is charmed into attacking allies (False)
        bool* player_immuneToDMG; //Determines which player is immune to damage (False)
        bool* player_defensesUp; //Determines whether defense applies during damage calculation (True)
        bool* player_blocking; //Determines whether player is blocking; see block_reduction (False)
        int* player_attack_max; //Determines how many times given player can attack per turn (1)
        

        //Monster Modifiers
        
        bool monster_active = true; //Determines whether monster can take action or not
        bool monster_defensesUp = true; //Determines whether damage taken accounts for defenses
        int monster_innate_armor = 0;
        int monster_attack_max = 1; //Determines how many times monster can attack per turn
        //If possible, make another bool for if two monster combat can be implemented

        //DMG % reduced if a player chooses to block rather than attack; stacks multiplicately with armor
        double block_reduction = 25; //Percent form

        //Determines who attacks in current turn; player always starts first
        bool player_turn = true;
        bool monster_turn = false;

        void battleRewards(); //Victory
        void battleRetreat(); //Retreat
        void removeStatusesHelper(vector<Status> &vect, int target_duration);
        void fullDelete(Status*& list);
    public:
        Battle(Party *party, Monster *monster); //Constructor; pass memory addresses
        ~Battle(); //Destructor

        bool isMonsterIndex(int index){ return (index == -1);} //Update with more monsters per battle
        void addStatus(Effect new_effect, int target_index);
        void removeStatuses(int target_index, int target_duration);
        void updated_addStatuses(Effect new_effect, int target_index);
        void updated_removeStatuses(int target_index, int target_duration);
        int test();
        void displayEffects(string name, int health, vector<Status> vect);
        //void activateZeroDurationEffect(Effect effect, int target_index);
        void activateEffect(Status T);
        int playerCombat(); //Update this and monsterCombat
        int monsterCombat(); //Calculates how monster attacks
        int adjustAttack(int target_index, int attacker_index);

        //void activateEffect(Party &party, Monster &monster, Effect effect, int target, bool in_effect);
        void battleActivate(Party &party, Monster &monster, Effect effect, int target);
        void postBattleActivate(Party &party, Monster &monster);
};


//Update Battle class to use <Status> single link lists instead of <Status> vectors
//  - If necessary, use templates


//Ideas for effects to implement
/*
 *Effect that any damage taken from player heals monster instead
 *Replace "Crit Multiplier" effect with another; make crit innate to weapons and monsters (Ongoing)
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
        * IMPORTANT: Set up cases when effects activate DURING TURN (i.e. Enemy's HP effect)
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