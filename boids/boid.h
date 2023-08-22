#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "AI.h"
#include <cmath>
#include <tgmath.h> 
#include <iostream>

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

	GLfloat SPEED = 1;
	GLfloat SIZE = 0.003f;

	GLfloat vx = 0;
	GLfloat vy = 0;

	GLfloat boundBoxPx = 40;
	int spatialIndex = -1;

	vector_2 getPos();
	void setPos(vector_2 _pos);
	std::vector<GLfloat> rotateVec(std::vector <GLfloat> vert);
	std::vector<GLfloat> makeAbsoluteVec(std::vector <GLfloat> vert, float x, float y);
	std::vector<GLfloat> getVertices(int width, int height);
	std::vector<GLfloat> getBoundVertices(int width, int height);

	void setAI(char ai, std::vector<Boid*> boids);

	void update(std::vector<Boid*> allBoids, double xpos, double ypos);
	vector_2 pos;

	int label = 0;

protected:
	
	vector_2 direction = {0, 0, 0};


	AI* ai;
	
};

#endif

