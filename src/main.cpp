#include <iostream>

#include "Generator.hpp"
#include "Image.hpp"
#include "EscapeTimeAlgos/EscapeTimeAlgoDistanceEstimation.hpp"
#include "ColoringStrategies/ColoringStrategyBasicDistanceEstimation.hpp"

int main()
{
    auto escapeTimeAlgo = std::make_shared<mandelbrot::EscapeTimeAlgoDistanceEstimation>();
    escapeTimeAlgo->setMaxIterations(10000);
    auto coloringStrategy = std::make_shared<mandelbrot::ColoringStrategyBasicDistanceEstimation>();
    mandelbrot::Generator generator(std::thread::hardware_concurrency(), 1920, 1080, 10000, escapeTimeAlgo);
    generator.next();
    auto result = generator.getResult();
    mandelbrot::Image image(result, 1920, 1080, coloringStrategy);
    image.saveAsJPEG("test.jpg");
    generator.exit();
    return 0;
}