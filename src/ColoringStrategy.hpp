#ifndef MANDELBROT_COLORING_STRATEGY_HPP_
#define MANDELBROT_COLORING_STRATEGY_HPP_

#include <boost/gil.hpp>

#include "Calculator.hpp"

namespace mandelbrot {

class ColoringStrategy
{
public:
    virtual boost::gil::rgb8_pixel_t computePixel(Calculator::Result& result, uint32_t x, uint32_t y) = 0;
};

}

#endif