#include "Calculator.hpp"

namespace mandelbrot {


Calculator::Pixel::Pixel()
    : iterations(0)
    , distanceEstimator(0.0)
{}

Calculator::Pixel::Pixel(uint32_t iterations, long double distanceEstimator)
    : iterations(iterations)
    , distanceEstimator(distanceEstimator)
{}

Calculator::Parameters::Parameters()
    : pixelsX(0)
    , pixelsY(0)
    , minReal(0)
    , maxReal(0)
    , minImag(0)
    , maxImag(0)
    , maxIterations(0)
{}

Calculator::Parameters::Parameters(uint32_t pixelsX, uint32_t pixelsY, long double minReal, long double maxReal, long double minImag, long double maxImag, uint32_t maxIterations)
    : pixelsX(pixelsX)
    , pixelsY(pixelsY)
    , minReal(minReal)
    , maxReal(maxReal)
    , minImag(minImag)
    , maxImag(maxImag)
    , maxIterations(maxIterations)
{}

Calculator::Calculator(boost::asio::io_context& ioc, uint32_t numberOfTasks, std::shared_ptr<EscapeTimeAlgo> pEscapeTimeAlgo)
    : m_ioc(ioc)
    , m_numberOfTasks(numberOfTasks)
    , m_pEscapeTimeAlgo(pEscapeTimeAlgo)
{}

void Calculator::setParameters(Parameters params)
{
    m_params = params;

    if(m_numberOfTasks > m_params.pixelsY)
        m_numberOfTasks = m_params.pixelsY;

    m_result.params = m_params;
}

void Calculator::postTasks()
{
    // clear the pixel vector and resize it to the current parameter metrics
    m_result.pixels.clear();
    m_result.pixels.resize(m_params.pixelsX * m_params.pixelsY);

    // create and post tasks
    uint32_t yPixelsPerTask = m_params.pixelsY / m_numberOfTasks;
    for(uint32_t i = 0; i < m_numberOfTasks; ++i)
    {
        uint32_t yCoordFrom = i * yPixelsPerTask;
        uint32_t yCoordTo = yCoordFrom + yPixelsPerTask;

        bool isLastTask = i == m_numberOfTasks-1;
        if(isLastTask)
            yCoordTo = m_params.pixelsY;

        m_ioc.post(std::bind(&Calculator::task, this, yCoordFrom, yCoordTo));
    }
}

Calculator::Result& Calculator::getResult()
{
    return m_result;
}

/**
 * Task will calculate from 'yCoordFrom' to 'yCoordTo' excluded.
 */
void Calculator::task(uint32_t yCoordFrom, uint32_t yCoordTo)
{
    std::vector<Pixel> pixels(m_params.pixelsX * (yCoordTo-yCoordFrom));

    for(uint32_t py = yCoordFrom; py < yCoordTo; ++py)
    {
        uint32_t relY = py-yCoordFrom;
        for(uint32_t px = 0; px < m_params.pixelsX; ++px)
        {
            auto mappedCoords = mapCoordinates(px, py);
            long double cx = std::get<0>(mappedCoords);
            long double cy = std::get<1>(mappedCoords);
            std::complex<long double> c { cx, cy };

            auto iterationResult = m_pEscapeTimeAlgo->iterate(c);
            uint32_t iterations = std::get<0>(iterationResult);
            long double distanceEstimator = std::get<1>(iterationResult);

            pixels[m_params.pixelsX * relY + px] = Pixel { iterations, distanceEstimator };
        }
    }

    task_copyPixelsToResult(yCoordFrom, yCoordTo, pixels);
}

void Calculator::task_copyPixelsToResult(uint32_t yCoordFrom, uint32_t yCoordTo, std::vector<Pixel>& pixels)
{
    std::lock_guard<std::mutex> lock(m_resultMutex);
    for(uint32_t py = yCoordFrom; py < yCoordTo; ++py)
    {
        uint32_t relY = py-yCoordFrom;
        for(uint32_t px = 0; px < m_params.pixelsX; ++px)
            m_result.pixels[m_params.pixelsX * py + px] = std::move(pixels[m_params.pixelsX * relY + px]);
    }
}

std::tuple<long double, long double> Calculator::mapCoordinates(uint32_t px, uint32_t py)
{
    long double xRatio = static_cast<long double>(px) / static_cast<long double>(m_params.pixelsX);
    long double x = xRatio * (m_params.maxReal-m_params.minReal) + m_params.minReal;

    long double yRatio = static_cast<long double>(py) / static_cast<long double>(m_params.pixelsY);
    long double y = -(yRatio * (m_params.maxImag-m_params.minImag) + m_params.minImag);

    return {x, y};
}


}