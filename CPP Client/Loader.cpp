/**
 * File : Loader.cpp
 *
 * Purpose: A class to handle the loading in of data to be used in the program
 * @author Zachery Harley zah2@aber.ac.uk
 * @version 15/12/2016
 */

#include <fstream>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

#include "Loader.h"
#include "Siding.h"
#include "Wagon.h"

using namespace std;
using namespace boost;

//Load sidings from the default siding location
vector<Siding> Loader::getSidings(vector<Wagon> wagons) {
    return getSidings(wagons, SIDING_DEFAULT);
}

//Load sidings from a given location
vector<Siding> Loader::getSidings(vector<Wagon> wagons, string file_path){
    string line, load_function, config_function;
    bool headshunt = true;
    vector<Siding> sidings; //Linked list of sidings
    
    //Init the load and config commands to be called after this executes
    load_function = "load ";
    config_function = "config ";

    //Load the given file and read the siding data
    ifstream file(file_path);
    while (getline(file, line))
    {
        line = cleanString(line);
        
        //Create the tokenizer that will break the line into parts
        escaped_list_separator<char> els(""," ","\"\'");
        tokenizer<escaped_list_separator<char>> tok(line, els);
        tokenizer<escaped_list_separator<char>>::iterator args = tok.begin();
        //Create new siding and add it to the linked list(vector)
        
        //the first argument is the length of the siding
        Siding new_siding = Siding(stoi(*args));
        //Add this siding to the config to build the command
        config_function += *args + " ";
        ++args;//Go to the next token, number of trains sometime ""
        while(*args == "") ++args;//get to next arg
        //Build the load function
        if(headshunt)
            headshunt = false;
        else
            load_function += *args + " ";
        
        if(new_siding.getLength() < stoi(*args)){
            cout << "To many wagons to load onto siding!" << endl;
            vector<Siding> error_siding;
            return error_siding;
        }
           
        ++args;//Go to next token, first train serial
        //Load the wagons on to the siding
        while(args != tok.end()){
            if(*args != ""){
                //Add the wagon to the siding
                new_siding.addWagon(stoi(*args), wagons);
            }
            ++args;//Next token
        }
        
        sidings.push_back(new_siding);
    }
    //Set the commands
    config_command = config_function;
    load_command = load_function;
    
    cout << "Sidings Loaded!\n";
    return sidings;
}

//Load the wagons from the default path
vector<Wagon> Loader::getWagonData(){
    return getWagonData(WAGONS_DEFAULT);
}

//Load the wagons from a given path
vector<Wagon> Loader::getWagonData(string file_path){
    string line;
    int wagon_count = 0;
    vector<Wagon> wagons; //Linked list of wagons
    
    ifstream file(file_path);
    while (getline(file, line)){
        //Create the tokenizer that will break the line into parts at commas
        escaped_list_separator<char> els("",",","\"\'");
        tokenizer<escaped_list_separator<char>> tok(line, els);
        tokenizer<escaped_list_separator<char>>::iterator args = tok.begin();
        //Create new wagon and add it to the linked list(vector)
        Wagon new_wagon = Wagon(args);
        wagons.push_back(new_wagon);
        //Increment the wagon counter
        wagon_count++;
    }
    cout << wagon_count << " wagons loaded!" << endl;
    return wagons;
}

//Get a file path from the user
string Loader::getFile(){
    string file_path;
    cout << "Please enter a directory: ";
    while(true){
        getline(cin,file_path);
        if(!std::ifstream(file_path))
            std::cout << "File not found!\n" << "Please check the path and try again: ";
        else
            return file_path;
    }
}

//Remove tabs and spaces from a string
string Loader::cleanString(string input){
    input.erase(remove(input.begin(), input.end(), '\t'), input.end());
    trim(input);
    return input;
}
