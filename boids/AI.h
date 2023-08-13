#pragma once
#include "boid.h"

#ifndef AI_H
#define AI_H

// Forward declaration of Boid class
class Boid;

class AI
{
public:
	virtual void update(Boid &boid, double xpos, double ypos) = 0;
};

class CursorFollowingAI : public AI {
public:
    void update(Boid& boid, double xpos, double ypos) override;
};

class BoidFollowingAI : public AI {
public:
    BoidFollowingAI(std::vector<Boid*> allBoids);

    void update(Boid& boid, double xpos, double ypos) override;

private:
    std::vector<Boid*> allBoids;

    double visual_range = 100;
    double turnfactor = 0.2;
    double protectedRange = 25;
    double centeringfactor = 0.00003;
    double avoidfactor = 0.0008;
    double matchingfactor = 0.006;
    double maxspeed = 1;
    double minspeed = 0.1;
};
#endif