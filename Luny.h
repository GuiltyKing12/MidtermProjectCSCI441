#include "Point.h"
#include <vector>
#include <math.h>

// the class that contains information on Luny my hero's familiar
class Luny {

// important variables
public:
    // the following contain information on moving the familiar
    Point position;
    std::vector<Point> path;
    float currentPoint;
    int pathPos;
    
    // controls the flapping of the wings
    float wingAngle;
    
    // default and normal constructor
    Luny() {}
    
    Luny(std::vector<Point> path) {
        this->path = path;
        wingAngle = 90;
        position = path[0];
        currentPoint = 0;
        pathPos = 0;
    }
    
// Calculation Functions
public:
    // Computes a location along a Bezier Curve.
    Point evaluateBezierCurve( Point p0, Point p1, Point p2, Point p3, float t );
    
    // Responsible for drawing a Bezier Curve as defined by four control points.
    //  Breaks the curve into n segments as specified by the resolution.
    void renderBezierCurve( Point p0, Point p1, Point p2, Point p3, int resolution );
    
// drawing functions
public:
    // function that draws the control cage and the bezier curve
    void drawPath(bool showCtrlPts, bool showCurve);
    
    // the actual drawing function that draws Luny
    void drawLuny();
    
    void drawWing();
    
// action functions
public:
    // function called that flap the wings in myTimer
    void flap();
    
    // function called that updates the current position of the familiar
    void move();
};
