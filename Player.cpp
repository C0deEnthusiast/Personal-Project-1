// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Player Class Implementation

#include <iostream>
#include "player.h"
#include "item.h"

using namespace std;

Player::Player(){
    name = "";
    health = defaultPlayerHealth;
    leader = false;
}

Player::Player(string new_name, int new_health, bool new_leader_status){
    name = new_name;
    //Checks for valid health input (i.e. health that won't immediately kill player)
    health = ((new_health > 0 && new_health <= defaultPlayerHealth) ? new_health : defaultPlayerHealth);
    leader = new_leader_status;
}

//Setters
void Player::setPlayerName(string new_name){
    name = new_name;

    return;
}

void Player::setPlayerHealth(int new_health){
    health = new_health;

    //Changes health if outside of bounds
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

void Player::setEquippedWeapon(Item item){
    equipped_weapon = item;

    return;
}

void Player::setEquippedArmor(Item item){
    equipped_armor = item;
    
    return;
}