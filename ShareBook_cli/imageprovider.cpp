#include "imageprovider.h"


#include "base64.h"
#include "client.h"
#include "clientInfo.h"


ImageProvider::ImageProvider()
    :QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize)
    Q_UNUSED(size)
    if (size)   *size = m_avatar.size();
    if(id=="avatar"){
        qDebug()<<"头像";
        return m_avatar;
    } else if(m_firstPics.find(id) != m_firstPics.end()){
        return m_firstPics[id];
    } else if(m_interestAvatars.find(id) != m_interestAvatars.end()){
        return m_interestAvatars[id];
    } else if(m_fanAvatars.find(id) != m_fanAvatars.end()){
        return m_fanAvatars[id];
    } else if(m_pushUIAvatars.find(id) != m_pushUIAvatars.end()){
        return m_pushUIAvatars[id];
    } else if(m_pushUIPics.find(id) != m_pushUIPics.end()){
        return m_pushUIPics[id];
    }else if(m_detailUIAvatars .find(id) != m_detailUIAvatars.end()){
        return m_detailUIAvatars[id];
    }else if(m_messageUIAvatars .find(id) != m_messageUIAvatars.end()){
        return m_messageUIAvatars[id];
    }else if(m_videoAvatars .find(id) != m_videoAvatars.end()){
        return m_videoAvatars[id];
    }

    return QPixmap();

//    return getPicture(id.toStdString());
}

QPixmap ImageProvider::getPicture(std::string path)
{
    qDebug()<<QString::fromStdString(path);
    nlohmann::json message{
        {"id",ID},
        {"request","GetPicture"},
        {"picPath",path}
    };

    std::string mes = message.dump(4);
    Client::getInstance()->reconnect();
    Client::getInstance()->send(mes.c_str(),message.size());
    char *pic = new char[9999999];
    Client::getInstance()->receive(pic);
    std::string photo=base64_decode(pic);

    QPixmap pixmap;
    pixmap.loadFromData((const unsigned char*)photo.c_str(),photo.size(),nullptr);
    delete []pic;
    return pixmap;
}


void ImageProvider::setAvatar(const QPixmap &newPixmap)
{
    m_avatar = newPixmap;
}

void ImageProvider::setHashPixmaps(const QHash<QString, QPixmap> &newPixmaps)
{
    m_firstPics = newPixmaps;
}

void ImageProvider::setInterestHashPixmaps(const QHash<QString, QPixmap> &newPixmaps)
{
    m_interestAvatars = newPixmaps;
}

void ImageProvider::setFanHashPixmaps(const QHash<QString, QPixmap> &newPixmaps)
{
    m_fanAvatars = newPixmaps;
}

void ImageProvider::setpushUIAvatars(const QHash<QString, QPixmap> &newPixmaps)
{
    m_pushUIAvatars = newPixmaps;
}

void ImageProvider::setMessageUIAvatars(const QHash<QString, QPixmap> &newPixmaps)
{
    m_messageUIAvatars = newPixmaps;
}

void ImageProvider::setpushUIPics(const QHash<QString, QPixmap> &newPixmaps)
{
    m_pushUIPics = newPixmaps;
}

void ImageProvider::setDetailUIAvatars(const QHash<QString, QPixmap> &newPixmaps)
{
    m_detailUIAvatars = newPixmaps;
}

void ImageProvider::setVideoAvatars(const QHash<QString, QPixmap> &newPixmaps)
{
    m_videoAvatars = newPixmaps;
}



