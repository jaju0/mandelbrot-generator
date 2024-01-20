#include <iostream>

#include "MySQLStore.hpp"

namespace mandelbrot {

MySQLStore::MySQLStore(std::string_view hostname, std::string_view username, std::string_view password, std::string_view database)
    : m_hostname(hostname)
    , m_mysqlHandshakeParams(username, password, database)
    , m_shouldTerminate(false)
{
    m_thread = std::thread(std::bind(&MySQLStore::threadLoop, this));
}

std::shared_ptr<MySQLQueryCreateImageTable> MySQLStore::createImageTable()
{
    auto query = std::make_shared<MySQLQueryCreateImageTable>(m_ioc, m_hostname, m_mysqlHandshakeParams);
    query->execute();
    m_cvIocExecution.notify_all();
    return query;
}

std::shared_ptr<MySQLQueryInsertImage> MySQLStore::operator()(ImageInsertionParameters params)
{
    auto query = std::make_shared<MySQLQueryInsertImage>(m_ioc, m_hostname, m_mysqlHandshakeParams, params);
    query->execute();
    m_cvIocExecution.notify_all();
    return query;
}

void MySQLStore::exit()
{
    std::cout << "shutting down mysql store" << std::endl;
    m_shouldTerminate = true;
    m_cvIocExecution.notify_all();
    m_thread.join();
}

void MySQLStore::threadLoop()
{
    while(true)
    {
        {
            std::unique_lock lk(m_cvIocExecutionMutex);
            m_cvIocExecution.wait(lk);

            if(m_shouldTerminate)
                return;
        }

        if(m_ioc.stopped())
            m_ioc.restart();
        else
            m_ioc.run();

        if(m_shouldTerminate)
            return;
    }
}

}