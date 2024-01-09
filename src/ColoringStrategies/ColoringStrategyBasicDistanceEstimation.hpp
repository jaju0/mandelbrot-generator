#ifndef MANDELBROT_COLORING_STRATEGY_BASIC_DISTANCE_ESTIMATION_HPP_
#define MANDELBROT_COLORING_STRATEGY_BASIC_DISTANCE_ESTIMATION_HPP_

#include "../ColoringStrategy.hpp"

namespace mandelbrot {

class ColoringStrategyBasicDistanceEstimation : public ColoringStrategy
{
public:
    boost::gil::rgb8_pixel_t computePixel(Calculator::Result& result, uint32_t x, uint32_t y) override;
};

}

#endif