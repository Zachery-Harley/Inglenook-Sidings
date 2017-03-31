/**
 * File : Controller.cpp
 *
 * Purpose: Handle reading in commands from the user and directing the command
 * to the appropriate function to carry out the action. Controller also handles
 * the send and reading of information to and from the server socket.
 * 
 * @author Zachery Harley zah2@aber.ac.uk
 * @version 15/12/2016
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "Controller.h"
#include "Loader.h"
#include "Command.h"


using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

//The main loop for the program
Controller::Controller(boost::shared_ptr<tcp::socket> server){
    server_socket = server;
    Loader load = Loader();
    Command comm = Command(this);
    int status = -1;
    string entered_command;
    
    cout << netRead() << endl;
    
    //Basic menu to load default or specify load files
    //Load wagons
    while(true){
        cout << "Load default wagons? [Y/N]" << endl;
        string input = getUserInput();
        //Check input
        if(input == "Y" || input == "y"){
            wagons = load.getWagonData();
            break;
        } else if (input == "N" || input == "n"){
            wagons = load.getWagonData(load.getFile());
            break;
        } else {
            cout << "\nInvalid Command!" << endl;
        }
    }
    
    //Load sidings
    while(true){
        cout << "Load default sidings? [Y/N]" << endl;
        string input = getUserInput();
        //Check input
        if(input == "Y" || input == "y"){
            sidings = load.getSidings(wagons);
            break;
        } else if (input == "N" || input == "n"){
            sidings = load.getSidings(wagons, load.getFile());
            break;
        } else {
            cout << "\nInvalid Command!" << endl;
        }
    }
    
    //Run config and load commands
    netSend(load.getConfigCommand());
    status = comm.getStatus(netRead());
    if(status != 1){
        cout << "Failed to run config!" << endl;
        exit(0);
    }
    cout << "SBP reports config run" << endl;
    
    netSend(load.getLoadCommand());
    status = comm.getStatus(netRead());
    if(status != 1){
        cout << "Failed to run load!" << endl;
        exit(0);
    }
    cout << "SBP reports load run" << endl << endl; 
    
    //print wagons out
    cout << "Wagon Details:" << endl;
    for(Wagon wag : wagons){
        cout << wag << endl;
    }
    
    cout << endl << "-1 -1 -1 to close connection" << endl << endl;
    
    comm.printSiding(0);
    
    //Start the main loop
    while(true){
        cout << "Enter <from siding number> <to siding number> <number of wagons>" << endl;
        entered_command = getUserInput();
        
        status = comm.runCommand(entered_command);
        if(status == -2)//Negative value given to close connection
            break;
    }
    
}

//Return the users input from the console
string Controller::getUserInput(){
    //Check if cin is bad
    if(cin.bad()){
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    //get the input 
    string output;
    getline(cin,output);
    //Check if cin is bad again
    if(cin.bad()){
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return output;
}

//Send a string through the server socket
void Controller::netSend(string message){
    system::error_code error;
    cout << "Sent SBP: " << message << endl;
    asio::write(*server_socket, asio::buffer(message),
            asio::transfer_all(), error);
}

//Read data from the server socket and return it as a string
string Controller::netRead(){
    system::error_code error;
    boost::array<char, 1024> output;
    string output_str;
    
    //Read message from the socket, convert the array to a string and return it
    size_t count = server_socket->read_some(asio::buffer(output), error);
    
    output_str = string(output.begin(), output.begin()+count);
    return output_str;
}
