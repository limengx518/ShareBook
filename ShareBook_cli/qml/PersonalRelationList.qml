import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import Qt5Compat.GraphicalEffects

Item {
    id:personalRelationList

    readonly property int rootWidth: personalRelationList.width
    readonly property int rootHeight: personalRelationList.height
    readonly property url personalPage: "PersonalPage.qml"

    property string ownName:"NULL"
    property string dataType:"interest"
    property var interestData
    property var fanData
    property var cycleData
    property var jsonData: interestData

    Component.onCompleted: {
        if(dataType==="interest"){
            jsonData=interestData
            rect_interest.visible=true
            rect_fan.visible=false
            rect_cycle.visible=false
        }else if(dataType==="fan"){
            jsonData=fanData
            rect_interest.visible=false
            rect_fan.visible=true
            rect_cycle.visible=false
        }else{
            jsonData=cycleData
            rect_interest.visible=false
            rect_fan.visible=false
            rect_cycle.visible=true
        }
    }

    Button{
        id:backButton

        Image{
            source:"qrc:/images/images/backbutton.png"
            fillMode: Image.PreserveAspectFit
            sourceSize: Qt.size(30, 30)
        }
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: rootWidth*0.02
        flat: true
        icon.color: "transparent"
        onClicked: {
            bottom_button.visible = true
            loader.source=personalPage_loader
        }
    }

    Text{
        id:netizen_name
        text:ownName
        anchors.top:parent.top
        anchors.topMargin: rootWidth*0.02
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 20
        font.bold: true
    }

    Row{
        id:banner
        spacing: parent.width*0.1
        anchors.top: netizen_name.bottom
        anchors.topMargin: rootWidth*0.03
        anchors.horizontalCenter: parent.horizontalCenter
        ColumnLayout{
            id:interest
            Text{
                id:interestText
                font.pixelSize: 15
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("关注")
            }
            Rectangle{
                id:rect_interest
                width: 30
                height: 5
                radius: 5
                color: "black"
                visible: true
                Layout.alignment: Qt.AlignHCenter
            }
            TapHandler{
                onTapped: {
                    interestText.font.bold = true
                    fanText.font.bold = false
                    cycleText.font.bold =false

                    rect_interest.color = "black"
                    rect_interest.visible=true
                    rect_fan.visible=false
                    rect_cycle.visible=false
                    jsonData=interestData
                    relationListView.update()
                }
            }
        }
        ColumnLayout{
            id:fan
            Text{
                id:fanText
                font.pixelSize: 15
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("粉丝")
            }
            Rectangle{
                id:rect_fan
                width: 30
                height: 5
                radius: 5
                color: "#000000"
                visible: false
                Layout.alignment: Qt.AlignHCenter
            }
            TapHandler{
                onTapped: {
                    fanText.font.bold = true
                    interestText.font.bold = false
                    cycleText.font.bold = false

                    rect_fan.color = "black"
                    rect_interest.visible=false
                    rect_fan.visible=true
                    rect_cycle.visible=false
                    jsonData=fanData
                    relationListView.update()
                }
            }
        }
        ColumnLayout{
            id:cycle
            Text{
                id:cycleText
                font.pixelSize: 15
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("推荐")
            }
            Rectangle{
                id:rect_cycle
                width: 30
                height: 5
                radius: 5
                color: "#000000"
                visible:false
                Layout.alignment: Qt.AlignHCenter
            }
            TapHandler{
                onTapped: {
                    interestText.font.bold = false
                    fanText.font.bold = false
                    cycleText.font.bold = true

                    rect_cycle.color = "black"
                    rect_interest.visible=false
                    rect_fan.visible=false
                    rect_cycle.visible=true
                    jsonData=cycleData
                    relationListView.update()
                }
            }
        }
    }

    Rectangle{
       id:searchBar
       anchors.top: banner.bottom
       anchors.margins:rootWidth*0.03
       anchors.horizontalCenter: parent.horizontalCenter
       anchors.bottomMargin:rootWidth*0.04
       width:parent.width*0.9
       height: 35
       border.color: "white"
       border.width: 1
       color: "#F5F5F5"
       radius: searchBar.height / 2

       Image {
           id: image_search
           sourceSize: Qt.size(20,20)
           anchors.left: parent.left
           anchors.leftMargin: rootWidth*0.02
           anchors.verticalCenter: parent.verticalCenter

           source: "qrc:/images/images/search.png"
       }

       TextInput {
           anchors.right: parent.right
           anchors.leftMargin: rootWidth*0.02
           anchors.verticalCenter: parent.verticalCenter
           anchors.left: image_search.right
           font.pointSize: 18
           color: "#BEBEBE"
           y:8
           text:"搜索用户"
           focus: true
           echoMode: TextInput.Normal
           maximumLength: 30
//           MouseArea  {
//               anchors.fill: parent
//               hoverEnabled: true

//               onEntered: {
//                   searchBar.border.color = "#fc1944"
//               }

//               onExited: {
//                   searchBar.border.color = "#fc5531"
//               }
//           }
       }

       MouseArea  {
           anchors.fill: parent
           hoverEnabled: true

           onEntered: {
           }

           onExited: {
           }
       }
   }


    ScrollView{
        width: rootWidth
        height: rootHeight*0.9
        anchors.top: searchBar.bottom
        anchors.topMargin: rootHeight*0.01
        Component{
            id:relationComponent
            Rectangle{
                id: relation
                width:rootWidth
                height: rootHeight*0.1
                Row{
                    id:rrow
                    anchors.left: parent.left
                    anchors.leftMargin: rootWidth*0.05
                    spacing: rootWidth*0.05

                    Rectangle {
                        id: img
                        width: rootWidth*0.12
                        height: rootWidth*0.12
                        radius: width/2
                        color: "black"

                        Image {
                            id: _image
                            smooth: true
                            visible: false
//                            source: "qrc:/images/images/headpic.png"
                            anchors.fill: parent
                            source: jsonData[index].avatarPath
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

                    Column{
                        spacing: rootWidth*0.02
                        anchors.verticalCenter: parent.verticalCenter
                        Text{
                            width: relation.width
                            text: jsonData[index].nickName
                        }
                        Text{
                            width: relation.width
                            text: jsonData[index].signal
                        }
                    }


                }

                Rectangle{
                    id:split
                    anchors.top: rrow.bottom
                    anchors.topMargin: rootWidth*0.02
                    width:rootWidth
                    height:1
                    color: "#D3D3D3"
                }

                TapHandler{
                    onTapped: {
                        loader.source=jsonData[index].file
                    }
                }
            }
        }


        ListView{
            id: relationListView
            anchors.fill: parent
            model: jsonData
            delegate: relationComponent
        }
    }
}
