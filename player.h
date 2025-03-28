// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Player Class

#include <iostream>
#include "item.h"

#ifndef PLAYER_H
#define PLAYER_H

//Defines default health of players
#define defaultPlayerHealth 100

#define invalidPlayer "Not a Player"

class Player{ //Consider whether to have Monster class inherit this
    private:
        std::string name;
        int health;
        //Currently not important; maybe will implement a leader effect
        bool leader; //False for companions
        Item equipped_weapon;
        Item equipped_armor;
    public:
        
        Player();
        Player(std::string new_name, int new_health, bool new_leader_status);

        //Getters
        std::string getPlayerName(){ return name;}
        int getPlayerHealth(){ return health;}
        bool getLeaderStatus(){ return leader;}
        Item getEquippedWeapon(){ return equipped_weapon;}
        Item getEquippedArmor(){ return equipped_armor;}

        //Setters
        void setPlayerName(std::string new_name);
        void setPlayerHealth(int new_health); //Beta-testing
        void setLeaderStatus(bool new_status);
        void setEquippedWeapon(Item item);
        void setEquippedArmor(Item item);
};

#endif