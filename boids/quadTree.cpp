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

QuadTree::~QuadTree() {
	delete northWest;
	delete northEast;
	delete southWest;
	delete southEast;
	found.~vector();
	found_ne.~vector();
	found_nw.~vector();
	found_se.~vector();
	found_sw.~vector();
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

QuadTree::QuadTree(const Rectangle& boundary, int capacity, int depth)
{
	this->boundary = boundary;
	this->capacity = capacity;
	this->depth = depth;
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
	this->northEast = new QuadTree(ne, this->capacity, depth - 1);

	Rectangle nw = Rectangle(x - w, y - h, w, h);
	this->northWest = new QuadTree(nw, this->capacity, depth - 1);

	Rectangle se = Rectangle(x + w, y + h, w, h);
	this->southEast = new QuadTree(se, this->capacity, depth - 1);

	Rectangle sw = Rectangle(x - w, y + h, w, h);
	this->southWest = new QuadTree(sw, this->capacity, depth - 1);
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

std::vector<Boid*> QuadTree::query(const Rectangle& range)
{

	if (!range.intersects(this->boundary)) {
		return std::vector<Boid*>();
	}

	if (this->divided) {
		found_ne = this->northWest->query(range);
		found.insert(found.end(), found_ne.begin(), found_ne.end());
		found_ne.clear();
		found_nw = this->northEast->query(range);
		found.insert(found.end(), found_nw.begin(), found_nw.end());
		found_nw.clear();
		found_se = this->southWest->query(range);
		found.insert(found.end(), found_se.begin(), found_se.end());
		found_se.clear();
		found_sw = this->southEast->query(range);
		found.insert(found.end(), found_sw.begin(), found_sw.end());
		found_sw.clear();
	}
	else {
		for (Boid* p : this->boids) {
			if (range.contains(*p)) {
				found.push_back(p);
			}
		}
	}


	std::vector<Boid*> res = found;
	found.clear();

	return res;
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
