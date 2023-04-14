// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Map Implementation

#include <iostream>
#include <cstdlib>
#include "map.h"

using namespace std;

Map::Map(){
    resetMap();
}

//Resets positions of player, NPCs, and rooms and clears map_data[][]
void Map::resetMap(){
    //Resets player position, count values, and initializes values in position arrays to -1 (out of bounds)
    player_position[0] = 0;
    player_position[1] = 0;

    //Set dungeon gate at fixed point
    dungeon_gate[0] = num_rows - 1;
    dungeon_gate[1] = num_cols - 1;

    for (int i = 0; i < max_npcs; i++){
        npc_positions[i][0] = -1;
        npc_positions[i][1] = -1;
        npc_found[i] = false;
    }

    for (int i = 0; i < max_rooms; i++){
        room_positions[i][0] = -1;
        room_positions[i][1] = -1;
    }

    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++)
        {
            map_data[i][j] = UNEXPLORED;
        }
    }
    map_data[dungeon_gate[0]][dungeon_gate[1]] = GATE;
    return;
}

//Set player position, if in range
void Map::setPlayerPosition(int row, int col){
    if (isOnMap(row, col)){
        player_position[0] = row;
        player_position[1] = col;
    }
    return;
}

//Set dungeon exit position, if in range
void Map::setDungeonGate(int row, int col){
    if (isOnMap(row, col)){
        if (dungeon_gate[0] != -1 && dungeon_gate[1] != -1){ //Removes already present gate
            map_data[dungeon_gate[0]][dungeon_gate[1]] = UNEXPLORED;
        }
        dungeon_gate[0] = row;
        dungeon_gate[1] = col;

        map_data[dungeon_gate[0]][dungeon_gate[1]] = GATE;
    }
    return;
}

//Checks if (row,col) positions is on the map
bool Map::isOnMap(int row, int col){
    return (row >= 0 && row < num_rows && col >= 0 && col < num_cols);
}

//Checks if the location is an NPC location
bool Map::isNPCLocation(int row, int col){
    if (!isOnMap(row, col)){
        return false;
    }

    for (int i = 0; i < npc_count; i++){
        if (row == npc_positions[i][0] && col == npc_positions[i][1]){
            return true;
        }
    }
    return false;
}

//Checks if the location is an room location
bool Map::isRoomLocation(int row, int col){
    if (!isOnMap(row, col)){
        return false;
    }

    for (int i = 0; i < room_count; i++){
        if (row == room_positions[i][0] && col == room_positions[i][1]){
            return true;
        }
    }
    return false;
}

//Checks if the given row and column is an explored space
bool Map::isExplored(int row, int col){
    if (!isOnMap(row, col)){
        return false;
    }

    if (map_data[row][col] == EXPLORED){
        return true;
    }

    for (int i = 0; i < npc_count; i++){
        if (npc_positions[i][0] == row && npc_positions[i][1] == col){
            if (npc_found[i]){
                return true;
            }
            break;
        }
    }
    return false;
}


//Checks if location is the dungeon gate
bool Map::isDungeonGate(int row, int col){
    return (row == dungeon_gate[0] && col == dungeon_gate[1]);
}

//Checks if the given row and column on map is a free space
bool Map::isFreeSpace(int row, int col){
    if (!isOnMap(row, col)){
        return false;
    }

    if (isNPCLocation(row, col)){
        return false;
    }

    if (isRoomLocation(row, col)){
        return false;
    }

    if (isDungeonGate(row, col)){
        return false;
    }
    return true;
}

//Add NPC on the map grid
bool Map::addNPC(int row, int col){
    if (npc_count >= max_npcs){
        return false;
    }

    if (!isFreeSpace(row, col)){
        return false;
    }

    if (row == 0 && col == 0){ //Starting player location
        return false;
    }

    npc_positions[npc_count][0] = row;
    npc_positions[npc_count][1] = col;
    npc_found[npc_count] = false;
    map_data[row][col] = NPC;
    npc_count++;
    return true;
}

//Create a room on the map
bool Map::addRoom(int row, int col){

    if (room_count >= max_rooms){
        return false;
    }

    //Location must be blank to spawn
    if (!isFreeSpace(row, col)){
        return false;
    }

    if (row == player_position[0] && col == player_position[1]){ //Starting player location
        return false;
    }

    room_positions[room_count][0] = row;
    room_positions[room_count][1] = col;
    room_count++;
    map_data[row][col] = ROOM;
    return true;
}

//Removes the NPC at (row, col) from the map
bool Map::removeNPC(int row, int col){
    for (int i = 0; i < npc_count; i++){
        if (npc_positions[i][0] == row && npc_positions[i][1] == col){
            // swap i'th npc with last npc
            npc_positions[i][0] = npc_positions[npc_count - 1][0];
            npc_positions[i][1] = npc_positions[npc_count - 1][1];
            npc_found[i] = npc_found[npc_count - 1];
            // reset last npc
            npc_positions[npc_count - 1][0] = -1;
            npc_positions[npc_count - 1][1] = -1;
            npc_found[npc_count - 1] = false;
            // decrement npc_count_
            npc_count--;
            // set map data to explored
            map_data[row][col] = EXPLORED;
            return true;
        }
    }
    return false;
}

//Removes the room at (row, col) from the map
bool Map::removeRoom(int row, int col){
    for (int i = 0; i < room_count; i++){
        if (room_positions[i][0] == row && room_positions[i][1] == col){
            // swap i'th room with last room
            room_positions[i][0] = room_positions[room_count - 1][0];
            room_positions[i][1] = room_positions[room_count - 1][1];
            // reset last room
            room_positions[room_count - 1][0] = -1;
            room_positions[room_count - 1][1] = -1;
            // decrement room_count_
            room_count--;
            // set map data to explored
            map_data[row][col] = EXPLORED;
            return true;
        }
    }
    return false;
}


//Mark (row, col) as explored, either revealing NPC or empty space
void Map::exploreSpace(int row, int col){
    for (int i = 0; i < npc_count; i++){
        if (row == npc_positions[i][0] && col == npc_positions[i][1]){
            // mark NPC as found
            npc_found[i] = true;
            return;
        }
    }

    if (isFreeSpace(row, col)){
        map_data[row][col] = EXPLORED;
    }
}


//Make the player move based on the given command
bool Map::move(char direction){
    switch (tolower(direction)){
    case 'w': //Move up if it is an allowed move
        if (player_position[0] > 0){
            player_position[0] -= 1;
        } else {
            return false;
        }
        break;
    case 's': //Move down if it is an allowed move
        if (player_position[0] < num_rows - 1){
            player_position[0] += 1;
        } else {
            return false;
        }
        break;
    case 'a': //Move left if it is an allowed move
        if (player_position[1] > 0){
            player_position[1] -= 1;
        } else {
            return false;
        }
        break;
    case 'd': //Move right if it is an allowed move
        if (player_position[1] < num_cols - 1){
            player_position[1] += 1;
        } else {
            return false;
        }
        break;
    default:
        return false;
    }
    // if new location is an NPC location, mark as explored
    if (isNPCLocation(player_position[0], player_position[1])){
        exploreSpace(player_position[0], player_position[1]);
    }
    return true;
}

//Prints the map_data[] grid
void Map::displayMap(){
    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            if (player_position[0] == i && player_position[1] == j){
                cout << PARTY;
            } else if (map_data[i][j] == 'N') { //NPC location, have to check if they were found yet
                for (int k = 0; k < npc_count; k++){
                    if (npc_positions[k][0] == i && npc_positions[k][1] == j){
                        if (npc_found[k]){
                            cout << NPC;
                        } else {
                            cout << UNEXPLORED;
                        }
                    }
                }
            } else {
                cout << map_data[i][j];
            }
        }
        cout << endl;
    }
}