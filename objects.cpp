#include "objects.h"

Objects::Objects() {
  file = "objects.csv";
}

Objects::Objects(std::string fn) {
  file = fn;
}

void Objects::load_and_draw() {
  ifstream fin(file.c_str());
  string line;
  getline(fin, line);
  num_objects = atoi(line.c_str());

  string object_type;
  string object_location;
  int object_size;
  
  for (int i = 0; i < num_objects; i ++) {
	float coords [2];
	
    getline(fin, line);
	object_type = line.c_str();
	
	int count = 0;
    int pos = 0;
	getline(fin, line);
	line += ',';
	for(int j = 0; j < line.length(); j++) {
      if(line.at(j) == ',') {
        coords[count] = atof(line.substr(pos, j).c_str());
        pos = j + 1;
        count++;
      }
    }
	
	getline(fin, line);
	object_size = atoi(line.c_str());
	
	if (object_type == "tree") {
	  glColor3f(0.5, 0.3, 0.1);
	  glPushMatrix(); {
        glTranslatef(coords[0], coords[1], coords[2]);
		glScalef(object_size, object_size, object_size);
		// Insert orientation here
		//glRotatef();
		// Also, make sure correct size
		glutSolidCube(8);
		
		glColor3f(0.2, 1, 0.2);
		glPushMatrix(); {
		  glTranslatef(0, 4, 0);
		  glRotatef(-90, 1, 0, 0);
		  glutSolidCone(8, 10, 23, 23);
		} glPopMatrix();
      } glPopMatrix();
	}
	else if (object_type == "rock") {
	  glColor3f(.7, .7, .7);
      glPushMatrix(); {
        glTranslatef(coords[0], coords[1], coords[2]);
		glScalef(1.2, 1, 1.5);
		glScalef(object_size, object_size, object_size);
		// Insert orientation here
		//glRotatef();
		// Also, make sure correct size
        glutSolidSphere(5, 23, 23);
      } glPopMatrix();
	}
  }

  fin.close();
}