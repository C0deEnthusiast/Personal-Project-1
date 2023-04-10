// CSCI 1300 Fall 2022
// Author: Rodolfo Martinez-Maldonado
// Recitation: 303 – Xuefei Sun
// Project 3 - Monster Implementation

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sstream>
#include "monster.h"

using namespace std;


Monster::Monster(){
    monster_name = "NaM";
    difficultyRating = 0;
    attack_power = 0;
    monster_health = 0;
    power = Effect();
}

Monster::Monster(string name, int rating, int attkPwr, int health, string effect, int effectValue,
int effectChance, int effectDuration){
    monster_name = name;
    difficultyRating = rating;
    attack_power = attkPwr;
    monster_health = health;
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
        
        vector<string> monster; //Temporary vector
        istringstream splice(line);

        while(getline(splice,line,separator)){
            monster.push_back(line);
        }

        if (monster.size() == monster_info_count){
            Monster create(monster.at(0),stoi(monster.at(1)),stoi(monster.at(2)),
            stoi(monster.at(3)),monster.at(4),stoi(monster.at(5)),stoi(monster.at(6)),stoi(monster.at(7)));
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


/*Algorithm: Dictate monster fights
    1) By default, room will be -1 so that in accidental encounters, rating of monster would be 1
    2) Creates random values for later
    3) Chosen rating is initialized by room
    4) Chooses random monster index
    5) Initializes minimum bound and maximum bound so that
    monsters found in member arrays have same rating as chosen rating
    6) Through loop, a random monster with specific rating is chosen
    9) Announces monster and gives player chance to fight or surrender
        Note: For cases where no weapons, this would occur outside function
    10) Exits if player surrenders
    11) Calculates battle outcome based on first three parameters and random values from Step 2
    12) If player wins (>0), returns rating of monster AND removes monster
    13) If player loses, returns -2
    Parameters: int weapon_stats, int armor, int variety, int room = ""
    Returns:
    -1 if player surrounders
    -2 if player loses
    Monster's rating if player wins*/
int Combat::monsterEncounter(Party& party, bool room){
    if (party.getCurrentWeaponCapacity() == 0){ //Party doesn't have any weapons
        return -1;
    }

    Monster temp;

    //Battle calculation Values
    int rating, vulnerability = 1, armor = party.getCurrentArmorCapacity(), variety = 4, battle = 0;

    //Monster name and rating
    //string chosen_monster = "", chosen_;
    int chosen_rating = 0, search_index = 0, remove_index = 0, random_monster;
    //Jesus,6,10,1000,Wrath,5,3
    string monst_name = "";
    int monst_rating;

    //Random variables for battle outcome
    int num1 = Functions().createRand(1,2); //1-2
    int num2 = Functions().createRand(temp.getMinRating(),temp.getMaxRating()-1); //1-6

    //Determines monster rating
    if (room){ //Currently at room location
        rating = party.getExploredRooms() + 2;
        party.setExploredRooms(party.getExploredRooms() + 1); //Increments Party Value
    } else { //Random Encounter from either Investigate or Pick a Fight
        rating = num2;
    }

    //Vulnerability increases the less armor party has
    //This equation is meant to punish players for going without armor
    vulnerability = temp.getMaxRating() - armor;

    if (armor <= 0){ //This is to minimize exceptions which terminate the program
        armor = 1;
    }

    if (countMonsterRating(rating) > 0){ //There is at least one monster with chosen rating
        //Picks random monster with chosen rating
        random_monster = Functions().createRand(1,countMonsterRating(rating));
    } else { //Creates a monster if all monsters with rating are dead
        /*monst_name = "Wrathful Spectre";
        monst_rating = rating;
        remove_index = -1;*/
    }

    //Picks monster that hasn't been removed, and has chosen rating
    /*for (int i = temp.getMinRating(); i <= temp.getMaxRating(); i++){
        cout << "Check monster count rating of " << i << ": " << countMonsterRating(i) << endl;
    }*/
    for (int i = 0; i < monsterList.size(); i++){
        if(monsterList[i].difficultyRating == rating){
            search_index++;
            
            if(search_index == random_monster){
                //Monster is alive and is randomly chosen
                temp = monsterList[i];
                remove_index = i;
                break;
            }
        }
    }

    //Announces monster
    cout << temp.monster_name << " (Health: " << temp.monster_health;
    cout << ") (Rating: "<< temp.difficultyRating << ") ahead! Keep all eyes open!" << endl;

    //Allows player to fight or surrender
    cout << endl;
    cout << "Fight(1) or surrender(2)? Your call." << endl;
    string choose = "";
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
    battle = num1 + variety - (vulnerability * num2 * monst_rating)/armor;
    battle+=1000000;
    cout << "Printed Battle Calculation (So as to see how to make game harder)" << endl;
    cout << num1 << " + " << variety << " - ";
    cout << vulnerability << " X " << num2 << " X " << monst_rating << " / " << armor;
    cout << " + 1000000";
    cout << " = " << battle << endl;

    cout << "(Rand_1 * Combined Weapon Stats) + Variety";
    cout << "- (Vulnerability * Rand_2 * Monster Rating)/Armor" << endl;

    if (battle > 0){ //Victory
        removeMonster(remove_index);
        cout << "Test removeMonster" << endl;
        if (monsterList.size() == 1 && countMonsterRating(temp.getMaxRating()) == 1){
            removeMonster(0); //Removes 
            cout << "monsterRush should happen by now" << endl;
            return -10;
        }
        return battleVictory(party,monst_name,monst_rating, Functions().createRand(1,100));
    } else { //Defeat
        int rand1 = Functions().createRand(0,party.getPlayerSize());
        int rand2 = Functions().createRand(0,100);
        return battleDefeat(party, rand1, rand2);
    }
}

void Combat::displayMonsterEffect(Effect monsterEffect){
    cout << "    " << monsterEffect.getEffectName() << ": ";

    if (monsterEffect.getEffectName() == effect_Wrath){
        cout << "Has " << monsterEffect.getEffectChance();
        cout << "% chance to temporary boost its own attack by " << monsterEffect.getEffectValue() << ".";
    } else if (monsterEffect.getEffectName() == effect_Bleed){
        cout << "Inflicts " << monsterEffect.getEffectValue();
        cout << " damage onto target player every turn. Lasts " << monsterEffect.getEffectDuration();
        cout << " turns.";
    } else if (monsterEffect.getEffectName() == effect_Rampage){
        cout << "Has " << monsterEffect.getEffectChance();
        cout << "% chance to strike " << monsterEffect.getEffectValue();
        cout << " times per turn.";
    } else if (monsterEffect.getEffectName() == effect_Condemnation){
        cout << "Has " << monsterEffect.getEffectChance();
        cout << "% chance to confuse target into attacking teammates. Lasts ";
        cout << monsterEffect.getEffectDuration() << " turns.";
    } else if (monsterEffect.getEffectName() == effect_Savage_Wrath){
        cout << "Permanently boosts attack by " << monsterEffect.getEffectValue();
        cout << " every turn; has " << monsterEffect.getEffectChance();
        cout << "% chance to lose " << monsterEffect.getEffectValue() << " health.";
    } else if (monsterEffect.getEffectName() == effect_Wicked___){
        cout << "Complete effect";
    } else if (monsterEffect.getEffectName() == effect_Unholy_Judgement){
        cout << "Targeted player of attack will be instantly killed after "; 
        cout << monsterEffect.getEffectDuration() << " turns.";
    } else if (monsterEffect.getEffectName() == effect_Final_Requiem){ //Revise effect
        cout << "Exclusive to Boss Monster; stuns ALL party members, then kills them";
    }
    cout << endl;

    return;
}

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
    int rating, vulnerability = 1, armor = party.getCurrentArmorCapacity(), battle = 0;
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

    //Vulnerability increases the less armor party has
    //This equation is meant to punish players for going without armor
    vulnerability = temp.getMaxRating() - armor;

    if (armor <= 0){ //This is to minimize exceptions which terminate the program
        armor = 1;
    }

    //Monster is chosen
    temp = returnMonster(rating, remove_index);

    //For testing
    /*cout << "Check monster count ratings: ";
    for (int i = temp.getMinRating(); i <= temp.getMaxRating(); i++){
        cout << i << "(" << countMonsterRating(i) << ") -- ";
    }
    cout << endl;*/

    //Announces monster
    cout << temp.monster_name << " (Health: " << temp.monster_health;
    cout << ") (Rating: "<< temp.difficultyRating << ") ahead! Keep all eyes open!" << endl;
    displayMonsterEffect(temp.power);

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
    battle = (vulnerability * temp.difficultyRating)/armor;
    battle+=1000000;

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
        return battleDefeat(party,rand1,rand2);
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
        cout << "Attack: " << curParty->getWeapon(i).getStat() << endl;
    }

    displayEffects(curMonster->monster_name, curMonster->monster_health, monster_0);
    cout << "Monster attack: " << curMonster->attack_power << endl;

    return 0;
}

bool Battle::willActivate(int effect_chance){
    if (effect_chance == 100){ //Bypasses pseudo-random input
        return true;
    } else if (effect_chance >= Functions().createRand(1,100)){
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