#include "blur.h"
#include <cmath>
#include <cstdint>

Blur::Blur(const FilterArgs& args) {
    if (args.size() != 1) {
        throw std::runtime_error("count args != 1");
    }
    try {
        sigma_ = static_cast<Pixel::Channel>(std::stod(args[0]));
        if (sigma_ == 0) {
            throw std::runtime_error("sigma_ = 0");
        }
    } catch (...) {
        throw std::runtime_error("incorrect arguments");
    }
}

void Blur::Apply(Image& im) {
    Image new_image = im;

    int32_t radius = static_cast<int32_t>(ceil(3 * sigma_));
    std::vector<std::vector<Pixel::Channel>> gauss_core(radius + 1, std::vector<Pixel::Channel>(radius + 1, 0.0));
    for (int32_t i = 0; i <= radius; i++) {
        for (int32_t j = 0; j <= radius; j++) {
            gauss_core[i][j] =
                static_cast<Pixel::Channel>(pow(std::numbers::e_v<Pixel::Channel>,
                                                static_cast<Pixel::Channel>(-(i * i + j * j)) / (2 * sigma_ * sigma_)));
        }
    }

    for (int32_t i = 0; i < im.GetHeight(); i++) {
        std::vector<Pixel> gauss_without_column(im.GetWidth());
        for (int32_t a = std::max(0, i - radius); a < std::min(static_cast<int32_t>(im.GetHeight()), i + radius); a++) {
            for (int32_t j = 0; j < im.GetHeight(); j++) {
                gauss_without_column[j] = gauss_without_column[j] + im.GetPixel(a, j) * gauss_core[abs(a - i)][0];
            }
        }
        for (int32_t j = 0; j < im.GetWidth(); j++) {
            Pixel current_pixel = Pixel(0, 0, 0);

            for (int32_t b = std::max(0, j - radius); b < std::min(static_cast<int32_t>(im.GetWidth()), j + radius);
                 b++) {
                current_pixel = current_pixel + gauss_without_column[b] * gauss_core[0][abs(b - j)];
            }
            current_pixel = current_pixel / (2 * std::numbers::pi_v<Pixel::Channel> * sigma_ * sigma_);
            new_image.SetPixel(i, j, current_pixel);
        }
    }
    im = new_image;
}

BlurFactory::BlurFactory() {
}

std::unique_ptr<Filter> BlurFactory::Create(const FilterArgs& args) {
    try {
        return std::unique_ptr<Filter>(new Blur(args));
    } catch (std::exception& ex) {
        throw ex;
    }
}

std::string BlurFactory::Help() {
    std::string message;
    message += "Фильтр crop: -blur sigma\n";
    message += "Размывает картинку по функции гаусса с параметром sigma\n";
    message += "Параметр sigma вещественное число";
    return message;
}
