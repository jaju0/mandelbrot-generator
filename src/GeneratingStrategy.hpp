#ifndef MANDELBROT_GENERATING_STRATEGY_HPP_
#define MANDELBROT_GENERATING_STRATEGY_HPP_

#include <memory>
#include <functional>
#include <boost/numeric/ublas/vector.hpp>

#include "error.hpp"
#include "Calculator.hpp"

namespace mandelbrot {

class GeneratingStrategy
{
public:
    GeneratingStrategy(boost::numeric::ublas::vector<uint32_t> imageSize, uint32_t maxIterations);
    virtual std::error_code generate() = 0;

    void setCalculator(std::shared_ptr<Calculator> pCalculator);
    void setExecutionFunction(std::function<void()> execute);

protected:
    std::shared_ptr<Calculator> m_pCalculator;
    std::function<void()> m_execute;
    boost::numeric::ublas::vector<uint32_t> m_imageSize;
    uint32_t m_maxIterations;

    virtual std::error_code createImage(boost::numeric::ublas::vector<long double> zoomCenter, long double zoomFactor);
    bool isPrecisionValid(long double min, long double max, uint32_t size);
};

}

#endif