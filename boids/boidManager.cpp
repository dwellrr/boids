#include "boidManager.h"

boidManager::boidManager() {
	this->boids = {};
	hash = SpatialHashGrid{ 1920, 1080, 50 };
}

boidManager::boidManager(Boid *firstBoid) {
	this->boids = { firstBoid };
	hash = SpatialHashGrid{ 1920, 1080, 50 };
}

boidManager::boidManager(std::vector<vector_2> _boids)
{
	for (vector_2 i : _boids) {
		addBoid(i);
	}
	hash = SpatialHashGrid{ 1920, 1080, 50 };
}

boidManager::boidManager(int n) {

	Rectangle screen = Rectangle(975, 540, 975, 540);
	quad = QuadTree(screen, 2, 10);
	hash = SpatialHashGrid{ 1920, 1080, 20 };
	vector_2 v = { 1, 1, 0 };

	for (int i = 0; i < n; i++)
	{
		
		addBoid(v);
	}


}

void boidManager::addBoid(vector_2 v) {
	Boid *boid = new Boid(v);
	this->boids.push_back(boid);
	this->hash.addBoid(boid);
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

std::vector<GLfloat> boidManager::getBoidColors()
{
	GLfloat x, y, z;
	std::vector <GLfloat> colors;
	x = 0.3f;
	y = 0.3f;

	for (Boid* boid : this->boids) {
		z = boid->label % 10;
		z = z / 10;
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		boid->label = 0;
	}

	return colors;
}

std::vector<GLfloat> boidManager::getBoundColors()
{
	GLfloat x, y, z;
	std::vector <GLfloat> colors;
	x = 1.0f;
	y = 1.0f;
	z = 1.0f;

	for (Boid* boid : this->boids) {
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
		colors.push_back(x);
		colors.push_back(y);
		colors.push_back(z);
	}

	return colors;
}

void boidManager::updateBoids(double xpos, double ypos, bool isQuads, bool isHash, bool isDBSCAN) {
	if (isQuads) {
		quad = QuadTree(screen, 2, 4);
		for (Boid* boid : this->boids) {
			quad.insert(boid);
		}
		for (Boid* boid : this->boids) {
			std::vector<Boid*> neighbors;
			quad.query(Rectangle(boid->pos.x, boid->pos.y, boid->boundBoxPx, boid->boundBoxPx), neighbors);
			boid->update(neighbors, xpos, ypos);

		}
	}
	else if (isHash) {
		for (Boid* boid : this->boids) {
			auto neighbors = hash.radiusSearch(boid, 30);
			boid->update(neighbors, xpos, ypos);
			hash.updateBoid(boid);
		}
	}
	else if (isDBSCAN) {

		auto clusters = _dbscan.dbscanClusters(boids, 30, 5);
		for (auto cl : clusters) {
			
				for (Boid* boid : cl)
				{
					boid->update(cl, xpos, ypos);
				}
			

		}

	}
	else
	{
		for (Boid* boid : this->boids) {
			boid->update(boids, xpos, ypos);

		}
	}

	
}

void boidManager::setAI(char ai, std::vector<Boid*> boids) {
	for (Boid* boid : this->boids) {
		boid->setAI(ai, boids);
	}
}