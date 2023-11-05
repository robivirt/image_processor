#include "crop.h"

Crop::Crop(const FilterArgs& args) {
    if (args.size() != 2) {
        throw std::runtime_error("count args != 2");
    }
    try {
        width_ = std::stoi(args[0]);
        height_ = std::stoi(args[1]);
    } catch (...) {
        throw std::runtime_error("incorrect arguments");
    }
}

void Crop::Apply(Image& im) {
    im.SetWidth(std::min(im.GetWidth(), width_));
    im.SetHeight(std::min(im.GetHeight(), height_));
}

CropFactory::CropFactory() {
}

std::unique_ptr<Filter> CropFactory::Create(const FilterArgs& args) {
    try {
        return std::unique_ptr<Filter>(new Crop(args));
    } catch (std::exception& ex) {
        throw ex;
    }
}

std::string CropFactory::Help() {
    std::string message;
    message += "Фильтр crop: -crop width height\n";
    message += "Обрезует картинку до размера width height. width и height должны быть целые положительные";
    return message;
}
