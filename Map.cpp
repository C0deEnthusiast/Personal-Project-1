// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Map Implementation

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "map.h"
#include "functions.h"

using namespace std;

Map::Map(string filename, int new_row, int new_cols, int set_max_npc, int set_max_rooms,
 int set_max_hunt){
    raise_rating = 0;
    monsterFilename = filename;
    generateMonsterRoster(filename, false);

    //Adjusts rows, columns, NPCs, rooms, and hunting groups if invalid
    num_rows = new_row;
    if (num_rows <= 0){
        num_rows = default_Rows;
    }
    num_cols = new_cols;
    if (num_cols <= 0){
        num_cols = default_Cols;
    }
    max_npcs = set_max_npc;
    if (max_npcs < 0){
        max_npcs = default_Max_NPC;
    }
    max_rooms = set_max_rooms;
    if (max_rooms < 0){
        max_rooms = default_Max_Room;
    }
    max_hunt = set_max_hunt;
    if (max_hunt < 0){
        max_hunt = default_Max_Hunt_Count;
    }

    max_skulls = max_rooms;

    

    /*
    Checks if new dimensions for the grid can support player, gate, NPCs, rooms, and monsters.
    If it does not, changes dimensions to square grid to support it
    */
    if ((num_rows * num_cols) <= (2 + max_npcs + max_rooms + max_hunt)){
        int length = ceil(pow(2 + max_npcs + max_rooms + max_hunt, 1/2));
        num_rows = length;
        num_cols = length;
    }

    resetMap(); //Generates new map
}

//Resets positions of player, NPCs, and rooms and clears map grid
void Map::resetMap(){
    //Sets player at fixed point
    player_position = new int[2];
    player_position[0] = 0;
    player_position[1] = 0;

    //Sets dungeon gate at fixed point
    dungeon_gate = new int[2];
    dungeon_gate[0] = num_rows - 1;
    dungeon_gate[1] = num_cols - 1;

    //Creates NPC arrays
    npc_positions = new int*[max_npcs];
    npc_found = new bool[max_npcs];
    for (int i = 0; i < max_npcs; i++){
        npc_positions[i] = new int[2];
        //Sets NPC out of bounds
        npc_positions[i][0] = -1;
        npc_positions[i][1] = -1;
        //Sets NPC out of sight
        npc_found[i] = false;
    }

    //Creates room array
    room_positions = new int*[max_rooms];
    for (int i = 0; i < max_rooms; i++){
        room_positions[i] = new int[2];
        //Sets room out of bounds
        room_positions[i][0] = -1;
        room_positions[i][1] = -1;
    }

    //Creates hunt arrays
    hunt_positions = new int*[max_hunt];
    for (int i = 0; i < max_hunt; i++){
        hunt_positions[i] = new int[2];
        //Sets hunting group out of bounds
        hunt_positions[i][0] = -1;
        hunt_positions[i][1] = -1;
    }

    //Creates map grid
    map_grid = new char*[num_rows];
    for (int i = 0; i < num_rows; i++){
        map_grid[i] = new char[num_cols];

        for (int j = 0; j < num_cols; j++){
            map_grid[i][j] = unexplored;
        }
    }

    //Sets dungeon gate on map grid
    map_grid[dungeon_gate[0]][dungeon_gate[1]] = gate;

    return;
}

Map::~Map(){ //Destructor
    //Deletes player position
    delete[] player_position;
    player_position = nullptr;

    //Deletes dungeon gate position
    delete[] dungeon_gate;
    dungeon_gate = nullptr;

    //Deletes NPC arrays
    for (int i = 0; i < max_npcs; i++){
        delete[] npc_positions[i];
    }
    delete[] npc_positions;
    npc_positions = nullptr;

    delete[] npc_found;
    npc_found = nullptr;

    //Deletes room array
    for (int i = 0; i < max_rooms; i++){
        delete[] room_positions[i];
    }
    delete[] room_positions;
    room_positions = nullptr;

    //Deletes hunt array
    for (int i = 0; i < max_hunt; i++){
        delete[] hunt_positions[i];
    }
    delete[] hunt_positions;
    hunt_positions = nullptr;

    //Deletes map grid
    for (int i = 0; i < num_rows; i++){
        delete[] map_grid[i];
    }
    delete[] map_grid;
    map_grid = nullptr;

    //Clears monster vectors/lists
    monsterList.clear();
    refillList.clear();
}

//Checks Map Locations

//Checks if (row, col) positions is on the map
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

    if (map_grid[row][col] == explored){
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

//Setters

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
    if (!isOnMap(row, col)){
        return;
    }
    
    if (dungeon_gate[0] != -1 && dungeon_gate[1] != -1){ //Removes already present gate
        map_grid[dungeon_gate[0]][dungeon_gate[1]] = unexplored;
    }
    dungeon_gate[0] = row;
    dungeon_gate[1] = col;

    map_grid[dungeon_gate[0]][dungeon_gate[1]] = gate;
    return;
}

//Others

//Prints the map_data grid
void Map::displayMap(){
    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            if (i == player_position[0] && j == player_position[1]){
                cout << party;
            } else if (map_grid[i][j] == npc) { //NPC location, have to check if they were found yet
                for (int k = 0; k < npc_count; k++){
                    if (npc_positions[k][0] == i && npc_positions[k][1] == j){
                        cout << (npc_found[k] ? npc : unexplored);
                    }
                }
            } else {
                cout << map_grid[i][j];
            }
        }
        cout << "\n";
    }
}

//Make the player move (consider diagonal movement)
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

//Add NPC on the map grid
bool Map::addNPC(int row, int col){
    if (npc_count >= max_npcs){
        return false;
    }

    if (!isFreeSpace(row, col)){
        return false;
    }

    if (row == player_position[0] && col == player_position[1]){ //Player location
        return false;
    }

    npc_positions[npc_count][0] = row;
    npc_positions[npc_count][1] = col;
    npc_found[npc_count] = false; //Disguises NPC
    map_grid[row][col] = npc;
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

    if (row == player_position[0] && col == player_position[1]){ //Doesn't spawn on party
        return false;
    }

    room_positions[room_count][0] = row;
    room_positions[room_count][1] = col;
    map_grid[row][col] = room;
    room_count++;
    return true;
}

//Removes the NPC at (row, col) from the map
bool Map::removeNPC(int row, int col){
    for (int i = 0; i < npc_count; i++){
        if (npc_positions[i][0] == row && npc_positions[i][1] == col){
            //Swaps current position with position of last NPC
            npc_positions[i][0] = npc_positions[npc_count - 1][0];
            npc_positions[i][1] = npc_positions[npc_count - 1][1];
            npc_found[i] = npc_found[npc_count - 1];
            //Last NPC is removed
            npc_positions[npc_count - 1][0] = -1;
            npc_positions[npc_count - 1][1] = -1;
            npc_found[npc_count - 1] = false;

            npc_count--;
            map_grid[row][col] = explored;
            return true;
        }
    }
    return false;
}

//Removes the room at (row, col) from the map
bool Map::removeRoom(int row, int col){
    for (int i = 0; i < room_count; i++){
        if (room_positions[i][0] == row && room_positions[i][1] == col){
            //Swaps current position with position of last room
            room_positions[i][0] = room_positions[room_count - 1][0];
            room_positions[i][1] = room_positions[room_count - 1][1];
            //Last room is removed
            room_positions[room_count - 1][0] = -1;
            room_positions[room_count - 1][1] = -1;

            room_count--;
            map_grid[row][col] = explored;
            return true;
        }
    }
    return false;
}


//Mark (row, col) as explored, either revealing NPC or empty space
void Map::exploreSpace(int row, int col){
    for (int i = 0; i < npc_count; i++){
        if (row == npc_positions[i][0] && col == npc_positions[i][1]){
            npc_found[i] = true;
            return;
        }
    }

    if (isFreeSpace(row, col)){
        map_grid[row][col] = explored;
    }

    return;
}

void Map::generateMonsterRoster(string filename, bool new_rush){
    //If all monsters are defeated, they will be 'resurrected' with higher ratings
    if (new_rush && refillList.size() > 0){
        setRaiseRating(++raise_rating);
        cout << "Bloody hell! They're getting back up!" << endl;
        monsterList = refillList;
        return;
    }

    //Opens monster file
    ifstream file(filename);

    if (!file.is_open()){
        cout << "File not open" << endl;
        return;
    }

    string line;

    while(getline(file,line)){
        
        vector<string> temp; //Temporary vector
        istringstream splice(line);

        while(getline(splice,line,separator)){
            temp.push_back(line);
        }

        if (temp.size() == monster_info_count){
            Effect create_effect(temp.at(6),stoi(temp.at(7)),stoi(temp.at(8)), stoi(temp.at(9)));
            Monster create(temp.at(0),stoi(temp.at(1)),stoi(temp.at(2)),stoi(temp.at(3)),
            stoi(temp.at(4)),stoi(temp.at(5)), create_effect);

            monsterList.push_back(create);
            refillList.push_back(create);
        }
    }
    file.close(); //Closes file

    return;
}

void Map::setRaiseRating(int new_raise){
    raise_rating = new_raise;
    
    return;
}

//Removes monster from roster
void Map::removeMonster(int index){
    if (index < 0 || index >= static_cast<int> (monsterList.size())){
        return;
    }
    monsterList.erase(monsterList.begin() + index);

    return;
}

int Map::countMonsterRating(int rating){
    int count = 0;
    for (const auto x: monsterList){
        if (x.difficulty_rating == rating){
            count++;
        }
    }

    return count;
}


/*
-1 if surrender
-2 if loss
Monster's rating if win
*/
int Map::encounter(Party& party, bool room){
    if (party.getCurrentWeaponCapacity() == 0){ //Party doesn't have any weapons
        return -1;
    }

    Monster temp;
    //Battle calculation Values
    int rating, battle = 0;
    int chosen_rating = 0, search_index = 0, remove_index = 0, random_monster;

    //Random variables for battle outcome
    int rand_rating = Functions::createRand(minimumRating, bossRating - 1); //1-6

    //Determines monster rating
    if (room){ //Currently at room location
        //Boss is encountered only at 5th room, with final form (Rating of 8) at final phase
        rating = party.getExploredRooms() + 2;
        party.setExploredRooms(party.getExploredRooms() + 1); //Increments Party Value
    } else { //Random Encounter from either Investigate or Pick a Fight
        rating = rand_rating;
    }

    //Monster is chosen
    temp = returnMonster(rating, remove_index);
    //Raises rating based on raise_rating
    temp.setRating(temp.difficulty_rating + raise_rating);

    //For testing
    cout << "Check monster count ratings: ";
    for (int i = minimumRating; i <= bossRating; i++){
        cout << i << "(" << countMonsterRating(i) << ") -- ";
    }
    cout << endl;

    //Announces monster
    cout << temp.monster_name << " (Health: " << temp.monster_health;
    cout << ") (Rating: "<< temp.difficulty_rating << ") ahead! Keep all eyes open!" << endl;
    displayEffect(temp.power);

    //Allows player to fight or surrender
    cout << "\nFight(1) or surrender(2)? Your call.\n";
    string choose = "";
    //choose = "1"; //Debugging only
    do {
        getline(cin,choose);
        if (choose != "1" && choose != "2"){
            cout << "Make a bloody choice!" << endl;
        }
    } while (choose != "1" && choose != "2");

    if (choose == "2"){
        /*if (surrender){ //Player surrendered
            cout << "Foolish mortals! With surrender, comes sacrifice!" << endl;

            //Kills random player
            party.modifyPlayerHealth(kill_index, -100);
            if (party.getLivePlayerCount() == 0){
                return false;
            } else {
                return true;
            }
        }*/
        return -1;
    }

    //Calculates battle outcome
    battle = 1000000;

    //EffectList *system = new EffectList();
    Battle *system = new Battle(&party, &temp);
    cout << "system created" << endl;

    int te = system->test();

    //int test_combat = system->playerCombat(party,temp);
    delete system;
    system = nullptr;
    cout << "system destroyed" << endl;

    if (battle > 0){ //Victory
        removeMonster(remove_index);

        //Rewards (for now)
        //Money
        party.setMoney(party.getMoney() + 10 * temp.difficulty_rating);
        cout << "Congrats! You've slain " << temp.monster_name << "!";
        cout << " You won " << 10 * temp.difficulty_rating << " gold!\n\n";
        
        if (Functions::createRand(1,100) <= 10){ //Chance for key
            cout << "You got a key too!" << endl;
            party.setKeys(party.getKeys() + 1);
        }
        
        return true;
    } else { //Defeat
        party.setMoney(party.getMoney() * 3 / 4);
        cout << "One for all, and all will fall." << endl;
        for (int i = 1; i < party.getMaxPlayerSize(); i++){
            if (Functions::createRand(0,100) <= 10){
                //Kills player
                party.modifyPlayerHealth(i, -100);
                if (party.getLivingPlayerCount() == 0){
                    return false;
                }
            }
        }
        return true;
    }
}

Monster Map::returnMonster(int rating, int &monster_index){
    Monster pick;

    //If only the Boss is alive AND if player is not fighting the boss
    if (bossIsOnlyOneAlive() && rating != bossRating){
        //cout << "Renews monster roster" << endl;
        monsterList.clear(); //Removes Boss
        generateMonsterRoster(monsterFilename); //Creates monster roster again
    }

    int search = 0, remove_index = 0, random_monster = -1;

    if (countMonsterRating(rating) == 0){
        //Recurs function until there is monster that is alive
        return returnMonster(ratingReconfiguration(rating), monster_index);
    } else {
        random_monster = Functions::createRand(1,countMonsterRating(rating));
    }

    //Picks monster that hasn't been removed, and has chosen rating
    for (int i = 0; i < monsterList.size(); i++){
        if(monsterList[i].difficulty_rating == rating){
            if(++search == random_monster){
                monster_index = i;
                return monsterList[i];
            }
        }
    }

    return pick;
}

//Checks if boss has not yet to be defeated while others have
bool Map::bossIsOnlyOneAlive(){
    return (countMonsterRating(bossRating) == 1 && getMonsterCount() == 1);
}

/*Reconfigures monster rating by first checking if there are monsters with a rating BELOW
given rating; if so, then it returns the rating. If not, then it searches for monsters
with rating ABOVE given rating; if so, returns the rating. Otherwise, returns original rating*/
int Map::ratingReconfiguration(int &original_rating){
    int old_rating = original_rating;
    //Monster temp;
    while (old_rating >= minimumRating){
        if (countMonsterRating(--original_rating) > 0){
            return original_rating;
        }
    }

    original_rating = old_rating;

    while (old_rating < bossRating){
        if (countMonsterRating(++original_rating) > 0){
            return original_rating;
        }
    }

    return original_rating;
}