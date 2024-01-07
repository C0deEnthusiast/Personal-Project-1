# Personal Project 1 - Dungeon Adventure Game
# By: Rodolfo Martinez-Maldonado

## How to Compile and Run

### (Recall, still in development phase)

Use the following to compile:

```
g++ -std=c++17 dungeonAdventure.cpp Map.cpp Item.cpp Party.cpp Player.cpp Monster.cpp -o game
```

When compiled successfully, use the following 4 arguments to run the executable:

```
./game items.txt monsters.txt story.txt
```

## DEPENDENCIES

Current Header Files: item.h, map.h, monster.h, party.h, player.h
Text Files: inventoryDisplay.txt (Will likely be removed), items.txt, monsters.txt, story.txt

Not essential: split.h (in vscode only)

## ABOUT THIS PROJECT

This project is developing a text-based, adventure game where the user can create 5 characters
and interact with said characters and monsters in turn-based combat.

The original version of this was my final project for the introductory computer science class, CSCI 1300,
at the University of Colorado Boulder.