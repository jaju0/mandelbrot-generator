#include "GeneratingStrategy.hpp"

namespace mandelbrot {

GeneratingStrategy::GeneratingStrategy(boost::numeric::ublas::vector<uint32_t> imageSize, uint32_t maxIterations)
    : m_imageSize(imageSize)
    , m_maxIterations(maxIterations)
{}

void GeneratingStrategy::setCalculator(std::shared_ptr<Calculator> pCalculator)
{
    m_pCalculator = pCalculator;
}

void GeneratingStrategy::setExecutionFunction(std::function<void()> execute)
{
    m_execute = execute;
}

std::error_code GeneratingStrategy::createImage(boost::numeric::ublas::vector<CalcType> zoomCenter, CalcType zoomFactor)
{
    CalcType centerX = zoomCenter(0);
    CalcType centerY = zoomCenter(1);

    CalcType aspectRatio = static_cast<CalcType>(m_imageSize(0)) / static_cast<CalcType>(m_imageSize(1));

    CalcType halfY = 0.5 * 2.24 * zoomFactor;
    CalcType halfX = aspectRatio * halfY;

    CalcType minImag = centerY - halfY;
    CalcType maxImag = centerY + halfY;
    CalcType minReal = centerX - halfX;
    CalcType maxReal = centerX + halfX;

    if(!isPrecisionValid(minImag, maxImag, m_imageSize(1)) || !isPrecisionValid(minReal, maxReal, m_imageSize(0)))
        return mandelbrot::error::precision_issue;

    m_pCalculator->setParameters(Calculator::Parameters {
        m_imageSize(0),
        m_imageSize(1),
        minReal,
        maxReal,
        minImag,
        maxImag,
        m_maxIterations,
    });

    m_execute();

    return std::error_code();
}

bool GeneratingStrategy::isPrecisionValid(CalcType min, CalcType max, uint32_t size)
{
    CalcType diff = max-min;
    CalcType last = 0.0L;
    for(uint32_t i = 0; i < size; ++i)
    {
        CalcType current = min + static_cast<CalcType>(i) * diff / static_cast<CalcType>(size);
        if(current == last)
            return false;

        last = current;
    }

    return true;
}

}