------------------------
HOW TO COMPILE AND RUN
------------------------
Compile:
g++ -std=c++17 dungeonAdventure.cpp Map.cpp Item.cpp Party.cpp Player.cpp Monster.cpp -o game
Run:
./game items.txt monsters.txt
------------------------
DEPENDENCIES
------------------------
item.h, map.h, monster.h, party.h, player.h,
inventoryDisplay.txt, items.txt, and monsters.txt
must be in the same directory as the cpp files in order to compile.

Not essential: split.h, battleStats.h
------------------------
SUBMISSION INFORMATION
------------------------
// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Dungeon adventure Game
------------------------
ABOUT THIS PROJECT
------------------------
This project is developing an adventure-styled game where the user can create 5 characters
and interact with said characters and monsters in turn-based combat.

The original version of this was as my final project for the introductory computer science class, CSCI 1300,
at the University of Colorado Boulder.