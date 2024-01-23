// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Item Implementation

#include <iostream>
//#include <fstream>
#include "item.h"

using namespace std;


//Effect Class Implementation

string Effect::getEffectName(){ return effect_name;}

int Effect::getEffectValue(){ return effect_value;}

int Effect::getEffectChance(){ return effect_procChance;}

int Effect::getEffectDuration(){ return effect_duration;}

Effect::Effect(){
    effect_name = invalid_effect;
    effect_value = 0;
    effect_procChance = 0;
    effect_duration = 0;
}

Effect::Effect(string name, int value, int chance, int duration){
    effect_name = name;
    effect_value = value;
    effect_procChance = chance;
    effect_duration = duration;
}

void Effect::setEffectName(string new_name){
    effect_name = new_name;

    return;
}

void Effect::setEffectValue(int new_value){
    effect_value = new_value;

    return;
}

void Effect::setEffectChance(int new_chance){
    effect_procChance = new_chance;

    return;
}

void Effect::setEffectDuration(int new_duration){
    effect_duration = new_duration;

    return;
}


//Item Class Implementation

Item::Item(){
    item_name = default_item_name;
    item_type = "";
    item_cost = 0;
    item_stats = 0;
    crit_chance = 0;
    crit_boost = 0;
    weapon_effect = Effect();
}

Item::Item(string name, string type, int cost, int stat, int critChance, int critBoost,
string effectName, int effectValue, int effectChance, int effectDuration){
    item_name = name;
    item_type = type;
    item_cost = cost;
    item_stats = stat;

    crit_chance = critChance;
    crit_boost = critBoost;

    weapon_effect = Effect(effectName, effectValue, effectChance, effectDuration);
}

//Setters
void Item::setItemName(string new_name){
    item_name = new_name;
    
    return;
}

void Item::setItemType(string new_type){
    item_type = new_type;

    return;
}

void Item::setCost(int new_cost){
    if (new_cost > 0){ //Checks input
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