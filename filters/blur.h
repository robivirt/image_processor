#pragma once
#include <algorithm>
#include <cmath>
#include <exception>
#include <numbers>
#include "filter.h"

class Blur : public Filter {
private:
    Pixel::Channel sigma_;

public:
    explicit Blur(const FilterArgs& args);

    void Apply(Image& im) override;
};

class BlurFactory : public FilterFactory {
public:
    BlurFactory();

    std::unique_ptr<Filter> Create(const FilterArgs& args) override;

    std::string Help() override;
};
