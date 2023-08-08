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

std::vector<GLfloat> Boid::getVertices() {

	std::vector <GLfloat> vertices =
	{ -SIZE + pos.x, -SIZE * float(sqrt(3)) / 3 + pos.y, 0.0f, // Lower left corner
	SIZE + pos.x, -SIZE * float(sqrt(3)) / 3 + pos.y, 0.0f, // Lower right corner
	0.0f + pos.x, SIZE * float(sqrt(3)) * 2 / 3 + pos.y, 0.0f
	};

	return vertices;
}

void Boid::update(double xpos, double ypos) {
	ai->update(*this, xpos,  ypos);
}