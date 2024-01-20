#ifndef MANDELBROT_MYSQL_STORE_HPP_
#define MANDELBROT_MYSQL_STORE_HPP_

#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <boost/asio/io_context.hpp>

#include "MySQLQueryTypes.hpp"
#include "MySQLQueries/MySQLQueryCreateImageTable.hpp"
#include "MySQLQueries/MySQLQueryInsertImage.hpp"

namespace mandelbrot {

class MySQLStore
{
public:
    MySQLStore(std::string_view hostname, std::string_view username, std::string_view password, std::string_view database);
    std::shared_ptr<MySQLQueryCreateImageTable> createImageTable();
    std::shared_ptr<MySQLQueryInsertImage> operator()(ImageInsertionParameters params);
    void exit();

private:
    void threadLoop();

    std::string m_hostname;
    boost::asio::io_context m_ioc;
    boost::mysql::handshake_params m_mysqlHandshakeParams;
    std::thread m_thread;

    bool m_shouldTerminate;
    std::mutex m_cvIocExecutionMutex;
    std::condition_variable m_cvIocExecution;

};

}

#endif