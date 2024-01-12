#ifndef MANDELBROT_HELPER_HPP_
#define MANDELBROT_HELPER_HPP_

namespace mandelbrot { namespace helper {

template<typename T>
T map(T x, T inMin, T inMax, T outMin, T outMax)
{
    return (x-inMin) / (inMax-inMin) * (outMax-outMin) + outMin;
}

}}

#endif