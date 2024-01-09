#ifndef MANDELBROT_IMAGE_HPP_
#define MANDELBROT_IMAGE_HPP_

#include <string>
#include <vector>
#include <memory>
#include <boost/gil.hpp>
#include <boost/gil/io/write_view.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>
#include <boost/gil/extension/numeric/resample.hpp>

#include "Calculator.hpp"
#include "ColoringStrategy.hpp"

namespace mandelbrot {

class Image
{
public:
    Image(Calculator::Result& result, uint32_t width, uint32_t height, std::shared_ptr<ColoringStrategy> pColoringStrategy);
    void saveAsJPEG(const std::string& file);

private:
    void create(Calculator::Result& result);

    uint32_t m_width;
    uint32_t m_height;
    std::shared_ptr<ColoringStrategy> m_pColoringStrategy;
    boost::gil::rgb8_image_t m_gilImage;
    boost::gil::rgb8_image_t m_gilImageSized;
    boost::gil::rgb8_view_t m_gilView;
};

}

#endif