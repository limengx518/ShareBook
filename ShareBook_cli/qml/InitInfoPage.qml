import QtQuick
import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Item {
    id:initInfoPage
    readonly property int rootWidth: initInfoPage.width
    readonly property int rootHeight: initInfoPage.height

    Rectangle {
        id: logo
        width: rootWidth*0.3
        height: rootWidth*0.3
        anchors.horizontalCenter:parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: rootWidth*0.4
        radius: width/2
        color: "transparent"

        Image {
            id: img
            smooth: true
            visible: false
            anchors.fill: parent
            anchors.centerIn: parent.Center
            source: "qrc:/images/images/app.png"
            sourceSize: Qt.size(parent.size*0.8, parent.size*0.8)
            antialiasing: true
        }
        Rectangle {
            id: mask
            color: "black"
            anchors.fill: parent
            radius: width/2
            visible: false
            antialiasing: true
            smooth: true
        }
        OpacityMask {
            id:mask_image
            anchors.fill: img
            source: img
            maskSource: mask
            visible: true
            antialiasing: true
        }
    }



    ColumnLayout{
        id:midCol
        anchors.top:logo.bottom
        anchors.topMargin: rootWidth*0.2
        anchors.horizontalCenter:parent.horizontalCenter
        Row {
           spacing: 10

           Rectangle{
               width: rootWidth*0.3
               height: rootWidth*0.1
               Text {
                   anchors.fill: parent
                    text: qsTr("请输入ID：") ;
                    font.pointSize: 15
                    verticalAlignment: Text.AlignVCenter
               }
           }

           Rectangle {
               width: rootWidth*0.3
               height: rootWidth*0.1
               border.color: "grey"
               radius: 10

               TextInput{
                   id:idInput
                   anchors.fill: parent
                   anchors.margins: 2
                   font.pointSize: 14
                   focus: true
                   maximumLength: 6 //限制字符个数
               }
           }
       }


        Row {
           spacing: 10
           Rectangle{
               width: rootWidth*0.3
               height: rootWidth*0.1
               Text {
                    text: qsTr("请输入IPADDR：") ;
                    font.pointSize: 15
                    verticalAlignment: Text.AlignVCenter
               }
           }

           Rectangle {  //TextInput的可视化外观
               width: rootWidth*0.3
               height: rootWidth*0.1
               border.color: "grey"
               radius: 10

               TextInput{
                   id:ipInput
                   anchors.fill: parent
                   anchors.margins: 2
                   font.pointSize: 14
                   focus: true
                   maximumLength: 15 //限制字符个数
               }
           }
       }
    }
    Button{
         id:submit
         anchors.top: midCol.bottom
         anchors.topMargin: rootWidth*0.05
         anchors.horizontalCenter:parent.horizontalCenter
         text:qsTr("提交")
         background: Rectangle{
             anchors.fill: parent
             color:"transparent"
             border.color: "grey"
             border.width: 1
             radius: 10
         }
         width: rootWidth*0.3
         height: rootWidth*0.1
         onClicked: {
             console.log(idInput.text)
             console.log(ipInput.text)
             netizen.initClientInfo(idInput.text,ipInput.text)
             pushJottings.pushRecommendJottings()
             loader.setSource(pushPage_loader)
             bottom_button.visible = true
         }
    }
}
