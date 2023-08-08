#include "boid.h"

Boid::Boid() {
		this->pos.x = 0;
		this->pos.y = 0;
		this->ai = new CursorFollowingAI();
}
Boid::Boid(float x, float y) {
	this->pos.x = x;
	this->pos.y = y;
	this->ai = new CursorFollowingAI();
}

Boid::Boid(vector_2 v) {
	this->pos = v;
	this->ai = new CursorFollowingAI();
}

void Boid::setPos(vector_2 _pos) {
	this->pos.x = _pos.x;
	this->pos.y = _pos.y;
}

vector_2 Boid::getPos() {
	return this->pos;
}

std::vector<GLfloat> Boid::getVertices(int width, int height) {

	float x_norm = pos.x / (width / 2) - 1;
	float y_norm = -(pos.y / (height / 2) - 1);
	std::vector <GLfloat> vertices =
	{ -SIZE + x_norm, -SIZE * float(sqrt(3)) / 2 + y_norm, 0.0f, // Lower left corner
	SIZE + x_norm, -SIZE * float(sqrt(3)) / 2 + y_norm, 0.0f, // Lower right corner
	0.0f + x_norm, SIZE * float(sqrt(3)) * 2 / 2 + y_norm, 0.0f
	};

	return vertices;
}

void Boid::setAI(char ai, std::vector<Boid*> boids) {
	if (ai == 'b') {
		this->ai = new BoidFollowingAI(boids);
	}
	else {
		this->ai = new CursorFollowingAI();
	}
}

void Boid::update(double xpos, double ypos) {
	ai->update(*this, xpos,  ypos);
}