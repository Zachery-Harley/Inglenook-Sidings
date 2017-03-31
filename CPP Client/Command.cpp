/**
 * File : Commanad.cpp
 *
 * Purpose: Class that handles the parsing of the server statuses and the
 * handling of local commands
 * @author Zachery Harley zah2@aber.ac.uk
 * @version 15/12/2016
 */

#include <string>
#include <boost/tokenizer.hpp>
#include <iostream>
#include <sstream>
#include <exception>

#include "Command.h"

using namespace boost;
using namespace std;
//gets the status code from a given status string
int Command::getStatus(string status_string){
    //Create the tokenizer that will break the status into parts
    escaped_list_separator<char> els(""," ","\"\'");
    tokenizer<escaped_list_separator<char>> tok(status_string, els);
    tokenizer<escaped_list_separator<char>>::iterator args = tok.begin();
    
    //The second status holds the number
    ++args;
    while(*args == "") ++args;
    return stoi(*args);
}

//Return the correct error form the status code
string Command::throwError(int status){
    ostringstream os;
    
    switch(status){
        case 1 : os << "Success" << endl;
                 return os.str();
        case -1: os << "Not enough space in destination" << endl;
                 return os.str();
        case -2: os << "Not enough wagons in source" << endl;
                 return os.str();
        case -3: os << "Reference to non existing siding" << endl;
                 return os.str();
        case -4: os << "Invalid Command!" << endl;
                 return os.str();
        default: os << "Unknown Status!" << endl;
                 return os.str();
    }
}

//The public command handler, executes the correct command when passed the user input
int Command::runCommand(string command){
    int from, to, wagons, count, error;
    count = 0;
    
    //Create the tokenizer that will break the status into parts
    escaped_list_separator<char> els(""," ","");
    tokenizer<escaped_list_separator<char>> tok(command, els);
    tokenizer<escaped_list_separator<char>>::iterator args = tok.begin();
    
    //Ensure there are 3 arguments
    while(args != tok.end()){
        if(*args != ""){
            count++;
        }
        ++args;
    }
    
    if(count != 3){
        cout << "Invalid number of commands given!" << endl;
        return -1;
    }
    
    //Get the arguments as there was 3, first reset args
    args = tok.begin();
    while(*args == "") ++args; //remove any empty tokens
    from = stoi(*args);
    ++args;
    
    while(*args == "") ++args; //remove any empty tokens
    to = stoi(*args);
    ++args;
    
    while(*args == "") ++args; //remove any empty tokens
    wagons = stoi(*args);
    
    //Execute the commands to carry out the action
    error = execute(from, to, wagons);
    if(error == -1){
        printSiding(0);
    }
    return error;
}

//Executes the correct command based on the given data
int Command::execute(int from, int to, int wagons){
    string command_string;
    
    //Check for negative arguments to close the connection
    if(from < 0 || to < 0 || wagons < 0){
        cout << "Closing connection!" << endl;
        controller->netSend("close");
        //-2 wont trigger the drawing of the yard
        return -2;
    }
    
    cout << "Moving: " << wagons << " wagons from siding: " <<
            from << " to siding: " << to << endl;
    
    //Execute the commands needed
    if(from == 0 && to != 0){                  
        //If from is 0 then PUT
        return put(to, wagons);
        
    } else if (to == 0 && from != 0){            
        //If to is 0 then Take
        return take(from, wagons);
        
    } else if (from > 0 && to > 0){ 
        //If neither 0 then TAKE, PUT
        if(take(from, wagons) == 1){
            return put(to, wagons);
        } else {
            return -1;
        }
    } else {
        cout << "Invalid command given!" << endl;
        return -1;
    }
    
}

//Execute the take command
int Command::take(int from, int count){
    int status;
    string status_string;
    ostringstream command_string;
    
    //Build and run the command
    command_string << "take " << (from-1) << " " << count;
    controller->netSend(command_string.str());
    //Get result back
    status_string = controller->netRead();
    status = getStatus(status_string);
    
    if(status == 1){
        //Do siding take
        cout << "SBP reports " << count << " wagons moved to headshunt" << endl;
        return lTake(from, count);
    } else {
        cout << throwError(status);
        return -1;
    }    
}

//Execute the put command
int Command::put(int to, int count){
    int status;
    string status_string;
    ostringstream command_string;
    
    //Build and run the command
    command_string << "put " << (to-1) << " " << count;
    controller->netSend(command_string.str());
    //Get result back
    status_string = controller->netRead();
    status = getStatus(status_string);
    
    if(status == 1){
        //Do siding put
        cout << "SBP reports " << count << " wagons moved to siding: " << to << endl;
        return lPut(to, count);
    } else {
        cout << throwError(status);
        return -1;
    }    
}

//Try and carry out the take command locally
int Command::lTake(int from, int count){
    try{
        Siding * side_from = &controller->sidings.at(from);
        Siding * side_to = &controller->sidings.at(0);
        for(int i = 0; i < count; i++){
            //Get the wagon from
            Wagon taken = side_from->pop();
            if(taken.getSerialNumber() == -1){
                cout << "An error has occurred, out of sync! Close and start again!" << endl;
                return -1;
            }
            side_to->push(taken);
        }
        printSiding(from);
        return 1;
        
    } catch (int e) {
        cout << "An error has occurred, out of sync! Close and start again!" << endl;
        return -1;
    }
}

//Try and carry out the put command locally
int Command::lPut(int to, int count){
    try{
        Siding * side_from = &controller->sidings.at(0);
        Siding * side_to = &controller->sidings.at(to);
        for(int i = 0; i < count; i++){
            //Get the wagon from
            Wagon taken = side_from->pop();
            if(taken.getSerialNumber() == -1){
                cout << "An error has occurred, out of sync! Close and start again!" << endl;
                return -1;
            }
            side_to->getLine()->insert(side_to->getLine()->end(), taken);
            //side_to->push(taken);
        }
        printSiding(to);
        return 1;
        
    } catch (int e) {
        cout << "An error has occurred, out of sync! Close and start again!" << endl;
        return -1;
    }
}

//Print the sidings off graphicly
void Command::printSiding(int accessed){
    ostringstream os, headshunt;
    int headshunt_length;
    string leading_white, flipshunt;
    
    //Build the headshunt string backwards
    headshunt << controller->sidings.at(0);
    headshunt_length = headshunt.str().length();
    
    flipshunt = headshunt.str();
    flipshunt = string(flipshunt.rbegin(), flipshunt.rend());
    
    os << flipshunt;

    headshunt_length = os.str().length();
    
    //Build white space
    for(int i = 0; i < headshunt_length; i++) leading_white += " ";
    os << "\\\\" << endl;
    
    //Build ASCI track
    for(int i = 1; i < controller->sidings.size(); i++){
        if(i == accessed || i == controller->sidings.size()-1) {
            os << leading_white << "\\\\" << endl;
            os << leading_white << " \\\\" << controller->sidings.at(i) << endl;
        } else {
            os << leading_white << "||" << endl;
            os << leading_white << "|| " << controller->sidings.at(i) << endl;
        }
    }
    
    cout << os.str() << endl;
}