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
	QuadTree quad = QuadTree(screen, 2, 5);
	vector_2 v = { 1, 1, 0 };
	for (int i = 0; i < n; i++)
	{
		
		addBoid(v);
	}

	for (Boid* i : boids)
	{
		quad.insert(i);
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
	quad = QuadTree(screen, 5, 5);
	for (Boid* boid : this->boids) {
		quad.insert(boid);
	}
	for (Boid* boid : this->boids) {
		std::vector<Boid*> neighbors;
		neighbors = quad.query(Rectangle(boid->pos.x, boid->pos.y, boid->boundBoxPx, boid->boundBoxPx));
		boid->update(neighbors, xpos, ypos);
		quad.found.clear();
		quad.found_nw.clear();
		quad.found_ne.clear();
		quad.found_sw.clear();
		quad.found_se.clear();

	}
	
}

void boidManager::setAI(char ai, std::vector<Boid*> boids) {
	for (Boid* boid : this->boids) {
		boid->setAI(ai, boids);
	}
}