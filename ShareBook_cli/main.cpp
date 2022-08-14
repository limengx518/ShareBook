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

using json=nlohmann::json;

void se(){
    std::string ipaddr("192.168.155.100");
    json j={
        { "request", "ScanJottings"}
    };
    std::string message=j.dump();
    Client client;
    client.send(message.data(),sizeof(message),ipaddr.data());
}

void re(){
    std::string ipaddr("192.168.155.100");
    json j={
        { "request", "ScanJottings"}
    };
    std::string message=j.dump();
    Client client;
    std::cout<<client.receive(ipaddr.data()).dump()<<std::endl;
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

//    std::thread s(se);
//    std::thread r(re);

//    s.join();
//    r.join();
    std::string ipaddr("10.252.71.12");
    json j={
        { "request", "ScanJottings"}
    };
    std::string message=j.dump();
    Client client;
    client.send(message.data(),sizeof(message),ipaddr.data());
    std::cout<<client.receive(ipaddr.data()).dump()<<std::endl;

}
