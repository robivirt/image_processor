#pragma once
#include <exception>
#include <algorithm>
#include "filter.h"

class Negative : public Filter {
public:
    explicit Negative(const FilterArgs& args);

    void Apply(Image& im) override;
};

class NegativeFactory : public FilterFactory {
public:
    NegativeFactory();

    std::unique_ptr<Filter> Create(const FilterArgs& args) override;

    std::string Help() override;
};
