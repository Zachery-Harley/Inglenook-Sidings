/**
 * File : wagon.cpp
 *
 * Purpose: Wagon inherits the vehicle class and allows for the creation of
 * wagons able to store information about their cargo, owner and serial. 
 * @author Zachery Harley zah2@aber.ac.uk
 * @version 15/12/2016
 */

#include "Wagon.h"

//Constructer for the wagon
Wagon::Wagon(boost::tokenizer<boost::escaped_list_separator<char> >::iterator &data)
    : vehicle(data)
{   
    cargo_ = *data;
    ++data;    
};

//Returns a string that can be used with << 
std::string Wagon::streamHelper() const {
    std::stringstream os;
    os << vehicle::streamHelper() << " Cargo: " << cargo_;
    return os.str();
}
