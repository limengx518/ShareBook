/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       main.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/13
 * @history
 *****************************************************************************/

#include <iostream>

#include "boostserver.h"
#include "server.h"
#include "test.h"
#include "relationalbroker.h"

int main()
{
    //进行数据库的初始化操作
//    RelationalBroker rb;
//    rb.initDataBase();

//    BoostServer boostServer;
//    boostServer.start();

    Server server;
    server.start();
}
