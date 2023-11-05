#include "bmp_reader.h"
#include "bmp_writer.h"
#include "filters/crop.h"
#include "filters/grayscale.h"
#include "filters/negative.h"
#include "filters/sharp.h"
#include "filters/edge.h"
#include "filters/blur.h"
#include "filters/crystalize.h"
#include <exception>
#include <fstream>
#include <iterator>
#include <memory>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct FilterParams {
    std::string name;
    FilterArgs args;

    FilterParams(std::string name, FilterArgs args) : name(name), args(args) {
    }
};

struct Arguments {
    std::string input_file;
    std::string output_file;
    std::vector<FilterParams> params;

    Arguments() {
    }

    Arguments(int argc, char const* const* argv) {
        if (argc == 2) {
            throw std::runtime_error("Укажите файл для вывода");
        }
        input_file = std::string(argv[1]);
        output_file = std::string(argv[2]);

        for (int i = 3; i < argc; i++) {
            if (argv[i][0] == '-') {
                params.push_back(FilterParams(std::string(argv[i]), {}));
            } else {
                if (params.empty()) {
                    throw std::runtime_error("Перед указанием параметра должно быть название фильтра");
                }
                params.back().args.push_back(std::string(argv[i]));
            }
        }
    }
};

int main(int argc, char** argv) {
    std::unordered_map<std::string, std::unique_ptr<FilterFactory>> factories;
    factories["-crop"] = std::make_unique<CropFactory>();
    factories["-gs"] = std::make_unique<GrayscaleFactory>();
    factories["-neg"] = std::make_unique<NegativeFactory>();
    factories["-sharp"] = std::make_unique<SharpFactory>();
    factories["-edge"] = std::make_unique<EdgeFactory>();
    factories["-blur"] = std::make_unique<BlurFactory>();
    factories["-crystalize"] = std::make_unique<CrystalizeFactory>();

    if (argc == 1) {
        std::cout << "Программа должна запускать в следующем формате:" << std::endl;
        std::cout << "./image_processor [входной файл] [выходной файл] -фильтр1 параметры1 -фильтр2 параметры2 ..."
                  << std::endl;
        std::cout << "Формат входного и выходного файла должен быть bmp" << std::endl;
        std::cout << "Список фильтров :" << std::endl;
        for (const auto& factory : factories) {
            std::cout << "-------" << std::endl;
            std::cout << factory.second->Help() << std::endl;
        }
        return 0;
    }

    Arguments args;
    try {
        args = Arguments(argc, argv);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    std::ifstream ifs(args.input_file, std::ios::in | std::ios::binary);
    if (!ifs) {
        std::cout << "Входной файл не получилось открыть" << std::endl;
        return 0;
    }
    Image image;
    try {
        image = InputImage(ifs);
    } catch (...) {
        std::cout << "Входной файл имеет расширение не bmp" << std::endl;
    }

    std::vector<std::unique_ptr<Filter>> filters;
    for (const auto& filter : args.params) {
        try {
            filters.push_back(factories.at(filter.name)->Create(filter.args));
        } catch (std::out_of_range& e) {
            std::cout << "Отсутствует фильтр " << filter.name << std::endl;
            return 0;
        } catch (std::exception& e) {
            std::cout << "Не правильно указаны параметры у фильтра " << filter.name << std::endl;
            return 0;
        }
    }
    for (const auto& filter : filters) {
        filter->Apply(image);
    }

    std::ofstream ofs(args.output_file, std::ios::out | std::ios::binary);
    if (!ofs) {
        std::cout << "Выходной файл не получилось открыть" << std::endl;
        return 0;
    }
    WriteImage(image, ofs);
    return 0;
}
