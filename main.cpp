#include "main.h"

#include <GL/glut.h>
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "commandHandler.h"
#include "implicitShape.h"
#include "phongLightSource.h"
#include "shape.h"
#include "shapeCollection.h"
#include "surfaceShape.h"
#include "misc.h"
#include "viewport.h"

int windowSize;
float* pixelBuffer;
Viewport* viewport;
ShapeCollection* shapeCollection;
CommandHandler* commandHandler = new CommandHandler();

void display();

int main(int argc, char *argv[])
{
	// Seed the random number generator.
	srand(time(NULL));
	
	// Default window size is (300, 300).
	windowSize = 300;
	
	// Get window size from the command line.
	if (argc > 1)
	{
		windowSize = atoi(argv[1]);
	}
	// Ensure that the window is at least (100, 100).
	if (windowSize < 80) windowSize = 100;
	
	// Allocate a new pixel buffer, and initialize to color black.
	pixelBuffer = new float[windowSize * windowSize * 3];
	fill(RGB(0.0, 0.0, 0.0));
	
	// Initialize the viewport and shape collection.
	shapeCollection = new ShapeCollection();
	int viewportSize = windowSize - 20;
	viewport = new Viewport(Coord(10, 10), viewportSize, shapeCollection);
	shapeCollection->setViewport(viewport);
	
	// Draw the initial viewport
	viewport->drawOutline();
	viewport->fillBackground();
	viewport->redraw(true);
	
	// Initialize GLUT.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	// Set window size.
	glutInitWindowSize(windowSize, windowSize);
	// Set window position.
	glutInitWindowPosition(100, 100);

	// Create and set main window title.
	glutCreateWindow("Project 5");
	glClearColor(0, 0, 0, 0); // Clears the buffer of OpenGL.
	// Sets display function.
	glutDisplayFunc(display);

	glutMainLoop();// Main display loop, will display until terminate.
	return 0;
}

// Main display loop. This function will be called again and again by OpenGL.
void display()
{
	// Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// Draws pixels on screen. Width and height must match pixel buffer dimension.
	glDrawPixels(windowSize, windowSize, GL_RGB, GL_FLOAT, pixelBuffer);

	// Window refresh.
	glFlush();
	
	// Get user input.
	commandHandler->getUserInput();
	
	// Execute the command.
	bool redraw = true;
	commandHandler->execute(shapeCollection, viewport, redraw);
	
	// Redraw the viewport.
	if (redraw)
	{
		viewport->redraw(true);
	}
	
	glutPostRedisplay();
}


void makePix(int x, int y, RGB color)
{
	int height = windowSize;
	int width = windowSize;
	if (x < 0 || x >= width) return;
	if (y < 0 || y >= height) return;
	
	pixelBuffer[x * 3 + y * 3 * width] = color.red;
	pixelBuffer[x * 3 + y * 3 * width + 1] = color.green;
	pixelBuffer[x * 3 + y * 3 * width + 2] = color.blue;
}

RGB getPix(int x, int y)
{
	int height = windowSize;
	int width = windowSize;
	if (x < 0 || x >= width) return RGB();
	if (y < 0 || y >= height) return RGB();
	
	return RGB(
		pixelBuffer[x * 3 + y * 3 * width],
		pixelBuffer[x * 3 + y * 3 * width + 1],
		pixelBuffer[x * 3 + y * 3 * width + 2]
	);
}

void fill(RGB color)
{
	for (int i = 0; i < windowSize; i++)
	{
		for (int j = 0; j < windowSize; j++)
		{
			makePix(i, j, color);
		}
	}
}

bool isLineSimple(int x1, int y1, int x2, int y2)
{
	return ((x1 == x2) || (y1 == y2) || (abs(x1 - x2) == abs(y1 - y2)));
}

void drawLineSimple(int x1, int y1, int x2, int y2, RGB color)
{
	drawLineSimple(x1, y1, x2, y2, color, makePix);
}

void drawLineSimple(int x1, int y1, int x2, int y2, RGB color, void (*draw)(int, int, RGB))
{
	if (x1 == x2) // vertical line
	{
		
		for (int j = y1; y1 < y2 ? j <= y2 : j >= y2; y1 < y2 ? j++ : j--)
		{
			draw(x1, j, color);
		}
		
	}
	else if (y1 == y2) // horizontal line
	{
		for (int i = x1; x1 < x2 ? i <= x2 : i >= x2; x1 < x2 ? i++ : i--)
		{
			draw(i, y1, color);
		}
	}
	else if (abs(x1 - x2) == abs(y1 - y2)) // diagonal line
	{
		int leftX, leftY, rightX, rightY;
		leftX = x1 < x2 ? x1 : x2;
		leftY = x1 < x2 ? y1 : y2;
		rightX = x1 < x2 ? x2 : x1;
		rightY = x1 < x2 ? y2 : y1;
		
			
		int i = leftX, j = leftY;
		while (i <= rightX)
		{
			draw(i, j, color);
			
			i++;
			leftY < rightY ? j++ : j--;
		}
	}
	// This function will only draw the 4 simple cases.
}

void drawLineDDA(int x1, int y1, int x2, int y2, RGB color)
{
	drawLineDDA(x1, y1, x2, y2, color, makePix);
}

void drawLineDDA(int x1, int y1, int x2, int y2, RGB color, void (*draw)(int, int, RGB))
{
	if (isLineSimple(x1, y1, x2, y2))
	{
		drawLineSimple(x1, y1, x2, y2, color, draw);
		return;
	}
	
	if (abs(x1 - x2) > abs(y1 - y2)) // The line is longer in the X direction than the Y direction (wide). March X.
	{	
		int leftX = x1 < x2 ? x1 : x2;
		int leftY = x1 < x2 ? y1 : y2;
		int rightX = x1 < x2 ? x2 : x1;
		int rightY = x1 < x2 ? y2 : y1;
		float slope = (float)(rightY - leftY) / (float)(rightX - leftX);
		
		for (int i = 0; i <= (rightX - leftX); i++)
		{
			draw(leftX + i, leftY + std::round((float)i * slope), color);
		}
	}
	else // The line is longer in the Y direction than the X direction (tall). March Y.
	{
		int bottomX = y1 < y2 ? x1 : x2;
		int bottomY = y1 < y2 ? y1 : y2;
		int topX = y1 < y2 ? x2 : x1;
		int topY = y1 < y2 ? y2 : y1;
		float invSlope = (float)(topX - bottomX) / (float)(topY - bottomY);
		
		for (int j = 0; j <= (topY - bottomY); j++)
		{
			draw(bottomX + std::round((float)j * invSlope) , bottomY + j, color);
		}
	}
}

void drawLineBresenham(int x1, int y1, int x2, int y2, RGB color)
{
	drawLineBresenham(x1, y1, x2, y2, color, makePix);
}

void drawLineBresenham(int x1, int y1, int x2, int y2, RGB color, void (*draw)(int, int, RGB))
{
	if (isLineSimple(x1, y1, x2, y2))
	{
		drawLineSimple(x1, y1, x2, y2, color, draw);
		return;
	}
	
	int leftX = x1 < x2 ? x1 : x2;
	int leftY = x1 < x2 ? y1 : y2;
	int rightX = x1 < x2 ? x2 : x1;
	int rightY = x1 < x2 ? y2 : y1;
	
	int bottomX = y1 < y2 ? x1 : x2;
	int bottomY = y1 < y2 ? y1 : y2;
	int topY = y1 < y2 ? y2 : y1;
	
	int dx = abs(x1 - x2);
	int dy = abs(y1 - y2);
	bool posSlope = (rightY - leftY) > 0;
	
	int predicate = 0;
	bool move = false;
	
	if (dx > dy) // The line is longer in the X direction than the Y direction (wide).
	{
		int j = 0;
		for (int i = 0; i <= (rightX - leftX); i++)
		{
			draw(leftX + i, leftY + j, color);
			if (i == 0)
			{
				predicate = (2 * dy) - dx;
			}
			else
			{
				predicate = predicate + (2 * dy) - (2 * dx * (move ? 1 : 0));
			}
			
			move = (predicate > 0);
			if (move)
			{
				posSlope ? j++ : j--;
			}
		}
	}
	else // / The line is longer in the Y direction than the X direction (tall).
	{
		int i = 0;
		for (int j = 0; j <= (topY - bottomY); j++)
		{
			draw(bottomX + i, bottomY + j, color);
			if (j == 0)
			{
				predicate = (2 * dx) - dy;
			}
			else
			{
				predicate = predicate + (2 * dx) - (2 * dy * (move ? 1 : 0));
			}
			
			move = (predicate > 0);
			if (move)
			{
				posSlope ? i++ : i--;
			}
		}
	}
}
