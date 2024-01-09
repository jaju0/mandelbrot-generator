#include "error.hpp"

namespace mandelbrot {

category_impl& category_impl::getInst()
{
    static category_impl instance;
    return instance;
}

const char* category_impl::name() const noexcept
{
    return "mandelbrot";
}

std::string category_impl::message(int ev) const
{
    switch (static_cast<error>(ev))
    {
    case error::precision_issue:
        return "precision issue";
    default:
        return "unknown error";
    }

}

std::error_code make_error_code(error e)
{
    return std::error_code(
        static_cast<int>(e),
        category_impl::getInst()
    );
}

}