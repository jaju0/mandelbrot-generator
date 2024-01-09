#include "EscapeTimeAlgoIterationOnly.hpp"

namespace mandelbrot {

EscapeTimeAlgoIterationOnly::EscapeTimeAlgoIterationOnly()
    : m_maxIterations(0)
{}

std::tuple<uint32_t, long double> EscapeTimeAlgoIterationOnly::iterate(std::complex<long double> c)
{
    std::complex<long double> z, z2;
    std::uint32_t iterations = 0;

    while(z2.real() + z2.imag() <= 4.0L && iterations < m_maxIterations)
    {
        z.imag(2.0L * z.real() * z.imag() + c.imag());
        z.real(z2.real() - z2.imag() + c.real());
        z2.real(z.real() * z.real());
        z2.imag(z.imag() * z.imag());
        iterations++;
    }

    return { iterations, 0.0L };
}

void EscapeTimeAlgoIterationOnly::setMaxIterations(uint32_t maxIterations)
{
    m_maxIterations = maxIterations;
}

}