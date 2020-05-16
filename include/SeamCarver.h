#pragma once

#include "Image.h"

class SeamCarver
{
    using Seam = std::vector<std::pair<size_t, size_t>>;

public:
    SeamCarver(Image image);

    const Image& GetImage() const;
    size_t GetImageWidth() const;
    size_t GetImageHeight() const;
    size_t GetPixelEnergy(size_t rowId, size_t columnId) const;
    Seam FindHorizontalSeam() const;
    Seam FindVerticalSeam() const;

private:
    Image m_image;
};