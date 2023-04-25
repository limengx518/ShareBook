import QtQuick
import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import Qt5Compat.GraphicalEffects

//目录的导入
import "./mediaplayer" as QLL
Item {
    id:videoPage
    readonly property int rootWidth: videoPage.width
    readonly property int rootHeight: videoPage.height

    property var videosInfo: JSON.parse(pushVideos.videos).videos


    Component{
        id:videoComponent
        Rectangle{
            id:rec
            width:  ListView.view.width
            height: ListView.view.height


            Rectangle{
                id:topRec
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
                    }
                }

                Button{
                    id: share_jotting_button
                    width: rootWidth*0.08
                    height: rootWidth*0.08
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

            Popup {
                id: mediaError
                anchors.centerIn: Overlay.overlay
                Text {
                    id: mediaErrorText
                }
            }

            MediaPlayer{
                id:mediaPlayer
//                source: videosInfo[0].path
                videoOutput: videoOutput
                audioOutput: AudioOutput{
                    id:audio
                    muted: playbackControl.muted
                    volume: playbackControl.volume
                }
                onErrorOccurred: { mediaErrorText.text = mediaPlayer.errorString; mediaError.open() }

            }
            VideoOutput {
                id: videoOutput

                property bool fullScreen: false

                anchors.top: parent.top
                anchors.bottom: playbackControl.top
                anchors.left: parent.left
                anchors.right: parent.right

                MouseArea {
                    anchors.fill: parent
                    //双击
                    onDoubleClicked: {
                        parent.fullScreen ?  showNormal() : showFullScreen()
                        parent.fullScreen = !parent.fullScreen
                    }
                    //单击
                    onClicked: {
                        mediaPlayer.stop()
//                        mediaPlayer.source = text_src.text
                        mediaPlayer.source =videosInfo[listView.currentIndex].path
//                        mediaPlayer.source = "https://test-streams.mux.dev/x36xhzz/x36xhzz.m3u8"
                        mediaPlayer.play()
//                        console.log("打印视频路径："+ videosInfo[listView.currentIndex].path)
//                        console.log("当前索引:"+ listView.currentIndex)
    //                    console.log(currentIndex)
                    }
                }
            }

            //底部控件
            QLL.PlaybackControl{
                id: playbackControl
                anchors.bottom: parent.bottom
                anchors.bottomMargin: rootWidth*0.4
//                anchors.bottom: botRec.top
//                anchors.bottomMargin: rootWidth*0.02
                anchors.left: parent.left
                anchors.right: parent.right
                mediaPlayer: mediaPlayer
            }

            Rectangle{
                id:midRec
                width:rootWidth
                height:rootWidth*0.2
                anchors.top: playbackControl.bottom
//                anchors.topMargin: rootWidth*0.02
                anchors.left: parent.left
                anchors.leftMargin: rootWidth*0.02

                Rectangle{
                    id:rec001
                    width:rootWidth
                    height:rootWidth*0.15
                    Rectangle{
                        id:recImg
                        width: rootWidth*0.1
                        height: rootWidth*0.1
                        anchors.verticalCenter: parent.verticalCenter
                        Image {
                            id: head_image
                            smooth: true
                            visible: false
                            anchors.fill: parent
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.centerIn: parent.Center
                            source: "qrc:/images/images/head.png"
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
//                        text: qsTr(".....")
                        text: qsTr(videosInfo[listView.currentIndex].nickName)
                        anchors.left: recImg.right
                        anchors.leftMargin: rootWidth*0.03
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Button{
                    id: concern_button
                    anchors.left: netizen_name.right
                    anchors.leftMargin:  rootWidth*0.03
                    width: rootWidth*0.15
                    height: rootWidth*0.08
                    anchors.verticalCenter: parent.verticalCenter
                    background: Rectangle{
                        anchors.fill: parent
                        color:"red"
                        border.color: "grey"
                        border.width: 1
                        radius: rootWidth*0.15/2
                    }

                    Text{
                        id:concern_button_text
                        text: qsTr("关注")
                        color: "white"
                        anchors.centerIn: parent
                    }
                }

                }

                Text{
                    id:contentText
                    anchors.top: rec001.bottom
                    anchors.topMargin: rootWidth*0.02
                    anchors.bottom: midRec.bottom
                    anchors.bottomMargin: rootWidth*0.02
                    text: qsTr(videosInfo[listView.currentIndex].content)
//                    text: qsTr(".....")
                }

            }
        }
    }



    ListView{
        id:listView
        anchors.fill: parent
        snapMode: ListView.SnapOneItem
        orientation: ListView.Vertical
        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        onCurrentIndexChanged:{
//            console.log("当前索引:"+currentIndex)
//            mediaPlayer.stop()
//            mediaPlayer.source = videosInfo[listView.currentIndex].path
//            mediaPlayer.play()
        }

        model: videosInfo
        delegate: videoComponent
    }
}
