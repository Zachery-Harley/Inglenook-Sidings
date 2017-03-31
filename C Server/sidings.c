/* 
 * File:   sidings.c
 * Author: zachery
 *
 * Created on 29 November 2016, 13:40
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sidings.h"
#include "main.h"

siding * new_siding(int length){
    if(length > 0){
        //Reserve the space needed to store the struct siding
        siding * new = malloc(sizeof(siding));
        new->length = length;
        /*This line is a workaround to a problem created inside the
         * clear_siding method, the issue is when the siding is cleared
         * all emmory is freed hoever siding->line gets a new pointer at return
         * that points to nothing. This issue causes issues inside the
         * init_line method which when it reads the line with the pointer to
         * nothing, it tired to free the memory throwing an error
         * */
        new->line = (int *)malloc(sizeof(int));
        init_line(new);
        new->next = NULL;
        return new;
    } else {
        err("new_siding - Invalid length!");
        return NULL;
    }
}

int add_siding(siding * head, siding * new){
    //Check head is not a null pointer
    if(head != NULL){
        if(head->next == NULL){
            head->next = new;//Set the next pointer to the new siding
            return SUCCESS;
        } else {
            add_siding(head->next, new);//Move to the next siding
        }
    } else {
        //head was null
        err("add_siding - \"head\" was a NULL pointer!");
        return -1;
    }
}

void print_siding(siding * head){
    int offset;
    
    fprintf(stderr, "\n\nSiding Diagram\n");
    
    //Print of the head
    print_line(head->line, head->length);
    fprintf(stderr, "\\\\");
    offset = head->length * 5;
    
    //If head->next is ot a null pointer print its line using the slave method
    if(head->next != NULL){
        print_siding_(head->next, offset);
    }
    
}

void print_siding_(siding * node, int offset){    
    char indent[offset + 1]; //Will contain white space, the +1 to account for \0
    
    //Create indent
    memset(indent, ' ', sizeof(indent));
    indent[offset] = '\0'; //Add a null point terminator
    fprintf(stderr, "\n%s", indent);
    
    //Check if the node is the tail
    if(node->next == NULL){
        //Print tail ending
        fprintf(stderr, "\\\\\n");
        fprintf(stderr, "%s \\\\", indent);
    } else {
        //Print main line
        fprintf(stderr, "||");
    }
    
    //Move on to the next siding
    print_line(node->line, node->length);
    if(node->next != NULL){
        print_siding_(node-> next, offset);
    }
}

siding * get_siding(siding * head, int side){
    int index = -1;//To ensure that 0 points to the first siding
    
    //While head is not null, load the next siding
    do{
        //If the index matches the search then return the siding
        if(index == side)
            return head;
        else
            index++;
        
    } while((head = head->next) != NULL);
    //End of loop reached, no matching index found, return null
    return NULL;
}

void clr_sidings(siding ** node){
    siding * temp = *node;
    if(temp->next != NULL){
        clr_sidings(&temp->next);
    }
    /*Free siding pointers*/
    free(temp->line);
    temp->line = NULL;
    temp->next = NULL;
    free(*node);
    *node = NULL;
}

void init_line(siding * node){
    int i;
    
    //Ensure the node is not a NULL pointer
    if(node != NULL){
        //If line has a pointer clear it
        if(node->line != NULL)
            free(node->line);
        //Reserve space for the line and set all values to -1, or no train
        node->line = calloc(node->length, sizeof(int));
        for(i = 0; i< node->length; i++){
            node->line[i] = -1;
        }
    }
}

void print_line(int * line, int length){
    int i,t; //i:index, t:train number
    
    //Loop through the line and print the train data
    for(i = 0; i < length; i++){
        t = line[i];
        if(t == -1)
            fprintf(stderr, "=====");
        else
            fprintf(stderr, "=%d%d%d%d", t,t,t,t);
    }
}

int line_space(siding * node){
    int i, output = 0;
    //Loop through all line items and increment output on each free space (-1)
    for(i = 0; i < node->length; i++){
        if(node->line[i] == -1)
            output++;
    }
    return output;
}

int count_train(siding * node){
    return(node->length - line_space(node));
}