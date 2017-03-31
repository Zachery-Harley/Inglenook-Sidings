/**
 * File : Siding.cpp
 *
 * Purpose: Represent a siding able to hold a set number of wagons on the line,
 * this also allows for the pushing and popping similar to a stack.
 * 
 * @author Zachery Harley zah2@aber.ac.uk
 * @version 12/12/2016
 */

#include <string>
#include <iostream>

#include "Wagon.h"
#include "Siding.h"

//Constructer for a siding
Siding::Siding(int len){
    if(len > 0){
        length = len;
    } else {
        std::cout << "Siding length must be greater than 0\n";
    }
}

//Try and add a wagon to the line, and return the status
int Siding::push(Wagon wagon){
    line.shrink_to_fit();//Ensure there is no excess space
    if(line.size() < length){
        std::vector<Wagon>::iterator it = line.begin();
        line.insert(it, wagon);
        return 1;
    } else {
        //No space left
        std::cout << "Insufficient space in siding!\n";
        return -1;
    }
}

//Try and remove a wagon from the line, if fails return a wagon with serial -1
Wagon Siding::pop(){
    line.shrink_to_fit();//ensure there is no excess space
    if(line.size() > 0){
        //There are vehicle left, remove the last one added.
        Wagon wagons = line.back();
        line.pop_back();
        return wagons;
    } else {
        std::cout << "No vehicle in siding to remove\n";
        //Return a default wagon with serial -1
        return Wagon();
    }
}

//Add a wagon from a given vector to the line, no length checks here
int Siding::addWagon(int serial, std::vector<Wagon> wagons){
    for(int i = 0; i < wagons.size(); i++){
        //check if the current wagon matches the serial
        if(wagons.at(i).getSerialNumber() == serial){
            //Add the wagon to the siding if their is space
            push(wagons.at(i));
            return 1;
        }
    }
    //The train was not found!
    return -1;
}

//Helper for the inserter operator << overload function
std::string Siding::streamHelper() const {
    std::stringstream os;
    int empty_space = length - line.size();
    for(int i = 0; i < empty_space; i++)
        os << "=====";
    for(int i = 1; i < line.size()+1; i++)
        os << "=" << line.at(line.size() - i).getSerialNumber();
    return os.str();
}

//overloaded inserter operator
std::basic_ostream<char>& operator<<(
        std::basic_ostream<char>& os, const Siding& side) {
    return os << side.streamHelper();
}
