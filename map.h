// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Map Class

#ifndef MAP_H
#define MAP_H

#include <iostream>

using namespace std;

//Map Max Constants
#define Max_NPC_Count 5
#define Max_Room_Count 5 //Max number of rooms (at a time)
#define Max_Skull_Count 5 //Count of skulls party needs to enter gate and fight the boss
#define Max_Hunt_Count 3 //Max number of monsters chasing party at given moment

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
        const char unexplored = '-'; //Marker for unexplored spaces
        const char explored = ' '; //Marker for explored spaces
        const char room = 'R'; //Marker for room locations
        const char npc = 'N'; //Marker for NPC locations
        const char party = 'X'; //Marker for party position
        const char gate = 'G'; //Marker for dungeon gate (boss encounter)
        const char hunt = 'H'; //Marker for monsters chasing the party

        static const int num_rows = 6; //Number of rows in map
        static const int num_cols = 24; //Number of columns in map
        static const int max_npcs = 5;  //Max NPC count
        static const int max_rooms = 5; //Max number of rooms (at a time)
        static const int max_skulls = 5; //Count of skulls party needs to enter gate and fight the boss
        static const int max_hunt = 3; //Max number of monsters chasing party at given moment

        int player_position[2]; //Player position (row,col)
        int dungeon_gate[2]; //Gate location of the dungeon
        int npc_positions[max_npcs][2]; //Stores the (row,col) positions of NPCs present on map
        bool npc_found[max_npcs]; //Checks if npc has been found/spotted
        int room_positions[max_rooms][2]; //Stores the (row,col) positions of rooms present on map
        int hunt_positions[max_hunt][2]; //Stores the (row,col) positions of monsters chasing party
        char map_data[num_rows][num_cols]; //Stores the character that will be shown at a given (row,col)

        int npc_count = 0; //Stores number of misfortunes currently on map
        int room_count = 0; //Stores number of sites currently on map
        int skull_count = 0; //Stores skulls party collected by beating rooms
        int hunt_count = 0; //Stores how many monsters are chasing party
    public:
        Map();
        Map(int new_row, int new_cols, int set_max_npc, int set_max_rooms, int set_max_skulls,
        int set_max_hunt);

        void resetMap();

        //Getters
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

        //Setters
        void setPlayerPosition(int row, int col);
        void setDungeonGate(int row, int col);
        //void setHuntLocation(int row, int col);
        //void setSkullCount(int new_count);

        //Other
        void displayMap();
        bool move(char direction);
        bool addNPC(int row, int col);
        bool addRoom(int row, int col);
        //bool addHunt(int row, int col);
        bool removeNPC(int row, int col);
        bool removeRoom(int row, int col);
        //bool removeHunt(int row, int col);
        void exploreSpace(int row, int col);
        //void moveHunt(int row, int col, int hunt_index); //Idea: Maybe a DFS traversal
};

#endif