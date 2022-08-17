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

#include "boostclient.h"
#include "client.h"

#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <nlohmann/json.hpp>
#include <thread>

#define MAX 20

using json=nlohmann::json;

//test
void publish(){
    json jotting = {
        { "request", "PublishJottings"},
        { "name", "moomoo"},
        { "avator", "头像"},
        { "time", "20220809"},
        { "material", "path1"},
        { "comtent", "A new Jotting!!"}
    };
    std::string sendData=jotting.dump();

    char ipaddr[MAX] ={"192.168.149.100"};
    Client client(ipaddr);
    client.send(sendData.data(),sendData.length());
}
//test
void scan(){
    json jotting = {
        { "request", "ScanJottings"}
    };
    std::string sendData=jotting.dump();

    char ipaddr[MAX] ={"192.168.149.100"};
    Client client(ipaddr);
    client.send(sendData.data(),sendData.length());
}

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


    char ipaddr[MAX] ={"192.168.149.100"};
    Client client(ipaddr);

    json message = {
        { "request", "ScanJottings"},
        { "material","/root/01.mp4"}
    };

    std::string sendData = message.dump();
    client.send(sendData.data(),sendData.length());
    json j = client.receive();
    std::cout<<"Client recieve data:"<<j.dump()<<std::endl;


//    std::string sendData = jotting["material"];
//    client.sendFile(sendData.data(),sendData.length(),sendData);


//    BoostClient client;
//    client.send("192.168.149.100",message.dump());


//    std::thread t1(publish),t2(scan);
//     t1.join();
//     t2.join();

    return 0;
}
