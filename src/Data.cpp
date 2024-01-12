#include <sstream>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/lexical_cast.hpp>

#include "Data.hpp"

namespace mandelbrot {

Data::Data(Calculator::Result& result)
    : m_size(2)
    , m_maxIterations(result.params.maxIterations)
    , m_iterations(result.params.pixelsX * result.params.pixelsY)
    , m_distanceEstimators(result.params.pixelsX * result.params.pixelsY)
{
    m_size(0) = result.params.pixelsX;
    m_size(1) = result.params.pixelsY;

    for(uint32_t x = 0; x < result.params.pixelsX; ++x)
    {
        for(uint32_t y = 0; y < result.params.pixelsY; ++y)
        {
            m_iterations[result.params.pixelsX * y + x] = result.pixels.at(result.params.pixelsX * y + x).iterations;
            m_distanceEstimators[result.params.pixelsX * y + x] = result.pixels.at(result.params.pixelsX * y + x).distanceEstimator;
        }
    }
}

void Data::saveToFile(const std::string& file)
{
    std::stringstream stream;

    // write mandelbrot parameters
    stream.write(reinterpret_cast<char*>(&m_size(0)), sizeof(uint32_t));
    stream.write(reinterpret_cast<char*>(&m_size(1)), sizeof(uint32_t));
    stream.write(reinterpret_cast<char*>(&m_maxIterations), sizeof(uint32_t));

    // write data
    stream.write(reinterpret_cast<char*>(m_iterations.data()), m_iterations.size() * sizeof(m_iterations.at(0)));
    stream.write(reinterpret_cast<char*>(m_distanceEstimators.data()), m_distanceEstimators.size() * sizeof(m_distanceEstimators.at(0)));

    // compress data via gzip
    boost::iostreams::filtering_streambuf<boost::iostreams::input> filteringStreamBuffer;
    filteringStreamBuffer.push(boost::iostreams::gzip_compressor());
    filteringStreamBuffer.push(stream);

    // write the gzip file
    boost::filesystem::ofstream outputStream(file, std::ios_base::binary);
    boost::iostreams::copy(filteringStreamBuffer, outputStream);
    outputStream.close();
}

}