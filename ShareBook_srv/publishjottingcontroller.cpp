/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       publishjottingcontroller.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/14
 * @history
 *****************************************************************************/
#include "publishjottingcontroller.h"
#include <iostream>

PublishJottingController* PublishJottingController::m_instance = nullptr;

PublishJottingController* PublishJottingController::getInstance()
{
    if(m_instance==nullptr){
        m_instance=new PublishJottingController();
    }
    return m_instance;
}

std::string  PublishJottingController::publishJottings(nlohmann::json j)
{
    std::cout<<"发布笔记详情:"<<j.dump()<<std::endl;

    return "请求发送material完整信息";
}

PublishJottingController::PublishJottingController()
{

}
