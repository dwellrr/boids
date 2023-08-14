#pragma once
#include "boid.h"
#include <random>

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

    double visual_range = 40;
    double turnfactor = 5;
    double protectedRange = 10;
    double centeringfactor = 0.007;
    double avoidfactor = 0.05;
    double matchingfactor = 0.04;
    double maxspeed = 3;
    double minspeed = 0.3;

    std::random_device rd;  // Obtain a random seed from the operating system
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist;
};
#endif