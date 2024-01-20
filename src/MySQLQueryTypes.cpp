#include "MySQLQueryTypes.hpp"

namespace mandelbrot {

ImageInsertionParameters::ImageInsertionParameters(const Calculator::Result& result, const std::string& uuid)
    : result(result)
    , uuid(uuid)
{}

}