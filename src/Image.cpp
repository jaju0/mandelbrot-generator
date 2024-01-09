#include "Image.hpp"

namespace mandelbrot {

Image::Image(Calculator::Result& result, uint32_t width, uint32_t height, std::shared_ptr<ColoringStrategy> pColoringStrategy)
    : m_width(width)
    , m_height(height)
    , m_pColoringStrategy(pColoringStrategy)
    , m_gilImage(result.params.pixelsX, result.params.pixelsY)
    , m_gilImageSized(m_width, m_height)
    , m_gilView(boost::gil::view(m_gilImage))
{
    create(result);
}

void Image::saveAsJPEG(const std::string& file)
{
    boost::gil::image_write_info<boost::gil::jpeg_tag> writeInfo;
    writeInfo._quality = 99;

    boost::gil::write_view(file, boost::gil::const_view(m_gilImageSized), writeInfo);
}

void Image::create(Calculator::Result& result)
{
    for(uint32_t y = 0; y < result.params.pixelsY; ++y)
        for(uint32_t x = 0; x < result.params.pixelsX; ++x)
            m_gilView(x, y) = m_pColoringStrategy->computePixel(result, x, y);

    boost::gil::resize_view(boost::gil::const_view(m_gilImage), boost::gil::view(m_gilImageSized), boost::gil::bilinear_sampler());
}

}