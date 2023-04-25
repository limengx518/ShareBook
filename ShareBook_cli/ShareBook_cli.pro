QT += quick
QT += core-private
QT += gui-private
QT += widgets
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        base64.cpp \
        client.cpp \
        clientInfo.cpp \
        fileopendialog.cpp \
        filesavedialog.cpp \
        imageprovider.cpp \
        main.cpp \
        messageuicontrol.cpp \
        network.cpp \
        personaluicontrol.cpp \
        publishpagecontrol.cpp \
        pushpagecontrol.cpp \
    videopagecontrol.cpp

resources.files = qml/main.qml \
                    qml/ChooseMaterialPage.qml \
                    qml/InitInfoPage.qml \
                    qml/VideoPage.qml \
                    qml/JotDetailPage.qml \
                    qml/MessagePage.qml \
                    qml/PersonalPage.qml \
                    qml/PushPage.qml \
                    qml/PersonalRelationList.qml \
                    qml/PublishPage.qml \
                    qml/ProcessWidget.qml \
                    qml/Toast.qml \
                    qml/mediaplayer/AudioControl.qml \
                    qml/mediaplayer/PlaybackControl.qml \
                    qml/mediaplayer/PlaybackRateControl.qml \
                    qml/mediaplayer/PlaybackSeekControl.qml \
                    qml/mediaplayer/Mute_Icon.svg \
                    qml/mediaplayer/Speaker_Icon.svg

DISTFILES += \
    android_sources/AndroidManifest.xml \
    android_sources/res/drawable-hdpi/icon.png \
    android_sources/res/drawable-ldpi/icon.png \
    android_sources/res/drawable-mdpi/icon.png \
    android_sources/res/drawable-xhdpi/icon.png \
    android_sources/res/drawable-xxhdpi/icon.png \
    android_sources/res/drawable-xxxhdpi/icon.png \
    qml/ChooseMaterialPage.qml \
    qml/InitInfoPage.qml \
    qml/JotDetailPage.qml \
    qml/MessagePage.qml \
    qml/PersonalPage.qml \
    qml/PersonalRelationList.qml \
    qml/ProcessWidget.qml \
    qml/PublishPage.qml \
    qml/PushPage.qml \
    qml/Toast.qml \
    qml/VideoPage.qml \
    qml/main.qml \
    qml/mediaplayer/AudioControl.qml \
    qml/mediaplayer/Mute_Icon.svg \
    qml/mediaplayer/PlaybackControl.qml \
    qml/mediaplayer/PlaybackRateControl.qml \
    qml/mediaplayer/PlaybackSeekControl.qml \
    qml/mediaplayer/Speaker_Icon.svg \
    qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.pri

resources.prefix = /$${TARGET}
RESOURCES += resources \
    rec.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


include(qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.pri)
qtcAddDeployment()

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android_sources

HEADERS += \
    base64.h \
    client.h \
    clientInfo.h \
    fileopendialog.h \
    filesavedialog.h \
    imageprovider.h \
    messageuicontrol.h \
    network.h \
    personaluicontrol.h \
    publishpagecontrol.h \
    pushpagecontrol.h \
    videopagecontrol.h

