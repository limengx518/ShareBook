#ifndef PUBLISHPAGECONTROL_H
#define PUBLISHPAGECONTROL_H

#include <QObject>

class PublishPageControl:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString localMaterial READ getLocalMaterial WRITE setLocalMaterial NOTIFY localMaterialChanged)
public:
    explicit PublishPageControl(QObject *parent=nullptr);

    Q_INVOKABLE void findLocalMaterial();
    Q_INVOKABLE void publishJotting(QString paths,QString content,QString nickName,QString head_pic);

    void setLocalMaterial(const QString &localMaterial);
    QString getLocalMaterial()const;

signals:
    void localMaterialChanged(const QString localMaterial);

private:
    QString m_localMaterial;
};

#endif // PUBLISHPAGECONTROL_H
