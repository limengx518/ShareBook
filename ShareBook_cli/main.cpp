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
#include "base64.h"

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


    char ipaddr[MAX] ={"10.252.71.12"};
    Client client(ipaddr);

    json message = {
        { "request", "PublishJottings"},
        { "material","/root/01.jpg"}
    };

    //将图片解码为二进制流
    std::string picPath = message["material"];
    std::cout<<picPath<<std::endl;
    std::ifstream fin(picPath, std::ios::binary);
    fin.seekg(0, ios::end);
    int iSize = fin.tellg();
    char* szBuf = new (std::nothrow) char[iSize];
    fin.seekg(0, ios::beg);
    fin.read(szBuf, sizeof(char) * iSize);
    fin.close();

//    //图片解码后的二进制流
    string imgStr=base64_encode(szBuf,iSize);
//    client.sendPic(imgStr.data(),imgStr.size());

/*
    //发送端, 发送字节长度+结束标志位(end)
    string flag = to_string(imgStr.size())+"END";
    string flagData = base64_encode(flag.c_str(),flag.size());
    message["material"] = flagData;*/

    string sendData = message.dump();
    std::cout<<message.dump()<<std::endl;
    client.send(sendData.data(),sendData.size());

    string receiveData = client.receive();
    std::cout<<"Client recieve data:"<<receiveData<<std::endl;
    if(receiveData == "请求发送material完整信息"){
        std::cout<<"服务端收到标志数据，客户端发送图像字节流数据:"<<std::endl;
        std::cout<<"即将发送的字节流大小是"<<imgStr.size()<<std::endl;
        client.sendPic(imgStr.data(),imgStr.size());
    }

//    std::string sendData = jotting["material"];
//    client.sendFile(sendData.data(),sendData.length(),sendData);

//    BoostClient client;
//    client.send("192.168.149.100",message.dump());


//    std::thread t1(publish),t2(scan);
//     t1.join();
//     t2.join();

    return 0;
}
