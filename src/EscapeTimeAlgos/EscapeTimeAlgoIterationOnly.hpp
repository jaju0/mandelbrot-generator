#ifndef MANDELBROT_ESCAPE_TIME_ALGO_ITERATION_ONLY_HPP_
#define MANDELBROT_ESCAPE_TIME_ALGO_ITERATION_ONLY_HPP_

#include "../types.hpp"
#include "../EscapeTimeAlgo.hpp"

namespace mandelbrot {

class EscapeTimeAlgoIterationOnly : public EscapeTimeAlgo
{
public:
    EscapeTimeAlgoIterationOnly();
    virtual std::tuple<uint32_t, CalcType> iterate(std::complex<CalcType> c) override;

    void setMaxIterations(uint32_t maxIterations);

private:
    uint32_t m_maxIterations;
};

}

#endif