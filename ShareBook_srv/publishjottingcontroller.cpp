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
#include "json/include/nlohmann/json_fwd.hpp"
#include <iostream>
#include "netizen.h"
#include "netizenbroker.h"

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
    //将二进制流转为图片存储
    string materialData = j["material"];
    string material=base64_decode(materialData);
    string materialPath = "/root/newMaterial.jpg";
    std::ofstream fout(materialPath, std::ios::binary);
    fout.write(material.c_str(), material.size());
    fout.close();

    j["material"] = materialPath;
    std::cout<<"发布笔记详情如下:"<<std::endl;
    std::cout<<"==============================="<<std::endl;
    std::cout<<"name : "<<j["name"]<<std::endl;
    std::cout<<"content : "<<j["content"]<<std::endl;
    std::cout<<"time : "<<j["time"]<<std::endl;
    std::cout<<"material : "<<j["material"]<<std::endl;

    return "服务端已接收您发布笔记的详细信息!";
}

void PublishJottingController::comment(string netizenId,std::string comment,std::string jottingId)
{
     Netizen &netizen=NetizenBroker::getInstance()->findById(netizenId);
     netizen.comment(comment,jottingId);
}

PublishJottingController::PublishJottingController()
{

}
