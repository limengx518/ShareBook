/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       boostclient.h
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @data       2022-08-17
 * @history
 *****************************************************************************/
#ifndef BOOSTCLIENT_H
#define BOOSTCLIENT_H

#define BOOSERV_PORT 9876
#include <string>

class BoostClient
{
public:
    BoostClient();
    void send(std::string serverIp, std::string message);
};

#endif // BOOSTCLIENT_H
