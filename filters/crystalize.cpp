#include "crystalize.h"

Crystalize::Crystalize(const FilterArgs& args) {
    if (args.size() != 1) {
        throw std::runtime_error("count args != 1");
    }
    try {
        count_polygons_ = std::stoi(args[0]);
        if (count_polygons_ <= 0) {
            throw std::runtime_error("incorrect arguments");
        }
    } catch (...) {
        throw std::runtime_error("incorrect arguments");
    }
}

void Crystalize::Apply(Image& im) {
    Image new_image = im;
    std::vector<std::pair<int, int>> centers_polygons;
    int k = std::min(static_cast<int>(std::min(im.GetHeight(), im.GetWidth())),
                     static_cast<int>(floor(sqrt(count_polygons_))) + 1);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            int x = static_cast<int>(im.GetHeight()) / k * i + rand() % static_cast<int>(im.GetHeight()) / k;
            int y = static_cast<int>(im.GetWidth()) / k * j + static_cast<int>(rand() % im.GetWidth()) / k;
            centers_polygons.push_back({x, y});
        }
    }
    std::vector<std::vector<int>> distance(
        im.GetHeight(), std::vector<int>(im.GetWidth(), static_cast<int>(im.GetWidth() * im.GetHeight())));
    std::vector<std::vector<size_t>> centers(im.GetHeight(), std::vector<size_t>(im.GetWidth()));
    std::queue<std::pair<int, int>> q;
    std::vector<int> move_x = {-1, 0, 0, 1};
    std::vector<int> move_y = {0, -1, 1, 0};
    for (size_t i = 0; i < centers_polygons.size(); i++) {
        auto [x, y] = centers_polygons[i];
        distance[x][y] = 0;
        centers[x][y] = i;
        q.push(centers_polygons[i]);
    }
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (size_t k = 0; k < move_x.size(); k++) {
            int new_x = x + move_x[k];
            int new_y = y + move_y[k];
            if (0 <= new_x && new_x < im.GetHeight() && 0 <= new_y && new_y < im.GetWidth()) {
                if (distance[new_x][new_y] > distance[x][y] + 1) {
                    distance[new_x][new_y] = distance[x][y] + 1;
                    centers[new_x][new_y] = centers[x][y];
                    q.push({new_x, new_y});
                }
            }
        }
    }
    std::vector<Pixel> average_pixels(centers_polygons.size(), Pixel(0, 0, 0));
    std::vector<int> count_points(centers_polygons.size(), 0);
    for (uint32_t x = 0; x < im.GetHeight(); x++) {
        for (uint32_t y = 0; y < im.GetWidth(); y++) {
            average_pixels[centers[x][y]] = average_pixels[centers[x][y]] + im.GetPixel(x, y);
            count_points[centers[x][y]]++;
        }
    }
    for (uint32_t i = 0; i < centers_polygons.size(); i++) {
        average_pixels[i] = average_pixels[i] / static_cast<Pixel::Channel>(count_points[i]);
    }
    for (uint32_t x = 0; x < im.GetHeight(); x++) {
        for (uint32_t y = 0; y < im.GetWidth(); y++) {
            new_image.SetPixel(x, y, average_pixels[centers[x][y]]);
        }
    }
    im = new_image;
}

CrystalizeFactory::CrystalizeFactory() {
}

std::unique_ptr<Filter> CrystalizeFactory::Create(const FilterArgs& args) {
    try {
        return std::unique_ptr<Filter>(new Crystalize(args));
    } catch (std::exception& ex) {
        throw ex;
    }
}

std::string CrystalizeFactory::Help() {
    std::string message;
    message += "Фильтр crystalize: -crystalize count_polygons\n";
    message += "Разбивает изображение примерно на count_polygons многоугольников\n";
    message += "count_polygons должно быть целым положительным числом и быть не более разрешения изображения.\n";
    message += "Если count_polygons будет больше количества пикселей, то вернется исходное изображение";
    return message;
}
