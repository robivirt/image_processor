#include "image.h"
#include <cstdint>
#include <algorithm>

Pixel::Pixel(Pixel::Channel r, Pixel::Channel g, Pixel::Channel b) : r(r), g(g), b(b) {
}

Pixel operator+(const Pixel &p1, const Pixel &p2) {
    return Pixel(p1.r + p2.r, p1.g + p2.g, p1.b + p2.b);
}

Pixel operator-(const Pixel &p1, const Pixel &p2) {
    return Pixel(p1.r - p2.r, p1.g - p2.g, p1.b - p2.b);
}

Pixel::Channel operator*(const Pixel &p1, const Pixel &p2) {
    return p1.r * p2.r + p1.g * p2.g + p1.b * p2.b;
}

Pixel operator*(const Pixel &p, const Pixel::Channel &alpha) {
    return Pixel(p.r * alpha, p.g * alpha, p.b * alpha);
}

Pixel operator/(const Pixel &p, const Pixel::Channel &alpha) {
    return Pixel(p.r / alpha, p.g / alpha, p.b / alpha);
}

Image::Image() {
}

Image::Image(uint32_t width, uint32_t height, const std::vector<std::vector<Pixel>> &image)
    : width_(width), height_(height), image_(image) {
}

Image::Image(const Image &im) : width_(im.width_), height_(im.height_), image_(im.image_) {
}

uint32_t Image::GetWidth() const {
    return width_;
}

uint32_t Image::GetHeight() const {
    return height_;
}

Pixel Image::GetNearestPixel(int32_t i, int32_t j) const {
    i = std::clamp(i, 0, static_cast<int32_t>(height_ - 1));
    j = std::clamp(j, 0, static_cast<int32_t>(width_ - 1));
    return image_[i][j];
}

Pixel Image::GetPixel(uint32_t i, uint32_t j) const {
    return image_[i][j];
}

void Image::SetWidth(uint32_t width) {
    width_ = width;
    for (auto &row : image_) {
        row.resize(width_);
    }
}

void Image::SetHeight(uint32_t height) {
    height_ = height;
    image_.resize(height_);
}

void Image::SetPixel(uint32_t i, uint32_t j, const Pixel &p) {
    image_[i][j] = p;
}
