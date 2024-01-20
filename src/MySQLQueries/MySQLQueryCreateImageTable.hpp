#ifndef MANDELBROT_MYSQL_QUERY_CREATE_IMAGE_TABLE_HPP_
#define MANDELBROT_MYSQL_QUERY_CREATE_IMAGE_TABLE_HPP_

#include "../MySQLQuery.hpp"

namespace mandelbrot {

class MySQLQueryCreateImageTable : public MySQLQuery
{
public:
    MySQLQueryCreateImageTable(boost::asio::io_context& ioc, std::string_view hostname, boost::mysql::handshake_params mysqlHandshakeParams);

private:
    virtual void doQuery() override;
    std::shared_ptr<MySQLQueryCreateImageTable> shared_from_this();
};

}

#endif