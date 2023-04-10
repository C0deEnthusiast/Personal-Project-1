// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Party Implementation

//Check if you actually need all of these libraries
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <ctype.h>
#include "party.h"
#include "split.h" //Scrap with getline() and vector combo

//Overhaul everything in this cpp file AND main driver

using namespace std;

Party::Party(){ //Default constructor
    money = 100;
    current_armor = 0;
    danger_level = 100; //Game ends when reaches 0
    explored_rooms = 0;
    keys = 0;
    current_capacity = 0;
    current_weapon = 0;

    createMerchantList("items.txt"); //Default items text file
}

Party::Party(string filename){ //Parameterized Constructor
    money = 100;
    current_armor = 0;
    danger_level = 100; //Game ends when reaches 0
    explored_rooms = 0;
    keys = 0;
    current_capacity = 0;
    current_weapon = 0;

    createMerchantList(filename); //Parameterized items text file
}


//Getters (Maximums and Constants)

int Party::getPlayerSize(){
    return player_size;
}

int Party::getHealthRecover(){
    return food_recover;
}

int Party::getMaxCapacity(){
    return max_capacity;
}

int Party::getMaxWeaponCapacity(){
    return max_weapon;
}

int Party::getMaxArmorCapacity(){
    return max_armor;
}


//Normal Getters
int Party::getMoney(){
    return money;
}

int Party::getDangerLevel(){
    return danger_level;
}

int Party::getExploredRooms(){
    return explored_rooms;
}

int Party::getKeys(){
    return keys;
}

int Party::getCurrentCapacity(){
    return current_capacity;
}

int Party::getCurrentWeaponCapacity(){
    return current_weapon;
}

int Party::getCurrentArmorCapacity(){
    return current_armor;
}



//Counts how many players are alive
int Party::getPlayerCount(){
    int player_count = 0;

    for (int i = 0; i < player_size; i++){
        if (players[i].getPlayerHealth() > 0){ //Player has health(alive)
            player_count++;
        }
    }

    return player_count;
}

int Party::getMerchantCapacity(){
    return merchantList.size();
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

void Party::setCurrentCapacity(int new_current_capacity){
    //Current capacity can increase or decrease
    if (new_current_capacity >= 0){
        current_capacity = new_current_capacity;
    } else {
        current_capacity = 0;
    }

    return;
}

void Party::setCurrentArmorCapacity(int new_armor){
    if (new_armor >= 0){
        current_armor = new_armor;
    } else {
        current_armor = 0;
    }
    
    if (current_armor > max_armor){ //Limits armor to maximum
        current_armor = max_armor;
    }

    return;
}


//Returns false if at least one player is alive; true if all are literally dead
bool Party::areAllPlayersDead(){
    for (int i = 0; i < player_size; i++){
        if (players[i].getPlayerHealth() > 0){
            return false;
        }
    }

    return true;
}

Item Party::returnItem(string item_name){
    for (int i = 0; i < merchantList.size(); i++){
        if (merchantList[i].getItemName() == item_name){
            return merchantList[i];
        }
    }

    return Item();
}

void Party::cook(int probability){
    cout << "Cook no longer in function." << endl;
    return;
}


/*Algorithm: Modifies player's health
    1) Changes the health of player based on health_change
    2) Aside from obvious purpose, it also checks whether the player's new health is 0
    3) If the player's health is 0, the player is considered dead and so sends message of their death
    4) However, checks if dead player is the main player, if so, returns false to initiate gameLose()
    Parameters: index and health_change (int)
    Returns: false if main player dies from 0 health; true otherwise*/
bool Party::changePlayerHealth(int index, int health_change){
    //This is if a dead player takes lethal health, thereby "dying again"
    if (players[index].getPlayerHealth() == 0 && health_change < 0){
        cout << "Goddamn! Not afraid to beat up dead meat, are we?" << endl;
        return true;
    }

    //Changes specified player's health if the player is not dead
    players[index].setPlayerHealth(players[index].getPlayerHealth() + health_change);
    
    //Checks if health = 0 (e.g. player is dead or alive)
    if (players[index].getPlayerHealth() == 0){ //Player is dead
        //Announces player is dead
        cout << players[index].getPlayerName() << " is dead! Stay cautious!" << endl;

        if (players[index].getLeaderStatus()){ //Checks if dead player is the main player (e.g. the user)
            return false;
        }
    }

    return true;
}

/*Algorithm: Modifies player's health
    1) Changes the health of player based on health_change
    2) Aside from obvious purpose, it also checks whether the player's new health is 0
    3) If the player's health is 0, the player is considered dead and so sends message of their death
    4) If player is dead and is leader, activates a function regarding this
    Returns: void*/
void Party::modifyPlayerHealth(int index, int health_change){
    if (index < 0 || index >= player_size){
        return;
    }
    //This is if a dead player takes lethal health, thereby "dying again"
    if (players[index].getPlayerHealth() == 0 && health_change < 0){
        cout << "Goddamn! Not afraid to beat up dead meat, are we?" << endl;
        return;
    }

    //Changes specified player's health if the player is not dead
    players[index].setPlayerHealth(players[index].getPlayerHealth() + health_change);
    
    //Checks if health = 0 (e.g. player is dead or alive)
    if (players[index].getPlayerHealth() == 0 && health_change < 0){ //Player is killed
        //Announces player is dead
        cout << players[index].getPlayerName() << " is dead! Stay cautious!" << endl;

        if (players[index].getLeaderStatus()){ //Checks if dead player is the 'leader'
            cout << "Leader is dead. (Execute function regarding this.)" << endl;
            return;
        }
    }

    return;
}

void Party::modifyWeaponAttack(int index, int attack_change){
    if (index < 0 || index >= max_weapon){
        return;
    }

    //Changes specified weapon's attack value
    weapon_barracks[index].setStat(weapon_barracks[index].getStat() + attack_change);

    return;
}

//Adds player to players[] array
void Party::addPlayer(string player_name){
    for (int i = 0; i < player_size; i++){ //Searches for available Player 'slots'
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

bool Party::addItemHelper(Item itemList[], int list_max_capacity, int &list_current_capacity, Item item){
    if (list_current_capacity >= max_capacity){
        return false;
    }

    for (int i = 0; i < list_max_capacity; i++){
        if (itemList[i].getItemName() == default_item_name){
            //Creates new item in inventory
            itemList[i] = item;
            list_current_capacity++;//Adds to capacity
            return true;
        }
    }

    return false;
}

/*Adds item to inventory[] array or other relevant Item arrays

Returns: True if Item is added successfully; false otherwise*/
bool Party::addItem(Item item){
    if (item.getItemType() == is_weapon){
        return addItemHelper(weapon_barracks,max_weapon,current_weapon,item);
    } else if (item.getItemType() == is_armor){
        return addItemHelper(armorSets,max_armor,current_armor,item);
    } else {
        return addItemHelper(inventory,max_capacity,current_capacity,item);
    }
}

bool Party::removeItemHelper(Item itemList[], int list_max_capacity, int &list_current_capacity, Item item){
    if (list_current_capacity == 0){
        return false; //Quickly returns false to give efficient output rather than wait for loop
    }

    for (int i = 0; i < list_max_capacity; i++){
        if (itemList[i].getItemName() == item.getItemName()){
            //Default constructor is used to override current item; effectively removing it
            itemList[i] = Item();
            list_current_capacity--;//Reduces capacity
            return true;
        }
    }

    return false; //This occurs only if item is not found for deletion
}

/*Removes Item from inventory[] array and other relevant Item arrays

Returns: False if current capacity is 0 or item is not found; true if item is removed successfully*/
bool Party::removeItem(Item item){
    bool remove_weapon = false; //This is if the item to be removed would also be in weapon_barracks[]
    bool remove_armor = false; //This is if the item to be removed would also be in armorSets[]

    if (item.getItemType() == is_weapon){
        return removeItemHelper(weapon_barracks,max_weapon,current_weapon,item);
    } else if (item.getItemType() == is_armor){
        return removeItemHelper(armorSets,max_armor,current_armor,item);
    } else {
        return removeItemHelper(inventory,max_capacity,current_capacity,item);
    }
}

//Shows status information of party inventory
void Party::showInventory(){
    cout << "+-------------+" << endl;
    cout << "| INVENTORY   | " << current_capacity << "/" << max_capacity << endl;
    cout << "+-------------+" << endl;
    cout << "| Gold        | " << money << endl;
    cout << "| Weapons     | " << current_weapon << "/" << max_weapon << endl;
    cout << "| Armor       | " << current_armor << "/" << max_armor << endl;
    //cout << "| Potions     | " << endl;

    cout << "| Treasures   | R: " << countItem("Silver ring");
    cout << " | N: " << countItem("Ruby necklace");
    cout << " | B: " << countItem("Emerald bracelet");
    cout << " | C: " << countItem("Diamond circlet");
    cout << " | G: " << countItem("Gem-encrusted goblet") << endl;

    return;
}

//Accesses inventory array and returns count of specified item
int Party::countItem(string item_name){
    int count = 0;

    for (int i = 0; i < max_capacity; i++){
        if(inventory[i].getItemName() == item_name){
            count++;
        }
    }

    return count;
}

//Checks for numeric input by checking if each character is a digit
bool Party::isNumber(string line){ //Checks for digits
    if (line.length() == 0){
        return false;
    }
    for (int i = 0; i < line.length(); i++){ //Checks if amount is a number
        if (!isdigit(line[i])){
            return false;
        }
    }

    return true;
}

/*Algorithm: General-purpose purchasing process for all items
This would allow for efficient maintenance
    1) Starts prompt and asks string input for confirmation of purchase; repeats process if incorrect input
    2) Based on exchange_type ("food","armor","weapon", and "cookware"), setters ("food" and "armor")
    would be called to change data members while addItem() would be used to create item with
    'item' as the first parameter and 'amount' as the second for "weapon" and "treasure"
    Parameters: int amount,int total_cost,string exchange_type,string item
    Note: string item has default argument of ""*/
void Party::purchaseProcess(int amount,int total_cost, Item purchasedItem){
    string confirm = ""; //Confirms or rejects purchase

    cout << "You want to buy " << amount << " ";
    if (purchasedItem.getItemType() == "weapon"){
        cout << "(+" << purchasedItem.getStat() << ") ";
    }

    cout << purchasedItem.getItemName() << "(s)";

    cout << " for " << total_cost << " Gold? (y/n)" << endl;
    getline(cin,confirm);
    cout << endl;

    //Checks confirmation and if party has enough money
    if ((confirm == "Y" || confirm == "y") && total_cost <= getMoney()){ //Purchase is successful
        //Adds weapon to party's inventory list using weapon name
        for (int i = 0; i < amount; i++){
            if (!addItem(purchasedItem)){
                //This occurs if desired count of items can NOT entirely be added
                //Adjusts total cost if necessary
                total_cost *= i;
                total_cost /= amount;

                //Informs player of adjusted price and item count
                cout << "It appears you don't have enough inventory space." << endl;
                cout << "As such, I added " << i << " out of the " << amount;
                cout << " items that you requested." << endl;
                cout << "I also adjusted the cost to reflect this!" << endl;
                cout << "No need to thank me!" << endl;
                break;
            }
        }
        
        setMoney(getMoney() - total_cost); //Deducts money from party
        //Show inventory capacity
        cout << "Thank you for your patronage! What else can I get for you?" << endl;
    } else if (confirm == "n" || confirm == "N"){ //Denies purchase
        cout << "Oh well. I won't judge you, for now." << endl;
    } else if (total_cost > getMoney()){ //Low on money
        cout << "Sorry, my friend, but you need more money for that." << endl;
        cout << "I recommend purchasing less quantity." << endl;
        cout << "Let's negotiate again." << endl;
    } else {
        cout << "Valid input please." << endl;
        purchaseProcess(amount,total_cost,purchasedItem); //Recurs process if invalid input
    }

    return;
}

//Merchant-exclusive; creates ALL items for merchant's Item array
void Party::createMerchantList(string filename){
    //Opens file
    ifstream file(filename);

    if (!file.is_open()){
        cout << "File not open" << endl;
        return;
    }

    string line;
    while (getline(file,line)){

        vector<string> temp;

        if (line.length() == 0){
            continue;
        }

        istringstream splice(line);

        while (getline(splice,line,separator)){
            temp.push_back(line);
        }

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
    if (target == is_weapon){
        cout << "(+";
    } else if (target == is_armor){
        cout << "(DMG Reduction: ";
    }

    if (target != is_treasure && target != is_potion){
        cout << item.getStat();
    }

    if (target == is_armor){
        cout << "%) ";
    } else if (target == is_weapon){
        cout << ") ";
    }

    cout << item.getItemName() << " [";

    if (target != is_treasure){
        cout << item.getCost() * tax;
    } else {
        cout << item.getCost();
    }

    cout << " Gold]\n";

    if (target == is_weapon){
        cout << " - Crit Chance: " << item.getCritChance();
        cout << "%; Crit Boost: " << item.getCritBoost() << "%\n";
    }

    if (target == is_weapon || target == is_potion){
        displayEffect(item.weapon_effect);
    }
}

/*Algorithm: Opens merchant prompt and allow user to make purchases or sell treasures
    1) Sets up costs for food and armor since neither are from Item class
    2) Sets up wrong_input, a penalty that ensures that players would be careful
    with 'wasting' the merchant's time
    3) Sets up the 'tax' that increases prices based on explored rooms
    4) Sets up 'purchase', which keeps the prompt active until user chooses to exit merchant prompt
    5) Updates the inventory text file and initiates merchant prompt
    6) In a while loop, the user would be able to make purchases or sell treasures based on input
    7) Each input would repeat if input is invalid
    No parameters or returns*/
void Party::merchant(){
    //If players deliberately make wrong inputs (i.e. letters for number input) at certain points
    //They will be punished
    int wrong_input = 0;

    double tax = 1 + (0.25 * explored_rooms); //Modifies prices based on how many rooms were explored

    cout << "If you're looking to get supplies, you've come to the right place." << endl;
    cout << "I would be happy to part with some of my wares...for the proper price!" << endl;

    while (true){
        //All are string to avoid players trying to "brick" the game
        string choice = "", amount = ""; //These are strings in case of invalid input
        int confirm_total;

        //These are for quick access to the names and original costs of items
        vector<Item> displayed_items;

        cout << endl;
        showInventory();
        cout << endl;
        cout << "Choose one of the following:" << endl;
        cout << "1. Weapons: It's dangerous to go alone, take this!" << endl;
        cout << "2. Armor: If you want to survive monster attacks, you will need some armor." << endl;
        cout << "3. Potions: Dangerous and potent, but unfair. To your enemies that is." << endl;
        cout << "4. Sell treasures: If you find anything shiny, ";
        cout << "I would be happy to take it off your hands." << endl;
        cout << "5. Leave: Make sure you get everything you need, I'm leaving after this sale!" << endl;
        cout << endl;

        getline(cin,choice);

        while (!isNumber(choice)){
            cout << "I need a valid choice!" << endl;
            getline(cin,choice);
        }

        if (stoi(choice) >= 1 && stoi(choice) <= 4){
            //Weapons, Treasures, or Potions
            string target = ""; //Selected item type

            if (stoi(choice) == 1){ //Weapons
                cout << "I have a plentiful collection of weapons to choose from,";
                cout << " what would you like?" << endl;
                cout << "Note that each weapon have base attack power,";
                cout << " marked by a (+X)." << endl;
                cout << "Plus, each got a little ... bonus." << endl;
                cout << endl;
                cout << "Choose one of the following:" << endl;

                target = is_weapon;
            } else if (stoi(choice) == 2){ //Armor
                cout << "Looking for suits of armor, I see." << endl;
                cout << "None of these will make you look like a dashing knight in shining armor";
                cout << " (except one), but they do well in keeping you alive." << endl;
                cout << "Keep in mind that you can only carry a maximum of ";
                cout << max_armor << " sets of armor." << endl;
                cout << endl;
                cout << "Have a look:" << endl;
                target = is_armor;
            } else if (stoi(choice) == 3){ //Potions
                cout << "I have many potions to choose from, what would you like?" << endl;
                cout << "Each have their own ... special quirks." << endl;
                cout << endl;
                cout << "Pick and choose:" << endl;
                target = is_potion;
            } else { //Treasures
                cout << "Oooh! You got treasures? Brilliant!" << endl;
                cout << "Let's see, here are the treasures I'm looking for:" << endl;
                cout << endl;
                cout << "Which ones do you have?" << endl;

                target = is_treasure;
            }

            //Prints all items in merchant array with target item type
            for (int i = 0; i < merchantList.size(); i++){
                if (merchantList[i].getItemType() == target){
                    displayed_items.push_back(merchantList[i]);

                    cout << displayed_items.size() << ". ";

                    presentMerchantItem(displayed_items.at(displayed_items.size() - 1),target,tax);
                    cout << endl;
                }
            }
            cout << (displayed_items.size() + 1) << ". Cancel" << endl;

            while (true){
                getline(cin,choice);

                if (isNumber(choice)){
                    if (stoi(choice) > 0 && stoi(choice) <= (displayed_items.size() + 1)){
                        break;
                    }
                }

                cout << "Valid Input Please" << endl;
            }

            //Cancels weapon selection if choice is the Cancel option
            if (stoi(choice) == (displayed_items.size() + 1)){
                continue;
            }

            if (target != is_treasure){ //Purchasing
                //Complete this crap
                while (true){
                    cout << "(Enter how many items you wish to purchase, or 0 to cancel)" << endl;
                    getline(cin,amount);

                    if (isNumber(amount)){ //Valid Number
                        if (stoi(amount) >= 0){ //Non-negative
                            break;
                        }
                    }

                    cout << "I need an appropriate amount." << endl;
                    cout << endl;
                }

                //Calculates price
                confirm_total = displayed_items.at(stoi(choice) - 1).getCost() * tax * stoi(amount);

                if (stoi(amount) != 0){ //0 is Cancel
                    purchaseProcess(stoi(amount),confirm_total,displayed_items.at(stoi(choice) - 1));
                }
            } else { //Treasure; selling
                while (true){
                    string confirm = "";
                    //Checks for confirmation of selling treasures
                    do{
                        cout << endl;
                        cout << "You want to sell " << displayed_items.at(stoi(choice) - 1).getItemName();
                        cout << " for " << displayed_items.at(stoi(choice) - 1).getCost() << " Gold? (y/n)\n";

                        getline(cin,confirm);

                        if (confirm != "y" && confirm != "Y" && confirm != "n" && confirm != "N"){
                            cout << "Valid confirmation please." << endl;
                        }
                    } while (confirm != "y" && confirm != "Y" && confirm != "n" && confirm != "N");

                    cout << endl;
                    
                    if (confirm == "y" || confirm == "Y"){ //Sells treasure
                        if (removeItem(displayed_items.at(stoi(choice) - 1))){ //Sold treasure successfully
                            //Gives party more gold
                            setMoney(money + displayed_items.at(stoi(choice) - 1).getCost());
                        } else {
                            cout << "How unfortunate, it appears you don't have the treasure." << endl;
                        }
                    } else {
                        cout << "Damn. Very well, keep your treasure then." << endl;
                    }

                    break;
                }
            }
        } else if (stoi(choice) == 5){ //Leave; Terminates merchant event
            cout << "Farewell!" << endl;
            break;
        } else {
            cout << "Why are we beating around the bush?" << endl;
            cout << "(Enter a character to display merchant's menu)" << endl;
            getline(cin,choice);
        }
    }
    cout << endl;

    return;
}

void Party::displayEffect(Effect effect){
    cout << "     Ability: " << effect.getEffectName() << " - ";

    //Weapon
    if (effect.getEffectName() == effect_Burn){
        cout << "Inflicts " << effect.getEffectValue() << " damage onto enemy every turn. ";
        cout << "Lasts for " << effect.getEffectDuration() << " turn(s).";
    } else if (effect.getEffectName() == effect_EnemyHP){
        cout << "Inclicts damage equal to " << effect.getEffectValue() << "% of the enemy's ";
        cout << "current health (before base attack calculation).";
    } else if (effect.getEffectName() == effect_Freeze){
        cout << "Immobilize enemy for " << effect.getEffectDuration() << " turns.";
    } else if (effect.getEffectName() == effect_Pierce){
        cout << "Weapon Damage ignores enemy defenses, including armor.";
    }

    //Potion
    if (effect.getEffectName() == effect_Heal){
        cout << "Heals chosen player by " << effect.getEffectValue() << ".";
    } else if (effect.getEffectName() == effect_Rage){
        cout << "Chosen player gets to attack " << effect.getEffectValue();
        cout << " times. Lasts " << effect.getEffectDuration() <<" turns.";
    } else if (effect.getEffectName() == effect_Undying){
        cout << "Chosen player takes no damage for " << effect.getEffectDuration() << " turns.";
    } else if (effect.getEffectName() == effect_Resurrection){
        cout << "Revives chosen dead player.";
    } else if (effect.getEffectName() == effect_Godslayer){
        cout << "No god will stand against you. Not anymore.";
    }
    cout << endl;

    return;
}

//Displays Inventory status, party status, and other information
void Party::showPartyStatus(){
    cout << "+-------------+" << endl;
    cout << "| STATUS      |" << endl;
    cout << "+-------------+" << endl;
    cout << "| Rooms Cleared: " << getExploredRooms();
    cout << " | Keys: " << getKeys() << " | Patience Level: " << getDangerLevel() << endl;
    cout << "+-------------+" << endl;
    cout << "| PARTY       |" << endl;
    cout << "+-------------+";
    //Main player is first displayed
    for (int i = 0; i < player_size; i++){
        cout << "\n| " << players[i].getPlayerName();
        if (armorSets[i].getItemName() != default_item_name){
            cout << " (" << armorSets[i].getItemName() << ")";
        }
        cout << " | Health: " << players[i].getPlayerHealth() << " | ";
        
        //Displays weapons
        if (weapon_barracks[i].getItemName() != default_item_name){
            cout << "Equipped: " << weapon_barracks[i].getItemName();
            cout << "(Atk: " << weapon_barracks[i].getStat() << ")";
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
    cout << "Answer carefully; one shot, mate." << endl;
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
            if (!changePlayerHealth(kill_index,-100)){ //Kills player
                return false; //Main player is dead
            }
        }
    } else if (outcome == -2){ //Defeat
        setMoney(getMoney()/4);
        cout << "One for all, and all will fall." << endl;
        for (int i = 1; i < player_size; i++){
            if (health_chance <= 10){
                //Kills player
                if (!changePlayerHealth(i,-100)){
                    return false;
                }
            }
        }
    }

    for (int i = 0; i < getPlayerSize(); i++){
        if (health_chance <= 50){ //Percentage form; random chance for EACH member
            if (!changePlayerHealth(i,-1)){ //i is index of player
                return false; //Main player is dead
            }
        }
    }

    return true;
}