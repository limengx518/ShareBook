import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick

Item {
    id:choosePage

    readonly property int rootWidth: choosePage.width
    readonly property int rootHeight: choosePage.height

//    property var materialItem:[]
    property var materialPath:[]
    property string content_property: ""

    property var jottingInfo:{"netizenName":"","time":"0000-00-00 00:00","content":"","picPath":[],"comment":[],"likeCount":"0","collectCount":"0","commentCount":"0"}

    function clearJotInfo(){
        for(var i=0;i<materialPath.length;i++){
            jottingInfo.picPath.pop()
            jottingInfo.comment.pop()
        }
    }

    function saveJotInfo(){
        var pathString=""
        for(var i=0;i<materialPath.length-1;i++){
            pathString+=materialPath[i]+","
        }
        pathString+=materialPath[materialPath.length-1]
        publishControl.publishJotting(pathString,content_edit.text,netizen.nickName,"qrc:/images/images/avatar.png")
    }

    Button{
        id:backButton
        icon.source: "qrc:/images/images/backbutton.png"
        icon.width: rootWidth*0.1
        icon.height: rootWidth*0.1
        width: icon.width
        height: icon.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: rootWidth*0.02
        flat: true
        onClicked: {
            loader.setSource(chooseMaterialPage_loader,{"selectMaterialData":materialPath,"materialCount":materialPath.length})
            bottom_button.visible = false
        }
    }

    Rectangle{
        id:preview_button
        radius: 45
        border.width: 1
        border.color: "black"
        width: rootWidth*0.2
        height: rootHeight*0.04
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: rootWidth*0.03
        Text {
            anchors.centerIn: parent
            text: qsTr("Preview")
        }
        TapHandler{
            onTapped: {
                clearJotInfo()
                saveJotInfo()
                loader.setSource(jotDetailPage_loader,{"jottingInfo":jottingInfo,"type":"preview"})
            }
        }
    }

    Rectangle{
        id:seperator_line_1
        width: parent.width
        height: 1
        color: "#000000"
        anchors.top: backButton.bottom
        anchors.topMargin: parent.width*0.02
    }

    Component{
        id:materialComponent
        Rectangle{
            id: material
            width:rootWidth*0.2
            height: width
            Image{
                width:parent.width
                height: width
                source: materialPath[index].path
                IconImage{
                    width: parent.width*0.2
                    height: width
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.topMargin: -parent.width*0.05
                    anchors.rightMargin: -parent.width*0.05
                }
            }

            TapHandler{
                onTapped: {

                }
            }
        }
    }

    ListView {
        id: materialsListView
        width: rootWidth
        height: rootHeight*0.15
        anchors.top: seperator_line_1.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: rootWidth*0.03
        anchors.leftMargin: rootWidth*0.03
        anchors.rightMargin: rootWidth*0.03
        orientation: ListView.Horizontal
        spacing: parent.width*0.03
        model: materialPath
        delegate: materialComponent
    }

    Rectangle{
        width: rootWidth
        height: rootHeight*0.4
        anchors.top: materialsListView.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: rootWidth*0.03
        anchors.rightMargin: rootWidth*0.03
        border.width: 1
        border.color: "grey"
        TextArea{
            id:content_edit
            anchors.fill: parent
            wrapMode: TextEdit.WrapAnywhere
            topPadding:rootWidth*0.01
            leftPadding:rootWidth*0.01
            rightPadding:rootWidth*0.01
            placeholderText:qsTr("Enter Content")
            placeholderTextColor: "grey"
            text:content_property
        }
    }

    Rectangle{
        id:publish_button
        radius: 45
        border.width: 1
        border.color: "black"
        width: rootWidth*0.4
        height: rootHeight*0.05
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: rootWidth*0.1
        Text {
            anchors.centerIn: parent
            text: qsTr("Publish")
        }
        TapHandler{
            onTapped: {
                clearJotInfo()
                saveJotInfo()
//                loader.source=
            }
        }
    }

}
