#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
    Q_OBJECT
public:
    ImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

    void setAvatar(const QPixmap &newPixmap);
    void setHashPixmaps(const QHash<QString,QPixmap> &newPixmaps);
    void setInterestHashPixmaps(const QHash<QString,QPixmap> &newPixmaps);
    void setFanHashPixmaps(const QHash<QString,QPixmap> &newPixmaps);
    void setpushUIAvatars(const QHash<QString,QPixmap> &newPixmaps);

    void setMessageUIAvatars(const QHash<QString,QPixmap> &newPixmaps);
    void setpushUIPics(const QHash<QString,QPixmap> &newPixmaps);
    void setDetailUIAvatars(const QHash<QString,QPixmap> &newPixmaps);
    void setVideoAvatars(const QHash<QString,QPixmap> &newPixmaps);


    QPixmap getPicture(std::string path);
private:
    QPixmap m_avatar;
    QVector<QPixmap> m_pixmaps;
    QHash<QString,QPixmap> m_firstPics;
    QHash<QString,QPixmap> m_interestAvatars;
    QHash<QString,QPixmap> m_fanAvatars;
    QHash<QString,QPixmap> m_pushUIAvatars;
    QHash<QString,QPixmap> m_messageUIAvatars;
    QHash<QString,QPixmap> m_detailUIAvatars;
    QHash<QString,QPixmap> m_pushUIPics;
    QHash<QString,QPixmap> m_videoAvatars;
};

#endif // IMAGEPROVIDER_H
