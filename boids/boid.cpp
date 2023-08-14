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

std::vector<GLfloat> Boid::rotateVec(std::vector <GLfloat> vert) {
	float angle = atan2(this->vy, this->vx) + 3.14/2;
	std::vector<GLfloat> turned(9, 0);

	for (int i = 1; i < 10; i++)
	{
		if (i % 3 != 0)
		{
			if (i == 2 or i == 5 or i == 8)
			{
				turned[i - 1] = -vert[i - 2] * sin(angle) + vert[i - 1] * cos(angle);
			}
			else
			{
				turned[i - 1] = vert[i - 1] * cos(angle) + vert[i] * sin(angle);
			}
		}
		else {
			turned[i - 1] = vert[i - 1];
		}
	}

	return turned;
}

std::vector<GLfloat> Boid::makeAbsoluteVec(std::vector <GLfloat> vert, float x, float y) {

	for (int i = 1; i < 10; i++)
	{
		if (i % 3 != 0)
		{
			if (i == 2 or i == 5 or i == 8)
			{
				vert[i - 1] += y;
			}
			else
			{
				vert[i - 1] += x;
			}
		}
	}
	return vert;
}

std::vector<GLfloat> Boid::getVertices(int width, int height) {

	float angle = atan(this->vy / this->vx);
	//float rel_x, rel_y;
	//rel_x = this->pos.x * cos(angle) + this->pos.y * sin(angle);
	//rel_y = -this->pos.x * sin(angle) + this->pos.y * cos(angle);

	float x_norm = pos.x / (width / 2) - 1;
	float y_norm = -(pos.y / (height / 2) - 1);
	/*std::vector <GLfloat> vertices =
	{ - SIZE + x_norm, -SIZE * float(sqrt(3)) / 2 + y_norm, 0.0f, // Lower left corner
	SIZE + x_norm, -SIZE * float(sqrt(3)) / 2 + y_norm, 0.0f, // Lower right corner
	0.0f + x_norm, SIZE * float(sqrt(3)) * 2 / 2 + y_norm, 0.0f
	}; //establish*/

	std::vector <GLfloat> vertices =
	{ -SIZE, -SIZE * float(sqrt(3)) / 2, 0.0f, // Lower left corner, relative, no rotation
	SIZE, -SIZE * float(sqrt(3)) / 2, 0.0f, // Lower right corner, relative, no rotation
	0.0f, (SIZE * float(sqrt(3)) * 2 / 2), 0.0f
	};

	vertices = rotateVec(vertices);
	vertices = makeAbsoluteVec(vertices, x_norm, y_norm);
	// + ((SPEED - 0.005f) * 4)

	return vertices;
}

std::vector<GLfloat> Boid::getBoundVertices(int width, int height) {

	float x_norm = pos.x / (width / 2) - 1;
	float y_norm = -(pos.y / (height / 2) - 1);
	GLfloat boundBox_x = boundBoxPx / (width / 2);
	GLfloat boundBox_y = -(boundBoxPx / (height / 2));

	std::vector <GLfloat> vertices = {
		x_norm - boundBox_x, y_norm - boundBox_y, 0.0f,
		x_norm - boundBox_x, y_norm + boundBox_y, 0.0f,
		x_norm - boundBox_x, y_norm + boundBox_y, 0.0f,
		x_norm + boundBox_x, y_norm + boundBox_y, 0.0f,
		x_norm + boundBox_x, y_norm + boundBox_y, 0.0f,
		x_norm + boundBox_x, y_norm - boundBox_y, 0.0f,
		x_norm + boundBox_x, y_norm - boundBox_y, 0.0f,
		x_norm - boundBox_x, y_norm - boundBox_y, 0.0f
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