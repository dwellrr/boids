#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "AI.h"

#pragma once
#ifndef BOID_H
#define BOID_H

class AI;

struct vector_2 {
	float x;
	float y;
	float z;
};
class Boid {
public:
	Boid();
	Boid(float x, float y);
	Boid(vector_2 v);

	vector_2 getPos();
	void setPos(vector_2 _pos);
	std::vector<GLfloat> getVertices();

	void update();

protected:
	vector_2 pos;
	vector_2 direction = {0, 0, 0};
	float SPEED = 0.3f;
	float SIZE = 0.008f;

	AI* ai;
	
};

#endif

