# Personal Project 1 - Dungeon Adventure Game
# By: Rodolfo Martinez-Maldonado

## ONGOING PROJECT

## ABOUT THIS PROJECT

This personal project is about developing a text-based, adventure game where the user can create multiple characters and interact with said characters in turn-based combat against monsters.

Originally, this was my final project for the introductory computer science class, CSCI 1300, at the University of Colorado Boulder. It is my goal to use the skills I have learned up until now as a sophmore undergraduate student to enhance my initial design and make this a more enjoyable and interactive experience.

## HOW TO COMPILE AND RUN

Use the following command to compile this directory:

```
g++ -std=c++17 dungeonAdventure.cpp Map.cpp Item.cpp Party.cpp Player.cpp Monster.cpp -o game
```

When compiled successfully, use the following 4 arguments to run the executable:

```
./game items.txt monsters.txt story.txt
```

## DEPENDENCIES

Header Files: functions.h, item.h, map.h, monster.h, party.h, player.h

Text Files: items.txt, monsters.txt, story.txt