#include <cmath>
#include "SeamCarver.h"

using namespace std;

SeamCarver::SeamCarver(Image image)
        : m_image(std::move(image)) {
    energy_matrix = vector<vector<double>>(GetImageWidth(), vector<double>(GetImageHeight(), 0));
    for (size_t x = 0; x < GetImageWidth(); x++) {
        for (size_t y = 0; y < GetImageHeight(); y++) {
            energy_matrix[x][y] = GetPixelEnergy(x, y);
        }
    }
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

SeamCarver::Seam SeamCarver::FindHorizontalSeam() const {
    Seam path;
    vector<vector<double>> table(GetImageWidth(), vector<double>(GetImageHeight(), 0));
    for (size_t y = 0; y < GetImageHeight(); y++) {
        table[0][y] = energy_matrix[0][y];
    }
    for (size_t i = 0; i < GetImageHeight(); i++) {
        for (size_t j = 1; j < GetImageWidth(); j++) {
            table[j][i] = table[j - 1][i];
            if (i > 0 && table[j - 1][i - 1] < table[j][i]) table[j][i] = table[j - 1][i - 1];
            if (i < GetImageHeight() - 1 && table[j - 1][i + 1] < table[j][i]) table[j][i] = table[j - 1][i + 1];
            table[j][i] += energy_matrix[j][i];
        }
    }
    path.resize(GetImageWidth(), 0);
    size_t last = GetImageWidth() - 1;
    for (size_t i = 1; i < GetImageHeight(); i++) {
        if (table[last][i] < table[last][path[last]]) {
            path[last] = i;
        }
    }
    for (int i = last - 1; i >= 0; i--) {
        size_t prev = path[i + 1];
        path[i] = prev;
        if (prev > 0 && table[i][path[i]] > table[i][prev - 1]) path[i] = prev - 1;
        if (prev < GetImageHeight() - 1 && table[i][path[i]] > table[i][prev + 1]) path[i] = prev + 1;
    }
    return path;
}

SeamCarver::Seam SeamCarver::FindVerticalSeam() const {
    Seam path;
    vector<vector<double>> table(GetImageWidth(), vector<double>(GetImageHeight(), 0));
    for (size_t x = 0; x < GetImageWidth(); x++) {
        table[x][0] = energy_matrix[x][0];
    }
    for (size_t i = 1; i < GetImageHeight(); i++) {
        for (size_t j = 0; j < GetImageWidth(); j++) {
            table[j][i] = table[j][i - 1];
            if (j > 0 && table[j - 1][i - 1] < table[j][i]) table[j][i] = table[j - 1][i - 1];
            if (j < GetImageWidth() - 1 && table[j + 1][i - 1] < table[j][i]) table[j][i] = table[j + 1][i - 1];
            table[j][i] += energy_matrix[j][i];
        }
    }
    path.resize(GetImageHeight(), 0);
    size_t last = GetImageHeight() - 1;
    for (size_t i = 1; i < GetImageWidth(); i++) {
        if (table[i][last] < table[path[last]][last]) {
            path[last] = i;
        }
    }
    for (int i = last - 1; i >= 0; i--) {
        size_t prev = path[i + 1];
        path[i] = prev;
        if (prev > 0 && table[path[i]][i] > table[prev - 1][i]) path[i] = prev - 1;
        if (prev < GetImageWidth() - 1 && table[path[i]][i] > table[prev + 1][i]) path[i] = prev + 1;
    }
    return path;
}

void SeamCarver::RemoveVerticalSeam(const Seam &seam) {
    for (size_t i = 0; i < GetImageHeight(); i++) {
        for (size_t j = seam[i]; j < GetImageWidth() - 1; j++) {
            m_image.m_table[j][i] = m_image.m_table[j + 1][i];
        }
    }
    m_image.m_table.erase(m_image.m_table.end());
}

void SeamCarver::RemoveHorizontalSeam(const SeamCarver::Seam &seam) {
    for (size_t y = 0; y < GetImageWidth(); y++) {
        m_image.m_table[y].erase(m_image.m_table[y].begin() + seam[y]);
    }
}
