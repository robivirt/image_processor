#include "negative.h"

Negative::Negative(const FilterArgs& args) {
    if (!args.empty()) {
        throw std::runtime_error("count args != 0");
    }
}

void Negative::Apply(Image& im) {
    for (uint32_t i = 0; i < im.GetHeight(); i++) {
        for (uint32_t j = 0; j < im.GetWidth(); j++) {
            Pixel current_pixel = im.GetPixel(i, j);
            Pixel new_pixel = Pixel(1, 1, 1) - current_pixel;
            im.SetPixel(i, j, new_pixel);
        }
    }
}

NegativeFactory::NegativeFactory() {
}

std::unique_ptr<Filter> NegativeFactory::Create(const FilterArgs& args) {
    try {
        return std::unique_ptr<Filter>(new Negative(args));
    } catch (std::exception& ex) {
        throw ex;
    }
}

std::string NegativeFactory::Help() {
    std::string message;
    message += "Фильтр Negative: -Negative\n";
    message += "Делает негатив изображения, используется без параметров";
    return message;
}
