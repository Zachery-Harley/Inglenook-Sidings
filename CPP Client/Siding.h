/* 
 * File:   Siding.h
 * @author zah2@aber.ac.uk
 *
 * @version 11/12/2016
 */

#ifndef SIDING_H
#define SIDING_H

#include "Wagon.h"

class Siding
{
    //Allows for the overloaded insertion operator to be created <<
    friend std::basic_ostream<char>& operator<<(
            std::basic_ostream<char>&, const Siding&);
    
public:
    /**
     * Create a siding object able to hold a set number of wagons on the line,
     * contains functions that allow the pushing and popping of wagons similar
     * to a stack.
     * @param len: The number of wagons the line can hold
     */
    Siding(int len);
    
    /**
     * Push a wagon onto the line if there is room for it.
     * @param wagon: The wagon to be added to the line
     * @return int: Error code, -1 failed, 1 succeeded
     */
    int push(Wagon wagon);
    
    /**
     * Pop the first wagon off the line and return it.
     * @return Wagon: The wagon popped off, Wagon with serial -1 if failed.
     */
    Wagon pop();
    
    /**
     * Given a wagon serial and a vector of wagons, locate the wagon with a
     * matching serial from the vector list and add it to the siding.
     * @param serial: The serial number of the wagon to be added
     * @param wagons: The vector list of wagons that contains the wagon with
     * the given serial.
     * @return int: -1 Wagon not found, 1 Wagon added
     */
    int addWagon(int serial, std::vector<Wagon> wagons);
    
    std::vector<Wagon> * getLine(){
        return &line;
    }
    
    int getLength(){
        return length;
    }
protected:
    /*!
     * helper function used to create polymorphic behaviour for stream insertion.
     * Overrides of this function should call the parent class version and then
     * add information from this class to it. 
     * The insertion operator for each subclass then only needs to call 
     * this function (see operator<< below) for an example.    
     */
    std::string streamHelper() const;
    
private:
    int length;
    std::vector<Wagon> line;
};


#endif /* SIDING_H */

