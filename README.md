# Personal Project 1 - Dungeon Adventure Game

## How to Compile and Run

### (Recall, still in development phase)

Use the following to compile:
'''
g++ -std=c++17 dungeonAdventure.cpp Map.cpp Item.cpp Party.cpp Player.cpp Monster.cpp -o game
'''
Compile:

g++ -std=c++17 dungeonAdventure.cpp Map.cpp Item.cpp Party.cpp Player.cpp Monster.cpp -o game
Run: (Exactly four arguments)
./game items.txt monsters.txt story.txt
DEPENDENCIES
------------------------
Header Files: item.h, map.h, monster.h, party.h, player.h
Text Files: inventoryDisplay.txt, items.txt, monsters.txt, story.txt
These files must be in the same directory as the cpp files in order to compile.

Not essential: split.h, battleStats.h
------------------------
SUBMISSION INFORMATION
------------------------
Author: Rodolfo Martinez-Maldonado
Personal Project 1 - Dungeon Adventure Game
------------------------
ABOUT THIS PROJECT
------------------------
This project is developing an adventure-styled game where the user can create 5 characters
and interact with said characters and monsters in turn-based combat.

The original version of this was as my final project for the introductory computer science class, CSCI 1300,
at the University of Colorado Boulder.