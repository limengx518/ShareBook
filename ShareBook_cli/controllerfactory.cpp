/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       controllerfactory.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/14
 * @history
 *****************************************************************************/
#include "controllerfactory.h"

#include "controllerfactory.h"


ControllerFactory* ControllerFactory::m_instance=nullptr;


ControllerFactory *ControllerFactory::getInstance()
{
    if(m_instance==nullptr){
        m_instance=new ControllerFactory();
    }
    return m_instance;
}

PublishJottingController *ControllerFactory::createPublishJottingController()
{
    return PublishJottingController::getInstance();
}

ScanAndCheckJottingController *ControllerFactory::createScanAndCheckJottingController()
{
    return ScanAndCheckJottingController::getInstance();
}

ControllerFactory::ControllerFactory()
{

}

ControllerFactory::~ControllerFactory()
{
}
