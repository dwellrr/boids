#pragma once
#include "boid.h"
class AI
{
public:
	virtual void update(Boid &boid) = 0;
};

class CursorFollowingAI : public AI {
public:
    void update(Boid& boid) override;
};

class BoidFollowingAI : public AI {
public:
    BoidFollowingAI(std::vector<Boid*>& allBoids) : allBoids(allBoids) {};

    void update(Boid& boid) override;

private:
    std::vector<Boid*>& allBoids;
};