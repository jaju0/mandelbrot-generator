#ifndef MANDELBROT_GENERATOR_HPP_
#define MANDELBROT_GENERATOR_HPP_

#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>

#include "error.hpp"
#include "Calculator.hpp"
#include "GeneratingStrategy.hpp"
#include "EscapeTimeAlgo.hpp"

namespace mandelbrot {

class Generator
{
public:
    Generator(uint32_t numberOfThreads, std::shared_ptr<GeneratingStrategy> pGeneratingStrategy, std::shared_ptr<EscapeTimeAlgo> pEscapeTimeAlgo);
    void next();
    void exit();

    Calculator::Result& getResult();

private:
    void waitForResult();
    void execute();
    void threadLoop();

    boost::asio::io_context m_ioc;

    uint32_t m_numberOfThreads;
    std::atomic_uint32_t m_availableThreads;
    std::vector<std::thread> m_threads;
    bool m_shouldTerminate;

    std::mutex m_cvIocExecutionMutex;
    std::condition_variable m_cvIocExecution;

    std::mutex m_cvWorkDoneMutex;
    std::condition_variable m_cvWorkDone;

    std::shared_ptr<GeneratingStrategy> m_pGeneratingStrategy;
    std::shared_ptr<Calculator> m_pCalculator;
};

}

#endif