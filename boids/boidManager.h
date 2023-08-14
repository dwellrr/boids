#include "boid.h"
#include <vector>

class boidManager {
public:
	boidManager();
	boidManager(Boid *firstBoid);
	boidManager(std::vector<vector_2> _boids);
	boidManager(int n);

	std::vector<GLfloat> getAllVert(int width, int height);
	std::vector<GLfloat> getBoundVertices(int width, int height);
	void addBoid(vector_2 v);
	void updateBoids(double xpos, double ypos);
	void setAI(char ai, std::vector<Boid*> boids);
	std::vector<Boid*> boids;

private:
	

	
};