#include "boid.h"
#include <vector>

class boidManager {
public:
	boidManager();
	boidManager(Boid firstBoid);
	boidManager(std::vector<vector_2> _boids);

	std::vector<GLfloat> getAllVert();
	void addBoid(vector_2 v);
private:
	std::vector<Boid> boids;

	
};