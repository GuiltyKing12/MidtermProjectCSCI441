#pragma once

#include "Point.h"
#include "Creature.h"

#include <vector>
#include <math.h>

class Finjuh {
public:
	float dirX, dirY, dirZ;
	float posX, posY, posZ;
	float orientation; // allows for recalculation of the direction in which the hero is facing

	float appAngle; // keeps track of the angle that the hero's appendages change by
	int appMovmtSpd; // sets how much the hero's appendages change when moving
	bool showCtrlCage, showCurve;

	Creature pet;

public:
	Finjuh() {}

	Finjuh(float heroDirX, float heroDirY, float heroDirZ,
            float heroBodyX, float heroBodyY, float heroBodyZ,
            std::vector<Point> path) {
		this->dirX = heroDirX;
		this->dirY = heroDirY;
		this->dirZ = heroDirZ;
		
		this->posX = heroBodyX;
		this->posY = heroBodyY;
		this->posZ = heroBodyZ;
		
		this->orientation = 0;
		this->appAngle = 0;
		this->appMovmtSpd = 2;

        showCtrlCage = true;
        showCurve = true;
		pet = Creature(path);
	}

public:
	void drawFinjuh();
	void drawUpperBody();
	void drawLowerBody();
	void drawHead();
	void drawArm();
	void drawLeg();

	void recomputeDirection();
	void moveForward();
	void moveBackward();
};
