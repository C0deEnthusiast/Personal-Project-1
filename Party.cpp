// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Party Implementation

//Check if you actually need all of these libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctype.h>
#include "party.h"
#include "player.h"
#include "item.h"
#include "functions.h"

//Overhaul everything in this cpp file AND main driver

using namespace std;

Party::Party(){ //Default constructor
    createMerchantList("items.txt"); //Default items text file
}

Party::Party(string filename){ //Parameterized Constructor
    createMerchantList(filename); //Parameterized items text file
}


//Special Getters

//Counts how many players are alive
int Party::getLivePlayerCount(){
    int player_count = 0;

    for (auto& x: players){
        if (x.getPlayerHealth() > 0){ //Player is alive
            player_count++;
        }
    }

    return player_count;
}

vector<Item> Party::copyMerchantList(){
    return merchantList;
}

Player Party::getPlayer(int index){
    if (isPlayerIndex(index)){
        return players[index];
    } else {
        return Player();
    }
}

Item Party::getWeapon(int index){
    if (isPlayerIndex(index)){ //Each player has individual weapon
        return weapon_barracks[index];
    } else {
        return Item();
    }
}

Item Party::getArmor(int index){
    if (isPlayerIndex(index)){ //Each player has individual armor
        return armorSets[index];
    } else {
        return Item();
    }
}

//Setters
void Party::setMoney(int new_money){
    if (new_money >= 0){
        money = new_money;
    } else{
        money = 0;
    }

    return;
}

//This setter is bool rather than void as danger level of 0 would lead to user's defeat
bool Party::setDangerLevel(int new_level){
    if (new_level >= 0 && new_level <= 100){
        danger_level = new_level;
    }

    //If danger level reaches 0, game is lost
    if (danger_level == 0){
        return false;
    }

    return true;
}

void Party::setExploredRooms(int new_rooms){
    if (new_rooms >= 0){
        explored_rooms = new_rooms;
    } else {
        explored_rooms = 0;
    }

    return;
}

void Party::setKeys(int new_keys){
    if (new_keys >= 0){
        keys = new_keys;
    } else {
        keys = 0;
    }

    return;
}

//Returns false if at least one player is alive; true if all are literally dead
bool Party::areAllPlayersDead(){
    for (int i = 0; i < playerCount; i++){
        if (players[i].getPlayerHealth() > 0){
            return false;
        }
    }

    return true;
}

Item Party::returnItem(string item_name){
    for (auto& x: merchantList){
        if (x.getItemName() == item_name){
            return x;
        }
    }

    return Item();
}


/*Algorithm: Modifies player's health
    1) Changes the health of player based on health_change
    2) Aside from obvious purpose, it also checks whether the player's new health is 0
    3) If the player's health is 0, the player is considered dead and so sends message of their death
    4) If player is dead and is leader, activates a function regarding this
    Returns: void*/
void Party::modifyPlayerHealth(int index, int health_change){
    if (!isPlayerIndex(index)){
        return;
    }
    //This is if a dead player takes lethal health, thereby "dying again"
    if (players[index].getPlayerHealth() == 0 && health_change < 0){
        cout << "Goddamn! Not afraid to beat up dead meat, are we?" << endl;
        return;
    }

    //Resurrects player
    /*if (players[index].getPlayerHealth() == 0 && health_change > 0){
        player_count++;
    }*/

    //Changes specified player's health if the player is not dead
    players[index].setPlayerHealth(players[index].getPlayerHealth() + health_change);
    
    //Checks if health = 0 (e.g. player is dead or alive)
    if (players[index].getPlayerHealth() == 0 && health_change < 0){ //Player is killed
        //Announces player is dead
        cout << players[index].getPlayerName() << " is dead! Stay cautious!" << endl;
        //Swaps last player with dead player
        /*Player temp = players[index];
        players[index] = players[player_count - 1];
        players[player_count - 1] = temp;*/
        player_count--;

        if (players[index].getLeaderStatus()){ //Checks if dead player is the 'leader'
            cout << "Leader is dead. (Execute function regarding this.)" << endl;
            return;
        }
    }

    return;
}

void Party::modifyWeaponAttack(int index, int attack_change){
    if (!isPlayerIndex(index)){
        return;
    }

    //Changes specified weapon's attack value
    //weapon_barracks[index].setStat(weapon_barracks[index].getStat() + attack_change);
    int original_attack = players[index].getEquippedWeapon().getStat();
    players[index].getEquippedWeapon().setStat(original_attack + attack_change);

    return;
}

//Adds player to players[] array
void Party::addPlayer(string player_name){
    for (int i = 0; i < playerCount; i++){ //Searches for available Player 'slots'
        if (players[i].getPlayerName() == ""){ //Checks for empty player slot/index
            players[i].setPlayerName(player_name);
            if (i == 0){//First Player will be a 'leader'
                players[i].setLeaderStatus(true);
            }
            break;
        }
    }

    return;
}

bool Party::addItemProto(Item item){
    //Checks if item should go into inventory
    if (!(item.getItemType() == isWeapon || item.getItemType() == isArmor)){ //Adds to inventory
        if (current_inventory_capacity >= max_inventory_capacity){ //Full inventory
            return false;
        }

        for (auto& x: inventory){
            if (x.getItemName() == default_item_name){
                //Adds item to inventory
                x = item;
                current_inventory_capacity++;
                return true;
            }
        }
    }

    //Adding weapon or armor
    for (auto& x: players){
        //Equips new armor
        if (x.getEquippedArmor().getItemName() == default_item_name && item.getItemType() == isArmor){
            x.setEquippedArmor(item);
            current_armor_capacity++;
            return true;
        }
        //Equips new weapon
        if (x.getEquippedWeapon().getItemName() == default_item_name && item.getItemType() == isWeapon){
            x.setEquippedWeapon(item);
            current_weapon_capacity++;
            return true;
        }
    }

    return false;
}

bool Party::removeItemProto(Item item){
    //Removes from inventory
    if (!(item.getItemType() == isWeapon || item.getItemType() == isArmor)){
        for (auto& x: inventory){
            if (x.getItemName() == item.getItemName()){
                x = Item();
                current_inventory_capacity--;
                return true;
            }
        }
    }

    //Adding weapon or armor
    for (auto& x: players){
        //Removes armor
        if (x.getEquippedArmor().getItemName() == item.getItemType()){
            x.setEquippedArmor(Item());
            current_armor_capacity--;
            return true;
        }
        //Removes weapon
        if (x.getEquippedWeapon().getItemName() == item.getItemType()){
            x.setEquippedWeapon(Item());
            current_weapon_capacity--;
            return true;
        }
    }

    return false;
}

/*Adds item to inventory[] array or other relevant Item arrays
Returns: True if Item is added successfully; false otherwise*/
bool Party::addItem(Item item){
    if (item.getItemType() == isWeapon){
        return addItemHelper(weapon_barracks, max_weapon_capacity, current_weapon_capacity, item);
    } else if (item.getItemType() == isArmor){
        return addItemHelper(armorSets, max_armor_capacity, current_armor_capacity, item);
    } else {
        return addItemHelper(inventory, max_inventory_capacity, current_inventory_capacity, item);
    }
}

/*Removes Item from inventory[] array and other relevant Item arrays
Returns: False if current capacity is 0 or item is not found; true if item is removed successfully*/
bool Party::removeItem(Item item){
    if (item.getItemType() == isWeapon){
        return removeItemHelper(weapon_barracks, max_weapon_capacity, current_weapon_capacity, item);
    } else if (item.getItemType() == isArmor){
        return removeItemHelper(armorSets, max_armor_capacity, current_armor_capacity, item);
    } else {
        return removeItemHelper(inventory, max_inventory_capacity, current_inventory_capacity, item);
    }
}

//Shows status information of party inventory
void Party::showInventory(){
    cout << "+-------------+\n";
    cout << "| INVENTORY   | " << current_inventory_capacity << "/" << max_inventory_capacity;
    cout << "\n+-------------+";
    cout << "\n| Gold        | " << money;
    cout << "\n| Weapons     | " << current_weapon_capacity << "/" << max_weapon_capacity;
    cout << "\n| Armor       | " << current_armor_capacity << "/" << max_armor_capacity;
    //cout << "| Potions     | " << endl;
    cout << "\n| Treasures   | R: " << countItem("Silver ring");
    cout << " | N: " << countItem("Ruby necklace");
    cout << " | B: " << countItem("Emerald bracelet");
    cout << " | C: " << countItem("Diamond circlet");
    cout << " | G: " << countItem("Gem-encrusted goblet") << endl;

    return;
}

//Accesses inventory array and returns count of specified item
int Party::countItem(string item_name){
    int count = 0;

    for (int i = 0; i < max_inventory_capacity; i++){
        if(inventory[i].getItemName() == item_name){
            count++;
        }
    }

    return count;
}

/*General-purpose purchasing process for all items
    1) At confirmation of purchase, adds item(s); repeats process if invalid confirmation
    2) If successful, deducts party's money based on count of items added to inventory*/
void Party::purchaseProcess(int amount, int total_cost, Item purchasedItem){
    string confirm = ""; //Confirms or rejects purchase

    cout << "You want to buy " << amount << " ";
    if (purchasedItem.getItemType() == isWeapon){
        cout << "(+" << purchasedItem.getStat() << ") ";
    }
    cout << purchasedItem.getItemName() << "(s) for " << total_cost << " Gold? (y/n)" << endl;
    getline(cin,confirm);

    cout << endl;

    //Checks confirmation and if party has enough money
    if ((confirm == "Y" || confirm == "y") && total_cost <= getMoney()){ //Purchase is successful
        //Adds weapon to party's inventory list until all are added or inventory is full
        for (int i = 0; i < amount; i++){
            if (!addItemProto(purchasedItem)){ //!addItem(purchasedItem)
                //This occurs if desired count of items can NOT entirely be added
                //Adjusts total cost if necessary
                total_cost *= i;
                total_cost /= amount;

                //Informs player of adjusted price and item count
                cout << "It appears you don't have enough inventory space.\nAs such, I added ";
                cout << i << " out of the " << amount << " items that you requested.\n";
                cout << "I also adjusted the cost to reflect this!\nNo need to thank me!\n";
                break;
            }
        }
        
        setMoney(getMoney() - total_cost); //Deducts money from party
        //Show inventory capacity
        cout << "Thank you for your patronage! What else can I get for you?\n";
    } else if (confirm == "n" || confirm == "N"){ //Denies purchase
        cout << "Oh well. I won't judge you, for now.\n";
    } else if (total_cost > getMoney()){ //Low on money
        cout << "Sorry, my friend, but you need more money for that.\n";
        cout << "I recommend purchasing less quantity.\n";
        cout << "Let's negotiate again.\n";
    } else {
        cout << "Valid input please.\n";
        purchaseProcess(amount,total_cost,purchasedItem);
    }

    return;
}

//Merchant-exclusive; creates ALL items for merchant's Item array
void Party::createMerchantList(string filename){
    ifstream file(filename);

    if (!file.is_open()){
        cout << "File not open" << endl;
        return;
    }

    string line;
    while (getline(file, line)){

        vector<string> temp;

        Functions::vectorSplit(line, std_delimiter, temp);

        if (temp.size() != data_count){
            continue;
        }

        //Adds Item to merchantList vector
        Item new_item(temp[0],temp[1],stoi(temp[2]),stoi(temp[3]),stoi(temp[4]),stoi(temp[5]),
        temp[6], stoi(temp[7]),stoi(temp[8]),stoi(temp[9]));
        merchantList.push_back(new_item);
    }

    file.close();

    return;
}

void Party::presentMerchantItem(Item item, string target, double tax){
    if (target == isWeapon){
        cout << "(+";
    } else if (target == isArmor){
        cout << "(DMG Reduction: ";
    }

    if (target != isTreasure && target != isPotion){
        cout << item.getStat();
    }

    if (target == isArmor){
        cout << "%) ";
    } else if (target == isWeapon){
        cout << ") ";
    }

    cout << item.getItemName() << " [";

    if (target != isTreasure){
        cout << item.getCost() * tax;
    } else {
        cout << item.getCost();
    }

    cout << " Gold]\n";

    if (target == isWeapon){
        cout << " - Crit Information - Chance: " << item.getCritChance();
        cout << "%; Boost: " << item.getCritBoost() << "%\n";
    }

    if (target == isWeapon || target == isPotion){
        displayEffect(item.weapon_effect);
    }
}

/*Opens merchant prompt and allow user to make purchases or sell treasures
    1) Sets up a penalty in case user makes deliberate incorrect input for 'wasting' the merchant's time
    2) Sets up the tax that increases prices based on explored rooms*/
void Party::merchant(){
    //If players deliberately make wrong inputs (i.e. letters for number input) at certain points
    //They will be punished
    int wrong_input = 0;

    double tax = 1 + (0.25 * explored_rooms); //Modifies prices based on how many rooms were explored

    cout << "If you're looking to get supplies, you've come to the right place.\n";
    cout << "I would be happy to part with some of my wares...for the proper price!\n";

    while (true){
        string choice = "", amount = ""; //These are strings in case of invalid input
        int confirm_total;

        //These are for quick access to the names and original costs of items
        vector<Item> displayed_items;

        cout << endl;
        showInventory();
        cout << "\nChoose one of the following:\n";
        cout << "1. Weapons: It's dangerous to go alone, take this!\n";
        cout << "2. Armor: If you want to survive monster attacks, you will need some armor.\n";
        cout << "3. Potions: Dangerous and potent, but unfair. To your enemies that is.\n";
        cout << "4. Sell treasures: If you find anything shiny, ";
        cout << "I would be happy to take it off your hands.\n";
        cout << "5. Leave: Make sure you get everything you need, I'm leaving after this sale!\n\n";

        getline(cin,choice);

        while (!Functions::isNumber(choice)){
            cout << "I need a valid choice!\n";
            getline(cin,choice);
        }

        if (stoi(choice) >= 1 && stoi(choice) <= 4){
            //Weapons, Treasures, or Potions
            string target = ""; //Selected item type

            if (stoi(choice) == 1){ //Weapons
                cout << "I have a plentiful collection of weapons to choose from,";
                cout << " what would you like?\n";
                cout << "Note that each weapon have base attack power, marked by a (+X).\n";
                cout << "Plus, each got a little ... bonus.\n\n";
                cout << "Choose one of the following:\n";

                target = isWeapon;
            } else if (stoi(choice) == 2){ //Armor
                cout << "Looking for suits of armor, I see.\n";
                cout << "None of these will make you look like a dashing knight in shining armor";
                cout << " (except a few), but they do well in keeping you alive.\n";
                cout << "Keep in mind that you can only carry a maximum of ";
                cout << max_armor_capacity << " sets of armor.\n\n";
                cout << "Have a look:\n";

                target = isArmor;
            } else if (stoi(choice) == 3){ //Potions
                cout << "I have many potions to choose from, what would you like?\n";
                cout << "Each have their own ... special quirks.\n\n";
                cout << "Pick and choose:\n";
                target = isPotion;
            } else { //Treasures
                cout << "Oooh! You got treasures? Brilliant!\n";
                cout << "Let's see, here are the treasures I'm looking for:\n\n";
                cout << "Which ones do you have?\n";

                target = isTreasure;
            }

            //Prints all items in merchant array with target item type
            for (auto& x: merchantList){
                if (x.getItemType() == target){
                    displayed_items.push_back(x);

                    cout << displayed_items.size() << ". ";
                    presentMerchantItem(displayed_items.at(displayed_items.size() - 1),target,tax);
                    cout << endl;
                }
            }
            cout << (displayed_items.size() + 1) << ". Cancel\n";

            while (true){
                getline(cin,choice);

                if (Functions::isNumber(choice)){
                    if (stoi(choice) > 0 && stoi(choice) <= (displayed_items.size() + 1)){
                        break;
                    }
                }

                cout << "Valid Input Please\n";
            }

            //Cancels weapon selection if choice is the Cancel option
            if (stoi(choice) == (displayed_items.size() + 1)){
                continue;
            }

            Item target_item = displayed_items.at(stoi(choice) - 1);

            if (target != isTreasure){ //Purchasing
                while (true){
                    cout << "(Enter how many items you wish to purchase, or 0 to cancel)\n";
                    getline(cin,amount);

                    if (Functions::isNumber(amount)){ //Valid Number
                        if (stoi(amount) >= 0){ //Non-negative
                            break;
                        }
                    }

                    cout << "I need an appropriate amount.\n\n";
                }

                //Calculates price
                double adjustTax = tax * static_cast<double> (target_item.getCost());
                confirm_total = adjustTax * stoi(amount);

                if (stoi(amount) != 0){ //0 is Cancel
                    purchaseProcess(stoi(amount),confirm_total,target_item);
                }
            } else { //Treasure; selling
                while (true){
                    string confirm = "";
                    do { //Checks for confirmation of selling treasures
                        cout << "\nYou want to sell " << target_item.getItemName();
                        cout << " for " << target_item.getCost() << " Gold? (y/n)\n";

                        getline(cin,confirm);

                        if (confirm != "y" && confirm != "Y" && confirm != "n" && confirm != "N"){
                            cout << "Valid confirmation please.\n";
                        }
                    } while (confirm != "y" && confirm != "Y" && confirm != "n" && confirm != "N");

                    cout << endl;
                    
                    if (confirm == "y" || confirm == "Y"){ //Sells treasure
                        if (removeItem(target_item)){ //Sold treasure successfully
                            setMoney(money + target_item.getCost()); //Gives party more gold
                        } else {
                            cout << "How unfortunate, it appears you don't have the treasure.\n";
                        }
                    } else {
                        cout << "Damn. Very well, keep your treasure then.\n";
                    }

                    break;
                }
            }
        } else if (stoi(choice) == 5){ //Leave; Terminates merchant event
            cout << "Farewell!\n";
            break;
        } else {
            cout << "Why are we beating around the bush?\n";
            cout << "(Enter a character to display merchant's menu)\n";
            getline(cin,choice);
        }
    }
    cout << endl;

    return;
}

//Displays Inventory status, party status, and other information
void Party::showPartyStatus(){
    cout << "+-------------+\n";
    cout << "| STATUS      |\n";
    cout << "+-------------+\n";
    cout << "| Rooms Cleared: " << getExploredRooms();
    cout << " | Keys: " << getKeys() << " | Patience Level: " << getDangerLevel();
    cout << "\n+-------------+";
    cout << "\n| PARTY       |";
    cout << "\n+-------------+";
    //Main player is first displayed
    for (auto x: players){
        cout << "\n| " << x.getPlayerName();
        /*if (armorSets[i].getItemName() != default_item_name){
            cout << " (" << armorSets[i].getItemName() << ")";
        }*/
        if (x.getEquippedArmor().getItemName() != default_item_name){
            cout << " (" << x.getEquippedArmor().getItemName() << ")";
        }
        cout << " | Health: " << x.getPlayerHealth() << " | ";
        
        //Displays weapons
        /*if (weapon_barracks[i].getItemName() != default_item_name){
            cout << "Equipped: " << weapon_barracks[i].getItemName();
            cout << "(Atk: " << weapon_barracks[i].getItemStat() << ")";
        }*/
        if (x.getEquippedWeapon().getItemName() != default_item_name){
            cout << "Equipped: " << x.getEquippedWeapon().getItemName();
            cout << "(Atk: " << x.getEquippedWeapon().getStat() << ")";
        }
    }
    cout << "\n+-------------+\n\n";

    return;
}

/*Algorithm: Picks random riddle and allows user to answer it
    1) Opens txt file and checks if it's open
    2) While file is open, retrieves line and checks for empty/incomplete lines
    3) If line number matches chosen riddle location, then the current riddle would be used
    3) NPC will deliver exposition and then tell the player their riddle; player types input
    4) Compares input to actual answer, NPC will respond accordingly
    Parameters: riddle
    Returns: True if player answers correctly; false otherwise*/
bool Party::npcPuzzle(int riddle){
    //Opens riddles file
    ifstream file("riddles.txt");

    if (!file.is_open()){
        cout << "File not open" << endl;
        return false;
    }

    int pick_riddle = 0;

    //Information of the riddle
    string riddleContent[2];
    string line;

    while(getline(file,line)){
        if (line.length() == 0){
            continue;
        }

        vector<string> temp;
        istringstream splice(line);

        while (getline(splice,line,'~')){
            temp.push_back(line);
        }

        if (temp.size() != 2){
            continue;
        }

        if (pick_riddle == riddle){ //Exits loop with current temporary array
            riddleContent[0] = temp[0];
            riddleContent[1] = temp[1];
            break;
        } else {
            pick_riddle++; //Goes up line number
        }
    }

    file.close(); //Closes file

    string answer = "";

    cout << endl;
    cout << "Good day, mate! I see, you're interested in the goods I've got, yes?" << endl;
    cout << "Truth betold, I'm not impressed so far, so you need to up the ante." << endl;
    cout << "In which case, solve my riddle, then we'll negotiate. Good? Great!" << endl;
    cout << "Listen well:" << endl;

    //Prints riddle
    cout << riddleContent[0] << endl;
    cout << "Answer carefully; one shot, mate.\n";
    getline(cin,answer);
    cout << endl;

    //Compares user's answer to actual answer
    return (answer == riddleContent[1]);
}

//Based on value from Monster::monsterEncounter, different awards/punishments occur
bool Party::monsterOutcome(int outcome, int key_chance, int kill_index, int health_chance){
    //Returning false in this function means main player is dead
    //As for other fail conditions, they're outside function

    if (outcome > -1){ //Victory
        setMoney(getMoney() + 10 * outcome);
        //Make a function taht potentially gives potions
        cout << endl;
        cout << "Congrats! You've slain the monster! You won some gold and food!" << endl;
        cout << endl;

        if (key_chance <= 10){ //Chance for key
            cout << "You got a key too!" << endl;
            setKeys(getKeys() + 1);
        }
    } else if (outcome == -1){ //Surrender; dead player
        cout << "Foolish mortals! With surrender, comes sacrifice!" << endl;
        //Kills random player (that is alive, rather than beating a dead horse)
        if (players[kill_index].getPlayerHealth() > 0){
            modifyPlayerHealth(kill_index, -100); //Kills player
            if (getLivePlayerCount() == 0){
                return false;
            }
        }
    } else if (outcome == -2){ //Defeat
        setMoney(getMoney()/4);
        cout << "One for all, and all will fall." << endl;
        for (int i = 1; i < playerCount; i++){
            if (health_chance <= 10){
                //Kills player
                modifyPlayerHealth(i, -100);

                if (getLivePlayerCount() == 0){
                    return false;
                }
            }
        }
    }

    for (int i = 0; i < getPlayerSize(); i++){
        if (health_chance <= 50){ //Percentage form; random chance for EACH member
            modifyPlayerHealth(i, -1);
            if (getLivePlayerCount() == 0){
                return false;
            }
        }
    }

    return true;
}