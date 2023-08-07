#include "boid.h"
#include <vector>

class boidManager {
public:
	boidManager();
	boidManager(Boid *firstBoid);
	boidManager(std::vector<vector_2> _boids);

	GLfloat* getAllVert();
	int getAllVertSize();

	int* getAllIndices();
	int getAllIndicesSize();

	void addBoid(vector_2 v);
	void updateBoids();

	bool isUpdated() const { return updated; };
	void setUpdated(bool value) { updated = value; };

private:
	std::vector<Boid*> boids;
	bool updated;

	
};