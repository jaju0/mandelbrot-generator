#include "Config.hpp"

namespace mandelbrot {

Config::Config(std::string_view file)
    : m_file(file)
{}

bool Config::readIniFile()
{
    try
    {
        boost::property_tree::read_ini(m_file, m_tree);
    }
    catch(std::exception&)
    {
        std::cerr << "Failed to read file: \"" << m_file << "\"" << std::endl;
    }

    auto width = getConfigField<uint32_t>("width");
    auto height = getConfigField<uint32_t>("height");
    auto outputDir = getConfigField<std::string>("output_dir");
    auto thumbnailDir = getConfigField<std::string>("thumbnail_dir");
    auto thumbnailWidth = getConfigField<uint32_t>("thumbnail_width");
    auto thumbnailHeight = getConfigField<uint32_t>("thumbnail_height");
    auto maxIterations = getConfigField<uint32_t>("max_iterations");
    auto mysqlHostname = getConfigField<std::string>("mysql.hostname");
    auto mysqlUsername = getConfigField<std::string>("mysql.username");
    auto mysqlPassword = getConfigField<std::string>("mysql.password");
    auto mysqlDatabase = getConfigField<std::string>("mysql.database");

    if(
        !width.has_value() ||
        !height.has_value() ||
        !outputDir.has_value() ||
        !thumbnailDir.has_value() ||
        !thumbnailWidth.has_value() ||
        !thumbnailHeight.has_value() ||
        !maxIterations.has_value() ||
        !mysqlHostname.has_value() ||
        !mysqlUsername.has_value() ||
        !mysqlPassword.has_value() ||
        !mysqlDatabase.has_value()
    )
    {
        return false;
    }

    m_width = width.value();
    m_height = height.value();
    m_outputDir = outputDir.value();
    m_thumbnailDir = thumbnailDir.value();
    m_thumbnailWidth = thumbnailWidth.value();
    m_thumbnailHeight = thumbnailHeight.value();
    m_maxIterations = maxIterations.value();
    m_mysqlHostname = mysqlHostname.value();
    m_mysqlUsername = mysqlUsername.value();
    m_mysqlPassword = mysqlPassword.value();
    m_mysqlDatabase = mysqlDatabase.value();

    return true;
}

uint32_t Config::getWidth() const
{
    return m_width;
}

uint32_t Config::getHeight() const
{
    return m_height;
}

const std::string& Config::getOutputDir() const
{
    return m_outputDir;
}

const std::string& Config::getThumbnailDir() const
{
    return m_thumbnailDir;
}

uint32_t Config::getThumbnailWidth()
{
    return m_thumbnailWidth;
}

uint32_t Config::getThumbnailHeight()
{
    return m_thumbnailHeight;
}

uint32_t Config::getMaxIterations()
{
    return m_maxIterations;
}

const std::string& Config::getMysqlHostname() const
{
    return m_mysqlHostname;
}

const std::string& Config::getMysqlUsername() const
{
    return m_mysqlUsername;
}

const std::string& Config::getMysqlPassword() const
{
    return m_mysqlPassword;
}

const std::string& Config::getMysqlDatabase() const
{
    return m_mysqlDatabase;
}


}