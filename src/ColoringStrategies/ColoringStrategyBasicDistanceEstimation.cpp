#include "ColoringStrategyBasicDistanceEstimation.hpp"

namespace mandelbrot {

boost::gil::rgb8_pixel_t ColoringStrategyBasicDistanceEstimation::computePixel(Calculator::Result& result, uint32_t x, uint32_t y)
{
    auto& calcPixel = result.pixels[result.params.pixelsX * y + x];

    if(calcPixel.iterations == result.params.maxIterations)
        return {0, 0, 0};
    
    CalcType pixelSize = (result.params.maxReal-result.params.minReal) / static_cast<CalcType>(result.params.pixelsX);
    CalcType thickness = 0.08L;

    if(calcPixel.distanceEstimator < thickness*pixelSize)
        return {255, 0, 0};
    
    return {0, 0, 0};
}

}