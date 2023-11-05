#pragma once
#include <exception>
#include <algorithm>
#include "filter.h"

class Crop : public Filter {
private:
    uint32_t width_, height_;

public:
    explicit Crop(const FilterArgs& args);

    void Apply(Image& im) override;
};

class CropFactory : public FilterFactory {
public:
    CropFactory();

    std::unique_ptr<Filter> Create(const FilterArgs& args) override;

    std::string Help() override;
};
