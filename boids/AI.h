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
	virtual void update(Boid &boid, std::vector<Boid*> allBoids, double xpos, double ypos) = 0;
};

class CursorFollowingAI : public AI {
public:
    void update(Boid& boid, std::vector<Boid*> allBoids, double xpos, double ypos) override;
};

class BoidFollowingAI : public AI {
public:
    BoidFollowingAI();

    void update(Boid& boid, std::vector<Boid*> allBoids, double xpos, double ypos) override;

private:

    double visual_range = 40;
    double turnfactor = 5;
    double protectedRange = 8;
    double centeringfactor = 0.009;
    double avoidfactor = 0.07;
    double matchingfactor = 0.01;
    double maxspeed = 3;
    double minspeed = 0.3;

    std::random_device rd;  // Obtain a random seed from the operating system
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist;
};
#endif