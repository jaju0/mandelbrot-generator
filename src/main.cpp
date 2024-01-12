#include <csignal>
#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Generator.hpp"
#include "Image.hpp"
#include "Data.hpp"
#include "EscapeTimeAlgos/EscapeTimeAlgoDistanceEstimation.hpp"
#include "GeneratingStrategies/GeneratingStrategyRandomZoom.hpp"
#include "ColoringStrategies/ColoringStrategyBasicDistanceEstimation.hpp"

bool EXIT = false;

void signal_sigint(int signum)
{
    boost::ignore_unused(signum);
    EXIT = true;
}

int main()
{
    using VectorU32 = boost::numeric::ublas::vector<uint32_t>;

    std::cout << "press ctrl+c to exit..." << std::endl;
    signal(SIGINT, signal_sigint);

    uint32_t maxIterations = 1000;
    VectorU32 imageSize(2);
    imageSize(0) = 1920;
    imageSize(1) = 1080;

    auto escapeTimeAlgo = std::make_shared<mandelbrot::EscapeTimeAlgoDistanceEstimation>();
    auto generatingStrategy = std::make_shared<mandelbrot::GeneratingStrategyRandomZoom>(imageSize, 1000);
    auto coloringStrategy = std::make_shared<mandelbrot::ColoringStrategyBasicDistanceEstimation>();

    escapeTimeAlgo->setMaxIterations(maxIterations);

    mandelbrot::Generator generator(std::thread::hardware_concurrency(), generatingStrategy, escapeTimeAlgo);
    boost::uuids::random_generator uuidGenerator;

    while(!EXIT)
    {
        generator.next();
        auto result = generator.getResult();

        std::string uuid = boost::uuids::to_string(uuidGenerator());

        mandelbrot::Image image(result, imageSize(0), imageSize(1), coloringStrategy);
        image.saveAsJPEG("images/" + uuid + ".jpg");

        mandelbrot::Data mandelbrotData(result);
        mandelbrotData.saveToFile("data/" + uuid + ".gz");
    }

    generator.exit();

    return 0;
}