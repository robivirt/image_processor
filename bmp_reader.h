#pragma once
#include <fstream>
#include <iostream>
#include <cstdint>
#include <climits>
#include <type_traits>
#include "image.h"
#include "bmp_writer.h"

template <typename T>
void InputU(T &x, std::istream &f) {
    T result = 0;
    unsigned char data[sizeof(T)];
    f.read(reinterpret_cast<char *>(&data), sizeof(T));
    for (size_t i = 0; i < sizeof(T); i++) {
        result |= static_cast<T>(data[i]) << (CHAR_BIT * i);
    }
    x = result;
}

template <typename T>
void Input(T &x, std::istream &f) {
    T result = 0;
    if constexpr (std::is_unsigned_v<T>) {
        InputU(result, f);
    } else {
        InputU(result, f);
        result = static_cast<std::make_signed_t<T>>(result);
    }
    x = result;
}

struct BmpHeaderRead {
    int16_t id;
    int32_t size_bmp_file;
    int32_t unused_number;
    int32_t size_header;
};

void InputBmpHeader(std::istream &f) {
    BmpHeaderRead bmp_header;
    Input(bmp_header.id, f);
    Input(bmp_header.size_bmp_file, f);
    Input(bmp_header.unused_number, f);
    Input(bmp_header.size_header, f);

    BmpHeaderWrite right_bmp_header;
    if (bmp_header.id != right_bmp_header.id || bmp_header.size_header != right_bmp_header.size_header) {
        throw std::runtime_error("Incorrect file");
    }
}

struct DibHeaderRead {
    uint32_t number_byte_in_dib;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bits_in_pixel;
    uint32_t compression;
    uint32_t size_image;
    int32_t resolution_width;
    int32_t resolution_height;
    uint32_t color_used;
    uint32_t color_important;
};

void InputDibHeader(uint32_t &width, uint32_t &height, std::istream &f) {
    DibHeaderRead dib_header;
    Input(dib_header.number_byte_in_dib, f);
    Input(dib_header.width, f);
    Input(dib_header.height, f);
    Input(dib_header.planes, f);
    Input(dib_header.bits_in_pixel, f);
    Input(dib_header.compression, f);
    Input(dib_header.size_image, f);
    Input(dib_header.resolution_width, f);
    Input(dib_header.resolution_height, f);
    Input(dib_header.color_used, f);
    Input(dib_header.color_important, f);
    width = dib_header.width;
    height = dib_header.height;

    DibHeaderWrite right_dib_header;
    if (dib_header.number_byte_in_dib != right_dib_header.number_byte_in_dib) {
        throw std::runtime_error("Incorrect file");
    }
    if (dib_header.planes != right_dib_header.planes) {
        throw std::runtime_error("Incorrect file");
    }
    if (dib_header.bits_in_pixel != right_dib_header.bits_in_pixel) {
        throw std::runtime_error("Incorrect file");
    }
    if (dib_header.compression != right_dib_header.compression) {
        throw std::runtime_error("Incorrect file");
    }
    if (dib_header.color_used != right_dib_header.color_used) {
        throw std::runtime_error("Incorrect file");
    }
    if (dib_header.color_important != right_dib_header.color_important) {
        throw std::runtime_error("Incorrect file");
    }
}

void InputBitmap(Image &im, std::istream &f) {
    for (size_t i = 0; i < im.GetHeight(); ++i) {
        for (size_t j = 0; j < im.GetWidth(); ++j) {
            unsigned char r = 0;
            unsigned char g = 0;
            unsigned char b = 0;
            Input(b, f);
            Input(g, f);
            Input(r, f);
            Pixel::Channel r_channel = static_cast<Pixel::Channel>(r) / MAX_COLOR;
            Pixel::Channel g_channel = static_cast<Pixel::Channel>(g) / MAX_COLOR;
            Pixel::Channel b_channel = static_cast<Pixel::Channel>(b) / MAX_COLOR;
            im.SetPixel(im.GetHeight() - i - 1, j, Pixel(r_channel, g_channel, b_channel));
        }
        f.ignore(im.GetWidth() % 4);
    }
}

Image InputImage(std::ifstream &f) {
    InputBmpHeader(f);

    uint32_t width = 0;
    uint32_t height = 0;
    InputDibHeader(width, height, f);

    Image result;
    result.SetHeight(height);
    result.SetWidth(width);
    InputBitmap(result, f);

    return result;
}
