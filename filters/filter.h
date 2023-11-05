#pragma once
#include <memory>
#include <iostream>
#include "image.h"

using FilterArgs = std::vector<std::string>;

class Filter {
public:
    virtual void Apply(Image &im) = 0;
    virtual ~Filter() = default;

    Filter() = default;
};

class FilterFactory {
public:
    virtual std::unique_ptr<Filter> Create(const FilterArgs &args) = 0;
    virtual std::string Help() = 0;
    virtual ~FilterFactory() = default;
};
