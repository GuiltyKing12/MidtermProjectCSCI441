#include "Artoria.h"
#include "hero.h"
#ifdef __APPLE__			// if compiling on Mac OS
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <fstream>			// we'll use ifstream
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void Artoria::drawName() {
    char* c;
    glPushMatrix(); {
        glColor3f(1, 1, 1);
        glScalef(.08, .05, .08);
        glRotatef(180, 0, 1, 0);
        for(c = name; *c != '\0'; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);
        }
    } glPopMatrix();
}

void Artoria::drawSpear() {
    glPushMatrix(); {
        glColor3f(.65, .8, .9);
        glScalef(1, 10, 1);
        glutSolidCone(1, 3, 10, 10);
    } glPopMatrix();
}

void Artoria::drawTail() {
    // tail base
    glutSolidSphere(2, 20, 20);
    
    // constructs the tail where we draw in a curve fashion
    // starting from the base we rotate as we go out to draw each piece
    glPushMatrix(); {
        // draws the first part of the tail
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

// head function
void Artoria::drawHead() {
    // we scale a sphere to look more like a head
    glColor3f(1.0, .95, .8);
    glPushMatrix(); {
        glScalef(.75, 1, .75);
        glutSolidSphere(3, 20, 20);
    } glPopMatrix();
    
    glColor3f(1, 1, .6);
    // uses tail function to create a ponytail
    glPushMatrix(); {
        glScalef(1, .5, 1);
        drawTail();
    } glPopMatrix();
    
    // draws a cube and rorates to emulate a helmet
    glColor3f(.7, .7, .7);
    glPushMatrix(); {
        glTranslatef(0, 2, 0);
        glRotatef(45, 0, 1, 0);
        glutSolidCube(4);
        
        // adds bottom cube to cover the neck area
        glTranslatef(0, -4, 0);
        glutSolidCube(3);
    } glPopMatrix();
    
    // draws the horn on helmet
    glColor3f(1, 1, 1);
    glPushMatrix(); {
        glTranslatef(0, 2, 0);
        glRotatef(-160, 1, 0, 0);
        glutSolidCone(1, 10, 10, 10);
    } glPopMatrix();
    
}

// Arm drawing functions
void Artoria::drawArm() {
    glColor3f(.8, .8, .8);
    // draws the shoulder
    glutSolidSphere(2, 10, 10);
    
    // draws the upper arm
    glPushMatrix(); {
        glTranslatef(0, -3, 0);
        glScalef(1, 3, 1);
        glutSolidCube(2);
    } glPopMatrix();
    
    // draws the elbox and lower arm
    glPushMatrix(); {
        // first draws elbow and sets angle
        glTranslatef(0, -6, 0);
        glRotatef(armAngle2, 1, 0, 1);
        glutSolidSphere(2, 10, 10);
        
        // draws lower arm
        glTranslatef(0, -3, 0);
        glScalef(1, 2, 1);
        glutSolidCube(2);
        
        // draws hands
        glColor3f(1.0, .95, .8);
        glTranslatef(0, -1.5, 0);
        glScalef(1, .5, 1);
        glutSolidSphere(1.5, 10, 10);
        
        if(toArms) drawSpear();
    } glPopMatrix();
}

// the leg drawing functions that draws the leg as two rectangles connected by a sphere
// the front leg is designed to be shorter than the rear leg
void Artoria::drawFrontLeg() {
    // draws the part connected to body
    glutSolidSphere(3, 50, 50);
    
    // draws the thigh
    glPushMatrix(); {
        glTranslatef(0, -3, 0);
        glScalef(1, 5, 1);
        glutSolidCube(2);
    } glPopMatrix();
    
    // draws the knee + calf
    glPushMatrix(); {
        glTranslatef(0, -9, 0);
        glRotatef(-legAngle, 1, 0, 0);
        glutSolidSphere(2, 20, 20);
        glTranslatef(0, -3, 0);
        glScalef(1, 3, 1);
        glutSolidCube(2);
    } glPopMatrix();
}

void Artoria::drawRearLeg() {
    // draws the thigh
    glutSolidSphere(4, 50, 50);
    glPushMatrix(); {
        glTranslatef(0, -5, 0);
        glScalef(1, 6, 1);
        glutSolidCube(2);
    } glPopMatrix();
    
    // draws the calf
    glPushMatrix(); {
        glTranslatef(0, -9, 0);
        glRotatef(-legAngle, 1, 0, 0);
        glutSolidSphere(2, 20, 20);
        glTranslatef(0, -4, 0);
        glScalef(1, 4, 1);
        glutSolidCube(2);
    } glPopMatrix();
}

/*** MAIN COMPONENTS OF HERO DRAW ***/
// draws the upper human part
void Artoria::drawTorso() {
    // draws armor first
    glColor3f(.8, .8, .8);
    
    // first draws the main body that represents the torso
    glPushMatrix(); {
        glTranslatef(0, 8, 0);
        glScalef(4, 5, 3);
        glutSolidSphere(1, 20, 20);
    } glPopMatrix();
    
    // adds some details to the armor
    glPushMatrix(); {
        glTranslatef(0, 10, -2);
        glPushMatrix(); {
            glScalef(3, 1.5, 2);
            glutSolidSphere(1, 20, 20);
        } glPopMatrix();
    } glPopMatrix();
    
    // draws the arms
    glPushMatrix(); {
        // we move to both sides of the torso to draw the arms
        glTranslatef(0, 12, 0);
        // arms set to be at an angle away from body
        glPushMatrix(); {
            glTranslatef(4.5, 0, 0);
            glRotatef(20+armAngle, 1, 0, 1);
            drawArm();
        } glPopMatrix();
        
        // second arm drawing
        glPushMatrix(); {
            glTranslatef(-4.5, 0, 0);
            glRotatef(20+armAngle, 1, 0, -1);
            drawArm();
        } glPopMatrix();
    } glPopMatrix();
    
    // draws the head
    glPushMatrix(); {
        glTranslatef(0, 16, 0);
        drawHead();
        glTranslatef(15, 5, 0);
        drawName();
    } glPopMatrix();
}

// draws the lower horse body
void Artoria::drawLowerBody() {
    glPushMatrix(); {
        // first construct what will be the main body
        // start with the front
        glColor3f(.45, .4, .35);
        glutSolidSphere(5, 50, 50);
        
        // the main connecting part
        glPushMatrix(); {
            glScalef(.9, .7, 1.2);
            glTranslatef(0, 2, 4);
            glRotatef(-10, 1, 0, 0);
            glutSolidCube(8);
        } glPopMatrix();
        
        // the rear end
        glPushMatrix(); {
            glTranslatef(0, 2, 10);
            glutSolidSphere(4.5, 45, 45);
        } glPopMatrix();
        
        // draw the front legs
        glPushMatrix(); {
            glRotatef(legAngle, 1, 0, 0);
            glTranslatef(-4, 0, 0);
            drawFrontLeg();
            
            glTranslatef(8, 0, 0);
            drawFrontLeg();
        } glPopMatrix();
        
        // draw the rear legs
        glPushMatrix(); {
            glTranslatef(-2, 2, 11);
            glRotatef(-legAngle, 1, 0 ,0);
            drawRearLeg();
            
            glTranslatef(4, 0, 0);
            drawRearLeg();
        } glPopMatrix();
        
        // draws the tail
        glColor3f(1, 1, .6);
        glPushMatrix(); {
            glTranslatef(0, 4, 13);
            glRotatef(tailAngle, 1, 0, 0);
            drawTail();
        } glPopMatrix();
    } glPopMatrix();
}

/*** MAIN DRAW HERO ***/
void Artoria::drawHero() {
    // start by moving slightly up in order to draw the lower body
    glPushMatrix(); {
        Vector up(0, 1, 0);
        Vector axis = up.Cross(surfaceNormal);
        float angle = up.Dot(surfaceNormal);
        glTranslatef(position.x, position.y, position.z);
        //glRotatef(-angle * 180.0 / 3.14159, axis.x, axis.y ,axis.z);
        glRotatef(-orientationAngle * 180 / 3.14159, 0, 1, 0);
        drawLowerBody();
        drawTorso();
        glPushMatrix(); {
            glTranslatef(0, 20, 15);
            luny.drawLuny();
            luny.drawPath(showCtrlPts, showCurve);
        } glPopMatrix();
    } glPopMatrix();
    
    glColor3f(1, 1, 1);
}

void Artoria::recomputeHeroDirection() {
    direction.x = sin(orientationAngle * M_PI / 180);
    direction.z = cos(orientationAngle * M_PI / 180);
    
    float magnitude = pow(direction.x * direction.x + direction.z * direction.z, .5);
    
    direction.x = .1 * direction.x / magnitude;
    direction.z = .1 * direction.z / magnitude;
}

void Artoria::trackHeroHeading(Vector v) {
    if(direction.Dot(v) != 0) orientationAngle = direction.Dot(v);
    direction = v;
}

void Artoria::moveLegs() {
    // increments/decrements leg angle to move them until certain limits
    legAngle += legDirection;
    if(legAngle == 60 || legAngle == -60) legDirection = -legDirection;
    
    // puts the arm angles at a certain point
    armAngle = 30;
    armAngle2 = 50;
}

void Artoria::moveHeroForward() {
    // if position will pass the bounds we set them to the limit
    if((position.x -= direction.x) > 50)
        position.x = 50;
    else if((position.x -= direction.x) < -50)
        position.x = -50;
    
    if((position.z -= direction.z) > 50)
        position.z = 50;
    else if((position.z -= direction.z) < -50)
        position.z = -50;
    
    // increments/decrements leg angle to move them until certain limits
    legAngle += legDirection;
    if(legAngle == 60 || legAngle == -60) legDirection = -legDirection;
    
    // puts the arm angles at a certain point
    armAngle = 30;
    armAngle2 = 50;
}

// does the same ting as above, but opposite
void Artoria::moveHeroBackward() {
    if((position.x += direction.x) > 50)
        position.x = 50;
    else if((position.x += direction.x) < -50)
        position.x = -50;
    
    if((position.z += direction.z) > 50)
        position.z = 50;
    else if((position.z += direction.z) < -50)
        position.z = -50;
    
    legAngle -= legDirection;
    if(legAngle == 60 || legAngle == -60) legDirection = -legDirection;
    
    armAngle = 30;
    armAngle2 = 50;
}

void Artoria::shakeTail() {
    tailAngle += tailDirection;
    if(tailAngle == 30 || tailAngle == -30) tailDirection = -tailDirection;
}

void Artoria::getPath(string filename) {
    ifstream file;
    file.open(filename.c_str());
    //if(!file) return false;
    
    // read the first line which tells how many points there will be
    int points;
    file >> points;
    fprintf(stdout, "Number of Points: %d\n", points);
    
    // using that points number we read that many times to get x, y, z coordinates
    for(int i = 0; i < points; i++) {
        // read line as string and get substring using , in order to convert to float
        string line;
        file >> line;
        line += ',';
        
        // set checks for specific things to keep track of reading in the variables
        int counter = 0;
        int pos = 0;
        float coordinates [3];
        
        // we extract the numbers and put them into the array
        for(int k = 0; k < line.length(); k++) {
            if(line.at(k) == ',') {
                coordinates[counter] = atof(line.substr(pos, k).c_str());
                pos = k + 1;
                counter++;
            }
        }
        
        // create a new point with the coordinates and put into vector of control vector
        Point point(coordinates[0], coordinates[1], coordinates[2]);
        lunyPath.push_back(point);
        fprintf(stdout, "Point( %f, %f, %f)\n", coordinates[0], coordinates[1], coordinates[2]);
    }
    
    // close file when done
    file.close();
}
