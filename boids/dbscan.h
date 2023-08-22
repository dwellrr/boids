#pragma once
#include "boid.h"
#include "quadTree.h"
class dbscan
{
public:
	double calculateDistance(const Boid* p1, const Boid* p2);
	std::vector<std::vector<Boid*>> dbscanClusters(std::vector<Boid*> points, double eps, int minPts);
	std::vector<std::vector<Boid*>> dbscanClusters(std::vector<Boid*> points, QuadTree* quads, double eps, int minPts);
};

