// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Monster Implementation

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include "monster.h"
#include "party.h"
#include "player.h"
#include "item.h"

using namespace std;


Monster::Monster(){
    monster_name = "NaM";
    difficultyRating = 0;
    attack_power = 0;
    monster_health = 0;
    crit_chance = 0;
    crit_boost = 0;
    power = Effect();
}

Monster::Monster(string name, int rating, int attkPwr, int health, int critChance, int critBoost,
string effect, int effectValue, int effectChance, int effectDuration){
    monster_name = name;
    difficultyRating = rating;
    attack_power = attkPwr;
    monster_health = health;
    crit_chance = critChance;
    crit_boost = critBoost;
    power.setEffectName(effect);
    power.setEffectValue(effectValue);
    power.setEffectChance(effectChance);
    power.setEffectDuration(effectDuration);
}

void Monster::setRating(int new_rating){
    difficultyRating = new_rating;

    if (difficultyRating < minimumRating){
        difficultyRating = minimumRating;
    } else if (difficultyRating > bossRating){
        difficultyRating = bossRating;
    }
}

int Battle::test(){
    //Test
    for (int i = 0; i < Functions::createRand(1,3); i++){
        addStatus(curMonster->power,-1);
    }
    for (int i = 0; i < Functions::createRand(2,3); i++){
        addStatus(curMonster->power, 0);
    }
    for (int i = 0; i < Functions::createRand(1,3); i++){
        addStatus(curMonster->power, 1);
    }
    for (int i = 0; i < Functions::createRand(2,3); i++){
        addStatus(curMonster->power, 2);
    }
    for (int i = 0; i < Functions::createRand(1,3); i++){
        addStatus(curMonster->power, 3);
    }
    for (int i = 0; i < Functions::createRand(1,3); i++){
        addStatus(curMonster->power, 4);
    }
    //End of testing

    removeStatuses(-1,0);
    removeStatuses(0,0);
    removeStatuses(1,0);
    removeStatuses(2,0);
    removeStatuses(3,0);
    removeStatuses(4,0);

    for (auto& x: playerStatuses[0]){
        activateEffect(x);
    }
    for (auto& x: playerStatuses[1]){
        activateEffect(x);
    }
    for (auto& x: playerStatuses[2]){
        activateEffect(x);
    }
    for (auto& x: playerStatuses[3]){
        activateEffect(x);
    }
    for (auto& x: playerStatuses[4]){
        activateEffect(x);
    }
    for (auto& x: monster_0){
        activateEffect(x);
    }

    Player arr[5] = {curParty->getPlayer(0), curParty->getPlayer(1),
    curParty->getPlayer(2), curParty->getPlayer(3), curParty->getPlayer(4)};

    for (int i = 0; i < 5; i++){
        displayEffects(arr[i].getPlayerName(), arr[i].getPlayerHealth(), playerStatuses[i]);
        cout << "Attack: " << arr[i].getEquippedWeapon().getStat() << endl;
    }

    displayEffects(curMonster->monster_name, curMonster->monster_health, monster_0);
    cout << "Monster attack: " << curMonster->attack_power << endl;

    cout << "\nTesting AdjustAttack():\nBefore: " << altered_weapons[0].getStat();
    int temp = adjustAttack(0,0);
    cout << "\nAfter: " << temp << endl;

    return 0;
}

void Battle::activateEffect(Status T){
    int target = T.effect_target;
    //Effect effect = T.power;

    bool atMaxDuration = (T.power.getEffectDuration() == T.max_duration);
    bool atEnd = (T.power.getEffectDuration() == 0);

    //Bleed and Burn (both behave identically)
    if (T.power.getEffectName() == effect_Bleed || T.power.getEffectName() == effect_Burn){
        if (curParty->isPlayerIndex(target)){ //Targets player
            if (!player_immuneToDMG[target]){
                curParty->modifyPlayerHealth(target,-T.power.getEffectValue());
            }
        } else if (target == -1){ //Targets player
            curMonster->monster_health -= T.power.getEffectValue();
        }
    }
    
    //Unholy Judgement (activates when effect ends)
    if (T.power.getEffectName() == effect_Unholy_Judgement && atEnd){
        //Sets health to 0 when Unholy Judgement ends
        if (player_turn && curParty->isPlayerIndex(target)){ //Targets player on their turn
            if (!player_immuneToDMG[target]){
                curParty->modifyPlayerHealth(target, -curParty->getPlayer(target).getPlayerHealth());
            }
        } else if (isMonsterIndex(target) && monster_turn){ //Targets monster on their turn
            curMonster->monster_health = 0;
        }
    }

    //Wrath and Savage Wrath
    if (T.power.getEffectName() == effect_Wrath || T.power.getEffectName() == effect_Savage_Wrath){
        //cout << "Activating Wrath" << endl;
        if (!(atMaxDuration || atEnd)){ //Neither activated or at end of duration
            return;
        }
        //cout << "Modifying Attack" << endl;

        int changeAtk = T.power.getEffectValue();

        if (T.power.getEffectName() == effect_Wrath && atEnd){ //Removes attack boost if it is normal Wrath
            cout << "Removing Wrath" << endl;
            changeAtk = -changeAtk;
        }

        bool depleteHealth = false;

        //Savage Wrath's negative side effect
        if (T.power.getEffectName() == effect_Savage_Wrath && atMaxDuration){
            if (Functions::willOccur(T.power.getEffectChance())){
                depleteHealth = true;
            }
        }

        if (curParty->isPlayerIndex(target)){ //Targets player
            //curParty->modifyWeaponAttack(target, changeAtk);
            altered_weapons[target].setStat(altered_weapons[target].getStat() + changeAtk);
            if (depleteHealth){
                curParty->modifyPlayerHealth(target, -changeAtk);
            }
        } else if (isMonsterIndex(target)){ //Targets monster
            curMonster->attack_power += changeAtk;
            if (depleteHealth){
                curMonster->monster_health -= changeAtk;
            }
        }
    }

    //Condemnation
    if (T.power.getEffectName() == effect_Condemnation){
        if (isMonsterIndex(target)){ //Will NOT activate on monster (for now)
            return;
        }
        if (atMaxDuration && player_turn && curParty->isPlayerIndex(target)){ //Applies charm
            player_charmed[target] = true;
        } else if (atEnd && player_turn && curParty->isPlayerIndex(target)){ //Removes charm
            player_charmed[target] = false;
        }
    }

    //Enemy Current Health Damage
    if (T.power.getEffectName() == effect_EnemyHP){
        double curHealth;
        if (curParty->isPlayerIndex(target)){
            if (player_immuneToDMG[target]){
                return;
            }
            curHealth = static_cast <double> (curParty->getPlayer(target).getPlayerHealth());
        } else if (isMonsterIndex(target)){
            curHealth = static_cast <double> (curMonster->monster_health);
        }

        //double healthDMG = (effect.getEffectValue() / 100) * curHealth; //Determines damage
        curHealth *= (T.power.getEffectValue() / 100); //Determines damage

        if (curParty->isPlayerIndex(target)){
            curParty->modifyPlayerHealth(target, -curHealth);
        } else if (isMonsterIndex(target)){
           curMonster->monster_health -= curHealth;

        }
    }

    //Freeze
    if (T.power.getEffectName() == effect_Freeze){
        if (atMaxDuration){ //Applies freeze
            if (curParty->isPlayerIndex(target)){
                player_active[target] = false;
            } else if (isMonsterIndex(target)){
                monster_active = false;
            }
        } else if (atEnd){ //Removes freeze
            if (curParty->isPlayerIndex(target)){
                player_active[target] = true;
            } else if (isMonsterIndex(target)){
                monster_active = true;
            }
        }
    }

    //Heal
    if (T.power.getEffectName() == effect_Heal){
        //Heal will NOT work on dead monster/players
        if (curParty->isPlayerIndex(target)){
            if (curParty->getPlayer(target).getPlayerHealth() > 0){
                curParty->modifyPlayerHealth(target, T.power.getEffectValue());
            }
        } else if (isMonsterIndex(target) && curMonster->monster_health > 0){
            curMonster->monster_health += T.power.getEffectValue();
        }
    }

    //Pierce activates here; defensesUp changes to true in adjustAttack() function
    if (T.power.getEffectName() == effect_Pierce){
        if (curParty->isPlayerIndex(target)){
            player_defensesUp[target] = false;
        } else if (isMonsterIndex(target)){
            monster_defensesUp = false;
        }
    }

    //Attacking more than once
    if (T.power.getEffectName() == effect_Rage || T.power.getEffectName() == effect_Rampage){
        if (atMaxDuration){ //Applies Rage or Rampage
            if (curParty->isPlayerIndex(target)){
                player_attack_max[target] = T.power.getEffectValue();
            } else if (isMonsterIndex(target)){
                monster_attack_max = T.power.getEffectValue();
            }
        } else if (atEnd){ //Removes effect
            if (curParty->isPlayerIndex(target)){
                player_attack_max[target] = 1;
            } else if (isMonsterIndex(target)){
                monster_attack_max = 1;
            }
        }
    }

    if (T.power.getEffectName() == effect_Resurrection){
        if (curParty->isPlayerIndex(target)){
            if (curParty->getPlayer(target).getPlayerHealth() == 0){
                curParty->modifyPlayerHealth(target, defaultPlayerHealth);
            }
        } else if (isMonsterIndex(target)){
            //Applies to boss monster only
            if (curMonster->difficultyRating == bossRating && curMonster->monster_health == 0){
                curMonster->monster_health = bossFinalHealth;
            }
        }
    }

    if (T.power.getEffectName() == effect_Undying && curParty->isPlayerIndex(target)){
        if (atMaxDuration){
            player_immuneToDMG[target] = true;
        } else if (atEnd) {
            player_immuneToDMG[target] = false;
        }
    }

    if (T.power.getEffectName() == effect_Godslayer){
        //
    }

    if (T.power.getEffectName() == effect_Final_Requiem){
        //
    }

    return;
}

//Update this and monsterCombat
int Battle::playerCombat(){
    return 0;
}

//Calculates how monster attacks
int Battle::monsterCombat(){
    return 0;
}

/*Note: Monster will not attack itself

Adjusts attack in following order:
 - Target's defenses
 - Attacker's attack power*/
int Battle::adjustAttack(int target_index, int attacker_index){
    double curAttack = 0;

    if (curParty->isPlayerIndex(attacker_index)){
        curAttack = altered_weapons[attacker_index].getStat();
    } else if (isMonsterIndex(target_index)){
        curAttack = temp_monster.attack_power;
    }

    if (curAttack == 0){
        return 0;
    }


    //Adjusts starting with target's defenses
    if (curParty->isPlayerIndex(target_index)){
        if (player_immuneToDMG[target_index]){ //Doesn't take damage
            return 0;
        }
        if (player_defensesUp[target_index]){ //Accounts for armor IF defensesUp is true
            double player_armor_value = curParty->getPlayer(target_index).getEquippedArmor().getStat();
            //curAttack *= (1 - Functions::percentToDecimal(player_armor_value));
            curAttack *= (1 - static_cast<double> (player_armor_value) / 100);
        } else { //Bypasses armor
            player_defensesUp[target_index] = false;
        }
        if (player_blocking[target_index]){ //Player is blocking
            //curAttack *= (1 - Functions::percentToDecimal(block_reduction));
            curAttack *= (1 - static_cast<double> (block_reduction) / 100);
        }
    } else if (isMonsterIndex(target_index)){
        if (monster_defensesUp){ //Accounts for armor IF defensesUp is true
            //curAttack *= (1 - Functions::percentToDecimal(monster_innate_armor));
            curAttack *= (1 - static_cast<double> (monster_innate_armor) / 100);
        } else { //Bypasses armor
            monster_defensesUp = false;
        }
    }


    //Adjusts by attacker's stats
    if (curParty->isPlayerIndex(attacker_index)){
        if (Functions::willOccur(altered_weapons[attacker_index].getCritChance())){
            curAttack *= (1 + Functions::percentToDecimal(altered_weapons[attacker_index].getCritBoost()));
        }
    } else if (isMonsterIndex(attacker_index)){
        if (Functions::willOccur(temp_monster.crit_chance)){
            curAttack *= (1 + Functions::percentToDecimal(temp_monster.crit_boost));
        }
    }


    return round(curAttack);
}