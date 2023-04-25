import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import Qt5Compat.GraphicalEffects


Item {
    id:pushPage

    readonly property int rootWidth: pushPage.width
    readonly property int rootHeight: pushPage.height
    readonly property url personalPage: "JotDetailPage.qml"


    property bool isGetConcerned: false
    property bool isGetLocal: false

    property var recommend_jottings:JSON.parse(pushJottings.recommendJottings).jottings
    property var concerned_jottings
    property var local_jottings
    property var jottings: recommend_jottings



    property var jotting_detail
    //笔记索引
    property var jotIndex


    Row{
        id:top_banner
        spacing: parent.width*0.08
        anchors.top: parent.top
        anchors.topMargin: parent.height*0.02
        anchors.horizontalCenter: parent.horizontalCenter
        ColumnLayout{
            id:interest
            Text{
                id:interestText
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("关注")
            }
            Rectangle{
                id:rect_interest
                width: 30
                height: 5
                radius: 5
                color: "#000000"
                visible: false
                Layout.alignment: Qt.AlignHCenter
            }
            TapHandler{
                onTapped: {
                    if(!isGetConcerned){
                        pushJottings.pushConcernedJottings()
                        concerned_jottings=JSON.parse(pushJottings.concernedJottings).jottings
                        isGetConcerned=true
                    }
                    interestText.font.bold = true
                    commendText.font.bold = false
                    localText.font.bold = false

                    rect_interest.visible=true
                    rect_commend.visible=false
                    rect_local.visible=false
                    jottings=concerned_jottings
                    jottingsListView.update()
                }
            }
        }
        ColumnLayout{
            id:commend
            Text{
                id:commendText
                font.pixelSize: 25
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("推荐")
            }
            Rectangle{
                id:rect_commend
                width: 30
                height: 5
                radius: 5
                color: "#000000"
                visible: true
                Layout.alignment: Qt.AlignHCenter
            }
            TapHandler{
                onTapped: {
                    interestText.font.bold = false
                    commendText.font.bold = true
                    localText.font.bold = false

                    rect_interest.visible=false
                    rect_commend.visible=true
                    rect_local.visible=false
                    jottings=recommend_jottings
                    jottingsListView.update()
                }
            }
        }
        ColumnLayout{
            id:local
            Text{
                id:localText
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("同城")
            }
            Rectangle{
                id:rect_local
                width: 30
                height: 5
                radius: 5
                color: "#000000"
                visible:false
                Layout.alignment: Qt.AlignHCenter
            }
            TapHandler{
                onTapped: {
                    if(!isGetLocal){
                        pushJottings.pushLocalJottings()
                        local_jottings=JSON.parse(pushJottings.localJottings).jottings
                        isGetLocal=true
                    }
                    interestText.font.bold = false
                    commendText.font.bold = false
                    localText.font.bold = true

                    rect_interest.visible=false
                    rect_commend.visible=false
                    rect_local.visible=true
                    jottings=local_jottings
                    jottingsListView.update()
                }
            }
        }
    }
    //搜索
    Button{
        id:search_button
        anchors.top: parent.top
        anchors.topMargin: parent.height*0.025
        anchors.right: parent.right
        anchors.rightMargin: parent.width*0.02
        width:rootWidth*0.09
        height:rootWidth*0.09
        flat: true
        background: Rectangle{
            anchors.fill: parent
        }
        Image {
            id: setImg
            fillMode: Image.PreserveAspectFit
            sourceSize: Qt.size(25, 25)
            source:"qrc:/images/images/search.png"
        }
        onClicked: {}
    }


    ScrollView{
        width: rootWidth
        height: rootHeight*0.85
        anchors.top: search_button.bottom
        anchors.topMargin: rootHeight*0.02
        Component{
            id:jottingComponent
            Rectangle{
                id: jotting
                width:rootWidth
                height:rootHeight*0.5

                Rectangle{
                    id:seperator_line_2
                    width: parent.width
                    height: 1
                    color: "grey"
                    anchors.top: jotting.top
                    anchors.topMargin: parent.height*0.01
                }

                Rectangle{
                    id:head_banner
                    anchors.left: parent.left
                    height:rootWidth*0.1
                    width:rootWidth
                    color:"transparent"
                    anchors.top: seperator_line_2.bottom
                    anchors.topMargin: rootWidth*0.02
                    Rectangle {
                        id: headimg
                        width: rootWidth*0.1
                        height: rootWidth*0.1
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: rootWidth*0.02
                        radius: width/2
                        color: "black"

                        Image {
                            id: head_image
                            smooth: true
                            visible: false
                            anchors.fill: parent
                            anchors.centerIn: parent.Center
//                          source: "qrc:/images/images/headpic.png"
                            source: jottings[index].avatarPath
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
                        id:head_name
                        anchors.left: headimg.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: rootWidth*0.04
                        width: rootWidth*0.7
                        text: qsTr(jottings[index].nickName)
                    }

                    Button{
                        id:other_choice

                        width: rootWidth*0.08
                        height: rootWidth*0.08
                        Image{
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            sourceSize: Qt.size(parent.width*0.8, parent.width*0.8)
                            source:"qrc:/images/images/otherchoice.png"
                        }
                        anchors.left: head_name.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width*0.01
                        flat: true
                        icon.color: "transparent"
                        onClicked: {}
                    }
                }


                SwipeView{
                    id:image_area
                    width: rootWidth
                    height: rootHeight*0.3
                    anchors.top: head_banner.bottom
                    anchors.margins: rootWidth*0.02
                    currentIndex: indicator.currentIndex
                    Repeater{
                        model:jottings[jottingIndex.text].picPath
                        Image{
                            width: rootWidth
                            height: parent.height
                            fillMode: Image.PreserveAspectFit
//                            source:"qrc:/images/images/head.png"
                            source:jottings[jottingIndex.text].picPath[index].path

                            MouseArea{
                                anchors.fill: parent
                                onDoubleClicked: {
                                    like.state == "clicked" ? like.state = "" : like.state = 'clicked';
                                }
                            }
                        }
                    }
                }


                Rectangle{
                    id:content_button
                    width:rootWidth
                    height:rootWidth*0.07
                    anchors.top: image_area.bottom
                    anchors.left: parent.left
                    anchors.margins: rootWidth*0.02
                    anchors.bottomMargin: rootWidth*0.02
                    color: "transparent"

                    Button{
                        id:like
                        anchors.left: parent.left
                        state: ""
                        Image {
                            id:likeimg
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            sourceSize: Qt.size(28, 28)
                            source:"qrc:/images/images/like.png"
                        }
                        width: rootWidth*0.07
                        height: rootWidth*0.07
                        flat: true
                        icon.color: "transparent"
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            like.state == "clicked" ? like.state = "" : like.state = 'clicked';
                        }
                        states: [
                            State {
                                name: ""
                                PropertyChanges {
                                    target: likeimg; source:"qrc:/images/images/like.png"
                                }
                            },
                            State {
                                name: "clicked"
                                PropertyChanges {
                                    target: likeimg; source:"qrc:/images/images/liked.png"
                                }
                            }
                        ]
                    }
                    Button{
                        id:comment
                        anchors.left: like.right
                        anchors.leftMargin: rootWidth*0.03
                        Image {
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            sourceSize: Qt.size(28, 28)
                            source:"qrc:/images/images/comment.png"
                        }
                        anchors.verticalCenter: parent.verticalCenter
                        width: rootWidth*0.07
                        height: rootWidth*0.07
                        flat: true
                        icon.color: "transparent"
                        onClicked: {}
                    }
                    Button{
                        id:transit
                        anchors.left: comment.right
                        anchors.leftMargin: rootWidth*0.03
                        Image {
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            sourceSize: Qt.size(28, 28)
                            source:"qrc:/images/images/transit.png"
                        }
                        anchors.verticalCenter: parent.verticalCenter
                        width: rootWidth*0.07
                        height: rootWidth*0.07
                        flat: true
                        icon.color: "transparent"
                    }

                    PageIndicator {
                        id: indicator
                        count: image_area.count
                        currentIndex: image_area.currentIndex
                        anchors.centerIn: parent
                        interactive: true //可以点击

                    }
                    Button{
                        id:collect
                        state: ""
                        width: rootWidth*0.07
                        height: rootWidth*0.07
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: rootWidth*0.04
                        Image {
                            id:collectImg
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectFit
                            sourceSize: Qt.size(28, 28)
                            source:"qrc:/images/images/collect1.png"
                        }
                        flat: true
                        icon.color: "transparent"
                        Layout.alignment: Qt.AlignVCenter
                        onClicked: {
                            collect.state == "clicked" ? collect.state = "" : collect.state = 'clicked';
                        }
                        states: [
                            State {
                                name: ""
                                PropertyChanges {
                                    target: collectImg; source:"qrc:/images/images/collect1.png"
                                }
                            },
                            State {
                                name: "clicked"
                                PropertyChanges {
                                    target: collectImg; source:"qrc:/images/images/collected.png"
                                }
                            }
                        ]
                    }
                }


                Row{
                    id:content_show
                    width: parent.width
                    anchors.top:content_button.bottom
                    rightPadding:rootWidth*0.05
                    leftPadding:rootWidth*0.03
                    bottomPadding:rootWidth*0.01
                    topPadding:rootWidth*0.01
                    Text{
                        id:content_name
                        text: qsTr(jottings[index].nickName+": ")
                        font.pixelSize: 16
                        font.bold: true
                    }
                    Text{
                        id:jottingIndex
                        text:index
                        visible: false
                    }
                    Text{
                        id:content_text
                        text: qsTr(jottings[index].content)
                        font.pixelSize: 16
                        width: rootWidth*0.8
                        clip:true
                        elide: Text.ElideRight
                    }
                }

                Text{
                    id:jottingData
                    anchors.top:content_show.bottom
//                    text: qsTr("编辑于 "+"XXXX-XX-XX hh:mm:ss")
                    text: qsTr("编辑于 "+jottings[index].time)
                    color:"grey"
                    anchors.left: parent.left
                    font.pixelSize: 12
                    anchors.leftMargin: rootWidth*0.03
                }

                Text{
                    id:check_detail_button
                    anchors.top:jottingData.bottom
                    text: qsTr("查看全文")
//                    font.italic: true
                    color:"#668B8B"
                    rightPadding:rootWidth*0.05
                    bottomPadding:rootWidth*0.05
                    font.pixelSize: 14
                    anchors.right: parent.right
                    TapHandler{
                        onTapped: {}
                    }
                }

                TapHandler{
                    onTapped: {
                        console.log("打印点到的笔记ID为："+jottings[index].id)

                        bottom_button.visible = false
                        pushJottings.receiveOneJotting(jottings[index].id)

                        jotting_detail=JSON.parse(pushJottings.jotting)
                        loader.setSource(jotDetailPage_loader,{"type":"push","jottingInfo":jotting_detail})

                        console.log("===============================网民昵称："+netizen.nikeName)

//                        console.log("显示点到的笔记详情："+jotting_detail)
                    }
                }
            }
        }

        ListView{
            id: jottingsListView
            anchors.fill: parent
            model: jottings
            delegate: jottingComponent
        }
    }
}
