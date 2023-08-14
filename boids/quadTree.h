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
    std::vector<Boid> boids;
    bool divided = false;

    QuadTree* northWest = nullptr;
    QuadTree* northEast = nullptr;
    QuadTree* southWest = nullptr;
    QuadTree* southEast = nullptr;

public:
    QuadTree(const Rectangle& boundary, int capacity);
    ~QuadTree();

    bool insert(const Boid& boid);
    void subdivide();

    std::vector<Boid> query(const Rectangle& range, std::vector<Boid>& found);
    std::vector<GLfloat> getLines(int width, int height);
};

#endif // QUADTREE_H


