#pragma once
#include "filter.h"
#include "grayscale.h"
#include <string>
#include <exception>
#include <algorithm>

class Edge : public Filter {
private:
    std::unique_ptr<Filter> grayscale_;
    Pixel::Channel threshold_;
    const std::vector<std::vector<Pixel::Channel>> matrix_ = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    const std::vector<int32_t> move_i_ = {-1, 0, 1};
    const std::vector<int32_t> move_j_ = {-1, 0, 1};

public:
    explicit Edge(const FilterArgs& args);

    void Apply(Image& im) override;
};

class EdgeFactory : public FilterFactory {
public:
    EdgeFactory();

    std::unique_ptr<Filter> Create(const FilterArgs& args) override;

    std::string Help() override;
};
