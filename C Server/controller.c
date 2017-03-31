/* 
 * File:   controller.c
 * Author: zachery
 *
 * Created on 29 November 2016, 13:35
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "main.h"
#include "sidings.h"
#include "commands.h"
#include "controller.h"

void controller(int client_id){
    char buffer[BUFFER_SIZE]; //Message buffer
    siding headshunt = {NULL, 0, NULL}; //Head-shunt
    char * return_message;    
    int status = 1;
    
    //Start defaults, these are for both testing and preventing errors
    int args_count = count_parameters("config 3 5 3 3");
    int * args = parse_parameters("config 3 5 3 3", args_count);
    config(&headshunt, args_count, args);
    
    args_count = count_parameters("load 2 3 3");
    args = parse_parameters("load 2 3 3", args_count);
    load(&headshunt, args_count, args);
    
    
    net_send(client_id,"\nInglenook Sidings zah2\n");
    //Loop to take commands until the user ends the connection
    while(1){
        if(status > -1){
            status = net_read(client_id, buffer);
            fprintf(stderr, "\nPassed Command: %s", buffer);
        }
        //If the user enters quit as the string then leave this loop.
        if(strncmp("close", buffer, 5) == 0 || status == -1){
            //Clear the current siding data to prevent a leak
            free(headshunt.line);
            siding * shunt = headshunt.next;
            clr_sidings(&shunt);
            msg("\nConnection closed by client!");
            break;
        }
        
        //Read the command
        status = read_command(buffer, &headshunt, client_id);
        bzero(buffer, BUFFER_SIZE);
        
    }
}

int net_read(int socket_id, char * output){
    int error = -1;
    bzero(output, BUFFER_SIZE);
    //Clear the buffer then read incoming data
    error = read(socket_id, output, BUFFER_SIZE);
    if(error < 0) {
        err("net_read - Failed to read from socket!");
        return -1;
    }
    return 1;
}

int net_send(int socket_id, char * message){
    int error = -1;
    error = write(socket_id, message, strlen(message));
    if(error < 0){
        err("net_send - Failed to write to socket!");
        return -1;
    }
    return 1;
}

int count_parameters(char * command){
    char * p = command;
    int count = 0;
    
    //Loop through the whole command and count the number of arguments
    while(*p) {
        if(isdigit(*p)) {
            strtol(p, &p, 10);//Read the number in base 10 then discard
            count++;
        } else {
            //Otherwise move to the next character.
            p++;
        }
    }
    return count; 
}

int * parse_parameters(char * command, int count){
    //If no parameters, therfore count is zero, return null
    if(count == 0){
        err("parse_parameters - No parameters given!");
        return NULL;
    }
    
    //Reserve the memory needed for the output
    int * output = (int *)malloc(count * sizeof(int));
    char * p = command;
    int i = 0;
    
    //read the commands into the output int array
    while(*p){
        if(isdigit(*p)){
            output[i] = strtol(p, &p, 10);//Read the integer and store it
            //Ensure the number is positive
            if(output[i] < 0)
                output[i] = output[i] * -1;
            i++;//Move to the next pointer
        } else {
            //Otherwise move on to the next character
            p++;
        }
    }
    return output;
}

int read_command(char * command, siding * head, int client_id){
    int args_count, error, siding_index, send_status;
    int * args;
    char * status_output = (char *)malloc(BUFFER_SIZE * sizeof(char));
    char * track_status;
    
    /*Set siding to -1 so that if no command given to access them, the
     * status function knows this and return NORMALs
     */
    siding_index = -1;
    args_count = count_parameters(command);
    args = parse_parameters(command, args_count);
    
    if(args_count < 1)
        error = BAD_COMMAND;
    
    //Read the command from the first x letters and run the correct method
    if(strncmp("put", command, 3) == 0){
        error = put(head, args_count, args);
        siding_index = args[0];
    } else if(strncmp("take", command, 4) == 0){
        error = take(head, args_count, args);
        siding_index = args[0];
    } else if(strncmp("load", command, 4) == 0){
        error = load(head, args_count, args);
        
    } else if(strncmp("config", command, 6) == 0){
        error = config(head, args_count, args);
        
    } else if(strncmp("quit", command, 4) == 0){
        quit(client_id);
    } else {
        err("read_command - Invalid command!");
        error = BAD_COMMAND;
    }
    //print_siding(head);
    track_status = get_normal(head, siding_index, error);
    snprintf(status_output,BUFFER_SIZE, "Status: %d %s \n", error, track_status);
    return net_send(client_id, status_output);
}