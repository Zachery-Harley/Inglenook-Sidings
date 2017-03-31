/* 
 * File:   main.cpp
 * @author: zah2@aber.ac.uk
 *
 * @version 14/12/2016
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "Controller.h"
#include "Main.h"

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;


int main(int argc, char** argv) {   
    //Local variables
    string server_name;
    
    //Output to the console
    cout << "----Inglenook Siding Client zah2----" << endl;
    cout << "Inglenook Siding C Location: ";
    cin >> server_name;
    
    //Clear the cin
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    //Attempt to connect
    cout << "Trying to connect to " << server_name << endl;
    
    try{
        asio::io_service io_service;
        
        tcp::resolver resolver(io_service);
        
        tcp::resolver::query query(server_name, LAMBPORT);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        
        //Connect to the server and create a shared pointer socket
        boost::shared_ptr<tcp::socket> server_socket(new tcp::socket(io_service));
        boost::asio::connect(*server_socket, endpoint_iterator);
        
        //Pass the tcp socket to the controller
        Controller controle = Controller(server_socket);
        
        //Try and close the socket
       
        server_socket->close();
        
        
    } catch (std::exception& e) {
        cerr << e.what() << endl;
    }
    
    cout << endl << endl << "Thank-you for choosing zah2 for all your"
            << " Inglenook Siding solutions";
    
    
    return 0;
}

