/*
 * Harris Ferguson
 * 11198169
 * haf094
 * cgr_plot.c Source code file
 * CMPT 214 Assignment 4
 */

/*
 * C Library
 */
#include <netpbm/pbm.h>

/*
 * Local Headers
 */

#include "cgr_aux.h"
#include "cgr_plot.h"
#include "cgr.h"

/*
 * Global Variables
 */

bit** Plot; 

/*
 * Plotting functions for CGR
 */

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
    // if a coord has overflowed, output what was plotted so far 
    // and exit with an error. We should never reach this point, 
    // and if we do it indicates a failure of reduce_point in
    // cgr_aux.c
    if ( coord.denom == 0 )
    {
        fprintf( stderr, "Error: Coordinate overflowed\n");
        output_plot();
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
    Plot[x_coord][y_coord] = PBM_BLACK;
}

void output_plot ( void )
{
	for ( int i = Scale - 1 ; i > 0 ; i--)
	{
		pbm_writepbmrow( stdout, Plot[i], Scale, false );
	}
}

