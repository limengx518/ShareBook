import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import UIControl 1.0

ApplicationWindow {
    id:rootWindow
    width: 543
    height: 1000
    visible: true
    title: qsTr("ShareBook")
    color: "#ffffff"

    readonly property url initInfoPage_loader: "InitInfoPage.qml"
    readonly property url pushPage_loader: "PushPage.qml"
    readonly property url videoPage_loader: "VideoPage.qml"
    readonly property url publishPage_loader: "PublishPage.qml"
    readonly property url messagePage_loader: "MessagePage.qml"
    readonly property url personalPage_loader: "PersonalPage.qml"
    readonly property url jotDetailPage_loader: "JotDetailPage.qml"


    readonly property int iconSize_avg:rootWindow.width * 0.2

    property var materialData


    QtObject {
        id: settings
        // Toggle default visibility of these views
        property bool showSettingsView: true
        property bool showLoggingView: false
        // Fonts in pointSizes
        // These are used mostly on examples in 3D side
        property real fontSizeLarge: 16
        // These are used mostly on settings
        property real fontSizeSmall: 10
    }



    PushPageControl{
        id:pushJottings
    }

    VideoPageControl{
        id:pushVideos
    }

    PublishPageControl{
        id:publishControl
    }

    MessageUIControl{
        id:pushMessages
    }


    PersonalUIControl{
        id:netizen
    }

//    Toast{
//        id:toast
//        rootWidth: rootWindow.width
//        rootHeight: rootWindow.height
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: rootWindow.height*0.1
//    }

    Loader {
        id: loader
        anchors.fill: parent
        asynchronous: true
        source:initInfoPage_loader
//        source:pushPage_loader
        Component.onCompleted: {
//            netizen.initData()
//            pushJottings.pushRecommendJottings()
        }
    }


    TabBar{
        id:bottom_button
        anchors.bottom: parent.bottom
        contentWidth: parent.width
        contentHeight: 60
        visible: false

        TabButton {
            height: parent.height
            background: Rectangle{
                color: "white"
                opacity: 0.7
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 1
                Image {
                    id: homeImg
                    fillMode: Image.PreserveAspectFit
                    sourceSize: Qt.size(30, 30)
                    source:"qrc:/images/images/home.png"
                }
                Text {
                    id: homeText
                    text: qsTr("首页")
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: 12
                }
            }
            onClicked: {
//                pushJottings.pushRecommendJottings()
                loader.source = pushPage_loader
                bottom_button.visible = true
                console.log("加载 "+ loader.source)
            }
            states: [
                State {
                    name: "default"
                    when: bottom_button.currentIndex !== 0
                    PropertyChanges {
                        target: homeImg; source:"qrc:/images/images/home.png"
                    }
//                    PropertyChanges {
//                        target: homeText; font.pointSize: 12
//                    }
                },
                State {
                    name: "isCurrentIndex"
                    when: bottom_button.currentIndex === 0
                    PropertyChanges {
                        target: homeImg; source:"qrc:/images/images/homeIng.png"
                    }
//                    PropertyChanges {
//                        target: homeText; font.pointSize: 14
//                    }
                }
            ]
        }
        TabButton {
            height: parent.height
            background: Rectangle{
                color: "white"
                opacity: 0.7
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 1
                Image {
                    id: cycleImg
                    fillMode: Image.PreserveAspectFit
                    sourceSize: Qt.size(30, 30)
                    source:"qrc:/images/images/vedio.png"
                }
                Text {
                    id: cycleText
                    text: qsTr("视频")
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: 12
                }
            }
            onClicked: {
                pushVideos.pushRecommendVideos()
                loader.source = videoPage_loader
                bottom_button.visible = true
                console.log("加载 "+ loader.source)
            }

            states: [
                State {
                    name: "default"
                    when: bottom_button.currentIndex !== 1
                    PropertyChanges {
                        target: cycleImg; source:"qrc:/images/images/vedio.png"
                    }
//                    PropertyChanges {
//                        target: cycleText; font.pointSize: 12
//                    }
                },
                State {
                    name: "isCurrentIndex"
                    when: bottom_button.currentIndex === 1
                    PropertyChanges {
                        target: cycleImg; source:"qrc:/images/images/vedioIng.png"
                    }
//                    PropertyChanges {
//                        target: cycleText; font.pointSize: 14
//                    }
                }
            ]
        }
        TabButton {
            height: parent.height
            background: Rectangle{
                color: "white"
                opacity: 0.7
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 1
                Image {
                    id: publishImg
                    fillMode: Image.PreserveAspectFit
                    sourceSize: Qt.size(30, 30)
                    source:"qrc:/images/images/publish.png"
                }
                Text {
                    id: publishText
                    text: qsTr("发布")
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: 12
                }
            }
            onClicked: {
                loader.source = publishPage_loader
                bottom_button.visible = false
                console.log("加载 "+ loader.source)
            }
        }

        TabButton {
            height: parent.height
            background: Rectangle{
                color: "white"
                opacity: 0.7
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 1
                Image {
                    id: messageImg
                    fillMode: Image.PreserveAspectFit
                    sourceSize: Qt.size(30, 30)
                    source:"qrc:/images/images/message.png"
                }
                Text {
                    id: messageText
                    text: qsTr("消息")
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: 12
                }
            }
            onClicked: {

                loader.source = messagePage_loader
                bottom_button.visible = true
                pushMessages.scanMessage()
                console.log("加载 "+ loader.source)
            }
            states: [
                State {
                    name: "default"
                    when: bottom_button.currentIndex !== 3
                    PropertyChanges {
                        target: messageImg; source:"qrc:/images/images/message.png"
                    }
//                    PropertyChanges {
//                        target: messageText; font.pointSize: 12
//                    }
                },
                State {
                    name: "isCurrentIndex"
                    when: bottom_button.currentIndex === 3
                    PropertyChanges {
                        target: messageImg; source:"qrc:/images/images/messageIng.png"
                    }
//                    PropertyChanges {
//                        target: messageText; font.pointSize: 14
//                    }
                }
            ]
        }

        TabButton {
            height: parent.height
            background: Rectangle{
                color: "white"
                opacity: 0.7
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 1
                Image {
                    id: myImg
                    fillMode: Image.PreserveAspectFit
                    sourceSize: Qt.size(30, 30)
                    source:"qrc:/images/images/personal.png"
                }
                Text {
                    id: myText
                    text: qsTr("我")
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: 12
                }
            }
            onClicked: {
                loader.source = personalPage_loader
                bottom_button.visible = true
                //发信号给C++ 请求客户端获取服务器传来的个人信息
                netizen.initData()
                console.log("加载 "+ loader.source)
            }
            states: [
                State {
                    name: "default"
                    when: bottom_button.currentIndex !== 4
                    PropertyChanges {
                        target: myImg; source:"qrc:/images/images/personal.png"
                    }
                },
                State {
                    name: "isCurrentIndex"
                    when: bottom_button.currentIndex === 4
                    PropertyChanges {
                        target: myImg; source:"qrc:/images/images/personalIng.png"
                    }
                }
            ]
        }
    }

}
