#include "Finjuh.h"

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

// Functions used when drawing hero
void Finjuh::drawHead() {
	glColor3f(.83, .42, .13);
	// draws head and beard
    glPushMatrix(); {
		glTranslatef(0, 12, 0);
		glPushMatrix(); {
			glScalef(.86, 1.15, .86);
			glutSolidSphere(3, 50, 50);
			glColor3f(0, 0, 0);
			glTranslatef(0, -2, -1);
			glutSolidSphere(2.5, 50, 50);
		} glPopMatrix();
		
		// draws nose
		glPushMatrix(); {
			glTranslatef(0, .25, -1.5);
			glRotatef(-180, 1, 0, 0);
			glutSolidCone(1.15, 2, 23, 23);
		} glPopMatrix();
		
		// draws eyes
		glColor3f(1, 1, 1);
		glPushMatrix(); {
			glTranslatef(1, 1, -2.25);
			glutSolidCone(.75, 2, 23, 23);
		} glPopMatrix();
		
		glPushMatrix(); {
			glTranslatef(-1, 1, -2.25);
			glutSolidCone(.75, 2, 23, 23);
		} glPopMatrix();
		
		// draws horns
		glColor3f(0.2, 0.2, 0.2);
		glPushMatrix(); {
			glTranslatef(1.5, 1.5, 0);
			glRotatef(-150, 1, 0, 0);
			glutSolidCone(1, 5, 23, 23);
		} glPopMatrix();
		
		glPushMatrix(); {
			glTranslatef(-1.5, 1.5, 0);
			glRotatef(-150, 1, 0, 0);
			glutSolidCone(1, 5, 23, 23);
		} glPopMatrix();
    } glPopMatrix();
}

void Finjuh::drawArm() {
	glColor3f(.83, .42, .13);
	glutSolidSphere(2, 50, 50);
    glPushMatrix(); {
		glTranslatef(0, -4, 0);
        glScalef(2.3, 7, 2.3);
        glutSolidCube(1);
    } glPopMatrix();
}

void Finjuh::drawLeg() {
	// draws leg
    glPushMatrix(); {
        glTranslatef(0, -5, 0);
        glScalef(1, 4.5, 1);
        glutSolidCube(3);
    } glPopMatrix();

	// draws hoof
	glColor3f(0, 0, 0);
	glPushMatrix(); {
		glTranslatef(0, -10, 0);
		glutSolidCube(3.2);
	} glPopMatrix();
	glColor3f(.545, .275, .075);
}

void Finjuh::drawUpperBody() {
	glColor3f(.83, .42, .13);
	
	// draws the main torso area
    glPushMatrix(); {
		glTranslatef(0, 3, 0);
		glScalef(1.25, 1.75, 1);
		glutSolidSphere(4, 50, 50);
	} glPopMatrix();
		
	// draws the shoulders
	glPushMatrix(); {
		glPushMatrix(); {
			glTranslatef(0, 7, 0);
			glScalef(1.45, .75, .85);
			glutSolidSphere(4, 50, 50);
		} glPopMatrix();
		
		// makes calls to draw arms
		glPushMatrix(); {
			glTranslatef(6, 7, 0);
			glRotatef(appAngle, 1, 0, 0);
			drawArm();
		} glPopMatrix();
		
		glPushMatrix(); {
			glTranslatef(-6, 7, 0);
			glRotatef(-appAngle, 1, 0, 0);
			drawArm();
		} glPopMatrix();
	} glPopMatrix();
}

void Finjuh::drawLowerBody() {
    glPushMatrix(); {
        glColor3f(.545, .275, .075);
		
		// draws the groin area
		glPushMatrix(); {
			glScalef(1.5, 1, 1);
			glutSolidSphere(4, 50, 50);
		} glPopMatrix();
        
        // makes calls to draw the legs
        glPushMatrix(); {
            glRotatef(appAngle, 1, 0, 0);
            glTranslatef(-3, 0, 0);
            drawLeg();
		} glPopMatrix();
		
		glPushMatrix(); {
			glRotatef(-appAngle, 1, 0, 0);
            glTranslatef(3, 0, 0);
            drawLeg();
        } glPopMatrix();
    } glPopMatrix();
}

void Finjuh::drawFinjuh() {
    glPushMatrix(); {
        glTranslatef(posX, 12, posZ);
        glRotatef(orientation, 0, 1 ,0);
        drawLowerBody();
        drawUpperBody();
		drawHead();
		glPushMatrix(); {
			glColor3f(.9, .9, .95);
            glTranslatef(8, 10, 0);
            pet.drawCreature();
            pet.drawPath(showCtrlCage, showCurve);
        } glPopMatrix();
    } glPopMatrix();
    
    glColor3f(1, 1, 1);
}

// changing the hero heading, similar to changing camera angle, but only in 2D because the hero doesn't need to change Y height
void Finjuh::recomputeDirection() {
    dirX = sin(orientation * M_PI / 180);
    dirZ = cos(orientation * M_PI / 180);
    
    float squares = pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2);
	float mag = pow(squares, 0.5);
    
    dirX = .5 * dirX / mag;
    dirZ = .5 * dirZ / mag;
}

void Finjuh::moveForward() {
    if((posX -= dirX * .25) > 50)
        posX = 50;
    else if((posX -= dirX * .25) < -50)
        posX = -50;
    
    if((posZ -= dirZ * .25) > 50)
        posZ = 50;
    else if((posZ -= dirZ * .25) < -50)
        posZ = -50;
    
    appAngle += appMovmtSpd;
    if(appAngle == 60 || appAngle == -60) appMovmtSpd = -appMovmtSpd;
}

void Finjuh::moveBackward() {
    if((posX += dirX * .25) > 50)
        posX = 50;
    else if((posX += dirX * .25) < -50)
        posX = -50;
    
    if((posZ += dirZ * .25) > 50)
        posZ = 50;
    else if((posZ += dirZ * .25) < -50)
        posZ = -50;
    
    appAngle -= appMovmtSpd;
    if(appAngle == 60 || appAngle == -60) appMovmtSpd = -appMovmtSpd;
}