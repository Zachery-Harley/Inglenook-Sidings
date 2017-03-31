/* 
 * FFile:   Command.h
 * @author zah2@aber.ac.uk
 *
 * @version 15/12/2016
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "Controller.h"

using namespace std;

class Command {
public:
    
    /**
     * Execute commands on the server and locally
     * @param comm: The Controller, in most cases 'this'
     */
    Command(Controller * comm){controller = comm;};
    
    /**
     * Given a status string from the server, locate and return the
     * status number
     * @param status_string: String from the server
     * @return int: the value of the status
     */
    int getStatus(string status_string);
    
    /**
     * Builds and returns the error information from the given input
     * @param status: The status id given from the SBP
     * @return string: A human readable error message
     */
    string throwError(int status);
    
    /**
     * When passed a user input of 3 ints in the order of, siding from, to and 
     * the number of wagons to move, this will handle the commands to run,
     * execute them and handle any errors.
     * @param command: 3 integers in a string
     * @return 1 if the command run, -1 failed, -2 closed connection
     */
    int runCommand(string command);
    
    /**
     * Print off the siding in a neat acsi format
     * @param accessed, The siding to be accessed
     */
    void printSiding(int accessed);
    
private:
    
    /**
     * Run the command take on the server and locally
     * @param from: The siding to take from, not 0
     * @param count: The number of wagons to move
     * @return 1 is success, -1 otherwise
     */
    int take(int from, int count);
    
    /**
     * Run the take command locally and print the yard out
     * @param to: The siding index to move from to headshunt
     * @param count: The number of wagons to move
     * @return 1 if success, -1 otherwise
     */
    int lTake(int from, int count);
    
    /**
     * Run the put command on the server and locally 
     * @param to: The siding index to move to
     * @param count: The number of siding to move from the headshunt
     * @return 1 is success, -1 otherwise
     */
    int put(int to, int count);
    
    /**
     * Run the put command locally and print the yard out
     * @param to: The siding index to move to from headshunt
     * @param count: The number of wagons to move
     * @return 1 if success, -1 otherwise
     */
    int lPut(int to, int count);
    

    
    /**
     * Execute put, take or both depending on the given values.
     * @param from: index of siding to take from
     * @param to: index of siding to put on
     * @param wagons: number of wagons to move
     * @return 1 if success, -1 failed, -2 close connection
     */
    int execute(int from, int to, int wagons);
    Controller * controller;
};

#endif /* COMMAND_H */

