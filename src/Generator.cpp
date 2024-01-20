#include <iostream>

#include "Generator.hpp"

namespace mandelbrot {

Generator::Generator(uint32_t numberOfThreads, std::shared_ptr<GeneratingStrategy> pGeneratingStrategy)
    : m_numberOfThreads(numberOfThreads)
    , m_availableThreads(m_numberOfThreads)
    , m_shouldTerminate(false)
    , m_pGeneratingStrategy(pGeneratingStrategy)
{
    m_pCalculator = std::make_shared<Calculator>(m_ioc, m_numberOfThreads);
    m_pGeneratingStrategy->setCalculator(m_pCalculator);
    m_pGeneratingStrategy->setExecutionFunction(std::bind(&Generator::execute, this));
    m_pGeneratingStrategy->initialize();

    for(uint32_t i = 0; i < m_numberOfThreads; ++i)
        m_threads.emplace_back(&Generator::threadLoop, this);
}

void Generator::next()
{
    m_pGeneratingStrategy->generate();
}

Calculator::Result& Generator::getResult()
{
    return m_pCalculator->getResult();
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

void Generator::execute()
{
    m_pCalculator->postTasks();
    m_cvIocExecution.notify_all();
    waitForResult();
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