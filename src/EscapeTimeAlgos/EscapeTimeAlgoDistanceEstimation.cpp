#include "EscapeTimeAlgoDistanceEstimation.hpp"

namespace mandelbrot {

EscapeTimeAlgoDistanceEstimation::EscapeTimeAlgoDistanceEstimation()
    : m_maxIterations(0)
{}

std::tuple<uint32_t, long double> EscapeTimeAlgoDistanceEstimation::iterate(std::complex<long double> c)
{
    std::complex<long double> z, z2;
    std::complex<long double> dz(1.0L, 0.0L);
    std::uint32_t iterations = 0;

    while(z2.real() + z2.imag() <= 4.0L && iterations < m_maxIterations)
    {
        z.imag(2.0L * z.real() * z.imag() + c.imag());
        z.real(z2.real() - z2.imag() + c.real());
        z2.real(z.real() * z.real());
        z2.imag(z.imag() * z.imag());
        dz = 2.0L * z * dz + 1.0L;
        iterations++;
    }

    auto modZ = std::abs(z);
    auto moddZ = std::abs(dz);
    long double distanceEstimator = 2.0L * modZ * std::log(modZ) / moddZ;

    return { iterations, distanceEstimator };
}

void EscapeTimeAlgoDistanceEstimation::setMaxIterations(uint32_t maxIterations)
{
    m_maxIterations = maxIterations;
}

}