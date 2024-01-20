#ifndef MANDELBROT_MYSQL_QUERY_INSERT_IMAGE_HPP_
#define MANDELBROT_MYSQL_QUERY_INSERT_IMAGE_HPP_

#include "../MySQLQuery.hpp"
#include "../MySQLQueryTypes.hpp"

namespace mandelbrot {

class MySQLQueryInsertImage : public MySQLQuery
{
public:
    MySQLQueryInsertImage(boost::asio::io_context& ioc, std::string_view hostname, boost::mysql::handshake_params mysqlHandshakeParams, ImageInsertionParameters params);

protected:
    virtual void doQuery() override;
    void executeStatement(boost::mysql::statement statement);
    std::shared_ptr<MySQLQueryInsertImage> shared_from_this();

    ImageInsertionParameters m_params;
};

}

#endif