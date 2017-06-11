#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

/* viewport.h
 * 
 * Defines an area of the screen to draw on, and all components of a scene that can be drawn.
 * Holds information about a shape collection, viewing camera position, and lighting information.
 * 
 * 
 */

#include <vector>

#include "misc.h"

class ShapeCollection;
class SurfaceShape;
struct PhongLightSource;



class Viewport
{
	public:
		/*** Public Member Functions ***/
		Viewport(Coord _origin, int _size, ShapeCollection* _shapes);
		
		// Equivalent to makePix, but draws relative to this viewport.
		void pixelMake(int x, int y, RGB color);
		// Equivalent to getPix, but relative to this viewport.
		RGB pixelGet(int x, int y);
		// Returns true if the pixel can be drawn in the viewport.
		bool pixelIn(int x, int y);
		
		/** File I/O **/
		void readSceneAttributes(std::istream& s);
		void writeSceneAttributes(std::ostream& s);
		
		// Returns the size of this viewport.
		int getSize();
		
		// Camera Viewing Model getters/setters.
		void setFromPoint(FCoord3D ff);
		FCoord3D getFromPoint();
		void setAtPoint(FCoord3D a);
		FCoord3D getAtPoint();
		void setUpVector(FCoord3D u);
		FCoord3D getUpVector();
		void setViewingAngle(float alphaDeg);
		
		// Moves the camera/atPoint/fromPoint in relation to the current viewing direction.
		void moveCamera(Direction d, float f);
		void moveAtPoint(Direction d, float f);
		void moveFromPoint(Direction d, float f);
		
		// Add/delete a light source to/from the scene.
		void addLight(PhongLightSource* light);
		void deleteLight(int index);
		
		// Draws an outline of the viewport. The outline is drawn on the pixels outside the
		// drawing area. Shapes inside the viewport will never draw over the outline.
		void drawOutline();
		// Fills the viewport with the background color.
		void fillBackground();
		
		// Draws a small diamond at the specified position.
		void drawDiamond(Coord coord, RGB color);
		// Draws a line in the viewport.
		void drawLine(Coord p1, Coord p2, RGB color);
		
		// Re-renders all pixels of the viewport.
		void redraw(bool loadingText);
		// Performs ray tracing to calculate the color of the specified pixel.
		RGB calculatePixelColor(int i, int j);
		// Performs recursive ray tracing to calculate the color that a ray encounters.
		RGB calculatePhongColor(FCoord3D fromPoint, FCoord3D rayDir, int rLayer, std::vector<bool> inShape, float recursiveScaling);
		// Returns the ray direction of a pixel.
		FCoord3D getRayDir(int i, int j);
		// Returns the combined refractive index of a set of shapes (used if shapes overlap).
		float getRefractiveIndex(std::vector<bool> inShape);
		
	private:
		/*** Private Member Functions ***/
		static const RGB OUTLINE_COLOR_DEFAULT() {return RGB(0.5, 0.5, 0.5);}
		static const RGB BACKGROUND_COLOR_DEFAULT() {return RGB(0, 0, 0);}
		
		static const RGB CURVE_COLOR() {return RGB(1, 1, 1);}
		static const RGB CONTROL_COLOR() {return RGB(1, 0, 0);}
		
		/*** Private Member Variables ***/
		// Defines the origin of this viewport on the screen.
		Coord origin;
		// Defines the size of this viewport.
		int size;
		// Stores the shapes in this viewport.
		ShapeCollection* shapes;
		
		// The color that is drawn around the viewport.
		RGB outlineColor;
		// The color that is drawn when a ray does not hit any shape in the scene.
		RGB backgroundColor;
		
		/** CVM Parameters **/
		// The point where the camera resides.
		FCoord3D fromPoint;
		// The point where the camera is looking.
		FCoord3D atPoint;
		// The "up" direction.
		FCoord3D upVector;
		// How wide the camera view angle is.
		float viewingAngleDeg;
		
		/** Phong Paramaters **/
		// Holds all light sources in the scene.
		std::vector<PhongLightSource*>* lightSources;
		// Defines how bright ambient lighting is.
		float ambientIntensity;
		// Defines how many recursive calls to make (at max) to determine the color at a pixel.
		int rayTracingRecursionLayers;
		
};

#endif