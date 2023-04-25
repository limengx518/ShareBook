import QtQuick 2.12

Item{
Rectangle{
    id: toast

    property real time: defaultTime
    readonly property real defaultTime: 3000
    readonly property real fadeTime: 300

    property real margin: 10
    property bool selfDestroying: false

    property real rootWidth
    property real rootHeight

    z:1

    width: infomation.width+rootWidth*0.1
    height: rootHeight*0.05
    radius: 45
    opacity: 0
    color: "#000000"

    anchors.horizontalCenter: parent.horizontalCenter

    Text{
        id:infomation
        text:""
        anchors.centerIn: parent
        font.pixelSize: 18
        color: "white"
    }

    SequentialAnimation on opacity {
        id: animation

        running:false
        NumberAnimation{
            to:0.9
            duration: fadeTime
        }

        PauseAnimation {
            duration: time-2*fadeTime
        }
        NumberAnimation{
            to:0
            duration: fadeTime
        }
        onRunningChanged: {
            if(!running&&selfDestroying){
                toast.destroy()
            }
        }
    }

    function show(text,duration){
        infomation.text=text;
        if(typeof duration!=="undefined"){
            if(duration>=2*fadeTime){
                time=duration
            }else{
                time=2*fadeTime
            }
        }else{
            time=defaultTime
        }
        animation.start()
    }
}
}
