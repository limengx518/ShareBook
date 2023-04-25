#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "fileopendialog.h"
#include "imageprovider.h"
#include "messageuicontrol.h"
#include "personaluicontrol.h"
#include "pushpagecontrol.h"
#include "publishpagecontrol.h"
#include "videopagecontrol.h"


ImageProvider *imageProvider = nullptr;
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qmlRegisterType<PersonalUIControl>("UIControl",1,0,"PersonalUIControl");
    qmlRegisterType<PushPageControl>("UIControl",1,0,"PushPageControl");
    qmlRegisterType<PublishPageControl>("UIControl",1,0,"PublishPageControl");
    qmlRegisterType<FileOpenDialog>("UIControl", 1, 0, "FileOpenDialog");
    qmlRegisterType<VideoPageControl>("UIControl", 1, 0, "VideoPageControl");
    qmlRegisterType<MessageUIControl>("UIControl", 1, 0, "MessageUIControl");

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/ShareBook_cli/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    imageProvider = new ImageProvider();

    engine.addImageProvider("photos",imageProvider);

    engine.load(url);

    return app.exec();
}
