#include "boidManager.h"

boidManager::boidManager() {
	this->boids = {};
}

boidManager::boidManager(Boid *firstBoid) {
	this->boids = { firstBoid };
}

boidManager::boidManager(std::vector<vector_2> _boids)
{
	for (vector_2 i : _boids) {
		addBoid(i);
	}
}

boidManager::boidManager(int n) {

	Rectangle screen = Rectangle(975, 540, 975, 540);
	QuadTree quad = QuadTree(screen, 2);
	vector_2 v = { 100, 100, 0 };
	for (int i = 0; i < n; i++)
	{
		addBoid(v);
	}

	for (Boid* i : boids)
	{
		quad.insert(*i);
	}
}

void boidManager::addBoid(vector_2 v) {
	Boid *boid = new Boid(v);
	this->boids.push_back(boid);
}

std::vector<GLfloat> boidManager::getAllVert(int width, int height) {
	std::vector<GLfloat> ver = {};
	for (Boid* i : this->boids)
	{
		std::vector<GLfloat> boid_ver = i->getVertices(width, height);
		ver.insert(ver.end(), boid_ver.begin(), boid_ver.end());
	}
	return ver;
}

std::vector<GLfloat> boidManager::getBoundVertices(int width, int height) {
	std::vector<GLfloat> ver = {};
	for (Boid* i : this->boids)
	{
		std::vector<GLfloat> boid_ver = i->getBoundVertices(width, height);
		ver.insert(ver.end(), boid_ver.begin(), boid_ver.end());
	}
	return ver;
}

std::vector<GLfloat> boidManager::getQuadVert(int width, int height)
{
	return quad.getLines(width, height);
}

void boidManager::updateBoids(double xpos, double ypos) {
	quad = QuadTree(screen, 2);
	for (Boid* boid : this->boids) {
		boid->update(xpos, ypos);
		quad.insert(*boid);
	}
	
}

void boidManager::setAI(char ai, std::vector<Boid*> boids) {
	for (Boid* boid : this->boids) {
		boid->setAI(ai, boids);
	}
}