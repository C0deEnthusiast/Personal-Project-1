// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Dungeon Adventure Game

//Shortcut: g++ -std=c++17 dungeonAdventure.cpp Map.cpp Item.cpp Party.cpp Player.cpp Monster.cpp -o game
//Compile with with this: ./game items.txt monsters.txt story.txt

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "party.h"
#include "monster.h"
#include "map.h"
#include "functions.h"

//using std::cout;

using namespace std;

//Announces user's defeat; ends game
bool gameLose(int scenario = 0){ //0 is when player gives up
    if (scenario == -1){ //God's patience reached zero
        cout << "You know what? You've ran my patience to the ground!" << endl;
        cout << "Begone worms!" << endl;
    } else if (scenario == -2){ //Main player died
        cout << "You know, I am quite disappointed. I expected more from you." << endl;
    } else if (scenario == -3){ //All players die
        cout << "How depressing. I expected more." << endl;
    }
    cout << endl;
    cout << "Game Over" << endl;
    return false;
}

//Announces user's victory; ends game
bool gameWin(){
    cout << "Congrats! You've made my time worthwhile!" << endl;
    return false;
}

char cardinalMove(){
    string direction = "";
    bool cardinal = false;

    cout << "Pick a cardinal direction using: " << endl;
    cout << "w (up), a (left), s (down), d (right)" << endl;
    
    do {
        getline(cin,direction);

        //Checks for valid input
        if (direction == "w" || direction == "a" || direction == "s" || direction == "d"){
            cardinal = true;
        } else {
            cout << "Valid cardinal direction please." << endl;
        }
    } while (!cardinal);

    return direction[0];
}

int main(int argc, char *argv[]){
    if (argc != 4){
        cout << "Need in order:\n";
        cout << "{Program} {Item Text File} {Monster Text File} {Story Text File}\n";
        return 0;
    }

    srand(time(0)); //Refreshes pseudo-random generator every time application starts

    Map dungeon(argv[2]);
    Party party(argv[1]); //User interacts with this

    bool game = true; //Keeps game running
    string player; //For player names
    string skip; //For exposition

    //These arrays are to make it easier to maintain displaying text
    int firstLineList[] = {0,3,7,13,15,18};
    int lastLineList[] = {2,6,12,14,17,30};

    vector<string> read = Functions::copyFile(argv[3]);

    cout << "???:" << endl;
    //Introduction exposition; creates 3 rooms
    for (int i = 0; i < 3; i++){
        for (int line = firstLineList[i]; line <= lastLineList[i]; line++){
            cout << read.at(line) << endl;
        }
        //read(firstLineList[i],lastLineList[i]);
        Functions::convenientStop();
        cout << endl;
    }

    cout << "Wonderful, now pick a leader.\n(Enter a name; don't waste my godly patience)\n";

    do { //Checks for valid name inputs
        getline(cin,player);
        if (player.length() == 0){
            cout << "Really?\nAgain.\n";
        }
    } while (player.length() == 0);
    party.addPlayer(player);

    cout << "\nOh, so the leader is " << player << ". How peachy\n";
    cout << "How about your 4 comrades?\n(Enter their names)\n";
    for (int i = 0; i < 4; i++){
        player = ""; //Resets string
        cout << "\nEnter your teammate's name.\n";
        do { //Checks for valid name inputs
            getline(cin,player);
            if (player.length() == 0){
                cout << "Really?\nAgain.\n";
            }
        } while (player.length() == 0);
        party.addPlayer(player);
    }





    //Remove this later; testing only
    party.setMoney(1000000);
    int val = 2;
    party.addItems(party.returnItem("Diamond circlet"), val);
    party.setKeys(3);
    //party.addItem(party.returnItem("Blighted Dagger")); //Debugging only
    /*party.merchant(); //Testing
    for (int i = 0; i < 23; i++){ //23
        if (fight.encounter(party) == -1){
            party.showPartyStatus();
            return 0;
        }
    }*/
    //return 0;
    //End of removal






    //Room Generation
    for (int i = 0; i < dungeon.getMaxRoom(); i++){
        int row = Functions::createRand(0,dungeon.getNumRows() - 1);
        int col = Functions::createRand(0,dungeon.getNumCols() - 1);
        if(!dungeon.addRoom(row, col)){
            i--;
        }
    }

    cout << endl;

    for (int i = 3; i < 5; i++){
        for (int line = firstLineList[i]; line <= lastLineList[i]; line++){
            cout << read.at(line) << endl;
        }
        Functions::convenientStop();
        cout << endl;
    }

    cout << "Give our merchant a second to prepare his goods...\n\n";

    //NPC Generation
    for (int i = 0; i < dungeon.getMaxNPC(); i++){
        int row = Functions::createRand(0,dungeon.getNumRows() - 1);
        int col = Functions::createRand(0,dungeon.getNumCols() - 1);
        if(!dungeon.addNPC(row,col)){
            i--;
        } else {
            dungeon.exploreSpace(row,col); //Highlights locations; keep this until testing is done
        }
    }

    cout << "Merchant:" << endl;
    for (int line = firstLineList[5]; line <= lastLineList[5]; line++){
        cout << read.at(line) << endl;
    }
    Functions::convenientStop();
    cout << endl;
    cout << "Now, time for my salesman's speech!" << endl;

    party.merchant();

    cout << endl;
    cout << "Commence, mortals!" << endl;


    //Actual game
    while (game){
        string action = "";

        bool exit = dungeon.isDungeonGate(dungeon.getPlayerRow(),dungeon.getPlayerCol());
        if (exit && party.getExploredRooms() == dungeon.getMaxRoom()){
            game = gameWin();
        }

        if (party.getLivingPlayerCount() == 0){ //All Players are dead
            game = gameLose(-3);
        }

        cout << endl;
        party.showPartyStatus();
        party.showPartyArsenal();
        dungeon.displayMap();

        //Actions
        bool npc = dungeon.isNPCLocation(dungeon.getPlayerRow(),dungeon.getPlayerCol());
        bool room = dungeon.isRoomLocation(dungeon.getPlayerRow(),dungeon.getPlayerCol());

        //Options presented based on whether player is at location of NPC, Room, or neither
        if (npc){
            cout << "Found an NPC!" << endl;
        } else if (room){
            cout << "Found Room!" << endl;
        }

        //Shared move option
        cout << "Select one:" << endl;
        cout << "0: Check Inventory" << endl;
        cout << "1. Move" << endl;

        if (npc){ //NPC-specific options
            cout << "2. Speak to NPC" << endl;
            cout << "3. Give up" << endl;
        } else if (room){
            cout << "2. Open Door" << endl;
            cout << "3. Give up" << endl;
        } else { //No NPC or Room
            cout << "2. Investigate" << endl;
            cout << "3. Pick a Fight" << endl;
            cout << "4. Customize Party Arsenal" << endl;
            cout << "5. Give up" << endl;
            cout << "6 (Test). Drink up on heal potions" << endl;
        }

        getline(cin,action); //Action
        cout << endl;

        if (action == "0"){ //Show Inventory
            party.showInventory();
            cout << "+-------------+" << endl;

            cout << endl;
            Functions::convenientStop();
        } else if (action == "1"){ //Universal Move Option
            //Starts prompt for player to move
            //dungeon.move(cardinalMove());
            if (!dungeon.move(cardinalMove())){
                cout << "Invalid move\n";
            }

            //20% for each member to lose 1 health
            for (int i = 0; i < party.getMaxPlayerSize(); i++){
                if (Functions::willOccur(20)){ //20% for given member to lose 1 HP
                    party.modifyPlayerHealth(i, -1);
                    game = party.getLivingPlayerCount();
                    if (party.getLivingPlayerCount() == 0){
                        game = false;
                    } else {
                        game = true;
                    }
                }
            }

            //Lowers God's patience
            if (!dungeon.isExplored(dungeon.getPlayerRow(),dungeon.getPlayerCol())){
                bool stillPatient = party.setDangerLevel(party.getDangerLevel() - 1);
                if (!stillPatient){ //God loses patience
                    game = gameLose(-1);
                }
            }
        } else if (action == "2" && npc){ //Talk to NPC
            //Space is now explored
            dungeon.exploreSpace(dungeon.getPlayerRow(),dungeon.getPlayerCol());

            //Talk with NPC
            if (party.npcPuzzle(Functions::createRand(1,20))){ //Correct answer
                cout << "Well, my God of Money! You answered well! Bravo." << endl;
                cout << "Care to barter? (y/n)" << endl;
                string confirm = "";

                //Confirm either meeting merchant or not
                do{
                    getline(cin,confirm);

                    if (confirm != "Y" && confirm != "y" && confirm != "N" && confirm != "n"){
                        cout << "Need a valid answer. Have a job to do, you know." << endl;
                    }
                } while (confirm != "Y" && confirm != "y" && confirm != "N" && confirm != "n");

                if (confirm == "Y" || confirm == "y"){
                    cout << "Splendid" << endl;
                    cout << endl;
                    //Opens merchant menu
                    party.merchant();
                } else { //Decline merchant
                    cout << "What a pity. Safe travels!" << endl;
                }
            } else { //Incorrect answer
                cout << "Well, that was a pity. Tough luck, chap." << endl;
                cout << "Oh dear. . . We've got company!" << endl;
                cout << "Wish you good luck!" << endl;

                int win = dungeon.encounter(party);
                //game = monsters.monsterEncounter(party);
                int rand1 = Functions::createRand(1,10);
                int rand2 = Functions::createRand(1,party.getMaxPlayerSize());
                int rand3 = Functions::createRand(1,100);

                party.monsterOutcome(win, rand1, rand2, rand3);
            }

            dungeon.removeNPC(dungeon.getPlayerRow(),dungeon.getPlayerCol());

            cout << endl;
            Functions::convenientStop();
        } else if (action == "2" && room){ //Entering Room

            //Space is now explored
            cout << "Entered room" << endl;
            dungeon.exploreSpace(dungeon.getPlayerRow(),dungeon.getPlayerCol());
            if (party.getKeys() >= 1){
                cout << "Door is open! There's a monster too!" << endl;
            } else {
                cout << "Foolish! Get a key first, and don't waste my patience!" << endl;
                continue;
            }

            int win = dungeon.encounter(party,true);
            

            cout << "Check room count: " << party.getExploredRooms() << endl;
            int rand1 = Functions::createRand(1,10);
            int rand2 = Functions::createRand(2,party.getMaxPlayerSize());
            int rand3 = Functions::createRand(1,100);

            game = party.monsterOutcome(win,rand1,rand2,rand3);
            //Removes room
            dungeon.removeRoom(dungeon.getPlayerRow(),dungeon.getPlayerCol());
        } else if (action == "2"){ //Investigating Space

            //Checks if tile is NOT explored
            if (!dungeon.isExplored(dungeon.getPlayerRow(),dungeon.getPlayerCol())){
                //Space is now explored
                dungeon.exploreSpace(dungeon.getPlayerRow(),dungeon.getPlayerCol());

                //Possibile outcomes
                int chance = Functions::createRand(1,100);
                if (chance <= 10){ //Party finds key (10%)
                    cout << "Impressive! You found a key!" << endl;
                    party.setKeys(party.getKeys() + 1);
                } else if (chance > 10 && chance <= 30){ //Party finds treasure
                    string treasure = ""; //Treasure name
                    //Following determines which treasure player gets

                    //Rework this as a Party class function
                    if (party.getExploredRooms() <= 1){ //0-1 Room
                        treasure = "Silver ring";
                    } else if (party.getExploredRooms() <= 2){ //2 Rooms
                        treasure = "Ruby necklace";
                    } else if (party.getExploredRooms() <= 3){ //3 Rooms
                        treasure = "Emerald bracelet";
                    } else if (party.getExploredRooms() <= 4){ //4 Rooms
                        treasure = "Diamond circlet";
                    } else { //5 Rooms
                        treasure = "Gem-encrusted goblet";
                    }

                    int val = 1;
                    party.addItems(party.returnItem(treasure), val);

                    if (val == 0){ //Treasure was added successfully
                        cout << "You got a " << treasure << "!" << endl;
                    } else { //Treasure not added successfully
                        cout << "Treasure couldn't be collected." << endl;
                    }
                } else if (chance > 30 && chance <= 50){ //Party fights random monster
                    cout << "There's a monster!" << endl;

                    int win = dungeon.encounter(party);

                    int rand1 = Functions::createRand(1,10);
                    int rand2 = Functions::createRand(2,party.getMaxPlayerSize());
                    int rand3 = Functions::createRand(1,100);
                    party.monsterOutcome(win, rand1, rand2, rand3);

                } else {
                    cout << "Nothing found here." << endl;
                }

                //50% of losing health
                if (chance <= 30 || chance > 50){ //All outcomes except fighting a monster
                    for (int i = 0; i < party.getMaxPlayerSize(); i++){
                        if (Functions::createRand(1,100) <= 50){ //Percentage form
                            party.modifyPlayerHealth(i, -1);
                            if (party.getLivingPlayerCount() == 0){
                                game = false;
                            } else {
                                game = true;
                            }
                        }
                    }
                }
            } else { //Tile is explored
                cout << "The area cannot be investigated." << endl;
                cout << "It appears to have been explored." << endl;
                cout << "Try another option." << endl;
            }

            cout << endl;
            Functions::convenientStop();
        } else if (action == "3"){ //Fight monster or Give Up Option
            if (npc || room){ //Give Up Option for NPC and Room actions
                game = gameLose();
                continue;
            }

            dungeon.exploreSpace(dungeon.getPlayerRow(),dungeon.getPlayerCol());

            //Fight a Monster
            cout << "There's a monster!" << endl;

            int win = dungeon.encounter(party);

            int rand1 = Functions::createRand(1,10);
            int rand2 = Functions::createRand(2,party.getMaxPlayerSize());
            int rand3 = Functions::createRand(1,100);
            party.monsterOutcome(win, rand1, rand2, rand3);

            cout << endl;
            Functions::convenientStop();
        } else if (action == "4" && !npc && !room){ //Customize Party Arsenal
            party.customizeArsenal();
        } else if (action == "5" && !npc && !room){ //Give Up Option for Normal Space
            game = gameLose();
        } else { //Incorrect Input
            cout << "Valid Input please." << endl;
        }
    }

    //Print final results
    cout << "Final Results:" << endl;
    party.showPartyStatus();
    party.showPartyArsenal();
    party.showInventory();

    return 0;
}