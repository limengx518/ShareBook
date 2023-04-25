#include "personaluicontrol.h"

#include "clientInfo.h"
#include "client.h"
#include "base64.h"
#include "imageprovider.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

extern ImageProvider *imageProvider;

PersonalUIControl::PersonalUIControl(QObject *parent):
    QObject(parent),m_nickName(""),m_sign("")
{

}

void PersonalUIControl::setNickName(const QString &nickName)
{
    m_nickName=nickName;
}

QString PersonalUIControl::getNickName() const
{
    return m_nickName;
}

void PersonalUIControl::setSign(const QString &sign)
{
    m_sign=sign;
    emit signChanged(m_sign);
}

QString PersonalUIControl::getSign() const
{
    return m_sign;
}

void PersonalUIControl::setAvatar(const QString &avatar)
{
    m_avatar=avatar;
    emit avatarChanged(m_avatar);
}

QString PersonalUIControl::getAvatar() const
{
    return m_avatar;
}

void PersonalUIControl::setJotting(const QString &jotting)
{
     m_jotting=jotting;
}

QString PersonalUIControl::getJotting() const
{
    return m_jotting;
}


void PersonalUIControl::setJottings(const QString &jottings)
{
    m_jottings=jottings;
    emit jottingsChanged(m_jottings);
}

QString PersonalUIControl::getJottings() const
{
    return m_jottings;
}


void PersonalUIControl::setInterest(const QString &interest)
{
    m_interest=interest;
    emit interestChanged(m_interest);
}

QString PersonalUIControl::getInterest() const
{
    return m_interest;
}

void PersonalUIControl::setFans(const QString &fans)
{
    m_fans=fans;
    emit fansChanged(m_fans);
}

QString PersonalUIControl::getFans() const
{
    return m_fans;
}

void PersonalUIControl::setJottingCount(const QString &jottingCount)
{
    m_jotting_count=jottingCount;
    emit jottingCountChanged(m_jotting_count);
}

QString PersonalUIControl::getJottingCount() const
{
    return m_jotting_count;
}

void PersonalUIControl::setInterestCount(const QString &interestCount)
{
    m_interest_count=interestCount;
    emit interestCountChanged(m_interest_count);
}

QString PersonalUIControl::getInterestCount() const
{
    return m_interest_count;
}

void PersonalUIControl::setFanCount(const QString &fanCount)
{
    m_fan_count=fanCount;
    emit fanCountChanged(m_fan_count);
}

QString PersonalUIControl::getFanCount() const
{
    return m_fan_count;
}

QJsonValue stringToJson(const QString &str)
{
    QString context = QString("{%1}").arg(str);
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(context.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError) {
        return QJsonValue();
    }

    return doc.object();
}

QJsonObject transition(nlohmann::json js)
{
    std::string info = js.dump();
    const char *ch=info.data();

    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonObject rootObj;   //最后转换出来的json 对象
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(ch), &json_error);
    if(json_error.error == QJsonParseError::NoError) {
           if(parse_doucment.isObject())
           {
               rootObj = parse_doucment.object();
           }
    } else {
        qDebug()<<json_error.errorString();
    }
    return rootObj;
}

void PersonalUIControl::initData()
{

//    发送请求获取网名id为~~~的相关数据
    nlohmann::json message = {
        {"id",ID},
        {"request","InitPersonalInfo"}
    };
    std::string sendData = message.dump();
    Client::getInstance()->reconnect();
    Client::getInstance()->send(sendData.c_str(),sendData.length());
    std::cout<<"Client"<<"客户端初始化用户信息......"<<std::endl;
    char *receiveData = new char[9999999];

    Client::getInstance()->receive(receiveData);

    //获取到的网民基本信息 以 json形式
    nlohmann::json personInfo = nlohmann::json::parse(receiveData);
    cout<<personInfo.dump(4)<<endl;

    QJsonObject rootObj = transition(personInfo);   //最后转换出来的json 对象
    //昵称
    m_nickName = rootObj["name"].toString();
    //个签
    m_sign = rootObj["signal"].toString();
    //粉丝数量
    m_fan_count = QString::number(rootObj["fanCnt"].toInt());
    m_interest_count = QString::number(rootObj["interestCnt"].toInt());

   //获取头像信息
    std::string avatarData=base64_decode(personInfo["avatar"]);
    QPixmap newpixmap;
    newpixmap.loadFromData((unsigned char *)avatarData.data(),avatarData.length());
    imageProvider->setAvatar(newpixmap);

    //获取笔记信息
    QJsonArray jottingsArray = rootObj["jottings"].toArray();
    QJsonArray array;
    QHash<QString,QPixmap> firstPicHash;
    for(int i=0;i<jottingsArray.size();i++){
        QJsonObject jot_Obj = jottingsArray[i].toObject();

        string imgId = personInfo["jottings"][i]["material_firstPath"]["picId"];
        string data = base64_decode(personInfo["jottings"][i]["material_firstPath"]["picContent"]);
        QPixmap pixmap;
        pixmap.loadFromData((unsigned char *)data.data(),data.length());

        firstPicHash.insert(QString::fromStdString(imgId),pixmap);

        string picPath = "image://photos/"+ imgId;

        jot_Obj.insert("jottingId",QString::fromStdString(personInfo["jottings"][i]["id"]));
        jot_Obj.insert("picPath",QString::fromStdString(picPath));
        jot_Obj.insert("file","JotDetailPage.qml");
        array.append(QJsonValue(jot_Obj));
    }
    imageProvider->setHashPixmaps(firstPicHash);


    m_jotting_count=QString::number(array.count());
    QJsonDocument jot_doc(array);
    m_jottings=QString(jot_doc.toJson(QJsonDocument::Indented));

    //关注者信息
    qDebug()<<"关注者信息。。。。。。。。。。。。。";


    QJsonArray arrayIn;
    QHash<QString,QPixmap> interestAvatorVec;
    //没有关注者
    if(personInfo["concernedInfo"].is_null()){
        qDebug()<<"没有关注者。。。。。。。。。。。。。";
        QJsonObject interest_json1;
        interest_json1.insert("nickName","NULL");
        interest_json1.insert("signal","NULL");
        interest_json1.insert("avatarPath","NULL");
        interest_json1.insert("file","qrc:/qml/qml/PersonalPage.qml");
        arrayIn.append(QJsonValue(interest_json1));
        QJsonDocument interest_doc(arrayIn);
        m_interest=QString(interest_doc.toJson(QJsonDocument::Indented));
    } else {
        QJsonArray interestsArray = rootObj["concernedInfo"].toArray();
        qDebug()<<interestsArray.isEmpty();
        for(int i=0;i<interestsArray.size();i++){
            QJsonObject interest_Obj = interestsArray[i].toObject();
            string imgId = personInfo["concernedInfo"][i]["avatarId"];
            string avatarPath = "image://photos/"+ imgId;
            string data = base64_decode(personInfo["concernedInfo"][i]["avatar"]);
            QPixmap pixmap;
            pixmap.loadFromData((unsigned char *)data.data(),data.length());

            interestAvatorVec.insert(QString::fromStdString(imgId),pixmap);

            interest_Obj.insert("avatarPath",QString::fromStdString(avatarPath));
            interest_Obj.insert("file","qrc:/qml/qml/PersonalPage.qml");
            arrayIn.append(QJsonValue(interest_Obj));
        }
        imageProvider->setInterestHashPixmaps(interestAvatorVec);
        QJsonDocument interest_doc(arrayIn);
        m_interest=QString(interest_doc.toJson(QJsonDocument::Indented));
    }

    qDebug()<<m_interest;

    //粉丝信息
    QJsonArray fansArray = rootObj["fansInfo"].toArray();
    QJsonArray arrayFan;
    QHash<QString,QPixmap> fanAvatorVec;
    for(int i=0;i<fansArray.size();i++){
        QJsonObject fan_Obj = fansArray[i].toObject();
        string imgId = personInfo["fansInfo"][i]["avatarId"];
        string avatarPath = "image://photos/"+ imgId;
        string data = base64_decode(personInfo["fansInfo"][i]["avatar"]);
        QPixmap pixmap;
        pixmap.loadFromData((unsigned char *)data.data(),data.length());

        fanAvatorVec.insert(QString::fromStdString(imgId),pixmap);

        fan_Obj.insert("avatarPath",QString::fromStdString(avatarPath));
        fan_Obj.insert("file","qrc:/qml/qml/PersonalPage.qml");
        arrayFan.append(QJsonValue(fan_Obj));
    }
    imageProvider->setFanHashPixmaps(fanAvatorVec);
    QJsonDocument fan_doc(arrayFan);
    m_fans=QString(fan_doc.toJson(QJsonDocument::Indented));

    if(m_fans.isEmpty()){
        m_fans = "";
    }
}


//获取某一篇笔记的详细信息
void PersonalUIControl::getOneJottingDetail(QString jotting_id)
{
    qDebug()<<"进来函数： PersonalUIControl::getOneJottingDetail============================";
    qDebug()<<"获取个人页面某篇笔记的详细信息";

    string id = jotting_id.toStdString();
    //发送请求 服务端推送个人页面确认的某篇笔记的详细信息
    nlohmann::json message = {
        {"id",ID},
        {"jottingId",id},
        {"request","GetInfoJottingDetail"}
    };

    std::string sendData = message.dump();
    Client::getInstance()->reconnect();
    Client::getInstance()->send(sendData.c_str(),sendData.length());
    std::cout<<"Client"<<"客户端获取用户推送个人页面笔记的详细信息......"<<std::endl;
    char *receiveData = new char[10000000];
    Client::getInstance()->receive(receiveData);

    //获取到的笔记的详细信息
    nlohmann::json jottingInfo = nlohmann::json::parse(receiveData);
    std::cout<<jottingInfo.dump(4)<<std::endl;

    nlohmann::json jotting;
    jotting["netizenName"] =  jottingInfo["netizen"]["nickName"];
    jotting["content"] =  jottingInfo["content"];
    jotting["time"] =  jottingInfo["time"];

    //笔记作者头像信息
    std::string avatarId = jottingInfo["netizen"]["avatarId"];
    string avatarPath = "image://photos/"+avatarId;
    std::string avatarData=base64_decode(jottingInfo["netizen"]["avatar"]);
    QPixmap pixmap;
    pixmap.loadFromData((unsigned char *)avatarData.data(),avatarData.length());
    imageProvider->setAvatar(pixmap);
    jotting["avatar"] = avatarPath;
    m_avatar = QString::fromStdString(avatarPath);


    //笔记素材信息
    nlohmann::json picPath;
    QHash<QString,QPixmap> allPics;
    for(int j=0;j<jottingInfo["materials"].size();j++){
        //确定路径
        string picId = jottingInfo["materials"][j]["picId"];
        string picNewPath = "image://photos/"+picId;

        //图片解码
        QPixmap newpixmap;
        std::string picData=base64_decode(jottingInfo["materials"][j]["picContent"]);
        newpixmap.loadFromData((unsigned char *)picData.data(),picData.length());

        picPath["path"] = picNewPath;

        jotting["picPath"].push_back(picPath);
        allPics.insert(QString::fromStdString(picId),newpixmap);
    }
    imageProvider->setpushUIPics(allPics);

    //笔记评论信息
    QHash<QString,QPixmap> avatars;
    for(auto &com:jottingInfo["comment"]){
        nlohmann::json comment;
        comment["netizenName"] = com["netizen"]["nickName"];
        comment["content"] = com["content"];
        comment["time"] = com["time"];
        //笔记作者头像信息
        std::string avatarId = com["netizen"]["avatarId"];
        string avatarPath = "image://photos/"+avatarId;
        std::string avatarData=base64_decode(com["netizen"]["avatar"]);
        QPixmap pixmap;
        pixmap.loadFromData((unsigned char *)avatarData.data(),avatarData.length());
        avatars.insert(QString::fromStdString(avatarId),pixmap);
        comment["pic"]=avatarPath;
        jotting["comment"].push_back(comment);
    }

    m_jotting=QString::fromStdString(jotting.dump());

    cout<<"==============笔记的详细信息：=============="<<endl;
    cout<<m_jotting.toStdString();
}

void PersonalUIControl::initClientInfo(QString id, QString ipaddr)
{
    std::cout<<id.toStdString()<<"\n"<<ipaddr.toStdString()<<endl;
    ID = id.toStdString();
    IPADDR  =ipaddr.toStdString();
}
