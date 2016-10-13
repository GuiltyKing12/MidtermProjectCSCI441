#include "object.h"

Object::Object() {
}

Object::Object(string t, Point p, float s) {
	this->type = t;
	this->loc = p;
	this->size = s;
}

void Object::draw(Point pos, Vector norm) {
	Vector up(0,1,0);
	Vector axis = norm.Cross(up);
	float angle = norm.Dot(up)*180/M_PI;

	if (type == "tree") {
	  glColor3f(0.5, 0.3, 0.1);
	  glPushMatrix(); {
        glTranslatef(pos.x, pos.y+5, pos.z);
		glRotatef(angle, axis.x, axis.y, axis.z);
		glScalef(size, 2*size, size);
		glutSolidCube(8);
		
		glColor3f(0.2, 1, 0.2);
		glPushMatrix(); {
		  glTranslatef(0, 4, 0);
		  glRotatef(-90, 1, 0, 0);
		  glutSolidCone(8, 10, 23, 23);
		} glPopMatrix();
      } glPopMatrix();
	}
	else if (type == "rock") {
	  glColor3f(.7, .7, .7);
      glPushMatrix(); {
        glTranslatef(pos.x, pos.y+5, pos.z);
		glRotatef(angle, axis.x, axis.y, axis.z);
		glScalef(1.2, 1, 1.5);
		glScalef(size, size, size);
        glutSolidSphere(5, 23, 23);
      } glPopMatrix();
	}
}