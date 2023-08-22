#include "boid.h"
#include <vector>
#include "quadTree.h"
#include "SpatialHashGrid.h"
#include "dbscan.hpp"

class boidManager {
public:
	boidManager();
	boidManager(Boid *firstBoid);
	boidManager(std::vector<vector_2> _boids);
	boidManager(int n);

	std::vector<GLfloat> getAllVert(int width, int height);
	std::vector<GLfloat> getBoundVertices(int width, int height);
	std::vector<GLfloat> getQuadVert(int width, int height);
	std::vector<GLfloat> getBoidColors();
	std::vector<GLfloat> getBoundColors();

	void addBoid(vector_2 v);
	void updateBoids(double xpos, double ypos, bool isQuads, bool isHash, bool isDBSCAN);
	void setAI(char ai, std::vector<Boid*> boids);
	std::vector<Boid*> boids;
	Rectangle screen = Rectangle(975, 540, 975, 540);
	QuadTree quad;
	SpatialHashGrid hash;
	//std::vector<point2> points;

private:
	

	
};