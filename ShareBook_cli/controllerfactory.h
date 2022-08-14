/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       controllerfactory.h
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/14
 * @history
 *****************************************************************************/
#ifndef ControllerFACTORY_H
#define ControllerFACTORY_H

#include "controller.h"
#include "publishjottingcontroller.h"
#include "scanandcheckjottingcontroller.h"

class ControllerFactory
{
public:
    static ControllerFactory *getInstance();
    ~ControllerFactory();

    PublishJottingController* createPublishJottingController();
    ScanAndCheckJottingController* createScanAndCheckJottingController();
private:
    ControllerFactory();
    static ControllerFactory* m_instance;

};

#endif // ControllerFACTORY_H
