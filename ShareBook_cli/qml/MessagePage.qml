import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import Qt5Compat.GraphicalEffects


Item {
    id:messagePage
    readonly property int rootWidth: messagePage.width
    readonly property int rootHeight: messagePage.height


    //消息数据
    property var messagesInfo: JSON.parse(pushMessages.notifications).notification
    property var jotting_detail

    Rectangle{
        id:topRec
        width:parent.width
        height:rootWidth*0.1
        Text {
            id: text
            anchors.topMargin: rootWidth*0.1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("消息")
            font.pointSize: 18
        }

        Button{
            id:groupChat
            height:parent.height
            anchors.left: text.right
            anchors.leftMargin:  rootWidth*0.28
            background: Rectangle{
                anchors.fill: parent
                color: "white"
            }
            anchors.right: parent.right
            anchors.rightMargin:  rootWidth*0.01
            anchors.verticalCenter: parent.verticalCenter
            RowLayout{
                anchors.verticalCenter: parent.verticalCenter
                Image{
                    id:img
                    source:"qrc:/images/images/groupChat.png"
                    fillMode: Image.PreserveAspectFit
                    sourceSize: Qt.size(15, 15)
                }
                Text{
                    text: qsTr("群聊")
                    font.pointSize: 15
                }
            }
            onClicked: {
                console.log("群聊 被点击了!")
            }
        }

    }


    RowLayout{
        id:midRow
        anchors.top: topRec.bottom
        anchors.topMargin: rootWidth*0.1
        anchors.bottomMargin: rootWidth*0.1
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: rootWidth*0.25
        Button{
            height: parent.height
            background: Rectangle{
                color: "white"
                opacity: 0.7
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: rootWidth*0.025
                Image {
                    id: likeImg
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignHCenter
                    sourceSize: Qt.size(40, 40)
                    source:"qrc:/images/images/message_like.png"
                }
                Text {
                    id: likeText
                    text: qsTr("赞和收藏")
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: 12
                }
            }
            onClicked: {
                console.log("赞和收藏被点击了!")
            }
        }

        Button{
            height: parent.height
            background: Rectangle{
                color: "white"
                opacity: 0.7
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: rootWidth*0.025
                Image {
                    id: followImg
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignHCenter
                    sourceSize: Qt.size(40, 40)
                    source:"qrc:/images/images/message_follow.png"
                }
                Text {
                    id: followText
                    text: qsTr("新增关注")
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: 12
                }
            }
            onClicked: {
                console.log("新增关注被点击了!")
            }
        }

        Button{
            height: parent.height
            background: Rectangle{
                color: "white"
                opacity: 0.7
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: rootWidth*0.025
                Image {
                    id: commentImg
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignHCenter
                    sourceSize: Qt.size(40, 40)
                    source:"qrc:/images/images/@.png"
                }
                Text {
                    id: commentText
                    text: qsTr("评论和@")
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: 12
                }
            }
            onClicked: {
                console.log("评论和@被点击了!")
            }
        }
    }



    ScrollView{
        width: rootWidth
        height: rootHeight*0.85
        anchors.top: midRow.bottom
        anchors.topMargin: rootWidth*0.1


        Component{
            id:messageComponent
            Rectangle{
                id:message
                width:rootWidth
                height:rootWidth*0.2
                anchors.bottomMargin:  rootWidth*0.01

                Button{
                    id:messageBtn
                    anchors.fill: parent
                    background: Rectangle{
                        color: "transparent"
                    }

                    Rectangle{
                        id:recImg
                        width: rootWidth*0.12
                        height: rootWidth*0.12
                        anchors.left: parent.left
                        anchors.leftMargin:  rootWidth*0.03
                         anchors.verticalCenter: parent.verticalCenter
                        Image {
                            id: head_image
                            smooth: true
                            visible: false
                            anchors.fill: parent
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.centerIn: parent.Center
//                            source: "qrc:/images/images/head.png"
                            source: messagesInfo[index].avatar
                            sourceSize: Qt.size(parent.size, parent.size)
                            antialiasing: true
                        }
                        Rectangle {
                            id: head_mask
                            color: "black"
                            anchors.fill: parent
                            radius: width/2
                            visible: false
                            antialiasing: true
                            smooth: true
                        }
                        OpacityMask {
                            id:mask_image
                            anchors.fill: head_image
                            source: head_image
                            maskSource: head_mask
                            visible: true
                            antialiasing: true
                        }
                    }

                    Rectangle{
                        height: rootWidth*0.1
                        anchors.left:recImg.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: rootWidth*0.05
                        anchors.right: parent.right
                        Text {
                            id: name
                            font.bold: true
                            font.pixelSize: 15
                            color: "grey"
                            text: messagesInfo[index].nickName
                        }
                        Text {
                            id: content
                            anchors.top: name.bottom
                            anchors.topMargin: rootWidth*0.01
                            font.pixelSize: 15
                            text: messagesInfo[index].content
                        }
                        Text {
                            id: comment_time
                            anchors.left: name.right
                            anchors.leftMargin: rootWidth*0.35
                            anchors.right: parent.right
                            anchors.rightMargin: rootWidth*0.05
                            color: "grey"
                            font.pixelSize: 15
                            text: messagesInfo[index].time
                        }
                    }


//                    Text{
//                        id:jottingId
//                        text:messagesInfo[index].jottingId

//                    }

                    onClicked: {
                        console.log("这个消息被点击了！,即将进入关注的人发布的新笔记！")
                        console.log("进入笔记的ID为 "+ messagesInfo[index].jottingId)

                        netizen.getOneJottingDetail(messagesInfo[index].jottingId)
                        jotting_detail = JSON.parse(netizen.jotting)

                        loader.setSource(jotDetailPage_loader,{"type":"message","jottingInfo":jotting_detail})
                    }
                }
            }
        }

        ListView{
            id:messageList
            anchors.fill: parent
            delegate:messageComponent
            model:messagesInfo
        }




    }



}
