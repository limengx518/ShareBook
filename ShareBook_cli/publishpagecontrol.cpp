#include "publishpagecontrol.h"


#include <QDebug>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


PublishPageControl::PublishPageControl(QObject *parent):
    QObject(parent)
{

}

void PublishPageControl::findLocalMaterial()
{
    json path;
    path["path"]="qrc:/images/images/avatar.png";
    path["isSelected"]=false;
    path["order"]="";
    json localMaterial;
    for(int i=0;i<20;i++){
        localMaterial["materials"].push_back(path);
    }

    m_localMaterial=QString::fromStdString(localMaterial.dump());
}

void PublishPageControl::publishJotting(QString paths, QString content, QString nickName, QString head_pic)
{
    QStringList pathList=paths.split(",");

    json jotting;
    jotting["avatar"]=head_pic.toStdString();
    jotting["netizenName"]=nickName.toStdString();
    jotting["content"]=content.toStdString();
    jotting["likeCount"]="0";
    jotting["collectCount"]="0";
    jotting["commentCount"]="0";
    jotting["time"]="0000-00-00 00:00";

    for(auto &path:pathList){
        json picPath;
        picPath["path"]=path.toStdString();
        jotting["picPath"].push_back(picPath);
    }
    qDebug()<<QString::fromStdString(jotting.dump());
}

void PublishPageControl::setLocalMaterial(const QString &localMaterial)
{
    m_localMaterial=localMaterial;
}

QString PublishPageControl::getLocalMaterial() const
{
    return m_localMaterial;
}
