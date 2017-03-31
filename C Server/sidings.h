/* 
 * File:   sidings.h
 * Author: zachery
 *
 * Created on 29 November 2016, 13:35
 */

#ifndef SIDINGS_H
#define SIDINGS_H

//Prevent the need to define siding as a struct
typedef struct siding siding;

/*
 * siding represents a siding, or part of the track
 * that comes off of the main line that can be used
 * to store trains/wagons
 */
struct siding {
    int * line;
    int length;
    siding * next;
};

/**
 * Create a new siding and return the memory pointer
 * @param length: The length of the line
 * @return Pointer to the new siding
 */
siding * new_siding(int length);

/**
 * Add a given siding to the current linked list of sidings
 * @param head: Pointer to the head of the lined list
 * @param new: Pointer to the new siding to append to the list
 * @return 1 if siding added successfully, -1 if failed
 */
int add_siding(siding * head, siding * new);

/**
 * Given the head-shunt will recustivley print off all the sidings presant
 * in the linked list.
 * @param head: Pointer to the head-shunt
 */
void print_siding(siding * head);

/**
 * Recursively print off the siding information working through the
 * linked list.
 * NOTE: Do note pass the head-shunt to this method, instead use the
 * helper method print_siding(siding * head) as this will print the
 * head shunt correctly and then call this method
 * @param node: Pointer to the node to start printing from
 * @param offset: The length of the required indentation
 */
void print_siding_(siding * node, int offset);

/**
 * Get a siding by its index.
 * NOTE: Sidings are stored as a linked list and so the is not actual index,
 * instead this method assigns each siding an index as it hits it.
 * @param head: Pointer to the head-shunt
 * @param side: The index of the siding wanted
 * @return Pointer to the siding if found, NULL otherwise
 */
siding * get_siding(siding * head, int side);

/**
 * Clear all siding likened to the node
 * NOTE: Do not pass the pointer to head-shunt, head-shunt pointer value
 * should not change.
 * @param node: Pointer to the siding from which to delete that is not 
 * head-shunt
 */
void clr_sidings(siding ** node);

/**
 * Clear the current line pointer, and create a new line with values -1
 * @param node: Pointer to the siding to reset line
 */
void init_line(siding * node);

/**
 * Print the line from a siding displaying train numbers
 * @param line: Pointer to the line to print
 * @param length: Length of the line
 */
void print_line(int * line, int length);

/**
 * Count free spaces left on the line
 * @param node: Pointer to the siding which contains the line to count
 * @return The number of free spaces left on the line
 */
int line_space(siding * node);

/**
 * Count the number of trains on the line
 * @param node: Pointer to the siding which contains the trains to be counted
 * @return The number of trains present on the line
 */
int count_train(siding * node);

#endif /* SIDINGS_H */