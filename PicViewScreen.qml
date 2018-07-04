import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "content/SettingLogic.js" as SLogic

Item {
    id: picViewScreen
    width: SLogic.applyWScale(Screen,SLogic.refWidth); height: SLogic.applyHScale(Screen,SLogic.refHeight)

    signal backButtonClicked
    property int space: 40
    property int i: 0
    property int j: 0
    property string pic0_0: "pics/grade/return_0.png"
    property string pic0_1: "pics/grade/return_1.png"
    property string pic0_2: "pics/grade/return_2.png"
    property string pic_return: "pics/return.png"


    function setPics()
    {
        model.clear()
        for( i = 1; i < SLogic.highestGrade+1;i++ )
        {
            var level = 10;
            if(i == SLogic.highestGrade)
                level = SLogic.highestLevel;
            for( j = 1; j < level;j++ )
            {
                model.append({url:"qrc:/pics/beautys/"+i.toString()+j.toString()+".jpg"})
            }
        }
    }
    function setTestPics()
    {
        model.clear()
        for( i = 1; i < 20;i++ )
        {
            model.append({url:"qrc:/pics/test/"+i.toString()+".png"})
        }
    }

    LLSoundEffect {
        id: click_sound
        source: "./sound/click.wav"
    }

    Rectangle {
            id: back_rec
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            color:"red"

            ListModel{
                   id:model
            }
            CoverFlow{
                    id:coverFlowXXXX
                    anchors.fill:parent
                    model:model
            }
    }
    //此处放开，游戏启动变慢，增加了耗时
    /*Component.onCompleted:
    {
        model.clear()
        for( i = 1; i < 7;i++ )
        {
            for( j = 1; j < 10;j++ )
            {
                model.append({url:"qrc:/pics/beautys/"+i.toString()+j.toString()+".jpg"})
            }
        }
    }*/
    //背景图片
    /*Rectangle {
        id: back_rec
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color:"red"
        Image {
            id: back_image
            x:0
            y:0
            width: parent.width
            height: parent.height
            source: "pics/button-help.png"
        }
    }*/
    /*返回*/
    Rectangle {
        id: return_rec
        x: space/4
        y: space/4
        width: parent.width/9//space
        height: parent.height/16//space
        color: "#00ffffff"

        Image {
            id: return_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic0_0
        }
        MouseArea {
            id: return_mouseArea
            anchors.fill:parent
            hoverEnabled: true
            onEntered:
            {
                return_image.source = pic0_1
            }
            onExited:
            {
                return_image.source = pic0_0
            }
            onPressed:
            {
                click_sound.play();
                return_image.source = pic0_2
                backButtonClicked();
            }
            onReleased:
            {
                return_image.source = pic0_0
            }
        }
    }

}
