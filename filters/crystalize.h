#pragma once
#include "filter.h"
#include <algorithm>
#include <exception>
#include <cmath>
#include <cstdlib>
#include <string>
#include <queue>

class Crystalize : public Filter {
private:
    int count_polygons_;

public:
    explicit Crystalize(const FilterArgs& args);

    void Apply(Image& im) override;
};

class CrystalizeFactory : public FilterFactory {
public:
    CrystalizeFactory();

    std::unique_ptr<Filter> Create(const FilterArgs& args) override;

    std::string Help() override;
};
