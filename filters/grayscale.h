#pragma once
#include <cstdint>
#include <exception>
#include <algorithm>
#include "filter.h"

class Grayscale : public Filter {
private:
    const Pixel transform_fixel_ = Pixel(0.299f, 0.587f, 0.114f);

public:
    explicit Grayscale(const FilterArgs& args);

    void Apply(Image& im) override;
};

class GrayscaleFactory : public FilterFactory {
public:
    GrayscaleFactory();

    std::unique_ptr<Filter> Create(const FilterArgs& args) override;

    std::string Help() override;
};
