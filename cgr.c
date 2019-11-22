/* Harris Ferguson
 * 11198169
 * haf094
 * CMPT 214 Assignment 3
 */

/*
 * C Library Headers
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Local Header Files
 */

#include "cgr.h"
#include "cgr_aux.h"
#include "cgr_plot.h"

/*
 * Global Variables
 */

int Scale;

// Main
// In:
//  int argc, char *argv[]
//  argv[1] will be the Scale to be used by Plot
// Out:
//  A Chaos Game Representation of a nucleotide sequence is produced on stdout
//
// Assumes / Asserts:
//  1 Argument provided on the command line, which must be a power of two,
//  and between 64 and 16 inclusive
//
//  After initializing a blank plot, initializing a starting midpoint
//  structure at (1/2,1/2), and allocating memory for Plot based on the Scale
//  argument given on the command line, the main loop will:
//      -Read in a character from stdin and assign it to current_tide
//      -pass current_tide and current_point (which starts at 1/2,1/2)
//       to the cgr function
//      -Assign the result of cgr to current_point
//  The loop stops on EOF, the Plot global variable is printed, 
//  and the allocated memory is freed 
int main ( int argc, char *argv[] )
{   
    // Check that:
    // 1 Argument was provided
    // That argument is between MIN_SCALE and MAX_SCALE
    // And the argument is a power of 2
    if( (argc) > ARGC_COUNT || (argc) < ARGC_COUNT )
        {
        fprintf( stderr, "Error: Invalid number of arguments\n");
        exit( EXIT_FAILURE );
    	}
    // Convert argv[1] vector to int and store it in scale
    Scale = strtol( argv[1], NULL, 10);
    
    if(  Scale < MIN_SCALE || Scale > MAX_SCALE )
        {
        fprintf( stderr, "Error: Scale Argument is out of bounds\n");
        exit( EXIT_FAILURE );
    	}
    if( is_power_of_2( Scale ) == false )
        {
        fprintf( stderr, "Error: Scale argument is not a power of two!\n");
        exit( EXIT_FAILURE );
   	}

    // Declare currentTide to use for getChar()
    char currentTide;

    // Allocate memory for Plot
    // need room for Scale * Scale graph points (scaled by sizeof char)
    // First, each pointer to vector is allocated
    Plot = (char**)malloc( Scale * sizeof(char*) );
    for( int i = 0 ; i < Scale ; i++ )
    {
        // Then, each pointer to character is allocated
        Plot[i] = (char*)malloc( Scale * sizeof(char) ); 
    }
    // Check that Plot is not NULL
    if ( Plot == NULL )
    {
        fprintf( stderr, "Error: Memory Allocation Failed!\n");
        exit( EXIT_FAILURE );
    }
    // initialize plot with space characters before entering the loop
    initialize_plot();
    // First point structure which is the centre point of the graph, at 1/2,1/2
    struct point_t current_point = { { 1, 2 }, { 1, 2 } }; 

    while ( ( currentTide = getchar() ) != EOF )
    {
        current_point = cgr( current_point, currentTide );
    }
    if ( valid_char_reached != 0 )
    {
        fprintf( stderr, "Warning: Non-nucleotide letter in input\n");
    }
    // Print the resulting plot from the above loop
    print_plot();

    // De-allocate Plot and Exit
    free( Plot );
    return EXIT_SUCCESS;
}
