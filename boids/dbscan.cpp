#include "dbscan.h"

#include <iostream>
#include <vector>
#include <cmath>


double dbscan::calculateDistance(const Boid* p1, const Boid* p2) {
    double dx = p1->pos.x - p2->pos.x;
    double dy = p1->pos.y - p2->pos.y;
    return dx * dx + dy * dy;
}

std::vector<std::vector<Boid*>> dbscan::dbscanClusters(std::vector<Boid*> points, double eps, int minPts) {
    int n = points.size();
    int clusterIdx = 0;

    for (int i = 0; i < n; ++i) {
        if (points[i]->label != 0) // Skip if point is already classified
            continue;

        std::vector<Boid*> neighbors;
        for (int j = 0; j < n; ++j) {
            if (i != j && calculateDistance(points[i], points[j]) <= eps * eps) {
                neighbors.push_back(points[j]);
            }
        }

        if (neighbors.size() < minPts) {
            points[i]->label = -1; // Noise
        }
        else {
            ++clusterIdx;
            points[i]->label = clusterIdx;
            for (Boid* neighbor : neighbors) {
                if (neighbor->label != -1) {
                    neighbor->label = clusterIdx;
                }
            }
        }
    }

    std::vector<std::vector<Boid*>> clusters(clusterIdx + 1);
    for (int i = 0; i < n; ++i) {
        if (points[i]->label != -1) {
            clusters[points[i]->label].push_back(points[i]);
        }
        else {
            clusters[0].push_back(points[i]);
        }
    }

    return clusters;
}

