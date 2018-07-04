import QtQuick 2.0

Item {
    id:root

    property real margin : 0.4
    property alias text: text.text
    property alias source: img.source

    //字体
    FontLoader { id: font1; source: "font/huawencaiyun.ttf" }
    //变量
    property int fontsize: 75
    property string fontname: font1.name


    Rectangle{
        id:btn
        anchors.fill: parent
        color: "transparent"

        Image{
            id: img
            anchors.fill: parent
            source: "qrc:/pics/button-play.png"
        }
        Text{
            id: text
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.width * root.margin
            font.family: fontname
            font.pixelSize: fontsize
            font.weight: Font.Bold
        }
    }

}
