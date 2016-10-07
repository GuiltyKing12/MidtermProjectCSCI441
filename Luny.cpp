#include "Luny.h"
#ifdef __APPLE__			// if compiling on Mac OS
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using namespace std;

// Computes a location along a Bezier Curve.
Point Luny::evaluateBezierCurve( Point p0, Point p1, Point p2, Point p3, float t ) {
    // using the equation we turng the 4 points into the eqaution using t as the value needed
    Point a = ((-1.0f * p0) + (3.0f * p1) + (-3.0f * p2) + p3) * pow(t, 3);
    Point b = ((3.0f * p0) + (-6.0f * p1) + (3.0f * p2)) * pow(t, 2);
    Point c = ((-3.0f * p0) + (3.0f * p1)) * t;
    
    Point curvePoint = a + b + c + p0;
    
    // return the point
    return curvePoint;
}

// Responsible for drawing a Bezier Curve as defined by four control points.
//  Breaks the curve into n segments as specified by the resolution.
void Luny::renderBezierCurve( Point p0, Point p1, Point p2, Point p3, int resolution ) {
    // convert resolution into the segments we want
    float segments = 1 / (float)resolution;
    
    // we want to draw a line strip and thus we need to get each point on the curve
    glBegin(GL_LINE_STRIP); {
        for(float i = 0; i < 1.; i += segments) {
            
            // call bezier curve equation to get specific point
            Point curvePoint = evaluateBezierCurve(p0, p1, p2, p3, i);
            glVertex3f(curvePoint.getX(), curvePoint.getY(), curvePoint.getZ());
        }
    } glEnd();
}

// function that draws the control cage and the bezier curve
void Luny::drawPath(bool showCtrlPts, bool showCurve) {
    glPushMatrix(); {
        
        // if the option is selected we draw the control cage
        if(showCtrlPts) {
            glColor3f(0, 1, 0);
            // we move to each point on the x,y,z and draw a sphere
            for(int i = 0; i < path.size(); i++) {
                glPushMatrix(); {
                    glTranslatef(path[i].getX(), path[i].getY(), path[i].getZ());
                    glutSolidSphere(1, 30, 30);
                } glPopMatrix();
            }
            
            // using the points as verticies we can draw lines connecting them
            glDisable( GL_LIGHTING );
            glColor3f(1, 1, 0);
            glLineWidth(3.0f);
            for(int i = 0; i < path.size() - 1; i++) {
                glPushMatrix(); {
                    glBegin(GL_LINES); {
                        glVertex3f(path[i].getX(), path[i].getY(), path[i].getZ());
                        glVertex3f(path[i + 1].getX(), path[i + 1].getY(), path[i + 1].getZ());
                    } glEnd();
                } glPopMatrix();
            }
        }
        
        // if the option is selected we draw teh bezier curve
        if(showCurve) {
            // for every four points we want to draw the curve
            glColor3f(0, 0, 1);
            for(int i = 0; i  < path.size() - 1; i+=3) {
                renderBezierCurve(path[i], path[i + 1], path[i + 2], path[i + 3], 40);
            }
        }
        glEnable( GL_LIGHTING );
    } glPopMatrix();
}

// the actual drawing function that draws Luny
void Luny::drawLuny() {
    glPushMatrix(); {
        // we draw Luny at the position defined by its position on the bezier curve
        glTranslatef(position.getX(), position.getY(), position.getZ());
        glutSolidSphere(2, 20, 20);
        
        // we seperate the drawing of both wings and rotate by flap angle
        glPushMatrix(); {
            glRotatef(wingAngle, 0, 1, 0);
            glScalef(.3, .8, 1.2);
            drawWing();
        } glPopMatrix();
        
        glPushMatrix(); {
            glRotatef(-wingAngle, 0, 1, 0);
            glScalef(.3, .8, 1.2);
            drawWing();
        } glPopMatrix();
        
    } glPopMatrix();
}

void Luny::drawWing() {
    // wing base
    glutSolidSphere(2, 20, 20);
    
    // constructs the wing where we draw in a curve fashion
    // starting from the base we rotate as we go out to draw each piece
    glPushMatrix(); {
        // draws the first part of the wing
        glRotatef(45, 1, 0, 0);
        glTranslatef(0, 3, 0);
        glScalef(1, 2, 1);
        glutSolidCube(2);
        
        // draws teh second part
        glRotatef(45, 1, 0, 0);
        glTranslatef(0, 1, 0);
        glutSolidCube(2);
        
        // draws the third part
        glRotatef(45, 1, 0, 0);
        glTranslatef(0, 1, 0);
        glScalef(1, 2, 1);
        glutSolidCube(2);
        
        // draws teh fourth part
        glRotatef(10, 1, 0, 0);
        glTranslatef(0, 2, 0);
        glScalef(.7, 3, .5);
        glutSolidCube(2);
    } glPopMatrix();
}

// function called that flap the wings in myTimer
void Luny::flap() {
    wingAngle += 10;
    if(wingAngle == 70 || wingAngle == 110) wingAngle = -wingAngle;
}

// function called that updates the current position of the familiar
void Luny::move() {
    // incrememnt by some resolution
    currentPoint += 1.0f / 40;
    
    // get position with
    position = evaluateBezierCurve(path[pathPos], path[pathPos + 1], path[pathPos + 2], path[pathPos + 3], currentPoint);
    
    // reset or move to next section of bezier curve
    if(currentPoint > 1) {
        pathPos += 3;
        currentPoint = 0;
    }
    
    if(pathPos == path.size() - 1) pathPos = 0;
}
