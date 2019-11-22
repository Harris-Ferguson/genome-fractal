/*
 * Harris Ferguson
 * 11198169
 * haf094
 * cgr_plot.h header file
 */

#ifndef PLOT_H
#define PLOT_H

/*
 * Function Prototypes
 */

/*
 * Function to plot a given point_t structure to the array Plot
 */
void plot_point( point_t point );

/*
 * Function to output the Plot array to a pbm image
 */
void output_plot( void );

/*
 * Given a coord_t structure, coordinate fraction will be scaled up by 
 * the Scale global variable
 */
void scale_coord( coord_t coord );

#endif // PLOT_H
