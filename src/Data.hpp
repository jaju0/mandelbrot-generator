#ifndef DATA_HPP_
#define DATA_HPP_

#include <string>
#include <boost/numeric/ublas/vector.hpp>

#include "Calculator.hpp"

namespace mandelbrot {

class Data
{
public:
    Data(Calculator::Result& result);
    void saveToFile(const std::string& file);

private:
    boost::numeric::ublas::vector<uint32_t> m_size;
    uint32_t m_maxIterations;
    std::vector<uint32_t> m_iterations;
    std::vector<long double> m_distanceEstimators;
};

}

#endif