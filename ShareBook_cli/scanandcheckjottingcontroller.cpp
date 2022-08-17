/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       scanandcheckjottingcontroller.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/14
 * @history
 *****************************************************************************/
#include "scanandcheckjottingcontroller.h"

ScanAndCheckJottingController * ScanAndCheckJottingController::m_instance = nullptr;

ScanAndCheckJottingController *ScanAndCheckJottingController::getInstance()
{
    if(m_instance==nullptr){
        m_instance=new ScanAndCheckJottingController();
    }
    return m_instance;
}

json ScanAndCheckJottingController::scanJottings(char *ipaddr)
{
    Client client(ipaddr);
    return client.receive();
}

ScanAndCheckJottingController::ScanAndCheckJottingController()
{

}
