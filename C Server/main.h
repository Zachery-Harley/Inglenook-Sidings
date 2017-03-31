/* 
 * File:   main.h
 * Author: zachery
 *
 * Created on 29 November 2016, 12:05
 */
#ifndef MAIN_H
#define MAIN_H

#define SERVER_PORT 10031
#define BUFFER_SIZE 1024
#define NO_SPACE -1
#define NO_TRAIN -2
#define NO_SIDING -3
#define BAD_COMMAND -4
#define SUCCESS 1

#define abort(param) { fprintf(stderr, "\nSignal Box Program: %s" , param);\
                        exit(1) ; }

#define msg(param) { fprintf(stderr, "\nSignal Box Program: %s" , param); }
#define err(param) { fprintf(stderr, "\n[ERROR]Signal Box Program: %s" , param); }

#endif /* MAIN_H */

