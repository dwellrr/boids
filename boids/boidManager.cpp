#include "boidManager.h"

boidManager::boidManager() {
	this->boids = {};
	updated = false;
}

boidManager::boidManager(Boid *firstBoid) {
	this->boids = { firstBoid };
	updated = false;
}

boidManager::boidManager(std::vector<vector_2> _boids)
{
	for (vector_2 i : _boids) {
		addBoid(i);
	}
	updated = false;
}

void boidManager::addBoid(vector_2 v) {
	Boid *boid = new Boid(v);
	this->boids.push_back(boid);
}

GLfloat* boidManager::getAllVert() {
	std::vector<GLfloat> ver = {};
	for (Boid* i : this->boids)
	{
		std::vector<GLfloat> boid_ver = i->getVertices();
		ver.insert(ver.end(), boid_ver.begin(), boid_ver.end());
	}

	GLfloat* vertices = &ver[0];
	return vertices;
}

int* boidManager::getAllIndices() {

	std::vector<int> v_res;
	int totalBoids = boids.size();

	for (int i = 0; i < totalBoids * 3; i++) {
		v_res.push_back(i);
	}

	int* res = &v_res[0];
	return res;
}

int boidManager::getAllIndicesSize() {

	int totalBoids = boids.size();
	int size = totalBoids * 3; 
	return size;
}

int boidManager::getAllVertSize() {
	std::vector<GLfloat> ver = {};
	for (Boid* i : this->boids)
	{
		std::vector<GLfloat> boid_ver = i->getVertices();
		ver.insert(ver.end(), boid_ver.begin(), boid_ver.end());
	}

	int res = ver.size();
	return res;
}

void boidManager::updateBoids() {
	for (Boid* boid : this->boids) {
		boid->update();
	}
	updated = true;
}