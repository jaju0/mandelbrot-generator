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

std::error_code GeneratingStrategy::createImage(boost::numeric::ublas::vector<long double> zoomCenter, long double zoomFactor)
{
    long double centerX = zoomCenter(0);
    long double centerY = zoomCenter(1);

    long double aspectRatio = static_cast<long double>(m_imageSize(0)) / static_cast<long double>(m_imageSize(1));

    long double halfY = 0.5 * 2.24 * zoomFactor;
    long double halfX = aspectRatio * halfY;

    long double minImag = centerY - halfY;
    long double maxImag = centerY + halfY;
    long double minReal = centerX - halfX;
    long double maxReal = centerX + halfX;

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

bool GeneratingStrategy::isPrecisionValid(long double min, long double max, uint32_t size)
{
    long double diff = max-min;
    long double last = 0.0L;
    for(uint32_t i = 0; i < size; ++i)
    {
        long double current = min + static_cast<long double>(i) * diff / static_cast<long double>(size);
        if(current == last)
            return false;

        last = current;
    }

    return true;
}

}