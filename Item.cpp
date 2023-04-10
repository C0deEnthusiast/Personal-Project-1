// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Item Implementation

#include <iostream>
#include <fstream>
#include "item.h"

using namespace std;

Item::Item(){
    item_name = default_item_name;
    item_type = "";
    item_cost = 0;
    item_stats = 0;
    weapon_effect = Effect();
}

/*Item::Item(string name, string type, int cost, int stat,
string effectName, int effectValue, int effectChance, int effectDuration){
    item_name = name;
    item_type = type;
    item_cost = cost;
    item_stats = stat;

    weapon_effect = Effect(effectName, effectValue, effectChance, effectDuration);
}*/

Item::Item(string name, string type, int cost, int stat, int critChance, int critBoost,
string effectName, int effectValue, int effectChance, int effectDuration){
    item_name = name;
    item_type = type;
    item_cost = cost;
    item_stats = stat;

    weapon_effect = Effect(effectName, effectValue, effectChance, effectDuration);

    crit_chance = critChance;
    crit_boost = critBoost;
}

//Getters
string Item::getItemName(){
    return item_name;
}

string Item::getItemType(){
    return item_type;
}

int Item::getCost(){
    return item_cost;
}

int Item::getStat(){ //breakage or upgrade_level
    return item_stats;
}

int Item::getCritChance(){
    return crit_chance;
}

int Item::getCritBoost(){
    return crit_boost;
}

//Setters
void Item::setItemName(string new_name){
    item_name = new_name;

    return;
}

void Item::setItemType(string new_type){
    //Checks for valid type input
    if (new_type == "weapon" || new_type == "cookware" || new_type == "treasure"){
        item_type = new_type;
    }

    return;
}

void Item::setCost(int new_cost){
    //Checks input
    if (new_cost > 0){
        item_cost = new_cost;
    }

    return;
}

void Item::setStat(int value){
    item_stats = value;

    return;
}

void Item::setCritChance(int value){
    crit_chance = value;

    return;
}

void Item::setCritBoost(int value){
    crit_boost = value;

    return;
}