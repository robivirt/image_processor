#include "grayscale.h"

Grayscale::Grayscale(const FilterArgs& args) {
    if (!args.empty()) {
        throw std::runtime_error("count args != 0");
    }
}

void Grayscale::Apply(Image& im) {
    for (uint32_t i = 0; i < im.GetHeight(); i++) {
        for (uint32_t j = 0; j < im.GetWidth(); j++) {
            Pixel current_pixel = im.GetPixel(i, j);
            Pixel new_pixel = Pixel(current_pixel * transform_fixel_, current_pixel * transform_fixel_,
                                    current_pixel * transform_fixel_);
            im.SetPixel(i, j, new_pixel);
        }
    }
}

GrayscaleFactory::GrayscaleFactory() {
}

std::unique_ptr<Filter> GrayscaleFactory::Create(const FilterArgs& args) {
    try {
        return std::unique_ptr<Filter>(new Grayscale(args));
    } catch (std::exception& ex) {
        throw ex;
    }
}

std::string GrayscaleFactory::Help() {
    std::string message;
    message += "Фильт grayscale: -grayscale\n";
    message += "Переводит картинку в оттенки серого, используется без параметров";
    return message;
}
