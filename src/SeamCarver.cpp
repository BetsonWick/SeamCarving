#include <cmath>
#include "SeamCarver.h"

using namespace std;

using vec = vector<double>;

SeamCarver::SeamCarver(Image image)
        : m_image(std::move(image)) {
    energy_matrix = vector<vec>(GetImageWidth(), vec(GetImageHeight(), 0));
    fillEnergy();
}

const Image &SeamCarver::GetImage() const {
    return m_image;
}

size_t SeamCarver::GetImageWidth() const {
    return m_image.m_table.size();
}

size_t SeamCarver::GetImageHeight() const {
    return m_image.m_table.empty() ? 0 : m_image.m_table.front().size();
}

double SeamCarver::GetPixelEnergy(size_t columnId, size_t rowId) const {

    Image::Pixel top = m_image.GetPixel(columnId, (rowId - 1 + GetImageHeight()) % GetImageHeight());
    Image::Pixel bottom = m_image.GetPixel(columnId, (rowId + 1 + GetImageHeight()) % GetImageHeight());
    Image::Pixel right = m_image.GetPixel((columnId + 1 + GetImageWidth()) % GetImageWidth(), rowId);
    Image::Pixel left = m_image.GetPixel((columnId - 1 + GetImageWidth()) % GetImageWidth(), rowId);
    double delta_x = pow(right.m_green - left.m_green, 2) +
                     pow(right.m_blue - left.m_blue, 2) +
                     pow(right.m_red - left.m_red, 2);
    double delta_y = pow(top.m_green - bottom.m_green, 2) +
                     pow(top.m_blue - bottom.m_blue, 2) +
                     pow(top.m_red - bottom.m_red, 2);
    return sqrt(delta_x + delta_y);
}

vector<size_t> getPath(vector<vector<double>> matrix) {
    size_t width = matrix.size();
    size_t height = matrix.empty() ? 0 : matrix.front().size();
    vector<vec> table(width, vec(height, 0));
    for (size_t x = 0; x < width; x++) {
        table[x][0] = matrix[x][0];
    }
    for (size_t i = 1; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            table[j][i] = table[j][i - 1];
            if (j > 0 && table[j - 1][i - 1] < table[j][i]) table[j][i] = table[j - 1][i - 1];
            if (j < width - 1 && table[j + 1][i - 1] < table[j][i]) table[j][i] = table[j + 1][i - 1];
            table[j][i] += matrix[j][i];
        }
    }
    vector<size_t> path(height, 0);
    size_t last = height - 1;
    for (size_t i = 1; i < width; i++) {
        if (table[i][last] < table[path[last]][last]) {
            path[last] = i;
        }
    }
    for (int i = static_cast<int>(last) - 1; i >= 0; i--) {
        size_t prev = path[i + 1];
        path[i] = prev;
        if (prev > 0 && table[path[i]][i] > table[prev - 1][i]) path[i] = prev - 1;
        if (prev < width - 1 && table[path[i]][i] > table[prev + 1][i]) path[i] = prev + 1;
    }
    return path;
}

vector<vec> getRotatedMatrix(vector<vec> matrix) {
    size_t width = matrix.size();
    size_t height = matrix.empty() ? 0 : matrix.front().size();
    vector<vec> res(height, vec(width, 0));
    for (size_t x = 0; x < width; x++) {
        for (size_t y = 0; y < height; y++) {
            res[y][x] = matrix[x][y];
        }
    }
    return res;
}

SeamCarver::Seam SeamCarver::FindHorizontalSeam() const {
    return getPath(getRotatedMatrix(energy_matrix));

}

SeamCarver::Seam SeamCarver::FindVerticalSeam() const {
    return getPath(energy_matrix);
}

void SeamCarver::RemoveVerticalSeam(const Seam &seam) {
    for (size_t i = 0; i < GetImageHeight(); i++) {
        for (size_t j = seam[i]; j < GetImageWidth() - 1; j++) {
            m_image.m_table[j][i] = m_image.m_table[j + 1][i];
        }
    }
    m_image.m_table.erase(m_image.m_table.end());
    fillEnergy();
}

void SeamCarver::RemoveHorizontalSeam(const SeamCarver::Seam &seam) {
    for (size_t y = 0; y < GetImageWidth(); y++) {
        m_image.m_table[y].erase(m_image.m_table[y].begin() + seam[y]);
    }
    fillEnergy();
}

void SeamCarver::fillEnergy() {
    for (size_t x = 0; x < GetImageWidth(); x++) {
        for (size_t y = 0; y < GetImageHeight(); y++) {
            energy_matrix[x][y] = GetPixelEnergy(x, y);
        }
    }
}

