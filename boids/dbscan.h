#pragma once
#include "boid.h"
class dbscan
{
public:
	double calculateDistance(const Boid* p1, const Boid* p2);
	std::vector<std::vector<Boid*>> dbscanClusters(std::vector<Boid*> points, double eps, int minPts);
};

