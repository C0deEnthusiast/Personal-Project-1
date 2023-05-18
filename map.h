// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Map Class

#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include "monster.h"

using namespace std;

//Map Max Constants
#define default_Max_NPC 5
#define default_Max_Room 5 //Max number of rooms (at a time)
#define default_Skull_Count 5 //Count of skulls party needs to enter gate and fight the boss
#define default_Max_Hunt_Count 3 //Max number of monsters chasing party at given moment

#define default_Rows 6
#define default_Cols 24

//Movement
#define dirNorth "w"
#define dirSouth "s"
#define dirWest "a"
#define dirEast "d"

//Diagonal Movement
#define dirNorthWest "wa"
#define dirNorthEast "wd"
#define dirSouthWest "sa"
#define dirSouthEast "sd"


class Map{
    private:
        //Map grid Markers
        const char unexplored = '-'; //Unexplored spaces
        const char explored = ' '; //Explored spaces
        const char room = 'R'; //Room locations
        const char npc = 'N'; //NPC locations
        const char party = 'X'; //Party position
        const char gate = 'G'; //Dungeon gate (Boss encounter)
        const char hunt = 'H'; //Monsters chasing the party

        //Maximums and Constants
        int num_rows; //Number of rows in map
        int num_cols; //Number of columns in map
        int max_npcs; //Max NPC count
        int max_rooms; //Max number of rooms (at a time)
        int max_skulls; //Count of skulls party needs to enter gate and fight the boss        
        int max_hunt; //Max number of monsters chasing party at given moment

        //Dynamic arrays
        char** map_grid; //Stores the character that will be shown at a given (row,col)
        bool* npc_found; //Checks if npc has been found/spotted

        //Syntax: arr[2] -> [0] = Row position, [1] = Column position
        int* player_position; //Player position
        int* dungeon_gate; //Gate location in dungeon
        //Syntax: arr[-][2] -> [0] = Row position, [1] = Column position
        int** npc_positions; //Stores the (row,col) positions of NPCs present on map
        int** room_positions; //Stores the (row,col) positions of rooms present on map
        int** hunt_positions; //Stores the (row,col) positions of monsters chasing party
        
        //Ever-changing
        int npc_count = 0; //Stores number of misfortunes currently on map
        int room_count = 0; //Stores number of sites currently on map
        int skull_count = 0; //Stores skulls party collected by beating rooms
        int hunt_count = 0; //Stores how many monsters are chasing party

        const char separator = ','; //Standard delimiter for monster text files
        static const int monster_info_count = 10; //Accounts all data sections per monster
        /*Increases after every consecutive use of monsterRush() after constructor call

        During battle/encounters, it would increment the monster's rating, adjusting
        rewards as well as monster's damage reduction*/
        int raise_rating;
        vector<Monster> refillList; //Designed to skip most of monsterRush() function
        vector<Monster> monsterList;
        string monsterFilename;
    public:
        Map(string filename, int new_row = default_Rows, int new_cols = default_Cols, 
        int set_max_npc = default_Max_NPC, int set_max_rooms = default_Max_Room,
        int set_max_hunt = default_Max_Hunt_Count); //Constructor
        ~Map(); //Destructor

        void resetMap();

        //Getters related to Map grid
        int getMaxRoom(){ return max_rooms;}
        int getMaxNPC(){ return max_npcs;}
        int getMaxSkulls(){ return max_skulls;}
        int getMaxHunt(){ return max_hunt;}
        int getPlayerRow(){ return player_position[0];}
        int getPlayerCol(){ return player_position[1];}
        int getDungeonGateRow(){ return dungeon_gate[0];}
        int getDungeonGateCol(){ return dungeon_gate[1];}
        int getRoomCount(){ return room_count;}
        int getNPCCount(){ return npc_count;}
        int getSkullCount(){ return skull_count;}
        int getHuntCount(){ return hunt_count;}
        int getNumRows(){ return num_rows;}
        int getNumCols(){ return num_cols;}

        //Checks Map Locations
        bool isOnMap(int row, int col);
        bool isNPCLocation(int row, int col);
        bool isRoomLocation(int row, int col);
        bool isExplored(int row, int col);
        bool isDungeonGate(int row, int col);
        bool isFreeSpace(int row, int col);
        //bool isHunt(int row, int col);

        //Setters related to map grid
        void setPlayerPosition(int row, int col);
        void setDungeonGate(int row, int col);
        //void setHuntLocation(int row, int col);
        //void setSkullCount(int new_count);

        //Adds components to map grid
        bool addNPC(int row, int col);
        bool addRoom(int row, int col);
        //bool addHunt(int row, int col);

        //Removes components from map grid
        bool removeNPC(int row, int col);
        bool removeRoom(int row, int col);
        //bool removeHunt(int row, int col);

        //Other functions related to map grid
        void displayMap();
        bool move(char direction);
        void exploreSpace(int row, int col);
        //void moveHunt(int row, int col, int hunt_index); //Idea: Maybe a DFS traversal

        //Functions related to Monsters
        int getMonsterCount(){ return monsterList.size();}
        void generateMonsterRoster(string filename, bool new_rush = true); //Fills monsterList
        void setRaiseRating(int new_raise);
        void removeMonster(int index);
        int countMonsterRating(int rating);

        //bool monsterAmbush() //Will randomly trigger encounters

        
        int encounter(Party& party, bool room = false); //Sets up parameters for Battle Class
        Monster returnMonster(int rating, int &monster_index);
        bool bossIsOnlyOneAlive();
        int ratingReconfiguration(int &original_rating);
};

#endif