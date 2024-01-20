#include "MySQLQueryCreateImageTable.hpp"

namespace mandelbrot {

MySQLQueryCreateImageTable::MySQLQueryCreateImageTable(boost::asio::io_context& ioc, std::string_view hostname, boost::mysql::handshake_params mysqlHandshakeParams)
    : MySQLQuery(ioc, hostname, mysqlHandshakeParams)
{}

void MySQLQueryCreateImageTable::doQuery()
{
    constexpr const char* query =
        "CREATE TABLE IF NOT EXISTS images ("
            "uuid VARCHAR(36),"
            "time_created TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "params_width INT,"
            "params_height INT,"
            "params_minReal TINYTEXT,"
            "params_maxReal TINYTEXT,"
            "params_minImag TINYTEXT,"
            "params_maxImag TINYTEXT,"
            "params_maxIterations INT"
        ")"
    ;

    m_connection.async_query(query, m_results, [self=shared_from_this()] (boost::mysql::error_code ec) {
        boost::mysql::throw_on_error(ec);
        self->close();
    });
}

std::shared_ptr<MySQLQueryCreateImageTable> MySQLQueryCreateImageTable::shared_from_this()
{
    return std::dynamic_pointer_cast<MySQLQueryCreateImageTable>(MySQLQuery::shared_from_this());
}

}