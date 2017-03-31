/* 
 * File:   Loader.h
 * @author Zachery Harley zah2@aber.ac.uk
 *
 * @version 11/12/2016
 */

#ifndef LOADER_H
#define LOADER_H

#include <string>
#include "Siding.h"
#include "Wagon.h"

using namespace std;

class Loader {
    
private:
    /**
     * Given a string, remove any tab characters and remove trailing and
     * leading spaces.
     * @param input: The string to be cleaned
     * @return string: The cleaned string
     */
    string cleanString(string input);
    
    //The load and config command generated inside of getSidings
    string config_command;
    string load_command;
    
    //Default file locations
    const string WAGONS_DEFAULT = "wagons";
    const string SIDING_DEFAULT = "siding";

public:
    /**
     * Class able to load given files and parse the data out of them returning
     * a list of either Wagons or Sidings.
     */
    Loader(){};
    
    /**
     * Prompt the user for a file, loops until a valid file given
     * @return string: Return file path
     */
    string getFile();
    
    /**
     * Given a file path to a wagon data file, load the wagons and store them
     * in a vector
     * @param file_path: The path to the file, including the file name
     * @return A vector list of wagons.
     */
    vector<Wagon> getWagonData(string file_path);
    
    /**
     * Load wagons from the default location defined in Loader.h WAGONS_DEFAULT
     * @return A vector list of wagons
     */
    vector<Wagon> getWagonData();
    
    /**
     * Given a vector list of wagons load siding from the default location
     * defined in Loader.h SIDING_DEFAULT, and add the wagons to the siding
     * line.
     * @param wagons: A vector list of wagons
     * @return A vector list of sidings
     */
    vector<Siding>getSidings(vector<Wagon> wagons);
    
    /**
     * Given a vector list of wagons load siding from the given path, and add 
     * the wagons to the siding line.
     * @param wagons: A vector list of wagons
     * @param file_path: The path to the file, including the file name
     * @return A vector list of sidings
     */
    vector<Siding>getSidings(vector<Wagon> wagons, string file_path);
    
    /**
     * Once the getSiding command has been run, this can return the command
     * needed to load the wagons in the C program
     * @return The load command 
     */
    string getLoadCommand(){return load_command;};
    
    /**
     * Once the getSiding command has been run, this can return the command
     * needed to load the sidings in the C program.
     * @return The config command
     */
    string getConfigCommand(){return config_command;};
    
};

#endif /* LOADER_H */

