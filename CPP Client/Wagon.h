/* 
 * File:   wagon.h
 * @author zah2@aber.ac.uk
 *
 * @version 12/12/2016
 */

#ifndef WAGON_H
#define WAGON_H

#include "Vehicle.h"

class Wagon: public vehicle{
    
    std::string cargo_;
    
public:
    /*!
     * Wagon inherits the vehicle class, and allows for the creation of wagons
     * that are able to store there company, serial, and cargo.
     * @param data - a tokenizer iterator. Allow the Vehicle to be constructed from
     *               the next three tokens. 
     *               token pattern: SERIAL->OWNER->CARGO.
     * this function will throw run time exceptions if the iterator fails 
     * this function will throw run time exceptions if the integer conversion fails
     */
    Wagon(boost::tokenizer<boost::escaped_list_separator<char> >::iterator &data);
    Wagon(){vehicle(-1,"No Owner");};
protected:
    /*!
     * helper function used to create polymorphic behaviour for stream insertion.
     * Overrides of this function should call the parent class version and then
     * add information from this class to it. 
     * The insertion operator for each subclass then only needs to call 
     * this function (see operator<< below) for an example.    
     */
    std::string streamHelper() const;  
    
};


#endif /* WAGON_H */

