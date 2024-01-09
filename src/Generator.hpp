#ifndef MANDELBROT_GENERATOR_HPP_
#define MANDELBROT_GENERATOR_HPP_

#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>

#include "error.hpp"
#include "Calculator.hpp"
#include "EscapeTimeAlgo.hpp"

namespace mandelbrot {

class Generator
{
public:
    Generator(uint32_t numberOfThreads, uint32_t imageWidth, uint32_t imageHeight, uint32_t maxIterations, std::shared_ptr<EscapeTimeAlgo> pEscapeTimeAlgo);
    void next();
    void exit();

    Calculator::Result& getResult();

private:
    void waitForResult();
    void threadLoop();

    std::error_code createImage(long double centerX, long double centerY, long double zoomFactor, uint32_t iterations);
    bool isPrecisionValid(long double min, long double max, uint32_t size);

    boost::asio::io_context m_ioc;

    uint32_t m_numberOfThreads;
    std::atomic_uint32_t m_availableThreads;
    std::vector<std::thread> m_threads;
    uint32_t m_imageWidth;
    uint32_t m_imageHeight;
    uint32_t m_maxIterations;
    bool m_shouldTerminate;

    std::mutex m_cvIocExecutionMutex;
    std::condition_variable m_cvIocExecution;

    std::mutex m_cvWorkDoneMutex;
    std::condition_variable m_cvWorkDone;

    Calculator m_calculator;
};

}

#endif