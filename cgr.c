/* Harris Ferguson
 * 11198169
 * haf094
 * CMPT 214 Assignment 3
 */

/*
 * C Library Files
 */
#include <stdio.h> //for getchar, printf, fprintf
#include <stdlib.h> //for calloc, free, strtol
#include <limits.h> //for INT_SIZE
#include <ctype.h> //for Toupper
#include <stdbool.h> //for _Bool
#include <stdint.h> //for uint64_t

/*
 * Constants
 */ 
#define MIN_SCALE 16 //Minimum scale for the plot
#define MAX_SCALE 64 //Maximum scale for the plot
#define NUM_TIDES 4  //Number of possible nucleotide letters (NOT USED)
#define ARGC_COUNT 2 //will exit if there aren't ARGC_COUNT arguments given
#define HIGH_ORDER_BIT sizeof(uint64_t)

/*
 * Structures
 */ 

// coord_t is a rational number made of 2 uint_64_t variables,
//numer/denom = some rational number
// This will be used as the coordinate points in the point_t structure
struct coord_t {
    uint64_t numer; //Numerator of the rational number
    uint64_t denom; //Denominator of the rational number
};

// point_t is made of 2 coord_t points, which are an x and a y coordinate
struct point_t {
    struct coord_t x_coord; //x coordinate
    struct coord_t y_coord; //y coordinate
};

struct ntide_t {
    char ntide;
    struct point_t vertex;
};

/*
 * Global Variables
 */ 

int Scale; //Scale of the plot
char** Plot; // Plot, which will be of size Scale * Scale
// 4 ntide_t variables, corresponding to each Vertex of the graph
static struct ntide_t tide_A = { 'A', { { 0, 0 }, { 0, 0 } } };
static struct ntide_t tide_C = { 'C', { { 0, 0 }, { 1, 1 } } };
static struct ntide_t tide_G = { 'G', { { 1, 1 }, { 1, 1 } } };
static struct ntide_t tide_T = { 'T', { { 1, 1 }, { 0, 0 } } };
int valid_char_reached = 0;

/*
 * Auxiliary Functions
 */ 

// is_power_of_2
// in:
//  integer (val)
// out:
//  false (0) if val is not a power of 2
//  true (1) if val is a power of 2
_Bool is_power_of_2 ( int val )
{
    // I found this power of 2 checking operation at:
    // http://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    return (val && !(val & (val - 1) ) );
}

// is_valid_tide
// in:
//  character (tide)
// out:
//  TRUE if tide is one of: {A,C,G,T}
//  FALSE if tide is not one of the above
// assumes:
//  Nucleotide character tide is a Capital letter (if true)
// effects:
//  if this function returns false AND tide is not alphanumeric,  
//  global variable valid_char_reached is set to 1
_Bool is_valid_tide ( char tide )
{
    if ( tide == 'A' || tide == 'C' || tide == 'G' || tide == 'T' )
    {
        return true;
    }
    else
    {
        // If we get here, that means char tide is a letter
        // but not a valid nucleotide. A warning message will be produced
        // by main()
        if ( isalnum( tide ) )
        {
            valid_char_reached = 1;
        }
        return false;
    }
}

// reduce_coord
// in:
//  coord_t structure  (old_coord)
// out:
//  coord_t structure either:
//  1) Returns the structure if the numerator of the coordinate doesn't have
//  its high-order bit set or
//  2) if it does have its high order bit set, reduces (divides by 2) the numerator
//  and denominator of the coordinate and returns the structure
struct coord_t reduce_coord ( struct coord_t old_coord )
{
    // Check if the denominator has its high order bit set
    if ( old_coord.denom & ( 1 <<HIGH_ORDER_BIT ) ) 
    {
        // Right shift each member by 1
        old_coord.denom >>= 1;
        old_coord.numer >>= 1;
        return old_coord;
    }
    else
    {
        // Return nothing if high-order bit is not set
        return old_coord;
    }

}

// reduce_point
// In:
//  point_t structure (old_point)
// Out:
//  point_t structure with the x and y coord_t points reduces if their denominators
//  have their high-order bit set
//  The checking for this is all done in the function reduce_coord
//  Since reduce_coord will not make any changes if none need to be made,
//  This function simply passes the coordinates to reduce_coord and returns whatever
//  the result may be
struct point_t reduce_point ( struct point_t old_point )
{
    old_point.y_coord = reduce_coord(old_point.y_coord);
    old_point.x_coord = reduce_coord(old_point.x_coord);
    return old_point;
}

// print_plot
// In:
//  nothing
// Out:
//  nothing
// Effects:
//  Data from global array Plot is displayed on stdout
// Assumes:
//  Global array Plot exists, type char**
//  Global variable scale exists, type int
void print_plot ( void )
{
    // First, the top of the graph is printed out
    for ( int i = 0 ; i < Scale ; i++)
    {
        // on the first iteration, the vertex C gets labeled
        if ( i == 0 )
        {
            printf("C+");
        }
        // on the last iteration, the vertex G gets labeled
        if ( i == Scale - 1 )
        {
            printf("-+G\n");
        }
        // otherwise, the top boundary is printed out
        else
        {
            printf("-");
        }
    }
    // Second, the data itself is printed
    // Rows are printed in reverse, and the columns normally
    for ( int j = Scale - 1; j >= 0 ; j--) 
    {
        for ( int i = 0 ; i <= Scale - 1 ; i++ )
            {
            // if the loop is at the start of a column,
            // a left boundary is printed
            if ( i == 0 )
            {       
                printf(" |");
            }
            // i,jth element of the array is printed
            printf("%c", Plot[i][j]);
        }
        // after each column, print a right boundary marker and a newline
        printf("|\n");
        
    }
    // Last, the bottom boundary is printed
    for ( int i = 0 ; i < Scale ; i++ )
    {
        // on the first iteration, the vertex A gets labeled
        if ( i == 0)
        {
            printf("A+");
        }
        // on the last iteration, the vertex T gets labeled
        if ( i == Scale - 1 )
        {
            printf("-+T");
        }
        // otherwise, the bottom boundary is printed out 
        else
        {
            printf("-");
        
        }
    }
    printf("\n");
}

// scale_coord
// in:
//  coord_t structure coord
// out:
//  unsgined int
//  this int will be the coordinate argument scaled up by the 
//  Scale global variable
//  Returns the numerator of the coordinate if it equals the Scale
//  if not, each coordinate numer and denom is shifted left or right
//  if the denom is larger or smaller than the scale respectivly
// Assumes:
//  Presence of Scale global variable
//  coord denominator is a power of two
unsigned scale_coord ( struct coord_t coord )
{
    // Assign Scale to a uint64_t variable to compare to coord.denom which is
    // the same type
    uint64_t scale = Scale;
    if ( coord.denom == 0 )
    {
        fprintf( stderr, "Coordinate overflowed\n");
        print_plot();
        free(Plot);
        exit( EXIT_FAILURE );
    }
    while ( scale != coord.denom )
    {
        if ( scale > coord.denom )
        {
            coord.numer <<= 1;
            coord.denom <<= 1;
        }
        if ( scale < coord.denom )
        {
            coord.numer >>= 1;
            coord.denom >>= 1;
        }
    }
    return coord.numer;

}

// plot_point
// in:
//  point_t structure (point)
// out:
//  nothing
// effects:
//  point, which is given from cgr(), is plotted to the global array Plot
// Assumes:
//  Global array Plot exists
void plot_point ( struct point_t point )
{
    uint64_t x_coord = scale_coord( point.x_coord );
    uint64_t y_coord = scale_coord( point.y_coord );
    Plot[x_coord][y_coord] = '*';
}

// determine_midpoint
// in:
//  2 point_t structures, point1 and point2
//  point2 is only ever used in this program as a vector point of the graph
// out:
//  point_t structure whos coordinates are the midpoint of the argument coordinates
// assumes:
//  point2 is a vector point, meaning its coordinates will only include 1 or 0
//  this function also does not check the denominator of the coordinates of point2 since
//  they are initialized in such a way that the numerator and denominator of 
//  each point are the same. This function assumes that is the case, 
//  and will not work properly for point_t structures which 
//  have differing numerators and denominators (i.e. non whole number coordinates) 
struct point_t determine_midpoint ( struct point_t point1, struct point_t point2 )
{
    struct point_t midpoint;
    
    midpoint.x_coord.numer = (point1.x_coord.denom * point2.x_coord.numer) +
        point1.x_coord.numer;
    midpoint.x_coord.denom = point1.x_coord.denom * 2;

    midpoint.y_coord.numer = (point1.y_coord.denom * point2.y_coord.numer) +
        point1.y_coord.numer;
    midpoint.y_coord.denom = point1.y_coord.denom * 2;

        
    return midpoint;
}

// cgr
// in:
//  point_T structure (in_point)
//  character (in_char)
// out:
//  returns the value of in_point if in_char is not a valid nucleotide
//  otherwise returns a point structure which is the midpoint of the
//  correct ntide vertex and in_point
// other effects:
//  midpoint structure (next_point) is passed to plot_point(), which will put a '*' char
//  at the appropriate point in the global array Plot
struct point_t cgr ( struct point_t in_point, char in_char )
{
    // Convert in_char to uppercase
    in_char = toupper(in_char);
    // Initialize a point_t structure called next_point to pass to plot_point()
    struct point_t next_point;

    // If in_char is a valid uppercase nucleotide letter:
    // reduce in_point with reduce_point()
    // set next_point to be the mid point between in_point and the appropriate vertex
    // pass next_point to plot_point()
    // return next_point
    if ( (is_valid_tide( in_char ) ) == true )
    {
        if ( in_char == 'A')
        {
            in_point = reduce_point( in_point );
            next_point = determine_midpoint( in_point, tide_A.vertex );
            plot_point( next_point );
            return next_point;
        }
        if ( in_char == 'C')
		{
                    in_point = reduce_point( in_point );
                    next_point = determine_midpoint( in_point, tide_C.vertex );
                    plot_point( next_point );
                    return next_point;
                }
        if ( in_char == 'G')
                {
                    in_point = reduce_point( in_point );
                    next_point = determine_midpoint( in_point, tide_G.vertex );
                    plot_point( next_point );
                    return next_point;
                }
        if ( in_char == 'T')
                {
                    in_point = reduce_point( in_point );
                    next_point = determine_midpoint( in_point, tide_T.vertex );
                    plot_point( next_point );
                    return next_point;
                }
    }
    // If in_char is not a valid nucleotide letter, return in_point
    else
    {
        return in_point;
    }
    return in_point;
}

// initialize_plot:
//   Assign each element of global character array Plot[][] to a space
//   character, ' '.
// In:
//   No input arguments.
// Out:
//   All characters in Plot[][] initialized.
//   No return value.
// Assumes:
//   Presence of global variables Plot and Scale.
//   Plot is of type char** and of dimension Scale X Scale.
void initialize_plot( void )
{
  int i, j;

  for( i=0; i<Scale; i++ ) {
      for( j=0; j<Scale; j++ ) {
          Plot[i][j] = ' ';
      }
  }
}

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
