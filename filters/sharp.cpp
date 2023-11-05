#include "sharp.h"
#include <algorithm>

Sharp::Sharp(const FilterArgs& args) {
    if (!args.empty()) {
        throw std::runtime_error("count args != 0");
    }
}

void Sharp::Apply(Image& im) {
    Image new_image = im;
    for (int32_t i = 0; i < im.GetHeight(); i++) {
        for (int32_t j = 0; j < im.GetWidth(); j++) {
            Pixel current_pixel = Pixel(0, 0, 0);
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    current_pixel = current_pixel + im.GetNearestPixel(i + move_i_[a], j + move_j_[b]) * matrix_[a][b];
                }
            }
            current_pixel.r = std::clamp(current_pixel.r, 0.0f, 1.0f);
            current_pixel.g = std::clamp(current_pixel.g, 0.0f, 1.0f);
            current_pixel.b = std::clamp(current_pixel.b, 0.0f, 1.0f);
            new_image.SetPixel(i, j, current_pixel);
        }
    }
    im = new_image;
}

SharpFactory::SharpFactory() {
}

std::unique_ptr<Filter> SharpFactory::Create(const FilterArgs& args) {
    try {
        return std::unique_ptr<Filter>(new Sharp(args));
    } catch (std::exception& ex) {
        throw ex;
    }
}

std::string SharpFactory::Help() {
    std::string message;
    message += "Фильтр sharp: -sharp\n";
    message += "Повышат резкость, используется без параметров";
    return message;
}
