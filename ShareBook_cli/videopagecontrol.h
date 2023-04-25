#ifndef VIDEOPAGECONTROL_H
#define VIDEOPAGECONTROL_H

#include <QObject>

class VideoPageControl :public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nickName READ getNickName WRITE setNickName NOTIFY nickNameChanged)
    Q_PROPERTY(QString avatar READ getAvatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(QString videos READ getVideos WRITE setVideos NOTIFY videosChanged)


public:
    explicit VideoPageControl(QObject *parent=nullptr);

    Q_INVOKABLE void pushRecommendVideos();


    void setNickName(const QString &nickName);
    QString getNickName()const;

    void setAvatar(const QString &avatar);
    QString getAvatar()const;

    void setVideos(const QString &videos);
    QString getVideos()const;
signals:
    void nickNameChanged(const QString nickName);
    void avatarChanged(const QString avatar);
    void videosChanged(const QString videos);

private:
    QString m_nickName;
    QString m_avatar;

    QString m_videos;

};

#endif // VIDEOPAGECONTROL_H
