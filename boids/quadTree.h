#pragma once
#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include "boid.h"

class Rectangle {
public:
    float x;
    float y;
    float w;
    float h;

    Rectangle();
    Rectangle(float x, float y, float w, float h);

    bool contains(const Boid& boid) const;
    bool intersects(const Rectangle& other) const;
};

class QuadTree {
private:
    Rectangle boundary;
    int capacity;
    
    bool divided = false;

    QuadTree* northWest = nullptr;
    QuadTree* northEast = nullptr;
    QuadTree* southWest = nullptr;
    QuadTree* southEast = nullptr;

    int depth;

public:
    std::vector<Boid*> boids;
    QuadTree(const Rectangle& boundary, int capacity, int depth);
    ~QuadTree();

    bool insert(Boid* boid);
    void subdivide();

    std::vector<Boid*> query(const Rectangle& range);
    std::vector<GLfloat> getLines(int width, int height);

    std::vector<Boid*> found;

    std::vector<Boid*> found_ne;
    std::vector<Boid*> found_nw;
    std::vector<Boid*> found_se;
    std::vector<Boid*> found_sw;

};

#endif // QUADTREE_H


