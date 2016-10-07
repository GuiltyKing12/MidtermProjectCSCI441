#pragma once

#include "point.h"
#include "vector.h"
//#include "Creature.h"
#include "hero.h"

#include <vector>
#include <math.h>

class Finjuh : public Hero {
public:
	float orientation; // allows for recalculation of the direction in which the hero is facing

	float appAngle; // keeps track of the angle that the hero's appendages change by
	int appMovmtSpd; // sets how much the hero's appendages change when moving
	bool showCtrlCage, showCurve;

	//Creature pet;

public:
	Finjuh() {}

	Finjuh(Point startPos, Vector startDir) {
        direction = startDir;
		
        position = startPos;
		
		this->orientation = 0;
		this->appAngle = 0;
		this->appMovmtSpd = 2;

        showCtrlCage = true;
        showCurve = true;
		//pet = Creature(path);
	}

public:
	virtual void drawHero() override;
	void drawUpperBody();
	void drawLowerBody();
	void drawHead();
	void drawArm();
	void drawLeg();

	void recomputeDirection();
	void moveForward();
	void moveBackward();
};
