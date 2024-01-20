#include "MySQLQuery.hpp"

namespace mandelbrot {

MySQLQuery::MySQLQuery(boost::asio::io_context& ioc, std::string_view hostname, boost::mysql::handshake_params mysqlHandshakeParams)
    : m_hostname(hostname)
    , m_resolver(ioc.get_executor())
    , m_connection(ioc.get_executor())
    , m_mysqlHandshakeParams(mysqlHandshakeParams)
    , m_isFinished(false)
{}

void MySQLQuery::waitForResult()
{
    if(m_isFinished)
        return;

    std::unique_lock lk(m_cvIocResultMutex);
    m_cvIocResult.wait(lk);
}

void MySQLQuery::execute()
{
    resolve();
}

void MySQLQuery::resolve()
{
    m_resolver.async_resolve(m_hostname, boost::mysql::default_port_string,
        [self=shared_from_this()] (boost::mysql::error_code ec, boost::asio::ip::tcp::resolver::results_type results) {
            boost::mysql::throw_on_error(ec);
            self->connect(results);
        }
    );
}

void MySQLQuery::connect(boost::asio::ip::tcp::resolver::results_type results)
{
    m_connection.async_connect(*results.begin(), m_mysqlHandshakeParams, m_mysqlDiagnostics,
        [self=shared_from_this()] (boost::mysql::error_code ec) {
            boost::mysql::throw_on_error(ec, self->m_mysqlDiagnostics);
            self->doQuery();
        }
    );
}

void MySQLQuery::close()
{
    m_connection.async_close(m_mysqlDiagnostics, [self=shared_from_this()] (boost::mysql::error_code ec) {
        boost::ignore_unused(ec);
        self->m_isFinished = true;
        self->m_cvIocResult.notify_all();
    });
}


}