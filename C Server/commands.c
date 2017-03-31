/*
 * File:   commands.h
 * Author: zachery
 *
 * Created on 29 November 2016, 20:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sidings.h"
#include "main.h"
#include "commands.h"


int config(siding * head, int num, int * args){
    int error, i;
    
    if(num < 3){
        err("config - Invalid number of arguments, must be at least 3!");
        return BAD_COMMAND;
    }
    
    if(head->next != NULL)
        clr_sidings(&head->next);
    
    //Initiate the head-shunt
    head->length = args[0];
    head->next = NULL;
    init_line(head);
    
    //Setup the other sidings
    for(i = 1; i < num; i++){
        //Create new siding and check for success
        siding * new = new_siding(args[i]);
        if(new == NULL){
            return BAD_COMMAND;
        }
        //Add new siding to linked list and check for success
        error = add_siding(head, new);
        if(error < 0){
            return BAD_COMMAND;
        }
    }
    return SUCCESS;
}

int load(siding * head, int num, int * args){
    int train_index, trains, error, i, x;
    train_index = 0;
    siding * node = head->next;
    
    if(num == 0)
        return BAD_COMMAND;
    
    //Loop through all the arguments setting the number of trains per siding
    for(i = 0; i < num; i++){
        trains = args[i];
        
        //Clear the current train line
        init_line(node);
        //Must be either 0 or more trains
        if(trains < 0){
            reset(head);
            return BAD_COMMAND;
        }
        
        //If more trains than line space
        if(trains > line_space(node)){
            reset(head);
            return NO_SPACE;
        }
        
        //If end of loop and the next pointer is not null, throw error
        if(i == (num-1) && node->next != NULL){
            err("load - Not enough arguments!");
            reset(head);
            return NO_SIDING;
        }
        
        //If no more sidings left, but still arguments given, throw error
        if(node->next == NULL && i < num-1){
            err("load - To many arguments!");
            reset(head);
            return NO_SIDING;
        }
        
        //Add trains to the line
        for(x = 0; x < trains; x++){
            error = add_train(node, train_index);
            if(error != SUCCESS)
                return error;
            //Increment the train index and clear the error
            train_index++;                       
        }
        
        //Load the next siding if possible
        node = node->next;
    }
    return SUCCESS;
}

int take(siding * head, int num, int * args){
    siding * source;
    int i, train_index;
    
    //Must only be 2 commands
    if(num != 2)
        return BAD_COMMAND;
    //Must be space in the head-shunt for the trains
    if(args[1] > line_space(head))
        return NO_SPACE;
    //Get source and check for error
    source = get_siding(head, args[0]);
    if(source == NULL)
        return NO_SIDING;
    //Are there enough trains to move on the source
    if(count_train(source) < args[1])
        return NO_TRAIN;
    
    //Move the trains from the source to the head-shunt
    for(i = 0; i< args[1]; i++){
        train_index = take_train(source);
        add_train(head, train_index);
    }
    return SUCCESS;
}

int put(siding * head, int num, int * args){
    siding * dest;
    int train_index, i;
    
    //Must be 2 commands
    if(num != 2)
        return BAD_COMMAND;
    
    //Get the destination and check for error
    dest = get_siding(head, args[0]);
    if(dest == NULL)
        return NO_SIDING;
    
    //Is there space in the destination
    if(args[1] > line_space(dest))
        return NO_SPACE;
    
    //Are there enough trains to move from head-shunt
    if(args[1] > count_train(head))
        return NO_TRAIN;
    
    //Move the trains
    for(i = 0; i < args[1]; i++){
        train_index = take_train(head);
        add_train(dest, train_index);
    }
    return SUCCESS;
}

int add_train(siding * node, int train){
    int * train_line = node->line;
    int length = node->length;
    int i, next;
    
    //Loop through the line until the last empty space is found
    for(i = 0; i < length; i++){
        //Check if current location is taken, if so track is full
        if(train_line[i] != -1)
            return NO_SPACE;
        
        next = i+1;
        if(next >= length){
            //Next location is out of bounds
            train_line[i] = train;
            return SUCCESS;
        } else if(train_line[next] != -1){
            /*Next location is taken, therfore set here to be the train
            *NOTE: Separate if functions and not used || to prevent
            *segmentation error
            */
            train_line[i] = train;
            return SUCCESS;
        }
    }
    //End of loop reached, line length was zero, or an error occurred
    err("add_train - Length 0, or error occured!");
    return NO_SPACE;
}

int take_train(siding * node){
    int * train_line = node->line;
    int length = node->length;
    int train_index, i;
    
    //Loop through train line and fetch first train
    for(i = 0; i < length; i++){
        if(train_line[i] != -1){
            train_index = train_line[i];
            train_line[i] = -1;
            return train_index;
        }
    }
    //End of loop reached no train!
    return NO_TRAIN;
}

char * get_normal(siding * head, int siding_index, int status_id){
    char * output = (char *)malloc((BUFFER_SIZE - 50) * sizeof(char));
    int cur_siding = 0;
    
    if(head->next != NULL)
        //Skip the head-shunt
        head = head->next;
    else
        return "run config!";
    
    do{
        if(head->next != NULL){
            if(status_id != SUCCESS || siding_index == -1)
                /*As the status reads failed all tracks will be normal.
                 * Or if the siding_index is -1, this means track manipulation
                 * was not carried out and so print all normal
                 */
                strcat(output, "NORMAL ");
            else {
                //These are not the tail and so can be reversed
                if(cur_siding < siding_index || cur_siding > siding_index ){
                    strcat(output, "NORMAL ");
                } else if(cur_siding == siding_index){
                    strcat(output, "REVERSE ");
                }
                cur_siding++;
            }
        }
    }while((head = head->next) != NULL);
    
    //Return the output
    return output;
}

void reset(siding * head){
    init_line(head);
    //Next siding
    if(head->next != NULL)
        reset(head->next);
}

void quit(int client_id){
    msg("Connection closed by client!");
    close(client_id);
    exit(0);
}