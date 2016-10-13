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
void Finjuh::drawName() {
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
        
        // draws name
        glPushMatrix(); {
            glTranslatef(10, 5, 0);
            drawName();
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

void Finjuh::drawHero() {
    glPushMatrix(); {
        Vector up(0, 1, 0);
        Vector axis = up.Cross(surfaceNormal);
        float angle = up.Dot(surfaceNormal);
        glTranslatef(position.x, position.y, position.z);
        glRotatef(-angle * 180.0 / 3.14159, axis.x, axis.y ,axis.z);
        glRotatef(orientation * 180 / 3.14159 + 180, 0, 1, 0);
        drawLowerBody();
        drawUpperBody();
		drawHead();
		glPushMatrix(); {
			glColor3f(.9, .9, .95);
            glTranslatef(8, 10, 0);
            //pet.drawCreature();
            //pet.drawPath(showCtrlCage, showCurve);
        } glPopMatrix();
    } glPopMatrix();
    
    glColor3f(1, 1, 1);
}

// changing the hero heading, similar to changing camera angle, but only in 2D because the hero doesn't need to change Y height
void Finjuh::recomputeDirection() {
    direction.x = sin(orientation * M_PI / 180);
    direction.z = cos(orientation * M_PI / 180);
    
    float squares = pow(direction.x, 2) + pow(direction.y, 2) + pow(direction.z, 2);
	float mag = pow(squares, 0.5);
    
    direction.x = .5 * direction.x / mag;
    direction.z = .5 * direction.z / mag;
}

void Finjuh::moveForward() {
    if((position.x -= direction.x * .25) > 50)
        position.x = 50;
    else if((position.x -= direction.x * .25) < -50)
        position.x = -50;
    
    if((position.z -= direction.z * .25) > 50)
        position.z = 50;
    else if((position.z -= direction.z * .25) < -50)
        position.z = -50;
    
    moveArms();
}

void Finjuh::moveBackward() {
    if((position.x += direction.x * .25) > 50)
        position.x = 50;
    else if((position.x += direction.x * .25) < -50)
        position.x = -50;
    
    if((position.z += direction.z * .25) > 50)
        position.z = 50;
    else if((position.z += direction.z * .25) < -50)
        position.z = -50;
    
    moveArms();
}

void Finjuh::moveArms() {
	appAngle += appMovmtSpd;
    if(appAngle == 60 || appAngle == -60) appMovmtSpd = -appMovmtSpd;
}
