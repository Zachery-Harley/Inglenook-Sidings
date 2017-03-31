/* 
 * File:   main.c
 * Author: zachery
 *
 * Created on 29 November 2016, 12:05
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "sidings.h"
#include "controller.h"

int main(int argc, char** argv) {
    int server_id, client_id, error;
    struct sockaddr_in server, client;
    server_id = -1;
    client_id = -1;
    
    //Create the server socket
    server_id = socket(AF_INET, SOCK_STREAM, 0);
    if(server_id < 0)
        abort("Could not create socket!");
    
    //Configure socket to return port immediately once closed connection
    int optval = 1;
    setsockopt(server_id, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    
    
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    
    //Bind the server socket
    error = bind(server_id, (struct sockaddr *) &server, sizeof(server));
    if(error < 0) abort("Could not bind socket!");
    
    //Listen on the socket, and allow up to 10 connections pending
    error = listen(server_id, 10);
    if(error < 0) abort("Could not listen on socket!");
    
    fprintf(stderr, "\nListening on port %d", SERVER_PORT);
    
    /*Loop until client closes connection with command quit*/
    while(1) {
        unsigned int client_length;
        
        /*Wait for a connection*/
        msg("Waiting to establish connection....");
        do {
            errno = 0; /* Clear errno as not to react to an old error */
            client_length = sizeof(client);
            client_id = accept(server_id, (struct sockaddr *) & client,
                   &client_length);
        }while(client_id < 0 && errno == EINTR);
        
        //If the connection failed
        if (client_id < 0) {
            err("Couldn't accept; waiting..");
            sleep(5);
            continue;
        }

        msg("Connection established!");
        
        //Pass the client socket to the controller
        controller(client_id);
        //Close the connection to the client
        close(client_id);
    }
    
    return (EXIT_SUCCESS);
}

