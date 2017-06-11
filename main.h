#ifndef __MAIN_H__
#define __MAIN_H__

/* main.h
 * 
 * Initializes OpenGL, and creates a window for drawing pixels.
 * Contains a function to draw a single pixel of a specified color, and simple line-drawing function.
 * 
 */

#include "misc.h"

struct RGB;

// Draws a pixel at the given coordinates.
void makePix(int x, int y, RGB color);
// Gets the RGB value from the pixel buffer at the given coordinates.
RGB getPix(int x, int y);

// Fills the entire drawing area with the specified color.
void fill(RGB color);
// Returns true if the line specified by the given coordinates is one of the 4 simple cases.
bool isLineSimple(int x1, int y1, int x2, int y2);
// Draws the line specified by the given coordinates, but only if that line is one of the 4 simple cases.
void drawLineSimple(int x1, int y1, int x2, int y2, RGB color);
void drawLineSimple(int x1, int y1, int x2, int y2, RGB color, void (*draw)(int, int, RGB));
// Draws a line using the DDA method.
void drawLineDDA(int x1, int y1, int x2, int y2, RGB color);
void drawLineDDA(int x1, int y1, int x2, int y2, RGB color, void (*draw)(int, int, RGB));
// Draws a line using the Bresenham method.
void drawLineBresenham(int x1, int y1, int x2, int y2, RGB color);
void drawLineBresenham(int x1, int y1, int x2, int y2, RGB color, void (*draw)(int, int, RGB));

#endif