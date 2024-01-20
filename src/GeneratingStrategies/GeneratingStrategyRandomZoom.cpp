#include <random>

#include "../helper.hpp"
#include "GeneratingStrategyRandomZoom.hpp"
#include "../EscapeTimeAlgos/EscapeTimeAlgoDistanceEstimation.hpp"

namespace mandelbrot {

GeneratingStrategyRandomZoom::GeneratingStrategyRandomZoom(boost::numeric::ublas::vector<uint32_t> imageSize, uint32_t maxIterations)
    : GeneratingStrategy(imageSize, maxIterations)
    , m_currentZoomFactor(1.3L)
    , m_currentZoomCenter(2)
{
    m_currentZoomCenter(0) = 0.0L;
    m_currentZoomCenter(1) = 0.0L;
}

void GeneratingStrategyRandomZoom::initialize()
{
    auto pEscapeTimeAlgo = std::make_shared<EscapeTimeAlgoDistanceEstimation>();
    pEscapeTimeAlgo->setMaxIterations(m_maxIterations);

    m_pCalculator->setEscapeTimeAlgo(pEscapeTimeAlgo);
}

std::error_code GeneratingStrategyRandomZoom::generate()
{
    using Vector = boost::numeric::ublas::vector<long double>;

    auto resetParams = [this] {
        m_currentZoomFactor = 1.3L;
        m_currentZoomCenter(0) = 0.0L;
        m_currentZoomCenter(1) = 0.0L;
    };

    auto updateParams = [this] (Vector& randomZoomCoordinate) {
        m_currentZoomFactor *= 0.3L;
        m_currentZoomCenter = randomZoomCoordinate;
    };

    std::error_code ec = createImage(m_currentZoomCenter, m_currentZoomFactor);
    if(ec == error::precision_issue)
    {
        resetParams();
        return ec;
    }
    if(ec)
        throw ec;

    auto randomZoomCoordinate = getRandomZoomCoordinate();
    if(randomZoomCoordinate.has_value())
    {
        updateParams(randomZoomCoordinate.value());
        return error::no_random_zoom_coordinate;
    }

    resetParams();

    return std::error_code();
}

std::optional<boost::numeric::ublas::vector<long double>> GeneratingStrategyRandomZoom::getRandomZoomCoordinate()
{
    using Vector = boost::numeric::ublas::vector<long double>;
    std::vector<Vector> zoomCoordinates = computeAllZoomCoordinates();
    if(zoomCoordinates.empty())
        return std::nullopt;

    static std::random_device rdev;
    static std::mt19937 rgen(rdev());
    std::uniform_int_distribution<uint32_t> idist(0, zoomCoordinates.size()-1);
    return zoomCoordinates[idist(rgen)];
}

std::vector<boost::numeric::ublas::vector<long double>> GeneratingStrategyRandomZoom::computeAllZoomCoordinates()
{
    using Vector = boost::numeric::ublas::vector<long double>;

    auto& result = m_pCalculator->getResult();

    std::vector<Vector> coordinates;
    for(uint32_t x = 0; x < result.params.pixelsX; ++x)
    {
        for(uint32_t y = 0; y < result.params.pixelsY; ++y)
        {
            Calculator::Pixel& pixel = result.pixels[result.params.pixelsX * y + x];
            if(pixel.iterations != result.params.maxIterations || !isMandelbrotCoordOnEdge(x, y))
                continue;

            Vector mappedCoordinate(2);
            mappedCoordinate(0) = helper::map(static_cast<long double>(x), 0.0L, static_cast<long double>(result.params.pixelsX), result.params.minReal, result.params.maxReal);
            mappedCoordinate(1) = helper::map(static_cast<long double>(y), 0.0L, static_cast<long double>(result.params.pixelsY), result.params.minImag, result.params.maxImag);
            coordinates.push_back(mappedCoordinate);
        }
    }

    return coordinates;
}

bool GeneratingStrategyRandomZoom::isMandelbrotCoordOnEdge(uint32_t x, uint32_t y)
{
    auto& result = m_pCalculator->getResult();

    int32_t signedX = static_cast<int32_t>(x);
    int32_t signedY = static_cast<int32_t>(y);

    uint32_t leftIndex = static_cast<uint32_t>(std::max(signedX-1, 0));
    uint32_t rightIndex = static_cast<uint32_t>(std::min(signedX+1, static_cast<int32_t>(result.params.pixelsX-1)));
    uint32_t topIndex = static_cast<uint32_t>(std::min(signedY+1, static_cast<int32_t>(result.params.pixelsY-1)));
    uint32_t bottomIndex = static_cast<uint32_t>(std::max(signedY-1, 0));

    std::vector<Calculator::Pixel*> neighbours;
    neighbours.push_back(&result.pixels[result.params.pixelsX * topIndex + x]);
    neighbours.push_back(&result.pixels[result.params.pixelsX * bottomIndex + x]);
    neighbours.push_back(&result.pixels[result.params.pixelsX * y + leftIndex]);
    neighbours.push_back(&result.pixels[result.params.pixelsX * y + rightIndex]);
    neighbours.push_back(&result.pixels[result.params.pixelsX * topIndex + leftIndex]);
    neighbours.push_back(&result.pixels[result.params.pixelsX * bottomIndex + leftIndex]);
    neighbours.push_back(&result.pixels[result.params.pixelsX * topIndex + rightIndex]);
    neighbours.push_back(&result.pixels[result.params.pixelsX * bottomIndex + rightIndex]);
    
    for(auto neighbour : neighbours)
    {
        if(neighbour->iterations < result.params.maxIterations)
            return true;
    }

    return false;
}

}