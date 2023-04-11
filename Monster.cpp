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

/*Monster::Monster(string name, int rating, int attkPwr, int health, string effect, int effectValue,
int effectChance, int effectDuration){
    monster_name = name;
    difficultyRating = rating;
    attack_power = attkPwr;
    monster_health = health;
    power.setEffectName(effect);
    power.setEffectValue(effectValue);
    power.setEffectChance(effectChance);
    power.setEffectDuration(effectDuration);
}*/

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

int Monster::getMinRating(){
    return min_rating;
}

int Monster::getMaxRating(){
    return max_rating;
}

Combat::Combat(string filename){
    //monster_size = 0;
    raise_rating = 0;
    monsterFilename = filename;
    monsterRush(monsterFilename,false);
}

/*Algorithm: Fills names and ratings array with every monster in txt file*/
void Combat::monsterRush(string filename, bool new_rush){
    //If all monsters are defeated, they will be 'resurrected' with higher ratings
    if (new_rush && refillList.size() > 0){
        setRaiseRating(++raise_rating);
        cout << "Bloody hell! They're getting back up!" << endl;
        monsterList = refillList;
        return;
    }

    //Opens monster file
    ifstream file(filename);

    if (!file.is_open()){
        cout << "File not open" << endl;
        return;
    }

    string line;

    while(getline(file,line)){
        
        vector<string> temp; //Temporary vector
        istringstream splice(line);

        while(getline(splice,line,separator)){
            temp.push_back(line);
        }

        if (temp.size() == monster_info_count){
            /*Monster create(monster.at(0),stoi(monster.at(1)),stoi(monster.at(2)),
            stoi(monster.at(3)),monster.at(4),stoi(monster.at(5)),stoi(monster.at(6)),stoi(monster.at(7)));*/
            Monster create(temp.at(0),stoi(temp.at(1)),stoi(temp.at(2)),stoi(temp.at(3)),
            stoi(temp.at(4)),stoi(temp.at(5)),
            temp.at(6),stoi(temp.at(7)),stoi(temp.at(8)), stoi(temp.at(9)));

            monsterList.push_back(create);
            refillList.push_back(create);
        }
    }
    file.close(); //Closes file

    return;
}

int Combat::getMonsterSize(){
    return monsterList.size();
}

void Combat::setRaiseRating(int new_raise){
    raise_rating = new_raise;

    return;
}


/*Returns:
    -1 if player surrounders
    -2 if player loses
    Monster's rating if player wins*/

//Removes monster from data member vectors IF there are monsters remaining
void Combat::removeMonster(int index){
    if (index == -1){
        return;
    }
    monsterList.erase(monsterList.begin() + index);

    return;
}

//Returns count of monsters with chosen rating
int Combat::countMonsterRating(int rating){
    int count = 0;
    for (int i = 0; i < monsterList.size(); i++){
        if (monsterList[i].difficultyRating == rating){
            count++;
        }
    }

    return count;
}

//Rewards player with gold (dependent on monster rating) and potentially a key; always returns true
bool Combat::battleVictory(Party& party,string monster_name, int monster_rating, int key_chance){
    party.setMoney(party.getMoney() + 10 * monster_rating);
    //Make function that potentially gives player a random potion
    //cout << endl;
    cout << "Congrats! You've slain " << monster_name << "!";
    cout << " You won " << 10 * monster_rating << " gold and " << 5 * monster_rating << " food!" << endl;
    cout << endl;

    if (key_chance <= 10){ //Chance for key
        cout << "You got a key too!" << endl;
        party.setKeys(party.getKeys() + 1);
    }

    return true;
}

/*Algorithm: Penalizes player based on either they surrendered or lost
    1) If player surrendered, kills one player randomly
    2) If player lost, kills one player if health_loss_chance is within 10% threshold;
    player also loses some money and food
    Returns: false if MAIN player/leader is dead; true otherwise
*/
bool Combat::battleDefeat(Party& party, int kill_index, int health_loss_chance, bool surrender){
    if (surrender){ //Player surrendered
        cout << "Foolish mortals! With surrender, comes sacrifice!" << endl;

        //Kills random player
        return party.changePlayerHealth(kill_index,-100);
    } else { //Player loses against fighting monster
        party.setMoney(party.getMoney()/4);
        cout << "One for all, and all will fall." << endl;
        for (int i = 1; i < party.getPlayerSize(); i++){
            if (health_loss_chance <= 10){
                //Kills player
                if (!party.changePlayerHealth(i,-100)){
                    return false;
                }
            }
        }
    }

    return true;
}

/*
-1 if surrender
-2 if loss
Monster's rating if win
*/
int Combat::encounter(Party& party, bool room){
    if (party.getCurrentWeaponCapacity() == 0){ //Party doesn't have any weapons
        return -1;
    }

    Monster temp;
    //Battle calculation Values
    int rating, armor = party.getCurrentArmorCapacity(), battle = 0;
    int chosen_rating = 0, search_index = 0, remove_index = 0, random_monster;

    //Random variables for battle outcome
    int rand_rating = Functions().createRand(temp.getMinRating(),temp.getMaxRating()-1); //1-6

    //Determines monster rating
    if (room){ //Currently at room location
        rating = party.getExploredRooms() + 2;
        party.setExploredRooms(party.getExploredRooms() + 1); //Increments Party Value
    } else { //Random Encounter from either Investigate or Pick a Fight
        rating = rand_rating;
    }

    if (armor <= 0){ //This is to minimize exceptions which terminate the program
        armor = 1;
    }

    //Monster is chosen
    temp = returnMonster(rating, remove_index);
    //Raises rating based on raise_rating
    temp.setRating(temp.difficultyRating + raise_rating);

    //For testing
    /*cout << "Check monster count ratings: ";
    for (int i = temp.getMinRating(); i <= temp.getMaxRating(); i++){
        cout << i << "(" << countMonsterRating(i) << ") -- ";
    }
    cout << endl;*/

    //Announces monster
    cout << temp.monster_name << " (Health: " << temp.monster_health;
    cout << ") (Rating: "<< temp.difficultyRating << ") ahead! Keep all eyes open!" << endl;
    Functions().displayEffect(temp.power);

    //Allows player to fight or surrender
    cout << endl;
    cout << "Fight(1) or surrender(2)? Your call." << endl;
    string choose = "";
    //choose = "1"; //Debugging only
    do {
        getline(cin,choose);
        if (choose != "1" && choose != "2"){
            cout << "Make a bloody choice!" << endl;
        }
    } while (choose != "1" && choose != "2");

    if (choose == "2"){
        return -1;
    }

    //Calculates battle outcome
    battle = 1000000;

    //EffectList *system = new EffectList();
    Battle *system = new Battle(&party, &temp);
    cout << "system created" << endl;

    int te = system->test();

    //int test_combat = system->playerCombat(party,temp);
    delete system;
    system = nullptr;
    cout << "system destroyed" << endl;

    if (battle > 0){ //Victory
        removeMonster(remove_index);
        
        return battleVictory(party,temp.monster_name,temp.difficultyRating,Functions().createRand(1,100));
    } else { //Defeat
        int rand1 = Functions().createRand(0,party.getPlayerSize());
        int rand2 = Functions().createRand(0,100);
        return battleDefeat(party, rand1, rand2);
    }
}

Monster Combat::returnMonster(int rating, int &monster_index){
    Monster pick;

    //If only the Boss is alive AND if player is not fighting the boss
    if (bossIsOnlyOneAlive() && rating != pick.getMaxRating()){
        //cout << "Renews monster roster" << endl;
        removeMonster(0); //Removes Boss
        monsterRush(monsterFilename); //Creates monster roster again
    }

    int search = 0, remove_index = 0, random_monster = -1;

    if (countMonsterRating(rating) == 0){
        //Recurs function until there is monster that is alive
        return returnMonster(ratingReconfiguration(rating), monster_index);
    } else {
        random_monster = Functions().createRand(1,countMonsterRating(rating));
    }

    //Picks monster that hasn't been removed, and has chosen rating
    for (int i = 0; i < monsterList.size(); i++){
        if(monsterList[i].difficultyRating == rating){
            if(++search == random_monster){
                monster_index = i;
                return monsterList[i];
            }
        }
    }

    return pick;
}

//Checks if boss has not yet to be defeated while others have
bool Combat::bossIsOnlyOneAlive(){
    return (countMonsterRating(Monster().getMaxRating()) == 1 && getMonsterSize() == 1);
}

/*Reconfigures monster rating by first checking if there are monsters with a rating BELOW
given rating; if so, then it returns the rating. If not, then it searches for monsters
with rating ABOVE given rating; if so, returns the rating. Otherwise, returns original rating*/
int Combat::ratingReconfiguration(int &og_rating){
    int original_rating = og_rating;
    Monster temp;
    while (og_rating >= temp.getMinRating()){
        if (countMonsterRating(--og_rating) > 0){
            return og_rating;
        }
    }

    og_rating = original_rating;

    while (og_rating < temp.getMaxRating()){
        if (countMonsterRating(++og_rating) > 0){
            return og_rating;
        }
    }

    return original_rating;
}

int Battle::test(){
    //Test
    for (int i = 0; i < Functions().createRand(1,3); i++){
        addStatus(curMonster->power,-1);
    }
    for (int i = 0; i < Functions().createRand(2,3); i++){
        addStatus(curMonster->power, 0);
    }
    for (int i = 0; i < Functions().createRand(1,3); i++){
        addStatus(curMonster->power, 1);
    }
    for (int i = 0; i < Functions().createRand(2,3); i++){
        addStatus(curMonster->power, 2);
    }
    for (int i = 0; i < Functions().createRand(1,3); i++){
        addStatus(curMonster->power, 3);
    }
    for (int i = 0; i < Functions().createRand(1,3); i++){
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
        cout << "Attack: " << curParty->getWeapon(i).getItemStat() << endl;
    }

    displayEffects(curMonster->monster_name, curMonster->monster_health, monster_0);
    cout << "Monster attack: " << curMonster->attack_power << endl;

    return 0;
}

bool Battle::willActivate(int chance){
    if (chance == 100){ //Bypasses pseudo-random input
        return true;
    } else if (chance >= Functions().createRand(1,100)){
        return true;
    }
    return false;
}

void Battle::activateEffect(Status effectData){
    int target = effectData.effect_target;
    Effect effect = effectData.power;

    bool atMaxDuration = (effect.getEffectDuration() == effectData.max_duration);
    bool atEnd = (effect.getEffectDuration() == 0);

    //Bleed and Burn (both behave identically)
    if (effect.getEffectName() == effect_Bleed || effect.getEffectName() == effect_Burn){
        if (target == -1){ //Targets monster
            curMonster->monster_health -= effect.getEffectValue();
        } else if (!player_immuneToDMG[target]) { //Targets player (if they can take damage)
            curParty->modifyPlayerHealth(target,-effect.getEffectValue());
        }
    }
    
    //Unholy Judgement
    if (effect.getEffectName() == effect_Unholy_Judgement && atEnd){
        //Sets health to 0 when Unholy Judgement ends
        if (target == -1 && monster_turn){
            curMonster->monster_health = 0;
        } else if (player_turn && !player_immuneToDMG[target]){
            curParty->modifyPlayerHealth(target, -curParty->getPlayer(target).getPlayerHealth());
        }
    }

    //Wrath and Savage Wrath
    if (effect.getEffectName() == effect_Wrath || effect.getEffectName() == effect_Savage_Wrath){
        cout << "Activating Wrath" << endl;
        if (!(atMaxDuration || atEnd)){ //Neither activated or at end of duration
            return;
        }
        cout << "Modifying Attack" << endl;

        int changeAtk = effect.getEffectValue();

        if (effect.getEffectName() == effect_Wrath && atEnd){ //Removes attack boost if it is normal Wrath
            cout << "Removing Wrath" << endl;
            changeAtk = -changeAtk;
        }

        bool depleteHealth = false;

        //Savage Wrath's negative side effect
        if (effect.getEffectName() == effect_Savage_Wrath == willActivate(effect.getEffectChance())){
            depleteHealth = true;
        }

        if (target == -1){ //Targets monster
            curMonster->attack_power += changeAtk;
            if (depleteHealth){
                curMonster->monster_health -= changeAtk;
            }
        } else { //Targets player
            curParty->modifyWeaponAttack(target, changeAtk);
            if (depleteHealth){
                curParty->modifyPlayerHealth(target, -changeAtk);
            }
        }
    }

    //Condemnation
    if (effect.getEffectName() == effect_Condemnation){ //Will not activate on monster
        if (atMaxDuration && player_turn){
            player_charmed[target] = true;
        } else if (atEnd && player_turn){
            player_charmed[target] = false;
        }
    }

    //Enemy Current Health Damage
    if (effect.getEffectName() == effect_EnemyHP){
        double curHealth;
        if (target == -1){
            curHealth = static_cast <double> (curMonster->monster_health);
        } else {
            if (player_immuneToDMG[target]){
                return;
            }
            curHealth = static_cast <double> (curParty->getPlayer(target).getPlayerHealth());
        }

        double healthDMG = (effect.getEffectValue() / 100) * curHealth; //Determines damage

        if (target == -1){
            curMonster->monster_health -= healthDMG;
        } else {
            curParty->modifyPlayerHealth(target, -healthDMG);
        }
    }

    //Freeze
    if (effect.getEffectName() == effect_Freeze){
        if (atMaxDuration){ //Applies freeze
            if (target == -1){
                monster_active = false;
            } else {
                player_active[target] = false;
            }
        } else if (atEnd){ //Removes freeze
            if (target == -1){
                monster_active = true;
            } else {
                player_active[target] = true;
            }
        }
    }

    //Heal
    if (effect.getEffectName() == effect_Heal){
        //Heal will NOT work on dead monster/players
        if (target == -1 && curMonster->monster_health != 0){
            curMonster->monster_health += effect.getEffectValue();
        } else if (curParty->getPlayer(target).getPlayerHealth() != 0){
            curParty->modifyPlayerHealth(target, effect.getEffectValue());
        }
    }

    //Pierce activates here; after attack (seperate) occurs, bool is manually changed to true
    if (effect.getEffectName() == effect_Pierce){
        if (target == -1){
            monster_defensesUp = false;
        } else {
            player_defensesUp[target] = false;
        }
    }

    //Attacking more than once
    if (effect.getEffectName() == effect_Rage || effect.getEffectName() == effect_Rampage){
        if (atMaxDuration){ //Applies Rage or Rampage
            if (target == -1){
                monster_attack_max = effect.getEffectValue();
            } else {
                player_attack_max[target] = effect.getEffectValue();
            }
        } else if (atEnd){ //Removes effect
            if (target == -1){
                monster_attack_max = 1;
            } else {
                player_attack_max[target] = 1;
            }
        }
    }

    if (effect.getEffectName() == effect_Resurrection){
        //Applies to boss monster
        if (target == -1 && curMonster->difficultyRating == bossRating && curMonster->monster_health == 0){
            curMonster->monster_health = bossFinalHealth;
        } else if (curParty->getPlayer(target).getPlayerHealth() == 0){ //Applies to players
            curParty->modifyPlayerHealth(target, defaultPlayerHealth);
        }
    }

    if (effect.getEffectName() == effect_Undying && target != -1){
        if (atMaxDuration){
            player_immuneToDMG[target] = true;
        } else {
            player_immuneToDMG[target] = false;
        }
    }

    if (effect.getEffectName() == effect_Godslayer){
        //
    }

    if (effect.getEffectName() == effect_Final_Requiem){
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

//Note: Monster will not attack itself
int Battle::adjustAttack(int base_attack, int target_index, int attacker_index){
    double curAttack = base_attack;

    if (target_index == -1){
        //Damage Reduction
        if (monster_defensesUp){ //Accounts for armor IF defensesUp is true
            curAttack *= (1 - (static_cast<double> (monster_innate_armor) / 100));
        } else { //Bypasses armor
            monster_defensesUp = false;
        }

        //Damage Boost
        if (willActivate(curParty->getWeapon(attacker_index).getCritChance())){
            curAttack *= (1.0 + curParty->getWeapon(attacker_index).getCritBoost());
        }
    } else {
        //Damage Reduction
        if (player_immuneToDMG[target_index]){ //Doesn't take damage
            return 0;
        }
        if (player_defensesUp[target_index]){ //Accounts for armor IF defensesUp is true
            curAttack *= (1 - (static_cast<double> (curParty->getWeapon(target_index).getItemStat()) / 100));
        } else { //Bypasses armor
            player_defensesUp[target_index] = false;
        }
        if (player_blocking[target_index]){ //Player is blocking
            curAttack *= (1 - (static_cast<double> (block_reduction) / 100));
        }

        //Damage Boost
        if (attacker_index == -1 && willActivate(curMonster->crit_chance)){
            curAttack *= (1.0 + curMonster->crit_boost);
        } else if (willActivate(curParty->getWeapon(attacker_index).getCritChance())){
            curAttack *= (1.0 + curParty->getWeapon(attacker_index).getCritBoost());
        }
    }
    //Deducts based on target's armor
    return (int)round(curAttack);
}