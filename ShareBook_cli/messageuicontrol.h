#ifndef MESSAGEUICONTROL_H
#define MESSAGEUICONTROL_H

#include <QObject>

class MessageUIControl:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString notifications READ getNotifications WRITE setNotifications NOTIFY notificationsChanged)

public:
    explicit MessageUIControl(QObject *parent=nullptr);
    Q_INVOKABLE void scanMessage();

    void setNotifications(const QString &notifications);
    QString getNotifications()const;

signals:
    void notificationsChanged(const QString notifications);
private:

    QString m_notifications;

};

#endif // MESSAGEUICONTROL_H
