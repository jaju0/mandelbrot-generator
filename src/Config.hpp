#ifndef MANDELBROT_CONFIG_HPP_
#define MANDELBROT_CONFIG_HPP_

#include <iostream>
#include <string>
#include <optional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace mandelbrot {

class Config
{
public:
    Config(std::string_view file);
    bool readIniFile();

    uint32_t getWidth() const;
    uint32_t getHeight() const;
    const std::string& getOutputDir() const;
    const std::string& getThumbnailDir() const;
    uint32_t getThumbnailWidth();
    uint32_t getThumbnailHeight();
    uint32_t getMaxIterations();
    const std::string& getMysqlHostname() const;
    const std::string& getMysqlUsername() const;
    const std::string& getMysqlPassword() const;
    const std::string& getMysqlDatabase() const;

private:
    template<typename T>
    std::optional<T> getConfigField(const std::string& fieldName)
    {
        try
        {
            return m_tree.get<T>(fieldName);
        }
        catch(std::exception&)
        {
            std::cerr << "Could not get config field \"" << fieldName << "\"" << std::endl;
        }

        return std::nullopt;
    }

    std::string m_file;
    boost::property_tree::ptree m_tree;
    uint32_t m_width;
    uint32_t m_height;
    std::string m_outputDir;
    std::string m_thumbnailDir;
    uint32_t m_thumbnailWidth;
    uint32_t m_thumbnailHeight;
    uint32_t m_maxIterations;
    std::string m_mysqlHostname;
    std::string m_mysqlUsername;
    std::string m_mysqlPassword;
    std::string m_mysqlDatabase;
};

}

#endif