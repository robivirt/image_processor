#include "edge.h"

Edge::Edge(const FilterArgs& args) {
    if (args.size() != 1) {
        throw std::runtime_error("count args != 1");
    }
    try {
        threshold_ = static_cast<Pixel::Channel>(std::stod(args[0]));
        FilterArgs grayscale_args;
        grayscale_ = std::unique_ptr<Filter>(new Grayscale(grayscale_args));
    } catch (...) {
        throw std::runtime_error("incorrect arguments");
    }
}

void Edge::Apply(Image& im) {
    grayscale_->Apply(im);
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
            if (current_pixel.r > threshold_) {
                current_pixel = Pixel(1, 1, 1);
            } else {
                current_pixel = Pixel(0, 0, 0);
            }
            new_image.SetPixel(i, j, current_pixel);
        }
    }
    im = new_image;
}

EdgeFactory::EdgeFactory() {
}

std::unique_ptr<Filter> EdgeFactory::Create(const FilterArgs& args) {
    try {
        return std::unique_ptr<Filter>(new Edge(args));
    } catch (std::exception& ex) {
        throw ex;
    }
}

std::string EdgeFactory::Help() {
    std::string message;
    message += "Фильтр edge: -edge threshold\n";
    message += "Выделяет границы\n";
    message += "Переводит изображение в оттенки серого.\n";
    message += "Если значение пикселя больше threshold, то делаем пиксель белым, иначе черным\n";
    message += "threshold должен быть вещественным числом";
    return message;
}
