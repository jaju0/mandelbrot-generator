#ifndef MANDELBROT_GENERATING_STRATEGY_RANDOM_ZOOM_HPP_
#define MANDELBROT_GENERATING_STRATEGY_RANDOM_ZOOM_HPP_

#include <vector>
#include <optional>
#include <boost/numeric/ublas/vector.hpp>

#include "../GeneratingStrategy.hpp"

namespace mandelbrot {

class GeneratingStrategyRandomZoom : public GeneratingStrategy
{
public:
    GeneratingStrategyRandomZoom(boost::numeric::ublas::vector<uint32_t> imageSize, uint32_t maxIterations);
    virtual void initialize() override;
    virtual std::error_code generate() override;

protected:
    std::optional<boost::numeric::ublas::vector<CalcType>> getRandomZoomCoordinate();
    std::vector<boost::numeric::ublas::vector<CalcType>> computeAllZoomCoordinates();
    bool isMandelbrotCoordOnEdge(uint32_t x, uint32_t y);

    CalcType m_currentZoomFactor;
    boost::numeric::ublas::vector<CalcType> m_currentZoomCenter;
};

}

#endif