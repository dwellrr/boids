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

    std::unique_ptr<QuadTree> northWest;
    std::unique_ptr<QuadTree> northEast;
    std::unique_ptr<QuadTree> southWest;
    std::unique_ptr<QuadTree> southEast;

    int depth;

public:
    std::vector<Boid*> boids;
    QuadTree();
    QuadTree(const Rectangle& boundary, int capacity, int depth);

    bool insert(Boid* boid);
    void subdivide();

    void query(const Rectangle& range, std::vector<Boid*>& result);
    std::vector<GLfloat> getLines(int width, int height);



};

#endif // QUADTREE_H


