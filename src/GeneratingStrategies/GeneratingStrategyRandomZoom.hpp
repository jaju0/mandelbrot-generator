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
    virtual std::error_code generate() override;

protected:
    std::optional<boost::numeric::ublas::vector<long double>> getRandomZoomCoordinate();
    std::vector<boost::numeric::ublas::vector<long double>> computeAllZoomCoordinates();
    bool isMandelbrotCoordOnEdge(uint32_t x, uint32_t y);

    long double m_currentZoomFactor;
    boost::numeric::ublas::vector<long double> m_currentZoomCenter;
};

}

#endif