#include <iostream>
#include <string>
#include <tuple>
#include <boost/lexical_cast.hpp>

#include "MySQLQueryInsertImage.hpp"

namespace mandelbrot {

MySQLQueryInsertImage::MySQLQueryInsertImage(boost::asio::io_context& ioc, std::string_view hostname, boost::mysql::handshake_params mysqlHandshakeParams, ImageInsertionParameters params)
    : MySQLQuery(ioc, hostname, mysqlHandshakeParams)
    , m_params(params)
{}

void MySQLQueryInsertImage::doQuery()
{
    constexpr const char* query = "INSERT INTO images ("
            "uuid, params_width, params_height, params_minReal, params_maxReal, params_minImag, params_maxImag, params_maxIterations"
        ") VALUES (?,?,?,?,?,?,?,?);"
    ;

    m_connection.async_prepare_statement(
        query,
        m_mysqlDiagnostics,
        [self=shared_from_this()] (boost::mysql::error_code ec, boost::mysql::statement statement) {
            boost::mysql::throw_on_error(ec, self->m_mysqlDiagnostics);

            if(ec)
            {
                std::cerr << "error occured while preparing statement for image insertion" << std::endl;
                return;
            }

            self->executeStatement(statement);
        }
    );
}

void MySQLQueryInsertImage::executeStatement(boost::mysql::statement statement)
{
    auto& resultParams = m_params.result.params;
    std::string uuid = m_params.uuid;

    m_connection.async_execute_statement(statement, std::tuple(
        uuid,
        static_cast<int32_t>(resultParams.pixelsX),
        static_cast<int32_t>(resultParams.pixelsY),
        boost::lexical_cast<std::string>(resultParams.minReal),
        boost::lexical_cast<std::string>(resultParams.maxReal),
        boost::lexical_cast<std::string>(resultParams.minImag),
        boost::lexical_cast<std::string>(resultParams.maxImag),
        static_cast<int32_t>(resultParams.maxIterations)
    ), m_results, m_mysqlDiagnostics,
        [self=shared_from_this()] (boost::mysql::error_code ec) {
            boost::mysql::throw_on_error(ec, self->m_mysqlDiagnostics);
            self->close();
        }
    );
}

std::shared_ptr<MySQLQueryInsertImage> MySQLQueryInsertImage::shared_from_this()
{
    return std::dynamic_pointer_cast<MySQLQueryInsertImage>(MySQLQuery::shared_from_this());
}

}