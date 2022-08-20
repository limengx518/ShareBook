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

std::string encodePhoto(std::string photoPath){
    //将图片解码为二进制流

    std::ifstream fin(photoPath, std::ios::binary);
    fin.seekg(0, std::ios::end);
    int iSize = fin.tellg();
    char* szBuf = new (std::nothrow) char[iSize];
    fin.seekg(0, std::ios::beg);
    fin.read(szBuf, sizeof(char) * iSize);
    fin.close();

    //图片解码后的二进制流 放进material
    return base64_encode(szBuf,iSize);
}


json ScanAndCheckJottingController::pushJottings()
{
    json jotting={
        { "name", "Jack"},
        { "avator","头像1" },
        { "content", "今天也是美好的一天" },
        { "time", "2022-08-01 13:00:12" },
        { "material", "root/02.jpg"}
    };

    //对图片进行解码为 二进制流
    std::string materialPath = jotting["material"];
    std::cout<<materialPath<<std::endl;
    jotting["material"] = encodePhoto(materialPath);
     std::cout<<jotting.dump()<<std::endl;

    return jotting;
}

ScanAndCheckJottingController::ScanAndCheckJottingController()
{

}
