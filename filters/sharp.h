#pragma once
#include "filter.h"
#include <exception>
#include <algorithm>

class Sharp : public Filter {
private:
    const std::vector<std::vector<Pixel::Channel>> matrix_ = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    const std::vector<int> move_i_ = {-1, 0, 1};
    const std::vector<int> move_j_ = {-1, 0, 1};

public:
    explicit Sharp(const FilterArgs& args);

    void Apply(Image& im) override;
};

class SharpFactory : public FilterFactory {
public:
    SharpFactory();

    std::unique_ptr<Filter> Create(const FilterArgs& args) override;

    std::string Help() override;
};
