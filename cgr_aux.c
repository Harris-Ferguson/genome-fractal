/*
 * Harris Ferguson
 * 11198169
 * haf094
 * cgr_aux.c source code file
 * CMPT 214 Assignment 4
 */

/*
 * C Library Functions
 */

#include <ctype.h> // for Toupper

/*
 * Local Headers
 */

#include "cgr_aux.h"
#include "cgr_plot.h"

/*
 * Global Variables
 */

static struct ntide_t tide_A = { 'A', { { 0, 0 }, { 0, 0 } } };
static struct ntide_t tide_C = { 'C', { { 0, 0 }, { 1, 1 } } };
static struct ntide_t tide_G = { 'G', { { 1, 1 }, { 1, 1 } } };
static struct ntide_t tide_T = { 'T', { { 1, 1 }, { 0, 0 } } };

/*
 * Auxillary Functions for cgr.c
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

