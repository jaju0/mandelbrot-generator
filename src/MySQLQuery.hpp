#ifndef MYSQL_QUERY_HPP_
#define MYSQL_QUERY_HPP_

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/mysql.hpp>

namespace mandelbrot {

class MySQLQuery : public std::enable_shared_from_this<MySQLQuery>
{
public:
    friend class MySQLStore;

public:
    MySQLQuery(boost::asio::io_context& ioc, std::string_view hostname, boost::mysql::handshake_params mysqlHandshakeParams);
    void waitForResult();

protected:
    void execute();
    void resolve();
    void connect(boost::asio::ip::tcp::resolver::results_type results);
    void close();
    virtual void doQuery() = 0;

    std::string m_hostname;
    boost::asio::ip::tcp::resolver m_resolver;
    boost::mysql::tcp_connection m_connection;
    boost::mysql::handshake_params m_mysqlHandshakeParams;
    boost::mysql::diagnostics m_mysqlDiagnostics;
    boost::mysql::results m_results;

    bool m_isFinished;
    std::mutex m_cvIocResultMutex;
    std::condition_variable m_cvIocResult;
};

}

#endif