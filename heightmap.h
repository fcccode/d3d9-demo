#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <vector>

class Heightmap {
public:
    Heightmap(int rows, int cols, const char *file);
    float get_height(int row, int col);

private:
    void average_filter();

private:
    std::vector<std::vector<float>> m_heightmap;
};

#endif
