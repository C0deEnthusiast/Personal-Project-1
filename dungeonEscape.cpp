// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Dungeon Escape Game

//Shortcut: g++ -std=c++17 dungeonEscape.cpp Map.cpp Item.cpp Party.cpp Player.cpp Monster.cpp -o game
//Compile with with this: ./game items.txt monsters.txt

#include <iostream>
#include <cstdlib>
#include <fstream>
#include "party.h"
#include "monster.h"
#include "map.h"

using namespace std;


/*
 * This is to let player read exposition; also stops stream from entering 'player' input
 * Intended to let player read what everything goddamn says*/
void convenientStop(string stop = ""){
    cout << "(Enter a character to continue)" << endl;
    getline(cin, stop);

    return;
}

/*Algorithm: Prints a desired count of lines from the story text file, starting from first_line (INCLUSIVE)
Range of function: Line 1 to end of text file*/
void read(int first_line, int last_line, string speaker = ""){
    //First line of text file is treated as 1, not 0

    ifstream file("story.txt"); //Opens story text file

    bool target = false; //Determines if first target line, first_line, is found
    int current_line = 1;

    if (!file.is_open()){ //Failed to open file
        cout << "File not open" << endl;
        return;
    }

    if (first_line < 1 || last_line < 1 || first_line > last_line){
        cout << "Range out of bounds" << endl;
        return;
    }

    while (!file.eof() && current_line <= last_line){
        string line;
        getline(file,line);

        if (line.length() == 0){ //Empty lines are ignored
            continue;
        }

        if (current_line == first_line){ //Prints a desired count of lines when first_line is found
            target = true;
        }
        
        if (target){ //Prints desired count of lines after first_line (Inclusive of first_line)
            if (speaker.length() != 0){ //Excludes default
                cout << speaker << ": ";
            }
            cout << line << endl;
        }

        current_line++;
    }

    file.close(); //Closes file

    if (!target){ //First line is above range (outside of text file)
        cout << "Target lines outside of text file" << endl;
    }

    return;
}

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
    if (argc != 3){
        cout << "Need in order: {Program} {Text file of Items} {Text file of Monsters}" << endl;
        return 0;
    }

    Map dungeon;
    Party party(argv[1]); //User interacts with this
    //Monster monsters(argv[2]);
    Combat fight(argv[2]);

    bool game = true; //Keeps game running
    string player; //For player names
    string skip; //For exposition

    //These arrays are to make it easier to maintain displaying text
    int firstLineList[] = {1,4,8,14,16};
    int lastLineList[] = {3,7,13,15,18};

    cout << "???:" << endl;
    //Introduction exposition; creates 3 rooms
    for (int i = 0; i < 3; i++){
        read(firstLineList[i],lastLineList[i]);
        cout << "(Enter a character to continue)" << endl;
        getline(cin,skip); //Allows player to read exposition; also stops stream from entering 'player' input
        cout << endl;
    }

    cout << "Wonderful, now pick a leader." << endl;
    cout << "(Enter a name; don't waste my godly patience)" << endl;

    do { //Checks for valid name inputs
        getline(cin,player);
        if (player.length() == 0){
            cout << "Really? Again." << endl;
        }
    } while (player.length() == 0);
    party.addPlayer(player);


    cout << endl;
    cout << "Oh, so the leader is " << player << ". How peachy" << endl;
    cout << "How about your 4 comrades?" << endl;
    cout << "(Enter their names)" << endl;
    for (int i = 0; i < 4; i++){
        player = ""; //Resets string
        cout << endl;
        cout << "Enter your teammate's name." << endl;
        do { //Checks for valid name inputs
            getline(cin,player);
            if (player.length() == 0){
                cout << "Really? Again." << endl;
            }
        } while (player.length() == 0);
        party.addPlayer(player);
    }





    //Remove this later; testing only
    party.setMoney(1000000);
    party.addItem(party.returnItem("Diamond circlet"));
    party.addItem(party.returnItem("Diamond circlet"));
    //party.addItem(party.returnItem("Blighted Dagger")); //Debugging only
    party.merchant(); //Testing
    for (int i = 0; i < 21; i++){ //21
        if (fight.encounter(party) == -1){
            party.showPartyStatus();
            return 0;
        }
    }
    return 0;
    //End of removal






    //Room Generation
    for (int i = 0; i < dungeon.getMaxRoom(); i++){
        int rand1 = Functions().createRand(0,dungeon.getNumRows() - 1);
        int rand2 = Functions().createRand(0,dungeon.getNumCols() - 1);
        if(!dungeon.addRoom(rand1, rand2)){
            i--;
        }
    }

    cout << endl;

    for (int i = 3; i < 5; i++){
        read(firstLineList[i],lastLineList[i]);
        //cout << "(Enter a character to continue)" << endl;
        //Allows player to read exposition; also stops stream from entering 'player' input
        //getline(cin,skip);
        convenientStop();
        cout << endl;
    }

    cout << "Give our merchant a second to prepare his goods..." << endl;

    //NPC Generation
    for (int i = 0; i < dungeon.getMaxNPC(); i++){
        int row = Functions().createRand(0,dungeon.getNumRows() - 1);
        int col = Functions().createRand(0,dungeon.getNumCols() - 1);
        if(!dungeon.addNPC(row,col)){
            i--;
        } else {
            dungeon.exploreSpace(row,col); //Highlights locations; keep this until testing is done
        }
    }

    cout << "Merchant:" << endl;
    read(19,31);
    //cout << "(Enter any character to continue)" << endl;
    //This is to let player read exposition; also stops stream from entering 'player' input
    //getline(cin,skip);
    convenientStop();
    cout << endl;
    cout << "Now, time for my salesman's speech!" << endl;

    party.merchant();

    cout << endl;
    cout << "Commence, mortals!" << endl;


    //Actual game
    while (game){
        string action = "";

        bool exit = dungeon.isDungeonExit(dungeon.getPlayerRow(),dungeon.getPlayerCol());
        if (exit && party.getExploredRooms() == dungeon.getMaxRoom()){
            game = gameWin();
        }

        if (party.getPlayerCount() == 0){ //All Players are dead
            game = gameLose(-3);
        }

        cout << endl;
        party.showPartyStatus();
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
            cout << "4. Cook and Eat; soon to be disabled" << endl;
            cout << "5. Give up" << endl;
            cout << "6 (Test). Drink up on heal potions" << endl;
        }

        getline(cin,action); //Action
        cout << endl;

        if (action == "0"){ //Show Inventory
            party.showInventory();
            cout << "+-------------+" << endl;

            cout << endl;
            convenientStop();
        } else if (action == "1"){ //Universal Move Option
            //Starts prompt for player to move
            dungeon.move(cardinalMove());

            //20% for each member to lose 1 health
            for (int i = 0; i < party.getPlayerSize(); i++){
                if (Functions().createRand(1,100) <= 20){ //Percentage form; random chance for EACH member
                    game = party.changePlayerHealth(i,-1); //i is index of player
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
            if (party.npcPuzzle(Functions().createRand(1,20))){ //Correct answer
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

                int win = fight.encounter(party);
                //game = monsters.monsterEncounter(party);
                int rand1 = Functions().createRand(1,10);
                int rand2 = Functions().createRand(2,party.getPlayerCount());
                int rand3 = Functions().createRand(1,100);

                party.monsterOutcome(win, rand1, rand2, rand3);
            }

            dungeon.removeNPC(dungeon.getPlayerRow(),dungeon.getPlayerCol());

            cout << endl;
            convenientStop();
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

            int win = fight.encounter(party,true);
            

            cout << "Check room count: " << party.getExploredRooms() << endl;
            int rand1 = Functions().createRand(1,10);
            int rand2 = Functions().createRand(2,party.getPlayerCount());
            int rand3 = Functions().createRand(1,100);

            game = party.monsterOutcome(win,rand1,rand2,rand3);
            //Removes room
            dungeon.removeRoom(dungeon.getPlayerRow(),dungeon.getPlayerCol());
        } else if (action == "2"){ //Investigating Space

            //Checks if tile is NOT explored
            if (!dungeon.isExplored(dungeon.getPlayerRow(),dungeon.getPlayerCol())){
                //Space is now explored
                dungeon.exploreSpace(dungeon.getPlayerRow(),dungeon.getPlayerCol());

                //Possibile outcomes
                int chance = Functions().createRand(1,100);
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

                    if (party.addItem(party.returnItem(treasure))){ //Treasure was added successfully
                        cout << "You got a " << treasure << "!" << endl;
                    } else { //Treasure not added successfully
                        cout << "Treasure couldn't be collected." << endl;
                    }
                } else if (chance > 30 && chance <= 50){ //Party fights random monster
                    cout << "There's a monster!" << endl;

                    int win = fight.encounter(party);

                    int rand1 = Functions().createRand(1,10);
                    int rand2 = Functions().createRand(2,party.getPlayerCount());
                    int rand3 = Functions().createRand(1,100);
                    party.monsterOutcome(win, rand1, rand2, rand3);

                } else {
                    cout << "Nothing found here." << endl;
                }

                //50% of losing health
                if (chance <= 30 || chance > 50){ //All outcomes except fighting a monster
                    for (int i = 0; i < party.getPlayerSize(); i++){
                        if (Functions().createRand(1,100) <= 50){ //Percentage form
                            party.changePlayerHealth(i,-1); //i is index of player
                        }
                    }
                }
            } else { //Tile is explored
                cout << "The area cannot be investigated." << endl;
                cout << "It appears to have been explored." << endl;
                cout << "Try another option." << endl;
            }

            cout << endl;
            convenientStop();
        } else if (action == "3"){ //Fight monster or Give Up Option
            if (npc || room){ //Give Up Option for NPC and Room actions
                game = gameLose();
                continue;
            }

            dungeon.exploreSpace(dungeon.getPlayerRow(),dungeon.getPlayerCol());

            //Fight a Monster
            cout << "There's a monster!" << endl;

            int win = fight.encounter(party);

            int rand1 = Functions().createRand(1,10);
            int rand2 = Functions().createRand(2,party.getPlayerCount());
            int rand3 = Functions().createRand(1,100);
            party.monsterOutcome(win, rand1, rand2, rand3);

            cout << endl;
            convenientStop();
        } else if (action == "4" && !npc && !room){ //Eat Food
            cout << "Food, cookware, and all cooking related stuff will be replaced by potions" << endl;
            convenientStop();
        } else if (action == "5" && !npc && !room){ //Give Up Option for Normal Space
            game = gameLose();
        } else { //Incorrect Input
            cout << "Valid Input please." << endl;
        }
    }

    //Print final results
    cout << "Final Results:" << endl;
    party.showPartyStatus();
    party.showInventory();

    return 0;
}