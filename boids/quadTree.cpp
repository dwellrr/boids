#include "quadTree.h"

Rectangle::Rectangle()
{
	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;
}

Rectangle::Rectangle(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}


bool Rectangle::contains(const Boid& boid) const
{
return (boid.pos.x >= this->x - this->w &&
		boid.pos.x <= this->x + this->w &&
		boid.pos.y >= this->y - this->h &&
		boid.pos.y <= this->y + this->h);
}

bool Rectangle::intersects(const Rectangle& range) const
{
return !(range.x - range.w > this->x + this->w ||
		range.x + range.w < this->x - this->w ||
		range.y - range.h > this->y + this->h ||
		range.y + range.h < this->y - this->h);
}

QuadTree::QuadTree() {
	this->capacity = 0;
	this->depth = 0;
	this->boids = std::vector<Boid*>();
}

QuadTree::QuadTree(const Rectangle& boundary, int capacity, int depth)
{
	this->boundary = boundary;
	this->capacity = capacity;
	this->depth = depth;
	this->boids = std::vector<Boid*>();
}								 

bool QuadTree::insert(Boid *boid)
{
	if (!this->boundary.contains(*boid)) {
		return false;
	}
	if (depth <= 0) {
		this->boids.push_back(boid);
		return true; // Stop splitting at the specified depth limit
	}
	if ((this->boids.size() < this->capacity) && (!this->divided)) {
		this->boids.push_back(boid);
		return true;
	}
	
	if (!this->divided) {
		this->subdivide();
	}



	return(
		this->northEast->insert(boid) ||
		this->northWest->insert(boid) ||
		this->southWest->insert(boid) ||
		this->southEast->insert(boid)
		);
}

void QuadTree::subdivide()
{
	//if (depth <= 0) {
		//return; // Stop splitting at the specified depth limit
	//}

	float x = this->boundary.x;
	float y = this->boundary.y;
	float w = this->boundary.w / 2;
	float h = this->boundary.h / 2;

	Rectangle ne = Rectangle(x + w, y - h, w, h);
	this->northEast = std::make_unique<QuadTree>(ne, this->capacity, depth - 1);

	Rectangle nw = Rectangle(x - w, y - h, w, h);
	this->northWest = std::make_unique<QuadTree>(nw, this->capacity, depth - 1);

	Rectangle se = Rectangle(x + w, y + h, w, h);
	this->southEast = std::make_unique<QuadTree>(se, this->capacity, depth - 1);

	Rectangle sw = Rectangle(x - w, y + h, w, h);
	this->southWest = std::make_unique<QuadTree>(sw, this->capacity, depth - 1);
	for (Boid* i : this->boids)
	{
		this->northEast->insert(i);
		this->northWest->insert(i);
		this->southWest->insert(i);
		this->southEast->insert(i);
	}

	this->boids.erase(this->boids.begin(), this->boids.end());

	this->divided = true;
}

void QuadTree::query(const Rectangle& range, std::vector<Boid*>& result)
{
	
	if (!range.intersects(this->boundary)) {
		return;
	}

	if (this->divided) {
		northWest->query(range, result);
		northEast->query(range, result);
		southWest->query(range, result);
		southEast->query(range, result);
	}
	else {
		for (Boid* p : this->boids) {
			if (range.contains(*p)) {
				result.push_back(p);
			}
		}
	}
}

std::vector<GLfloat> QuadTree::getLines(int width, int height)
{
	float x_norm = boundary.x / (width / 2) - 1;
	float y_norm = -(boundary.y / (height / 2) - 1);
	GLfloat boundBox_x = boundary.w / (width / 2);
	GLfloat boundBox_y = -(boundary.h / (height / 2));

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


	if (this->divided) {
		std::vector<GLfloat> subs_ne = northEast->getLines(width, height);
		std::vector<GLfloat> subs_nw = northWest->getLines(width, height);
		std::vector<GLfloat> subs_se = southEast->getLines(width, height);
		std::vector<GLfloat> subs_sw = southWest->getLines(width, height);

		vertices.insert(vertices.end(), subs_ne.begin(), subs_ne.end());
		vertices.insert(vertices.end(), subs_nw.begin(), subs_nw.end());
		vertices.insert(vertices.end(), subs_se.begin(), subs_se.end());
		vertices.insert(vertices.end(), subs_sw.begin(), subs_sw.end());
	}
	return vertices;
}

std::vector<GLfloat> QuadTree::getColors()
{
	GLfloat x, y, z;
	std::vector <GLfloat> colors;

	if (depth == 0) {
		x = 253.0f / 255.0f;
		y = 231.0f / 255.0f;
		z = 37.0f / 255.0f;
	}
	else if (depth == 1) {
		x = 160.0f / 255.0f;
		y = 218.0f / 255.0f;
		z = 57.0f / 255.0f;
	}
	else if (depth == 2) {
		x = 74.0f / 255.0f;
		y = 193.0f / 255.0f;
		z = 109.0f / 255.0f;
	}
	else if (depth == 3) {
		x = 31.0f / 255.0f;
		y = 161.0f / 255.0f;
		z = 135.0f / 255.0f;
	}
	else if (depth == 4) {
		x = 39.0f / 255.0f;
		y = 127.0f / 255.0f;
		z = 142.0f / 255.0f;
	}
	else if (depth == 5) {
		x = 54.0f / 255.0f;
		y = 92.0f / 255.0f;
		z = 141.0f / 255.0f;
	}
	else if (depth == 6) {
		x = 70.0f / 255.0f;
		y = 50.0f / 255.0f;
		z = 126.0f / 255.0f;
	}
	else if (depth == 7) {
		x = 70.0f / 255.0f;
		y = 50.0f / 255.0f;
		z = 126.0f / 255.0f;
	}
	else {
		// Default color values
		x = 1.0f;  // White
		y = 1.0f;
		z = 1.0f;
	}


	colors = {
		x, y, z,
		x, y, z,
		x, y, z,
		x, y, z,
		x, y, z,
		x, y, z,
		x, y, z,
		x, y, z
	};

	if (this->divided) {
		std::vector<GLfloat> subs_ne = northEast->getColors();
		std::vector<GLfloat> subs_nw = northWest->getColors();
		std::vector<GLfloat> subs_se = southEast->getColors();
		std::vector<GLfloat> subs_sw = southWest->getColors();

		colors.insert(colors.end(), subs_ne.begin(), subs_ne.end());
		colors.insert(colors.end(), subs_nw.begin(), subs_nw.end());
		colors.insert(colors.end(), subs_se.begin(), subs_se.end());
		colors.insert(colors.end(), subs_sw.begin(), subs_sw.end());
	}
	return colors;
}

