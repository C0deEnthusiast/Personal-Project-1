// CSCI 1300 Fall 2022
// Author: Rodolfo Martinez-Maldonado
// Recitation: 303 – Xuefei Sun
// Project 3 - Player Class

#include <iostream>
#include <vector>
#include "item.h"

#ifndef PLAYER_H
#define PLAYER_H

using namespace std;

class Player{
    private:
        string name;
        int health;
        //Currently not important; maybe will implement a leader effect
        bool leader; //False for four companions
        /*Item equipped_weapon;
        Item equipped_armor;*/
    public:
        Player();
        Player(string new_name, int new_health, bool leader);

        //Getters
        string getPlayerName();
        int getPlayerHealth();
        bool getLeaderStatus();
        /*Item getEquippedWeapon();
        Item getEquippedArmor();*/

        //Setters
        void setPlayerName(string new_name);
        void setPlayerHealth(int new_health); //Beta-testing
        void setLeaderStatus(bool new_status);
        /*void setEquippedWeapon(Item item);
        void setEquippedArmor(Item item);*/
};

#endif