// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Player Class

#include <iostream>
#include "item.h"

#ifndef PLAYER_H
#define PLAYER_H

//Defines default health of players
#define defaultPlayerHealth 100

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