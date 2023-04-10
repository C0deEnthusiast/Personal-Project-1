// CSCI 1300 Fall 2022
// Author: Rodolfo Martinez-Maldonado
// Recitation: 303 – Xuefei Sun
// Project 3 - Player Implementation

#include <iostream>
#include "player.h"

using namespace std;

Player::Player(){
    name = "";
    health = defaultPlayerHealth;
    leader = false;
}

Player::Player(string new_name, int new_health, bool leader_status){
    name = new_name;
    //Checks for valid health input (i.e. health that won't immediately kill player)
    if (new_health > 0 && new_health <= defaultPlayerHealth){
        health = new_health;
    } else {
        health = defaultPlayerHealth; //Resets health
    }
    leader = leader_status;
}

//Getters
string Player::getPlayerName(){
    return name;
}

int Player::getPlayerHealth(){
    return health;
}

bool Player::getLeaderStatus(){
    return leader;
}

/*Item Player::getEquippedWeapon(){
    return equipped_weapon;
}

Item Player::getEquippedArmor(){
    return equipped_armor;
}*/

//Setters
void Player::setPlayerName(string new_name){
    name = new_name;

    return;
}

void Player::setPlayerHealth(int new_health){
    health = new_health;

    //Changess health if outside of bounds
    if (health > defaultPlayerHealth){
        health = defaultPlayerHealth;
    } else if (health <= 0){
        health = 0;
    }

    return;
}

void Player::setLeaderStatus(bool new_status){
    leader = new_status;

    return;
}

/*void Player::setEquippedWeapon(Item item){
    equipped_weapon = item;

    return;
}

void Player::setEquippedArmor(Item item){
    equipped_armor = item;
    
    return;
}*/