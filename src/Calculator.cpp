#include "helper.hpp"
#include "Calculator.hpp"

namespace mandelbrot {


Calculator::Pixel::Pixel()
    : iterations(0)
    , distanceEstimator(0.0)
{}

Calculator::Pixel::Pixel(uint32_t iterations, CalcType distanceEstimator)
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

Calculator::Parameters::Parameters(uint32_t pixelsX, uint32_t pixelsY, CalcType minReal, CalcType maxReal, CalcType minImag, CalcType maxImag, uint32_t maxIterations)
    : pixelsX(pixelsX)
    , pixelsY(pixelsY)
    , minReal(minReal)
    , maxReal(maxReal)
    , minImag(minImag)
    , maxImag(maxImag)
    , maxIterations(maxIterations)
{}

Calculator::Calculator(boost::asio::io_context& ioc, uint32_t numberOfTasks)
    : m_ioc(ioc)
    , m_numberOfTasks(numberOfTasks)
{}

void Calculator::setEscapeTimeAlgo(std::shared_ptr<EscapeTimeAlgo> pEscapeTimeAlgo)
{
    m_pEscapeTimeAlgo = pEscapeTimeAlgo;
}

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
            CalcType cx = helper::map<CalcType>(static_cast<CalcType>(px), 0.0L, static_cast<CalcType>(m_params.pixelsX), m_params.minReal, m_params.maxReal);
            CalcType cy = helper::map<CalcType>(static_cast<CalcType>(py), 0.0L, static_cast<CalcType>(m_params.pixelsY), m_params.minImag, m_params.maxImag);
            std::complex<CalcType> c { cx, cy };

            auto iterationResult = m_pEscapeTimeAlgo->iterate(c);
            uint32_t iterations = std::get<0>(iterationResult);
            CalcType distanceEstimator = std::get<1>(iterationResult);

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


}