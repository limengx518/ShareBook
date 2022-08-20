/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       boostnetwork.h
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @data       2022-08-17 hh:mm:ss.zzz
 * @history
 *****************************************************************************/
#ifndef BOOSTNETWORK_H
#define BOOSTNETWORK_H


#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <boost/shared_ptr.hpp>

//socket没有默认构造函数，所以要用指针类型进行声明
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

class BoostNetwork
{
public:
    BoostNetwork(socket_ptr socket);
    ~BoostNetwork();

    std::string receiveMessage();
    int sendMessage(std::string message);
private:
    socket_ptr m_socket;
};

#endif // BOOSTNETWORK_H
