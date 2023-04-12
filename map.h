// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Map Class

#ifndef MAP_H
#define MAP_H

#include <iostream>

using namespace std;

class Map{
    private:
        const char UNEXPLORED = '-'; //Marker for unexplored spaces
        const char EXPLORED = ' '; //Marker for explored spaces
        const char ROOM = 'R'; //Marker for room locations
        const char NPC = 'N'; //Marker for NPC locations
        const char PARTY = 'X'; //Marker for party position
        const char GATE = 'G'; //Marker for dungeon gate (boss encounter)
        const char HUNT = 'H'; //Marker for monsters chasing the party

        static const int num_rows_ = 6; //Number of rows in map
        static const int num_cols_ = 24; //Number of columns in map
        static const int max_npcs_ = 5;  //Max NPC count
        static const int max_rooms_ = 5; //Max number of rooms
        static const int max_skulls = 5; //Count of skulls party needs to enter gate and fight the boss

        int player_position_[2]; //Player position (row,col)
        int dungeon_gate_[2]; //Gate location of the dungeon
        int npc_positions_[max_npcs_][3]; //Stores the (row,col) positions of NPCs present on map and if they have been found
        int room_positions_[max_rooms_][2]; //Stores the (row,col) positions of rooms present on map
        //
        char map_data_[num_rows_][num_cols_]; // stores the character that will be shown at a given (row,col)

        int npc_count_; //Stores number of misfortunes currently on map
        int room_count_; //Stores number of sites currently on map
        int skulls_count_; //Stores skulls party collected by beating rooms
        int hunt_count_ = 5; //Stores how many monsters are chasing party
    public:
        Map();

        void resetMap();

        // getters
        int getMaxRoom();
        int getMaxNPC();
        int getPlayerRow();
        int getPlayerCol();
        int getDungeonExitRow();
        int getDungeonExitCol();
        int getRoomCount();
        int getNPCCount();
        int getNumRows();
        int getNumCols();
        bool isOnMap(int row, int col);
        bool isNPCLocation(int row, int col);
        bool isRoomLocation(int row, int col);
        bool isExplored(int row, int col);
        bool isFreeSpace(int row, int col);
        bool isDungeonExit(int row, int col);

        // setters
        void setPlayerPosition(int row, int col);
        void setDungeonExit(int row, int col);

        // other
        void displayMap();
        bool move(char);
        bool addNPC(int row, int col);
        bool addRoom(int row, int col);
        bool removeNPC(int row, int col);
        bool removeRoom(int row, int col);
        void exploreSpace(int row, int col);
};

#endif