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

void boidManager::addBoid(vector_2 v) {
	Boid *boid = new Boid(v);
	this->boids.push_back(boid);
}

std::vector<GLfloat> boidManager::getAllVert() {
	std::vector<GLfloat> ver = {};
	for (Boid* i : this->boids)
	{
		std::vector<GLfloat> boid_ver = i->getVertices();
		ver.insert(ver.end(), boid_ver.begin(), boid_ver.end());
	}
	return ver;
}

void boidManager::updateBoids(double xpos, double ypos) {
	for (Boid* boid : this->boids) {
		boid->update(xpos, ypos);
	}
}