/*
 * File:   controller.h
 * Author: zachery
 *
 * Created on 29 November 2016, 20:23
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

/**
 * The controller will loop reading commands from the given client id,
 * then parsing and executing the command, and lastly will return the
 * status back to the client, until the user enters close or quit.
 * @param client_id: The socket id to read from
 */
void controller(int client_id);

/**
 * Read from a socket and store the message in the output
 * @param socket_id: Socket id to read from
 * @param output: Where to store the message.
 * @return int: 1 for success, -1 otherwise
 */
int net_read(int socket_id, char * output);

/**
 * Read a message from the given socket id
 * @param socket_id: The socket id to read from
 * @param message: The message to send to the given socket id
 * @return int: 1 for success, -1 otherwise
 */
int net_send(int socket_id, char * message);

/**
 * Count the number of integer parameters given in a command string.
 * @param command: The command to count the parameters in
 * @return The number of parameters present in the command
 */
int count_parameters(char * command);

/**
 * Read the integer arguments from a command string and return them.
 * NOTE: This command works similar to strtok(), however this only reads
 * integers and will ignore any rouge letters.
 * @param command
 * @param count
 * @return 
 */
int * parse_parameters(char * command, int count);

/**
 * Read a given command and execute the relevant method and send the results to
 * the client.
 * @param command: The command to be read
 * @param head: Pointer to the head-shunt
 * @param client_id: Socket id of the client
 * @return The error code, 1 if data sent, -1 if error
 */
int read_command(char * command, siding * head, int client_id);
#endif /* CONTROLLER_H */

