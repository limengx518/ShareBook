/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       main.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/14
 * @history
 *****************************************************************************/
#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include "client.h"
#include <nlohmann/json.hpp>
#include <thread>


#include "base64.h"

using json=nlohmann::json;
/*
void publish(){
    std::string ipaddr("192.168.149.197");

    json jotting = {
        { "request", "PublishJottings"},
        { "name", "moomoo"},
        { "avator", "头像"},
        { "time", "20220809"},
        { "material", "path1"},
        { "comtent", "A new Jotting!!"}
    };
    std::string sendData=jotting.dump();


    Client client("192.168.149.100");
//    sleep(10);
    client.send(sendData.data(),sendData.length(),ipaddr.data());
}

void scan(){
    std::string ipaddr("192.168.149.100");

    json jotting = {
        { "request", "ScanJottings"}
    };
    std::string sendData=jotting.dump();


    Client client("192.168.149.100");
//    sleep(10);
    client.send(sendData.data(),sendData.length(),ipaddr.data());
}*/

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

//    QTranslator translator;
//    const QStringList uiLanguages = QLocale::system().uiLanguages();
//    for (const QString &locale : uiLanguages) {
//        const QString baseName = "ShareBook_cli_" + QLocale(locale).name();
//        if (translator.load(":/i18n/" + baseName)) {
//            a.installTranslator(&translator);
//            break;
//        }
//    }

//    return a.exec();





    std::string ipaddr("192.168.149.100");
    Client client("192.168.149.100");

    //读取图片文件
    std::ifstream fin("/root/01.jpg", std::ios::binary);
    fin.seekg(0, ios::end);
    int iSize = fin.tellg();
    char* szBuf = new (std::nothrow) char[iSize];
    fin.seekg(0, ios::beg);
    fin.read(szBuf, sizeof(char) * iSize);
    fin.close();
    //将图片解码
    string imgEncode = base64_encode(szBuf,iSize);

    json jotting = {
        { "request", "PublishJottings"},
        { "material","/root/01.jpg"}
    };
//    std::cout<<jotting.dump()<<std::endl;
    jotting["material"]=imgEncode;
    string sendData = jotting.dump();
//    std::cout<<imgEncode.size()<<std::endl;

    client.send(sendData.data(),sendData.length(),ipaddr.data());

//     std::thread t1(publish),t2(scan);
//     t1.join();
//     t2.join();

}
