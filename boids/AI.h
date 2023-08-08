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
    double centeringfactor = 0.000000006;
    double avoidfactor = 0.000007;
    double matchingfactor = 0.0006;
    double maxspeed = 0.01;
    double minspeed = 0.005;
};
#endif