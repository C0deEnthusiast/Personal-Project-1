// Author: Rodolfo Martinez-Maldonado
// Personal Project 1 - Namespace Functions Implementation

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Basic helper functions
namespace Functions{
    //Outputs pseudo-random value within a specified interval
    inline int createRand(int min, int max){ return (rand() % (max - min + 1)) + min;}

    //Checks if event will occur based on likelihood; input probability in percentage form
    inline bool willOccur(int probability){
        //Bypasses createRand() if event is essentially guaranteed to occur
        if (probability >= 100){ return true;}

        return (probability >= createRand(1,100));
    }

    //Checks if input string is a number
    inline bool isNumber(std::string line){
        if (line.length() == 0){ return false;}

        for (auto x: line){ //Checks if there are any char that are not numbers
            if (!isdigit(x)){ return false;}
        }

        return true;
    }

    //Copies lines from the specified file into a string vector and returns the vector
    inline std::vector <std::string> copyFile(std::string fileName){
        std::ifstream file_(fileName);
        std::vector <std::string> v;
        std::string line;

        if (!file_.is_open()){
            std::cout << "File is not open\n";
            return v;
        }

        while (getline(file_,line)){
            if (line.length() == 0){ continue;}

            v.push_back(line);
        }

        file_.close();

        return v;
    }

    //Uses placeholder string as a stall to let user read what the exposition says
    inline void convenientStop(void){
        std::string stop;
        std::cout << "(Enter a character to continue)\n";
        std::getline(std::cin, stop);

        return;
    }

    /* Uses 'seperator' to splice input string and stores them into specified 'arr' array

    Returns splice count of input string*/
    inline int arraySplit(std::string input_string, char separator, std::string arr[], int arr_size){
        if (input_string.length() == 0 || arr_size <= 0){ //Makes sure input_string is proper length
            return 0;
        }

        std::istringstream splice(input_string);
        int split_count = 0;

        while(getline(splice,arr[split_count],separator)){
            split_count++;
            
            if (split_count == arr_size){
                break;
            }
        }
        return split_count;
    }

    // Uses 'seperator' to splice input string and stores them into specified 'vect' vector
    inline void vectorSplit(std::string input_string, char separator, std::vector <std::string> &vect){
        if (input_string.length() == 0){ //Makes sure input_string is proper length
            return;
        }

        std::istringstream splice(input_string);

        while (std::getline(splice,input_string,separator)){
            vect.push_back(input_string);
        }

        return;
    }

    inline double percentToDecimal(int value){ return (static_cast<double> (value) / 100);}

};

#endif