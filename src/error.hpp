#ifndef MANDELBROT_ERROR_HPP_
#define MANDELBROT_ERROR_HPP_

#include <string>
#include <system_error>

namespace mandelbrot {

enum class error
{
    precision_issue = 1,
    no_random_zoom_coordinate,
};

class category_impl : public std::error_category
{
public:
    static category_impl& getInst();
    virtual const char* name() const noexcept;
    virtual std::string message(int ev) const;
};

std::error_code make_error_code(mandelbrot::error e);

}

namespace std {

template<>
struct is_error_code_enum<mandelbrot::error> : public true_type {};

}

#endif