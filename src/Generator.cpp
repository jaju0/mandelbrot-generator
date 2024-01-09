#include <iostream>

#include "Generator.hpp"

namespace mandelbrot {

Generator::Generator(uint32_t numberOfThreads, uint32_t imageWidth, uint32_t imageHeight, uint32_t maxIterations, std::shared_ptr<EscapeTimeAlgo> pEscapeTimeAlgo)
    : m_numberOfThreads(numberOfThreads)
    , m_availableThreads(m_numberOfThreads)
    , m_imageWidth(imageWidth)
    , m_imageHeight(imageHeight)
    , m_maxIterations(maxIterations)
    , m_shouldTerminate(false)
    , m_calculator(m_ioc, m_numberOfThreads, pEscapeTimeAlgo)
{
    for(uint32_t i = 0; i < m_numberOfThreads; ++i)
        m_threads.emplace_back(&Generator::threadLoop, this);
}

void Generator::next()
{
    // TODO
    std::error_code ec = createImage(0.0L, 0.0L, 1.0L, m_maxIterations);
    if(ec == error::precision_issue)
    {
        // TODO: reset parameters
    }
    if(ec)
        throw ec;
}

Calculator::Result& Generator::getResult()
{
    return m_calculator.getResult();
}

std::error_code Generator::createImage(long double centerX, long double centerY, long double zoomFactor, uint32_t iterations)
{
    long double aspectRatio = static_cast<long double>(m_imageWidth) / static_cast<long double>(m_imageHeight);

    long double halfY = 0.5 * 2.24 * zoomFactor;
    long double halfX = aspectRatio * halfY;

    long double minImag = centerY - halfY;
    long double maxImag = centerY + halfY;
    long double minReal = centerX - halfX;
    long double maxReal = centerX + halfX;

    if(!isPrecisionValid(minImag, maxImag, m_imageHeight) || !isPrecisionValid(minReal, maxReal, m_imageWidth))
        return mandelbrot::error::precision_issue;

    m_calculator.setParameters(Calculator::Parameters {
        m_imageWidth,
        m_imageHeight,
        minReal,
        maxReal,
        minImag,
        maxImag,
        iterations,
    });

    m_calculator.postTasks();
    m_cvIocExecution.notify_all();
    waitForResult();

    return std::error_code();
}

bool Generator::isPrecisionValid(long double min, long double max, uint32_t size)
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

void Generator::exit()
{
    std::cout << "shutting down generator" << std::endl;
    m_shouldTerminate = true;
    m_cvIocExecution.notify_all();

    for(auto& thread : m_threads)
        thread.join();
}

void Generator::waitForResult()
{
    std::unique_lock lk(m_cvWorkDoneMutex);
    m_cvWorkDone.wait(lk);
}

void Generator::threadLoop()
{
    while(true)
    {
        {
            std::unique_lock lk(m_cvIocExecutionMutex);
            m_cvIocExecution.wait(lk);

            if(m_shouldTerminate)
                return;
        }

        m_availableThreads--;
        if(m_ioc.stopped())
            m_ioc.restart();
        else
            m_ioc.run();
        m_availableThreads++;

        if(m_availableThreads == m_numberOfThreads)
            m_cvWorkDone.notify_all();
        
        if(m_shouldTerminate)
            return;
    }
}


}