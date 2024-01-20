#ifndef MANDELBROT_MYSQL_QUERY_TYPES_HPP_
#define MANDELBROT_MYSQL_QUERY_TYPES_HPP_

#include "Calculator.hpp"

namespace mandelbrot {

struct ImageInsertionParameters
{
    Calculator::Result result;
    std::string uuid;

    ImageInsertionParameters(const Calculator::Result& result, const std::string& uuid);
};

}

#endif