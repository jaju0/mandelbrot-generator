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
     * - <1> (long double) distanceEstimator
     */
    virtual std::tuple<uint32_t, long double> iterate(std::complex<long double> c) = 0;
};

}

#endif