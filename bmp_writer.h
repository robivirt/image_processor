#pragma once
#include <bit>
#include <climits>
#include <cstdint>
#include <cmath>
#include <fstream>
#include "image.h"

template <typename T>
void Write(T x, std::ofstream &f, int64_t size_type = sizeof(T)) {
    char *data = nullptr;
    if (std::endian::native == std::endian::little) {
        data = reinterpret_cast<char *>(&x);
    } else {
        T reverse_x = 0;
        for (int64_t i = 0; i < size_type; i++) {
            reverse_x |= ((x >> (CHAR_BIT * i)) & ((1 << CHAR_BIT) - 1)) << (size_type - i - 1);
        }
        data = reinterpret_cast<char *>(&x);
    }
    f.write(data, size_type);
}

struct BmpHeaderWrite {
    const int16_t id = 0x4D42;
    const int32_t unused_number = 0;
    const int32_t size_header = 54;
};

void WriteBmpHeader(uint32_t file_size, std::ofstream &f) {
    BmpHeaderWrite bmp_header;
    Write(bmp_header.id, f, 2);
    Write(file_size, f);
    Write(bmp_header.unused_number, f, 4);
    Write(bmp_header.size_header, f, 4);
}

struct DibHeaderWrite {
    const uint32_t number_byte_in_dib = 0x00000028;
    const uint16_t planes = 0x0001;
    const uint16_t bits_in_pixel = 0x0018;
    const uint32_t compression = 0;
    const int32_t resolution_width = 0x00000B13;
    const int32_t resolution_height = 0x00000B13;
    const uint32_t color_used = 0;
    const uint32_t color_important = 0;
};

void WriteDibHeader(uint32_t width, uint32_t height, std::ofstream &f) {
    DibHeaderWrite dib_header;
    Write(dib_header.number_byte_in_dib, f, 4);
    Write(width, f);
    Write(height, f);
    Write(dib_header.planes, f, 2);
    Write(dib_header.bits_in_pixel, f, 2);
    Write(dib_header.compression, f, 4);
    Write(height * 3 + width + width % 4, f, 4);
    Write(dib_header.resolution_width, f, 4);
    Write(dib_header.resolution_height, f, 4);
    Write(dib_header.color_used, f, 4);
    Write(dib_header.color_important, f, 4);
}

void WriteBitmap(const Image &im, std::ofstream &f) {
    for (uint32_t i = 0; i < im.GetHeight(); ++i) {
        for (uint32_t j = 0; j < im.GetWidth(); ++j) {
            Write(static_cast<int>(round(im.GetPixel(im.GetHeight() - i - 1, j).b * MAX_COLOR)), f, 1);
            Write(static_cast<int>(round(im.GetPixel(im.GetHeight() - i - 1, j).g * MAX_COLOR)), f, 1);
            Write(static_cast<int>(round(im.GetPixel(im.GetHeight() - i - 1, j).r * MAX_COLOR)), f, 1);
        }
        for (uint32_t j = 0; j < im.GetWidth() % 4; j++) {
            Write(0x0, f, 1);
        }
    }
}

void WriteImage(const Image &im, std::ofstream &f) {
    BmpHeaderWrite bmp_header;
    WriteBmpHeader(im.GetHeight() * 3 * im.GetWidth() + im.GetWidth() % 4 + bmp_header.size_header, f);
    WriteDibHeader(im.GetWidth(), im.GetHeight(), f);
    WriteBitmap(im, f);
}
