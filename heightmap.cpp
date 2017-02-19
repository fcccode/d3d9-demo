#include <stdio.h>
#include <vector>
#include "heightmap.h"
#include "error.h"

using std::vector;

Heightmap::Heightmap(int rows, int cols, const char *filename) {
    m_heightmap = vector<vector<float>>(rows, vector<float>(cols));

    int elems = rows * cols;
    unsigned char *buffer = new unsigned char[elems];
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fatal(__FILE__, __LINE__, "fopen");
    }
    int count = fread(buffer, 1, elems, file);
    if (count != elems) {
        fatal(__FILE__, __LINE__, "fread");
    }
    fclose(file);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * cols + j;
            m_heightmap[i][j] = buffer[index];
        }
    }
    delete buffer;

    average_filter();
}

void Heightmap::average_filter() {
    vector<vector<float>> copy = m_heightmap;
    int rows = m_heightmap.size();
    int cols = m_heightmap[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float sum = 0;
            float num = 0;
            for (int m = i - 1; m <= i + 1; m++) {
                for (int n = j - 1; n <= j + 1; n++) {
                    if (m >= 0 && m < rows && n >= 0 && n < cols) {
                        sum += copy[m][n];
                        num += 1;
                    }
                }
            }
            m_heightmap[i][j] = sum / num;
        }
    }
}

float Heightmap::get_height(int row, int col) {
    return m_heightmap[row][col];
}
