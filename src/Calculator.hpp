#ifndef MANDELBROT_CALCULATOR_HPP_
#define MANDELBROT_CALCULATOR_HPP_

#include <cstdint>
#include <vector>
#include <tuple>
#include <memory>
#include <mutex>
#include <boost/asio/io_context.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "EscapeTimeAlgo.hpp"

namespace mandelbrot {

class Calculator
{
public:
    struct Pixel
    {
        uint32_t iterations;
        long double distanceEstimator;
        Pixel();
        Pixel(uint32_t iterations, long double distanceEstimator);
    };

    struct Parameters
    {
        uint32_t pixelsX;
        uint32_t pixelsY;
        long double minReal;
        long double maxReal;
        long double minImag;
        long double maxImag;
        uint32_t maxIterations;
        Parameters();
        Parameters(uint32_t pixelsX, uint32_t pixelsY, long double minReal, long double maxReal, long double minImag, long double maxImag, uint32_t maxIterations);
    };

    struct Result
    {
        Parameters params;
        std::vector<Pixel> pixels;
    };

public:
    Calculator(boost::asio::io_context& ioc, uint32_t numberOfTasks, std::shared_ptr<EscapeTimeAlgo> pEscapeTimeAlgo);
    void setParameters(Parameters params);
    void postTasks();
    Result& getResult();

private:
    void task(uint32_t yCoordFrom, uint32_t yCoordTo);
    void task_copyPixelsToResult(uint32_t yCoordFrom, uint32_t yCoordTo, std::vector<Pixel>& pixels);


    boost::numeric::ublas::vector<long double> mapCoordinates(uint32_t px, uint32_t py);

    boost::asio::io_context& m_ioc;
    uint32_t m_numberOfTasks;
    std::shared_ptr<EscapeTimeAlgo> m_pEscapeTimeAlgo;

    Parameters m_params;
    std::mutex m_resultMutex;
    Result m_result;
};

}

#endif