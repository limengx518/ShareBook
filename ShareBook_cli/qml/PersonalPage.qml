import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
//import QtGraphicalEffects 1.0

Item {
    id:personalPage
    anchors.fill: parent

    readonly property int rootWidth: personalPage.width
    readonly property int rootHeight: personalPage.height
    readonly property url personalJottingList: "PersonalJottingList.qml"
    readonly property url personalRelationList: "PersonalRelationList.qml"

    property var jsonData:JSON.parse(netizen.jottings)
    property var jotting_detail

    property var netizenName: netizen.nickName
    property var netizenAvatar: netizen.avatar

    ScrollView{
        anchors.fill: parent
        Rectangle{
            width:parent.width

            RowLayout{
                id:otherButtonRow
                anchors.top: parent.top
               anchors.topMargin:  rootWidth*0.01
//                topPadding:rootWidth*0.02
//                leftPadding : rootWidth*0.02

                Button {
                    id: otherSelect_button
                    icon.source: "qrc:/images/images/myotherselect.png"
//                    sourceSize: Qt.size(30, 30)
                    icon.width: rootWidth*0.05
                    icon.height: rootWidth*0.05
                    width: icon.width
                    height: icon.height
                    flat: true
                    icon.color: "transparent"
                    onClicked: {
                    }
                }

                Item{
                    width:rootWidth*0.65
                    height:rootWidth*0.1
                }

                Button {
                    id: share_button
                    icon.source: "qrc:/images/images/transit.png"
                    icon.width: rootWidth*0.06
                    icon.height: rootWidth*0.06
                    width: icon.width
                    height: icon.height
                    Layout.rightMargin:rootWidth*0.02
                    flat: true
                    icon.color: "transparent"
                    onClicked: {
                    }
                }
            }

            RowLayout{
                id:headPic_name
                width:rootWidth
                anchors.top:otherButtonRow.bottom
                anchors.left: parent.left
                anchors.leftMargin: rootWidth*0.05
                spacing: rootWidth*0.001


                Rectangle {
                    id: img
                    width: rootWidth*0.2
                    height: rootWidth*0.2
                    radius: width/2
                    color: "black"

                    Image {
                        id: _image
                        smooth: true
                        visible: false
                        anchors.fill: parent
                        source: "image://photos/avatar"
//                        source: "qrc:/images/images/headpic.png"
                        sourceSize: Qt.size(parent.size, parent.size)
                        antialiasing: true
                    }
                    Rectangle {
                        id: _mask
                        color: "black"
                        anchors.fill: parent
                        radius: width/2
                        visible: false
                        antialiasing: true
                        smooth: true
                    }
                    OpacityMask {
                        id:mask_image
                        anchors.fill: _image
                        source: _image
                        maskSource: _mask
                        visible: true
                        antialiasing: true
                    }
                }

                ColumnLayout{
                    id:headRight
                    //昵称
                    Text {
                        id: netizen_name
                        font.pixelSize: 25
                        text: qsTr(netizen.nickName)
                    }
                    Text{
                        id:netizenid
                        font.pixelSize: 20
                        color:"#DCDCDC"
                        text: qsTr("id号: ") + qsTr("数字")
                    }
                }
            }

            //个签
            Text {
                id: netizen_sign
                anchors.top: headPic_name.bottom
                anchors.left: parent.left
                anchors.topMargin: rootWidth*0.02
                anchors.leftMargin: rootWidth*0.03
                text: qsTr(netizen.sign)
            }


            //关注 粉丝 获赞与收藏
            RowLayout{
                id:followRow
                anchors.top: netizen_sign.bottom
                anchors.topMargin: rootWidth*0.05
                spacing: rootWidth*0.01
                //关注的人
                ColumnLayout{
                    id:interest
                    spacing: rootWidth*0.01
                    Layout.leftMargin: rootWidth*0.03
                    Layout.rightMargin: rootWidth*0.06
                    Text {
                        id: interest_count
                        font.pixelSize:15
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr(netizen.interestCount)
                    }
                    Text {
                        font.pixelSize:12
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("关注")
                    }
                    TapHandler{
                        onTapped: {
//                            netizen.initRelationData()
                            bottom_button.visible = false
                            loader.setSource(personalRelationList,{"ownName":netizen.nickName,"dataType":
                                                 "interest","interestData":JSON.parse(netizen.interest),
                                                 "fanData":JSON.parse(netizen.fans),
                                                 "cycleData":JSON.parse(netizen.interest)})
                        }
                    }
                }
                //粉丝
                ColumnLayout{
                    id:fan
                    spacing: rootWidth*0.01
                    Layout.rightMargin: rootWidth*0.06
                    Text {
                        id: fan_count
                        font.pixelSize:15
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr(netizen.fanCount)
                    }
                    Text {
                        font.pixelSize:12
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("粉丝")
                    }
                    TapHandler{
                        onTapped: {
                            bottom_button.visible = false
                            loader.setSource(personalRelationList,{"ownName":netizen.nickName,"dataType":"fan","interestData":JSON.parse(netizen.interest),"fanData":JSON.parse(netizen.fans),"cycleData":JSON.parse(netizen.interest)})
                        }
                    }
                }
                //获赞与收藏
                ColumnLayout{
                    id:like_collect
                    spacing: rootWidth*0.01
                    Layout.rightMargin: rootWidth*0.06
                    Text {
                        id: cycle_count
                        font.pixelSize:15
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr(netizen.interestCount)
                    }
                    Text {
                        font.pixelSize:12
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("获赞与收藏")
                    }
                    TapHandler{
                        onTapped: {
                            bottom_button.visible = false
                            loader.setSource(personalRelationList,{"ownName":netizen.nickName,"dataType":"cycle","interestData":JSON.parse(netizen.interest),"fanData":JSON.parse(netizen.fans),"cycleData":JSON.parse(netizen.interest)})
                        }
                    }
                }

                Item{
                    width:rootWidth*0.09
                    height: rootWidth*0.03
                }

                Button{
                    id:editInfo
                    background: Rectangle{
                        width:rootWidth*0.2
                        anchors.horizontalCenter: parent.horizontalCenter;
                        anchors.verticalCenter: parent.verticalCenter;
                        height: rootWidth*0.05
                        radius: rootWidth*0.15
                        color: "#DCDCDC"
                        opacity: 0.7
                    }
                    text: qsTr("编辑资料")
                    Layout.rightMargin: rootWidth*0.03
                    font.pixelSize: 15
                    onClicked: {
                        console.log("编辑资料被点击啦！")
                    }
                }

                Item{
                    width:rootWidth*0.03
                    height: rootWidth*0.03
                }

               Button{
                    id:set
                    background: Rectangle{
                        anchors.fill: parent
//                        anchors.horizontalCenter: parent.horizontalCenter;
//                        anchors.verticalCenter: parent.verticalCenter;
//                        radius: rootWidth*0.1
//                        color: "#DCDCDC"
//                        opacity: 0.7
                    }
                    Layout.rightMargin: rootWidth*0.03
                    Image {
                        id: setImg
                        anchors.horizontalCenter: parent.horizontalCenter;
                        anchors.verticalCenter: parent.verticalCenter;
                        fillMode: Image.PreserveAspectFit
                        sourceSize: Qt.size(25, 25)
                        source:"qrc:/images/images/system_talk.png"
                    }
                    onClicked: {
                        console.log("设置被点击啦！")
                    }
                }
            }

            Rectangle{
                id:seperator_line_1
                width: parent.width
                height: 1
                color: "#000000"
                anchors.top: followRow.bottom
                anchors.topMargin: parent.width*0.03
            }

            Row{
                id:banner
                spacing: parent.width*0.1
                anchors.top: seperator_line_1.bottom
                anchors.topMargin: parent.width*0.01
                anchors.horizontalCenter: parent.horizontalCenter
                //个人已发布的笔记
                ColumnLayout{
                    id:own_jotting
                    spacing: rootWidth*0.01
                    Text{
                        id:ownText
//                        font.family:"Helvetica [Cronyx]"
                        font.bold: true
                        font.pixelSize: 25
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("笔记")
                    }
                    Rectangle{
                        id:rect_own
                        width: 30
                        height: 5
                        radius: 5
                        color: "#000000"
                        visible: true
                        Layout.alignment: Qt.AlignHCenter
                    }
                    TapHandler{
                        onTapped: {
                            rect_own.visible=true
                            rect_like.visible=false
                            rect_collect.visible=false
                            ownText.font.bold=true
                            collectText.font.bold = false
                            likeText.font.bold = false
                            jsonData=JSON.parse(netizen.jottings)
                        }
                    }
                }
               //个人收集的笔记
                ColumnLayout{
                    id:collect_jotting
                     spacing: rootWidth*0.01
                    Text{
                        id:collectText
                        font.bold: false
                        font.pixelSize: 25
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("收藏")
                    }
                    Rectangle{
                        id:rect_collect
                        width: 30
                        height: 5
                        radius: 5
                        color: "#000000"
                        visible:false
                        Layout.alignment: Qt.AlignHCenter
                    }
                    TapHandler{
                        onTapped: {
                            rect_own.visible=false
                            rect_like.visible=false
                            rect_collect.visible=true
                            ownText.font.bold=false
                            collectText.font.bold = true
                            likeText.font.bold = false
                            jsonData=JSON.parse(netizen.jottings)
                        }
                    }
                }
                //个人点赞的笔记
                ColumnLayout{
                    id:like_jotting
                     spacing: rootWidth*0.01
                    Text{
                        id:likeText
                        font.bold: false
                        font.pixelSize: 25
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("赞过")
                    }
                    Rectangle{
                        id:rect_like
                        width: 30
                        height: 5
                        radius: 5
                        color: "#000000"
                        visible: false
                        Layout.alignment: Qt.AlignHCenter
                    }
                    TapHandler{
                        onTapped: {
                            rect_own.visible=false
                            rect_like.visible=true
                            rect_collect.visible=false
                            ownText.font.bold=false
                            collectText.font.bold = false
                            likeText.font.bold = true
                            jsonData=JSON.parse(netizen.jottings)
                        }
                    }
                }
            }

            Rectangle{
                id:seperator_line_2
                width: parent.width
                height: 1
                color: "#000000"
                anchors.top: banner.bottom
                anchors.topMargin: parent.width*0.02
            }

            ScrollView{
                width:rootWidth
                height: rootHeight*0.5
                anchors.top: seperator_line_2.bottom
                anchors.left: parent.left
                anchors.topMargin: rootWidth*0.02
                anchors.leftMargin: rootWidth*0.03
                Component{
                    id:jotComponent
                    Rectangle{
                        id: jot
                        width:jotsListView.width*0.465
                        height: jotsListView.height*0.65
                        border.color: "#000000"
                        border.width:1
                        radius: 5
                        ColumnLayout{
                            spacing: rootHeight*0.005
                            anchors.fill: parent
                            Rectangle{
                                id:picRec
                                width:jot.width*0.9
                                height:jot.height*0.8
                                Layout.topMargin: rootWidth*0.02
                                Layout.leftMargin: rootWidth*0.01
                                Layout.rightMargin: rootWidth*0.01
                                Image {
                                    id: pic
                                    anchors.fill: parent
//                                    source:"qrc:/images/images/head.png"
                                    source: jsonData[index].picPath
                                    fillMode: Image.PreserveAspectFit
                                    asynchronous: true
                                    cache: true
                                    smooth:true
                                }
                            }

                            Rectangle{
                                id:rec
                                width: jot.width
                                height:1
                                color: "#000000"
                            }
                            Text {
                                height:jotsListView.height*0.65*0.1
                                width:jot.width*0.8
                                font.pixelSize: 12
                                id: text_content
                                text: jsonData[index].content
                                wrapMode: Text.WordWrap
                                leftPadding:5
//                                clip:true
//                                elide: Text.ElideRight
                            }
                        }
                        TapHandler{
                            onTapped: {
                                console.log("打印点到的笔记ID为："+jsonData[index].jottingId)
                                bottom_button.visible = false

                                netizen.getOneJottingDetail(jsonData[index].jottingId)

                                jotting_detail = JSON.parse(netizen.jotting)

                                loader.setSource(jsonData[index].file,{"type":"self","jottingInfo":jotting_detail})
                            }
                        }
                    }
                }

                GridView{
                    id: jotsListView
                    anchors.fill: parent
                    flow: GridView.FlowLeftToRight
                    cellWidth: jotsListView.width*0.475
                    cellHeight: jotsListView.height*0.665
                    model: jsonData
                    delegate: jotComponent
                }
            }
        }
    }


}
