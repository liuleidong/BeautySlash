import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    signal gamePause;
    //暂停
    Rectangle {
        id:toolPause
        x:tool_space
        width:(parent.width - tool_space*(tool_num+1))/tool_num
        height:parent.height
        color: "transparent"
        Image{
            anchors.fill: parent
            source: "pics/tool/tool_pause.png"
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {
               gamePause();
            }
        }
    }
    //盾牌（生命）
    Rectangle {
        id:toolShield
        visible: false
        x:toolPause.width*3 + tool_space*4
        width:toolPause.width
        height:parent.height
        color: "transparent"
        Image{
            anchors.fill: parent
            source: "pics/tool/tool_shield.png"
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {

            }
        }
        Rectangle {
            id:toolShield_num
            color:"#8bc83c"
            x:parent.width*2/3
            y:parent.height*2/3
            width:parent.width/3
            height:parent.height/3
            radius: parent.width/6
            border.color: "black"
            border.width: 1
            Text {
                anchors.centerIn: parent
                id: toolShield_numText
                text: qsTr("5")
                font.bold: true
                color:"white"
            }
        }
    }
    //炸弹
    Rectangle {
        id:toolBomb
        visible: false
        x:toolPause.width*4 + tool_space*5
        width:toolPause.width
        height:parent.height
        color: "transparent"
        Image{
            anchors.fill: parent
            source: "pics/tool/tool_bomb.png"
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {

            }
        }
        Rectangle {
            id:toolBomb_num
            color:"#8bc83c"
            x:parent.width*2/3
            y:parent.height*2/3
            width:parent.width/3
            height:parent.height/3
            radius: parent.width/6
            border.color: "black"
            border.width: 1
            Text {
                anchors.centerIn: parent
                id: toolBomb_numText
                text: qsTr("5")
                font.bold: true
                color:"white"
            }
        }
    }
    //闪电
    Rectangle {
        id:toolLightning
        visible: false
        x:toolPause.width*5 + tool_space*6
        width:toolPause.width
        height:parent.height
        color: "transparent"
        Image{
            anchors.fill: parent
            source: "pics/tool/tool_lightning.png"
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {

            }
        }
        Rectangle {
            id:toolLightning_num
            color:"#8bc83c"
            x:parent.width*2/3
            y:parent.height*2/3
            width:parent.width/3
            height:parent.height/3
            radius: parent.width/6
            border.color: "black"
            border.width: 1
            Text {
                anchors.centerIn: parent
                id: toolLightning_numText
                text: qsTr("5")
                font.bold: true
                color:"white"
            }
        }
    }
    //剑
    Rectangle {
        id:toolSword
        visible: false
        x:toolPause.width*6 + tool_space*7
        width:toolPause.width
        height:parent.height
        color: "transparent"
        Image{
            anchors.fill: parent
            source: "pics/tool/tool_sword.png"
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {

            }
        }
        Rectangle {
            id:toolSword_num
            color:"#8bc83c"
            x:parent.width*2/3
            y:parent.height*2/3
            width:parent.width/3
            height:parent.height/3
            radius: parent.width/6
            border.color: "black"
            border.width: 1
            Text {
                anchors.centerIn: parent
                id: toolSword_numText
                text: qsTr("5")
                font.bold: true
                color:"white"
            }
        }
    }
}
