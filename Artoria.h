#pragma once
#include "Luny.h"
#include "point.h"
#include "vector.h"
#include "hero.h"

// is the main hero drawing class
class Artoria : public Hero {
public:
    // we need to keep track of hero pos, direction and angles of the joint
    // also need to keep track if we drawing certain things
    //Point position;
    //Vector direction;
    float orientationAngle;
    float legAngle;
    float armAngle, armAngle2;
    int legDirection, tailAngle, tailDirection;
    bool toArms, lunyPath, showCtrlPts, showCurve;
    
    // her pet we attach to her
    Luny luny;
    
// default and normal constructor
public:
    Artoria() {}
    
    ~Artoria() {}
    
    Artoria(Point startPos, Vector startDir) {
        direction = startDir;
        
        position = startPos;
		
		orientationAngle = 0;
		legAngle = 0;
		armAngle = 0;
		armAngle2 = 0;
		legDirection = 2;
		tailAngle = 0;
		tailDirection = 2;
		toArms = false;
        lunyPath = true;
        showCtrlPts = true;
        showCurve = true;
        
        // give Luny her path -> all the control points
        //luny = Luny(path);
    }

// all the drawing functions
public:
    void drawSpear();

    void drawTail();

    // head function
    void drawHead();
    
    // Arm drawing functions
    void drawArm();
    
    // the leg drawing functions that draws the leg as two rectangles connected by a sphere
    // the front leg is designed to be shorter than the rear leg
    void drawFrontLeg();
    
    void drawRearLeg();
    
    /*** MAIN COMPONENTS OF HERO DRAW ***/
    // draws the upper human part
    void drawTorso();
    
    // draws the lower horse body
    void drawLowerBody();
    
    /*** MAIN DRAW HERO ***/
    virtual void drawHero() override;
    
// do stuff functions for hero
public:
    void recomputeHeroDirection();
    
    void moveHeroForward();
    
    // does the same ting as above, but opposite
    void moveHeroBackward();
    
    void shakeTail();
};
