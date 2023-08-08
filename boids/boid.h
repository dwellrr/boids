#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "AI.h"

#pragma once
#ifndef BOID_H
#define BOID_H

class AI;

struct vector_2 {
	double x;
	double y;
	double z;
};
class Boid {
public:
	Boid();
	Boid(float x, float y);
	Boid(vector_2 v);

	double SPEED = 1;
	float SIZE = 0.005f;

	double vx = 1;
	double vy = 1;

	vector_2 getPos();
	void setPos(vector_2 _pos);
	std::vector<GLfloat> getVertices(int width, int height);

	void setAI(char ai, std::vector<Boid*> boids);

	void update(double xpos, double ypos);

protected:
	vector_2 pos;
	vector_2 direction = {0, 0, 0};


	AI* ai;
	
};

#endif

