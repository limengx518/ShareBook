import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import Qt5Compat.GraphicalEffects

Item {
    id:jotDetailPage

    readonly property int rootWidth: jotDetailPage.width
    readonly property int rootHeight: jotDetailPage.height

    property string type: "detail"

    property var jottingInfo

    property var temp: jottingInfo.comment

    property var netizenName: pushJottings.nickName
    property var netizenAvatar: pushJottings.avatar
    property int commentCnt: jottingInfo.comment.length

//    PushPage{
//        id:personalPage
//        visible: false
//    }

    Rectangle{
        id:netizen_info
        width:rootWidth
        height:rootWidth*0.12
        anchors.left: parent.left

        anchors.top: parent.top
        anchors.topMargin: rootWidth*0.02

        Button{
            id:backButton
            width: rootWidth*0.08
            height: rootWidth*0.08
            anchors.left: parent.left
            anchors.leftMargin: rootWidth*0.02
            anchors.verticalCenter: parent.verticalCenter
            Image {
                id: backImg
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(30, 30)
                source:"qrc:/images/images/backbutton.png"
            }

            flat: true
            icon.color: "transparent"
            onClicked: {
                if(type=="message"){
                    loader.source = messagePage_loader
                     bottom_button.visible = true
                }else if(type=="push"){
                    loader.source=pushPage_loader
                    bottom_button.visible = true
                }else if(type=="self"){
                    loader.source=personalPage_loader
                    bottom_button.visible = true
                }
            }
        }


        Rectangle{
            id:recImg
            width: rootWidth*0.1
            height: rootWidth*0.1
            anchors.left: backButton.right
            anchors.leftMargin:  rootWidth*0.02
             anchors.verticalCenter: parent.verticalCenter
            Image {
                id: head_image
                smooth: true
                visible: false
                anchors.fill: parent
                anchors.verticalCenter: parent.verticalCenter
                anchors.centerIn: parent.Center
                source: jottingInfo.avatar
                sourceSize: Qt.size(parent.size*0.8, parent.size*0.8)
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


        Text{
            id:netizen_name
            text: qsTr(jottingInfo.netizenName)
            anchors.left: recImg.right
            anchors.leftMargin: rootWidth*0.03
            anchors.verticalCenter: parent.verticalCenter
        }


        Button{
            id: concern_button
            anchors.left: netizen_name.right
            anchors.leftMargin:  rootWidth*0.35
            width: rootWidth*0.15
            height: rootWidth*0.08
            anchors.verticalCenter: parent.verticalCenter
            background: Rectangle{
                anchors.fill: parent
                color:"transparent"
                border.color: "grey"
                border.width: 1
                radius: 5
            }

            Text{
                id:concern_button_text
                text: qsTr("关注")
                color: "black"
                anchors.centerIn: parent
            }
        }

        Button{
            id: share_jotting_button
            width: rootWidth*0.08
            height: rootWidth*0.08/*
            anchors.left: concern_button.right
            anchors.leftMargin: rootWidth*0.15*/
            anchors.right: parent.right
            anchors.rightMargin: rootWidth*0.03
            Image {
                id: setImg
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(25, 25)
                source:"qrc:/images/images/transit.png"
            }
            flat: true
            icon.color: "transparent"
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Rectangle{
        id:seperator_line_1
        width: parent.width
        height: 1
        color: "grey"
        anchors.top: netizen_info.bottom
        anchors.topMargin: parent.height*0.01
    }

    ScrollView{
        id:jotting_content
        anchors.top: seperator_line_1.bottom
        anchors.topMargin: rootWidth*0.01
//        anchors.bottom: mark_banner.top
        width: rootWidth
        contentWidth:rootWidth
        contentHeight:image_area.height+indicator.height+content.height+time.height+comment_count.height+rootHeight*0.2+commentListView.contentHeight
        SwipeView{
            id:image_area
            width: rootWidth
            height: rootHeight*0.4
            currentIndex: indicator.currentIndex
            Repeater{
                model:jottingInfo.picPath
                Image{
                    width: rootWidth
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    source:jottingInfo.picPath[index].path
                }
            }
        }

        PageIndicator {
            id: indicator
            anchors.top: image_area.bottom
            anchors.topMargin: rootHeight*0.01
            count: image_area.count
            currentIndex: image_area.currentIndex
            interactive: true //可以点击
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text{
            id:content
            width: rootWidth*0.9
            anchors.top: indicator.bottom
            anchors.left: parent.left
            anchors.margins: rootWidth*0.02
            text:qsTr(jottingInfo.content)
            clip :true
            font.pixelSize: 22
            color: "black"
            wrapMode: Text.WrapAnywhere
            elide: Text.ElideRight
            maximumLineCount:2
        }

        Text{
            id:time
            anchors.top: content.bottom
            anchors.left: parent.left
            anchors.margins: rootWidth*0.02
            text: qsTr(jottingInfo.time)
            font.pixelSize: 18
            color: "grey"
        }

        Rectangle{
            id:seperator_line_2
            width: parent.width
            height: 1
            color: "grey"
            anchors.top: time.bottom
            anchors.topMargin: parent.height*0.01
        }

        Text{
            id:comment_count
            anchors.top: seperator_line_2.bottom
            anchors.left: parent.left
            anchors.margins: rootWidth*0.02
            text: qsTr("共 "+commentCnt +" 条评论")
            font.pixelSize: 18
            color: "grey"
        }

        Component{
            id:comment
            Rectangle{
                width:rootWidth
                Image{
                    id:comment_pic
                    fillMode: Image.PreserveAspectFit
                    source: jottingInfo.comment[index].pic
                    width: rootWidth*0.15
                    height: rootWidth*0.15
                }
                Rectangle{
                    anchors.left:comment_pic.right
                    anchors.margins: rootWidth*0.05
                    Text {
                        id: comment_name
                        font.bold: true
                        font.pixelSize: 20
                        color: "grey"
                        text: qsTr(jottingInfo.comment[index].netizenName)
                    }
                    Text {
                        id: comment_content
                        anchors.top: comment_name.bottom
                        anchors.topMargin: rootWidth*0.01
                        font.pixelSize: 20
                        text: qsTr(jottingInfo.comment[index].content)
                    }
                    Text {
                        id: comment_time
                        anchors.top: comment_content.bottom
                        anchors.topMargin: rootWidth*0.01
                        color: "grey"
                        font.pixelSize: 15
                        text: qsTr(jottingInfo.comment[index].time)
                    }
                }
            }
        }

        ListView{
            id:commentListView
            width: rootWidth
            height: rootHeight
            anchors.top: comment_count.bottom
            anchors.left: parent.left
            anchors.margins: rootWidth*0.02
            model:jottingInfo.comment
            delegate:comment
            spacing:rootHeight*0.1
            clip: true
        }
    }

    Rectangle{
        width: rootWidth
        height: rootHeight*0.05
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: rootWidth*0.02
//        topPadding:rootWidth*0.01
//        color:"red"
//        z:3
        Row{
            id:mark_banner

            spacing: rootWidth*0.04
            Rectangle{
                width: rootWidth*0.35
                height: rootHeight*0.05
                radius: 45
                color:"#DCDCDC"
                Row{
                    anchors.centerIn: parent
                    IconImage{
//                        width:rootWidth*0.1
//                        height:width
                        sourceSize: Qt.size(28, 28)
                        source: "qrc:/images/images/comment.png"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Text{
                        text: qsTr("发布评论")
                        font.pixelSize: 22
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    TapHandler{
                        onTapped: {
                            if(type!=="preview"){
                                jotDetailPage.showComment(edit_comment)
                            }
                        }
                    }
                }
            }
//            Item{
//                width: rootWidth*0.1
//                height: rootHeight*0.05
//            }

            Row{
                id:like_banner
                Button{
                    id: like_button_banner
                    width: rootWidth*0.1
                    height: rootWidth*0.1
                    Image {
                        anchors.horizontalCenter: parent.horizontalCenter;
                        anchors.verticalCenter: parent.verticalCenter;
                        fillMode: Image.PreserveAspectFit
                        sourceSize: Qt.size(25, 25)
                        source:"qrc:/images/images/like.png"
                    }
                    flat: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text{
                    id:like_count_banner
                    width: rootWidth*0.05
                    text: "0"
//                    text: qsTr(jottingInfo.likeCount)
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row{
                id:collect_banner
                Button{
                    id: collect_button_banner
                    width: rootWidth*0.1
                    height: rootWidth*0.1
                    Image {
                        anchors.horizontalCenter: parent.horizontalCenter;
                        anchors.verticalCenter: parent.verticalCenter;
                        fillMode: Image.PreserveAspectFit
                        sourceSize: Qt.size(25, 25)
                        source:"qrc:/images/images/collect.png"
                    }
                    flat: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text{
                    id:collect_count_banner
                    width: rootWidth*0.05
                    text: "0"
//                    text: qsTr(jottingInfo.collectCount)
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row{
                id:comment_banner
                Button{
                    id: comment_button_banner
                    width: rootWidth*0.1
                    height: rootWidth*0.1
                    Image {
                        anchors.horizontalCenter: parent.horizontalCenter;
                        anchors.verticalCenter: parent.verticalCenter;
                        fillMode: Image.PreserveAspectFit
                        sourceSize: Qt.size(25, 25)
                        source:"qrc:/images/images/comment.png"
                    }
                    flat: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text{
                    id:comment_count_banner
                    width: rootWidth*0.05
//                    text: qsTr(jottingInfo.commentCount)
                    text: "0"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    //comment popup
    Popup{
        id:comment_pop
        modal: true
        focus: true
        padding: 0
        closePolicy: Popup.CloseOnEscape|Popup.CloseOnPressOutside
        property var edit: null
        background: Rectangle{
            color: Qt.rgba(0,0,0,0)
        }
        Loader{
            id:loader_edit
            onLoaded: {
                comment_pop.x=(rootWidth - loader_edit.width) * 0.5
                comment_pop.y=rootHeight - loader_edit.height
            }
        }
        onOpened: {
            loader_edit.sourceComponent = comment_pop.edit
        }
        onClosed: {
            loader_edit.sourceComponent = null
            comment_pop.edit = null
        }
    }

    function showComment(edit){
        comment_pop.edit=edit
        comment_pop.open()
    }

    Component{
        id:edit_comment
        Rectangle{
            width: rootWidth
            height: comment_rect.height+rootHeight*0.05
            color: "#DCDCDC"
            Row{
                anchors.centerIn: parent
                spacing: rootWidth*0.02
                Rectangle{
                    id:comment_rect
                    width: rootWidth*0.8
                    height: rootHeight*0.05
                    color: "white"
                    radius: 45
                    anchors.verticalCenter: parent.verticalCenter
                    ScrollView{
                        anchors.fill: parent
                        TextArea{
                            id:comment_edit
                            wrapMode: TextEdit.WrapAnywhere
                            topPadding:rootWidth*0.02
                            leftPadding:rootWidth*0.02
                            rightPadding:rootWidth*0.02
//                            onTextChanged: {
//                                comment_rect.height=comment_edit.height+rootWidth*0.04
//                            }
                        }
                    }
                }
                Button{
                    id:sent_comment
                    text: qsTr("发送")
                    font.pixelSize: 25
                    flat: true
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {
                        if(comment_edit.text===""){
                            toast.anchors.bottomMargin=rootWindow.height*0.5
                            toast.show("请填写内容再发送!")
                            toast.anchors.bottomMargin=rootWindow.height*0.1
                        }else{
//                            netizen.getNickName()
                            console.log("评论的笔记的ID :" +jottingInfo.id +
                                        "\n评论的人是： "+ netizen.nikeName +
                                        "\n评论者头像是： "+ netizen.avatar +
                                        "\n评论的内容是： " +comment_edit.text)

                            jottingInfo.comment.push({"netizenName":netizenName,
                                                      "pic":netizenAvatar,
                                                      "content":comment_edit.text,
                                                      "time":"2022-06-01 14:12"})
                            pushJottings.commentJotting(comment_edit.text,jottingInfo.id)
                            commentListView.model=jottingInfo.comment
                            commentListView.update()
                            comment_pop.close()
                        }
                    }
                }
            }

        }
    }
}
