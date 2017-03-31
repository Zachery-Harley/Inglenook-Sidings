/* 
 * File:   commands.h
 * Author: zachery
 *
 * Created on 29 November 2016, 21:41
 */

#ifndef COMMANDS_H
#define COMMANDS_H

/**
 * Configure a new set of sidings, deleting the old ones in the process
 * @param head: Pointer to the head-shunt
 * @param num: Number of new sidings
 * @param args: Pointer to int array containing new siding sizes
 * @return The status code:
 *         SUCCESS -> Success
 *         NO_SPACE -> Destination insufficient space
 *         NO_TRAIN -> Not enough trains/wagons to move
 *         NO_SIDING -> Siding does not exist
 *         BAD_COMMAND -> Command not recognised
 */
int config(siding * head, int num, int * args);

/**
 * Load a number of trains onto the sidings removing any previous trains
 * @param head: Pointer to the head-shunt
 * @param num: Number of sidings present
 * @param args: Pointer to int array containing new tain line lengths
 * @return The status code:
 *         SUCCESS -> Success
 *         NO_SPACE -> Destination insufficient space
 *         NO_TRAIN -> Not enough trains/wagons to move
 *         NO_SIDING -> Siding does not exist
 *         BAD_COMMAND -> Command not recognised
 */
int load(siding * head, int num, int * args);

/**
 * Take a number of trains from a siding and store on the head-shunt
 * @param head: Pointer to the head-shunt
 * @param num: Number of arguments
 * @param args: args[0]->siding index. args[1]->trains to take
 * @return The status code:
 *         SUCCESS -> Success
 *         NO_SPACE -> Destination insufficient space
 *         NO_TRAIN -> Not enough trains/wagons to move
 *         NO_SIDING -> Siding does not exist
 *         BAD_COMMAND -> Command not recognised
 */
int take(siding * head, int num, int * args);

/**
 * Take a given number of trains from the head-shunt and put into given siding
 * @param head: Pointer to the head-shunt
 * @param num: Number of arguments given
 * @param args: args[0]->siding index. args[1]->trains to take
 * @return The status code:
 *         SUCCESS -> Success
 *         NO_SPACE -> Destination insufficient space
 *         NO_TRAIN -> Not enough trains/wagons to move
 *         NO_SIDING -> Siding does not exist
 *         BAD_COMMAND -> Command not recognised
 */
int put(siding * head, int num, int * args);

/**
 * Add a train to a given siding
 * @param node: Pointer to the siding in which to add the train
 * @param train: The index of the train to add
 * @return The status code:
 *         SUCCESS -> Success
 *         NO_SPACE -> Destination insufficient space
 *         NO_TRAIN -> Not enough trains/wagons to move
 *         NO_SIDING -> Siding does not exist
 *         BAD_COMMAND -> Command not recognised
 */
int add_train(siding * node, int train);

/**
 * Remove a train from the train line and return the trains index
 * @param node: Pointer to the siding from which to take the train
 * @return The status code:
 *         int >= 0 -> The trains_index
 *         NO_SPACE -> Destination insufficient space
 *         NO_TRAIN -> Not enough trains/wagons to move
 *         NO_SIDING -> Siding does not exist
 *         BAD_COMMAND -> Command not recognised
 */
int take_train(siding * node);

/**
 * Generates the track status response to be send back to the client
 * @param head: Pointer to the head-shunt
 * @param siding_index: Index of the siding that was accessed, -1 if no index
 * @param status_id: The status id of the program
 * @return A char * with all the line status
 */
char * get_normal(siding * head, int siding_index, int status_id);

/**
 * Reset the sidings line information. Should a load function fail, then this
 * should be called
 * @param head: Pointer to the head-shunt
 */
void reset(siding * head);

/**
 * Closes the connection to the client and ends the program with exit 0
 * @param client_id
 */
void quit(int client_id);


#endif /* COMMANDS_H */

