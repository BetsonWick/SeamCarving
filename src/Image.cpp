#include "Image.h"

Image::Image(std::vector<std::vector<Image::Pixel>> table)
    : m_table(std::move(table))
{}

Image::Pixel::Pixel(size_t red, size_t green, size_t blue)
    : m_red(red)
    , m_green(green)
    , m_blue(blue)
{}
