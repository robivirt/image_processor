#pragma once
#include <atomic>
#include <cstdint>
#include <vector>

const int MAX_COLOR = 255;

struct Pixel {
    using Channel = float;
    Channel r = 0;
    Channel g = 0;
    Channel b = 0;

    Pixel() = default;

    Pixel(Channel r, Channel g, Channel b);
};

Pixel operator+(const Pixel &p1, const Pixel &p2);

Pixel operator-(const Pixel &p1, const Pixel &p2);

Pixel::Channel operator*(const Pixel &p1, const Pixel &p2);

Pixel operator*(const Pixel &p, const Pixel::Channel &alpha);

Pixel operator/(const Pixel &p, const Pixel::Channel &alpha);

class Image {
private:
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    std::vector<std::vector<Pixel>> image_;

public:
    Image();

    Image(uint32_t width, uint32_t height, const std::vector<std::vector<Pixel>> &image);

    Image(const Image &im);

    uint32_t GetWidth() const;

    uint32_t GetHeight() const;

    Pixel GetNearestPixel(int32_t i, int32_t j) const;

    Pixel GetPixel(uint32_t i, uint32_t j) const;

    void SetWidth(uint32_t width);

    void SetHeight(uint32_t height);

    void SetPixel(uint32_t i, uint32_t j, const Pixel &p);
};
