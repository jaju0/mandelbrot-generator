#ifndef MANDELBROT_ESCAPE_TIME_ALGO_HPP_
#define MANDELBROT_ESCAPE_TIME_ALGO_HPP_

#include <complex>
#include <tuple>

namespace mandelbrot {

class EscapeTimeAlgo
{
public:
    /**
     * Returns:
     * - <0> (uint32_t) iterations
     * - <1> (CalcType) distanceEstimator
     */
    virtual std::tuple<uint32_t, CalcType> iterate(std::complex<CalcType> c) = 0;
};

}

#endif