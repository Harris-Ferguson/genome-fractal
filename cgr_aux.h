/*
 * Harris Ferguson
 * 11198169
 * haf094
 * cgr_aux.h header file
 * CMPT 214 Assignment 4
 */

#ifndef AUX_H
#define AUX_H

/*
 * C Library Headers
 */

#include <stdbool.h> // for _Bool
#include <stdint.h> // for uint64_t

/* 
 * Datatype Definitions
 */

// coord_t is a rational number made of 2 uint_64_t variables,
//numer/denom = some rational number
// This will be used as the coordinate points in the point_t structure
typedef struct coord_t {
    uint64_t numer; //Numerator of the rational number
    uint64_t denom; //Denominator of the rational number
} coord_t;
// point_t is made of 2 coord_t points, which are an x and a y coordinate
typedef struct point_t {
    struct coord_t x_coord; //x coordinate
    struct coord_t y_coord; //y coordinate
} point_t;
// ntide_t is a vertex point and a letter cooresponding to the 
// appropriate nucleotide letter for the vertex
// The coordinate points of the vertex can only be 0 or 1, since they
// are on the verticies of the plot
typedef struct ntide_t {
    char ntide;
    struct point_t vertex;
} ntide_t;

/*
 * Constants
 */

#define NUM_NTIDES 4 // Maximum Number of ntides (There are only 4 valid chars)
#define ARGC_COUNT 2 // Program will exit unless there is only 1 argument given
#define HIGH_ORDER_BIT sizeof(uint64_t)
/*
 * Static Variables
 */

// 4 ntide_t variables, corresponding to each Vertex of the graph
ntide_t tide_A = { 'A', { { 0, 0 }, { 0, 0 } } };
ntide_t tide_C = { 'C', { { 0, 0 }, { 1, 1 } } };
ntide_t tide_G = { 'G', { { 1, 1 }, { 1, 1 } } };
ntide_t tide_T = { 'T', { { 1, 1 }, { 0, 0 } } };

/*
 * Function Prototypes
 */

//Function to check if a given int is a power of two 
_Bool is_power_of_2( int val );

// Function to reduce the members of coord_t if the high order bit 
// of the denominator is set. Returns a coord_t structure
coord_t reduce_coord( coord_t old_coord );

// Function to reduce a point_t structure if the member coord_t structure 
// members have their high order bits set. Returns a point_t structure
point_t reduce_point( point_t old_point );

// Function to determine the midpoint between two given point_t structures
// returns a point_t structure of said midpoint
point_t determine_midpoint( point_t point1, point_t point2);

// Function which finds the midpoint between the correct vertex, reduces any 
// points that need reducing, and plots the midpoint point to the Plot global
// array. Returns a point_t structure to be used at the next starting point
point_t cgr( point_t in_point, char in_char );

#endif // AUX_H


