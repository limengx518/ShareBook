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


PublishJottingController* PublishJottingController::m_instance = nullptr;

PublishJottingController* PublishJottingController::getInstance()
{
    if(m_instance==nullptr){
        m_instance=new PublishJottingController();
    }
    return m_instance;
}

PublishJottingController::PublishJottingController()
{

}
