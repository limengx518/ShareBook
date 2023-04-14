#ifndef RELATIONALBROKER_H
#define RELATIONALBROKER_H
#include <mariadb/conncpp.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind/bind.hpp>

class Jotting;
class Netizen;
class Material;

class RelationalBroker
{
public:
    RelationalBroker();

    void initDataBase();

    sql::ResultSet* query(std::string command);
    void drop(std::string command);
    static void insert(std::string command);

    //清除某些缓存
    virtual ~RelationalBroker(){}
private:
    static std::unique_ptr<sql::Connection> m_connection;
};

#endif // RELATIONALBROKER_H
