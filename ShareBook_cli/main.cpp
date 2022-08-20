/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       main.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @data       2022-08-19
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


std::string encodePhoto(std::string photoPath){
    //将图片解码为二进制流

    std::ifstream fin(photoPath, std::ios::binary);
    fin.seekg(0, ios::end);
    int iSize = fin.tellg();
    char* szBuf = new (std::nothrow) char[iSize];
    fin.seekg(0, ios::beg);
    fin.read(szBuf, sizeof(char) * iSize);
    fin.close();

    //图片解码后的二进制流 放进material
    return base64_encode(szBuf,iSize);
}


//test
void publish(){
    std::cout<<"Client<< 请求发布笔记"<<std::endl;
    char ipaddr[MAX] ={"10.252.71.12"};
    Client client(ipaddr);

    json message = {
        {"request", "PublishJottings"},
        {"name","moomoo"},
        {"content","风景很美！"},
        {"time","2022-08-19 12:00:00"},
        { "material","/root/01.jpg"}
    };

    //对图片进行解码为 二进制流
    std::string materialPath = message["material"];
    message["material"] = encodePhoto(materialPath);
    std::string sendData = message.dump();

    std::cout<<"Client<< 消息长度:"<<sendData.size()<<std::endl;
    client.send(sendData.data(),sendData.size());
    char receiveData[100000];
    client.receive(receiveData);
    std::cout<<"Client recieve data:"<<receiveData<<std::endl;

}

//test
void scan(){
    std::cout<<"Client<< 请求浏览笔记"<<std::endl;
    json jotting = {
        { "request", "ScanJottings"}
    };
    std::string sendData=jotting.dump();

    char ipaddr[MAX] ={"10.252.71.12"};
    Client client(ipaddr);
    client.send(sendData.data(),sendData.length());
    char receiveData[100000];
    client.receive(receiveData);
    std::cout<<"Client<< 接收服务端推送的笔记: "<<receiveData<<std::endl;
}

//test
void publishB(){
    std::cout<<"Client<< 请求发布笔记"<<std::endl;
    string ipaddr = "10.252.71.12";
    BoostClient client(ipaddr);

    json message = {
        {"request", "PublishJottings"},
        {"name","moomoo"},
        {"content","风景很美！"},
        {"time","2022-08-19 12:00:00"},
        { "material","/root/01.jpg"}
    };

    //对图片进行解码为 二进制流
    std::string materialPath = message["material"];
    message["material"] = encodePhoto(materialPath);
    std::string sendData = message.dump();

    std::cout<<"Client<< 消息长度:"<<sendData.size()<<std::endl;
    client.send(sendData);
    string receiveData = client.receive();
    std::cout<<"Client recieve data:"<<receiveData<<std::endl;
}

//test
void scanB(){
    std::string ipaddr="10.252.71.12";
    BoostClient client(ipaddr);

    std::cout<<"Client<< 请求浏览笔记"<<std::endl;
    json message = {
        { "request", "ScanJottings"}
    };
    std::string sendData=message.dump();

    client.send(sendData);
    std::string receiveData =  client.receive();
    json jotting = json::parse(receiveData);
    //将二进制流转为图片存储
    string materialData = jotting["material"];
    string material=base64_decode(materialData);
    string materialPath = "/root/rec.jpg";
    std::ofstream fout(materialPath, std::ios::binary);
    fout.write(material.c_str(), material.size());
    fout.close();

    jotting["material"] = materialPath;
    std::cout<<"Client<< 接收服务端推送的笔记: "<<std::endl;
    std::cout<<"==============================="<<std::endl;
    std::cout<<"name : "<<jotting["name"]<<std::endl;
    std::cout<<"content : "<<jotting["content"]<<std::endl;
    std::cout<<"time : "<<jotting["time"]<<std::endl;
    std::cout<<"material : "<<jotting["material"]<<std::endl;
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

//    std::thread t1(publishB),t2(scanB);
//    t1.join();
//    t2.join();
//    publishB();
    scanB();

    return 0;
}
