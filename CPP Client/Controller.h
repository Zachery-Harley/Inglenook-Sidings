/* 
 * File:   Controller.h
 * @author zah2@aber.ac.uk
 * @version 15/12/2016
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "Wagon.h"
#include "Siding.h"


using boost::asio::ip::tcp;

class Controller {
    
public:
    //Wagons and siding lists
    std::vector<Wagon> wagons;
    std::vector<Siding> sidings;
    
    /**
    * Creates the main loop in the program to read and write to and from the
    * client passing the input commands to the server and executing the command
    * locally if status 1 returned form the server. 
    * 
    * @param server: Pointer to a boost socket connected to the C server
    */
    Controller(boost::shared_ptr<tcp::socket> server);
    
    /**
     * Send a string message to the server socket passed to the Controller
     * class.
     * @param message: Message to be send to the server
     */
    void netSend(std::string message);
    
    /**
     * Wait for a string of data to be received on the server socket passed to
     * the Controller object and return it.
     * @return string: The text received on the socket. 
     */
    std::string netRead();
    
    /**
     * Get an input from the console and return it.
     * @return string: The message entered by the user.
     */
    std::string getUserInput();
    private:
    //A pointer to a socket that is connected to the C server
    boost::shared_ptr<tcp::socket> server_socket;
};

#endif /* CONTROLLER_H */

