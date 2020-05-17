#pragma once

#include <vector>

struct Image
{
    struct Pixel
    {
        Pixel(size_t red, size_t green, size_t blue);

        size_t m_red;
        size_t m_green;
        size_t m_blue;
    };

    Image(std::vector<std::vector<Pixel>> table);

    std::vector<std::vector<Pixel>> m_table;
};