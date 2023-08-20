#ifndef BOIDS_SPATIALHASHGRID_H
#define BOIDS_SPATIALHASHGRID_H

#include<unordered_set>
#include <map>
#include "boid.h"

class SpatialHashGrid {
private:
    int m_cellSize;
    vector_2 grid_dim; // dim of the grid cells (e.g. 3 by 2)
    int x_window;
    int y_window;
    std::map<int, std::unordered_set<Boid*>> m_grid;

    // numOffsetsWithinDist[i] contains no. of cells in the grid which are within squared distance <i> from center of grid (cumulative sum)
    int* m_nOffsetsWithinSqDist; // maxSquaredDist + 1 in size
    int* m_globalOffset; // numCells in size

    int createKey(int x, int y);
    vector_2 getIndices(vector_2 p);

public:
    SpatialHashGrid() = default;
    SpatialHashGrid(int x_window, int y_window, int cellSize);
    ~SpatialHashGrid();

    SpatialHashGrid(SpatialHashGrid&& other) noexcept;
    SpatialHashGrid& operator=(SpatialHashGrid&& other) noexcept;

    void addBoid(Boid* boid);
    void removeBoid(Boid* boid);
    void updateBoid(Boid* boid);
    void clear();
    std::vector<Boid*> radiusSearch(Boid* query, int radius);
};

#endif //BOIDS_SPATIALHASHGRID_H
