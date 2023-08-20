#include <cstdlib>
#include <algorithm>

#include "SpatialHashGrid.h"

SpatialHashGrid::SpatialHashGrid(int x_window, int y_window, int cellSize)
{
    this->x_window = x_window;
    this->y_window = y_window;
    this->m_cellSize = cellSize;

    grid_dim.x = ceil(static_cast<double>(x_window) / cellSize);
    grid_dim.y = ceil(static_cast<double>(y_window) / cellSize);

    // Calculate the center of the grid
    vector_2 center;
    center.x = grid_dim.x / 2;
    center.y = grid_dim.y / 2;
    int centerKey = createKey(center.x, center.y);

    // Initialize distance offset map
    std::map<int, std::unordered_set<int>> offsetsByDistToCenter;
    int maxDistSquared = 0;

    // Iterate through all cells in the grid
    for (int rowIndex = 0; rowIndex < grid_dim.y; rowIndex++) {
        for (int colIndex = 0; colIndex < grid_dim.x; colIndex++) {
            if (rowIndex == center.y && colIndex == center.x) {
                // This is the center cell
                offsetsByDistToCenter[0].insert(0);
                continue;
            }

            // Calculate the distance between the current cell and the center cell
            int d_x = std::max(std::abs(int(colIndex - center.x)), 0);
            int d_y = std::max(std::abs(int(rowIndex - center.y)), 0);
            int distSquared = d_x * d_x + d_y * d_y;

            // Calculate the offset and store it in the map
            int key = createKey(colIndex, rowIndex);
            offsetsByDistToCenter[distSquared].insert(key - centerKey);

            // Update the maximum squared distance
            if (distSquared > maxDistSquared) {
                maxDistSquared = distSquared;
            }
        }
    }

    // Allocate memory for offsets
    m_nOffsetsWithinSqDist = new int[maxDistSquared + 1];
    int numCells = grid_dim.x * grid_dim.y;
    m_globalOffset = new int[numCells];

    int i = 0;

    // Calculate the number of offsets within squared distances
    for (int d2 = 0; d2 <= maxDistSquared; d2++) {
        if (offsetsByDistToCenter[d2].empty()) {
            // If there are no cells whose min. squared dist. to center is d2,
            // use the previous value for m_nOffsetsWithinSqDist
            m_nOffsetsWithinSqDist[d2] = m_nOffsetsWithinSqDist[d2 - 1];
        }
        else {
            // Otherwise, populate the global offset array
            for (int offset : offsetsByDistToCenter[d2]) {
                m_globalOffset[i] = offset;
                i++;
            }
            m_nOffsetsWithinSqDist[d2] = i;
        }
    }
}

void SpatialHashGrid::addBoid(Boid* boid) {
    // position of the base of boid arrow
    auto i1 = getIndices(boid->getPos());
    int idx = createKey(i1.x, i1.y);
    m_grid[idx].insert(boid);

    boid->spatialIndex = idx;
}

int SpatialHashGrid::createKey(int x, int y) {
    return x + y * grid_dim.x;
}

vector_2 SpatialHashGrid::getIndices(vector_2 p) {
    float x = p.x / x_window;
    float y = p.y / y_window;

    double xIndex = floor(x * (grid_dim.x - 1));
    double yIndex = floor(y * (grid_dim.y - 1));

    return {xIndex, yIndex};
}

void SpatialHashGrid::clear() {
    m_grid.clear();
}

std::vector<Boid*> SpatialHashGrid::radiusSearch(Boid* query, int radius) {

    std::vector<Boid*> res;

    double d = radius / static_cast<double>(m_cellSize); // convert radius in pixel space to grid space
    int n = std::floor(d * d);

    int cellIndexOfQuery = query->spatialIndex;

    for(int i = 0; i < m_nOffsetsWithinSqDist[n]; i++) {
        int offset = m_globalOffset[i];

        // since the offsets are taken relative to origin, these could be out of bounds, keep it within m_gridDimensions
        int maxPossibleOffset = grid_dim.x * grid_dim.y - 1;
        int offsetWithinBounds = std::min<int>(std::max(0, cellIndexOfQuery + offset), maxPossibleOffset);

        for(auto boid : m_grid[offsetWithinBounds]) {
            if(boid == query) continue; // ignore itself

            float d_x = boid->getPos().x - query->getPos().x;
            float d_y = boid->getPos().y - query->getPos().y;
            float sq = d_x * d_x + d_y * d_y;

            if(sq <= radius * radius) {
                res.push_back(boid);
            }
        }
    }

    return res;
}

void SpatialHashGrid::removeBoid(Boid* boid) {
    m_grid[boid->spatialIndex].erase(boid); 
}

void SpatialHashGrid::updateBoid(Boid* boid) {
    auto i1 = getIndices(boid->getPos());
    int key = createKey(i1.x, i1.y);

    if(key == boid->spatialIndex) {
        return; // same index occupied, don't update
    }

    removeBoid(boid);
    addBoid(boid);
}

SpatialHashGrid::~SpatialHashGrid() {
    delete[] m_nOffsetsWithinSqDist;
    delete[] m_globalOffset;
}

SpatialHashGrid::SpatialHashGrid(SpatialHashGrid &&other) noexcept {
    m_cellSize = other.m_cellSize;
    grid_dim = other.grid_dim;
    x_window = other.x_window;
    y_window = other.y_window;
    m_grid = other.m_grid;
    m_nOffsetsWithinSqDist = other.m_nOffsetsWithinSqDist;
    m_globalOffset = other.m_globalOffset;

    other.m_nOffsetsWithinSqDist = nullptr;
    other.m_globalOffset = nullptr;
}

SpatialHashGrid &SpatialHashGrid::operator=(SpatialHashGrid &&other) noexcept {
    if(this != &other) {
        delete[] m_nOffsetsWithinSqDist;
        delete[] m_globalOffset;

        m_cellSize = other.m_cellSize;
        grid_dim = other.grid_dim;
        x_window = other.x_window;
        y_window = other.y_window;
        m_grid = other.m_grid;
        m_nOffsetsWithinSqDist = other.m_nOffsetsWithinSqDist;
        m_globalOffset = other.m_globalOffset;

        other.m_nOffsetsWithinSqDist = nullptr;
        other.m_globalOffset = nullptr;
    }
    return *this;
}

