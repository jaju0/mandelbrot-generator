#include <csignal>
#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Config.hpp"
#include "Generator.hpp"
#include "MySQLStore.hpp"
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

    mandelbrot::Config config("config.ini");
    if(!config.readIniFile())
    {
        std::cerr << "abort" << std::endl;
        return 1;
    }

    uint32_t maxIterations = config.getMaxIterations();
    VectorU32 imageSize(2);
    imageSize(0) = config.getWidth();
    imageSize(1) = config.getHeight();

    auto generatingStrategy = std::make_shared<mandelbrot::GeneratingStrategyRandomZoom>(imageSize, maxIterations);
    auto coloringStrategy = std::make_shared<mandelbrot::ColoringStrategyBasicDistanceEstimation>();

    mandelbrot::Generator generator(std::thread::hardware_concurrency(), generatingStrategy);
    mandelbrot::MySQLStore mysqlStore(config.getMysqlHostname(), config.getMysqlUsername(), config.getMysqlPassword(), config.getMysqlDatabase());
    boost::uuids::random_generator uuidGenerator;

    mysqlStore.createImageTable()->waitForResult();
    while(!EXIT)
    {
        generator.next();
        auto result = generator.getResult();

        std::string uuid = boost::uuids::to_string(uuidGenerator());

        mysqlStore(mandelbrot::ImageInsertionParameters { result, uuid });

        mandelbrot::Image image(result, config.getThumbnailWidth(), config.getThumbnailHeight(), coloringStrategy);
        image.saveAsJPEG(config.getThumbnailDir() + uuid + ".jpg");

        mandelbrot::Data mandelbrotData(result);
        mandelbrotData.saveToFile(config.getOutputDir() + uuid + ".gz");
    }

    generator.exit();
    mysqlStore.exit();

    return 0;
}