import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "content/SettingLogic.js" as SLogic

Item {
    id: gameSubLevelScreen
    width: SLogic.applyWScale(Screen,SLogic.refWidth); height: SLogic.applyHScale(Screen,SLogic.refHeight)

    signal backButtonClicked
    signal subLevelButtonClicked(int level)

    //字体
    FontLoader { id: font1; source: "font/old_evils.ttf" }
    //变量
    property int fontsize: 55
    property string fontname: font1.name
    property string color1: "#744596"
    property int space: 40
    property int ww: (gameSubLevelScreen.width - space*4)/3
    property int hh: (gameSubLevelScreen.height - space*5)/3
    property string png1: "pics/level/3.png"
    property string pic1: "pics/level/levelBt_0.png"
    property string pic2: "pics/level/levelBt_1.png"
    property string pic3: "pics/level/levelBt_2.png"

    property string pic_return_0: "pics/grade/return_0.png"
    property string pic_return_1: "pics/grade/return_1.png"
    property string pic_return_2: "pics/grade/return_2.png"
    property string pic_lock: "pics/level/levelBt_lock.png"

    function setLevelStatus(i,type)
    {
        var status = true;
        if(type)
            status = !status;
        switch(i)
        {
        case 0:
            grade1_lock.visible = !status;
            grade1_text.visible = status;
            grade1_mouseArea.hoverEnabled = status;
            break;
        case 1:
            grade2_lock.visible = !status;
            grade2_text.visible = status;
            grade2_mouseArea.hoverEnabled = status;
            break;
        case 2:
            grade3_lock.visible = !status;
            grade3_text.visible = status;
            grade3_mouseArea.hoverEnabled = status;
            break;
        case 3:
            grade4_lock.visible = !status;
            grade4_text.visible = status;
            grade4_mouseArea.hoverEnabled = status;
            break;
        case 4:
            grade5_lock.visible = !status;
            grade5_text.visible = status;
            grade5_mouseArea.hoverEnabled = status;
            break;
        case 5:
            grade6_lock.visible = !status;
            grade6_text.visible = status;
            grade6_mouseArea.hoverEnabled = status;
            break;
        case 6:
            grade7_lock.visible = !status;
            grade7_text.visible = status;
            grade7_mouseArea.hoverEnabled = status;
            break;
        case 7:
            grade8_lock.visible = !status;
            grade8_text.visible = status;
            grade8_mouseArea.hoverEnabled = status;
            break;
        case 8:
            grade9_lock.visible = !status;
            grade9_text.visible = status;
            grade9_mouseArea.hoverEnabled = status;
            break;
        }

    }

    function setLevelLocks(grade)
    {
        for(var i = 0;i < 9;i++)
        {
            SLogic.locklevels[i] = true;
            setLevelStatus(i,true);
        }
        if(grade < SLogic.highestGrade)
        {
            for(var i = 0;i < 9;i++)
            {
                SLogic.locklevels[i] = false;
                setLevelStatus(i,false);
            }
        }
        else if(grade == SLogic.highestGrade)
        {
            for(var i = 0;i < SLogic.highestLevel;i++)
            {
                SLogic.locklevels[i] = false;
                setLevelStatus(i,false);
            }

        }

    }

    LLSoundEffect {
        id: click_sound
        source: "./sound/click.wav"
    }

    Rectangle {
        id: back
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "#ffffff"

        Image {
            id: back_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: png1
        }
    }
    /*返回*/
    Rectangle {
        id: return_rec
        x: space/4
        y: space/4
        width: parent.width/9//space
        height: parent.height/16//space
        color: "#00ffffff"

        Image {
            id: pic_return0
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic_return_0
        }
        MouseArea {
            id: return_mouseArea
            anchors.fill:parent
            hoverEnabled: true
            onEntered:
            {
                pic_return0.source = pic_return_1
            }
            onExited:
            {
                pic_return0.source = pic_return_0
            }
            onPressed:
            {
                click_sound.play();
                pic_return0.source = pic_return_2
                backButtonClicked();
            }
            onReleased:
            {
                pic_return0.source = pic_return_0
            }
        }
    }
    /*1*/
    Rectangle {
        id: grade1
        x: parent.width/16
        y: parent.height/6.5*1.5
        width: parent.width/4
        height: parent.height/6.5
        color: "#00ffffff"

        Image {
            id: grade1_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1
        }
        Image {
            id: grade1_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.locklevels[0])
        }
        Text{
            id:grade1_text
            anchors.verticalCenter: parent.verticalCenter
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            color: "#000000"
            text:"1"
            visible:(!(SLogic.locklevels[0]))
        }
        MouseArea {
            id: grade1_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.locklevels[0]))
            onEntered:
            {
                grade1_image.source = pic2
            }
            onExited:
            {
                grade1_image.source = pic1
            }
            onPressed:
            {
                click_sound.play();
                grade1_image.source = pic3
                if(!(SLogic.locklevels[0]))
                {
                    console.log("level 1 clicked")
                    subLevelButtonClicked(1)
                }
            }
            onReleased:
            {
                grade1_image.source = pic1
            }
        }
    }
    /*2*/
    Rectangle {
        id: grade2
        x: grade1.x + grade1.width + grade1.width/4
        y: grade1.y
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade2_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1
        }
        Image {
            id: grade2_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.locklevels[1])
        }
        Text{
            id:grade2_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            color: grade1_text.color
            text:"2"
            visible:(!(SLogic.locklevels[1]))
        }
        MouseArea {
            id: grade2_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.locklevels[1]))
            onEntered:
            {
                grade2_image.source = pic2
            }
            onExited:
            {
                grade2_image.source = pic1
            }
            onPressed:
            {
                click_sound.play();
                grade2_image.source = pic3
                if(!(SLogic.locklevels[1]))
                    subLevelButtonClicked(2)
            }
            onReleased:
            {
                grade2_image.source = pic1
            }
        }
    }
    /*3*/
    Rectangle {
        id: grade3
        x: grade2.x + grade1.width + grade1.width/4
        y: grade1.y
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade3_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1
        }
        Image {
            id: grade3_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.locklevels[2])
        }
        Text{
            id:grade3_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            color: grade1_text.color
            text:"3"
            visible:(!(SLogic.locklevels[2]))
        }
        MouseArea {
            id: grade3_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.locklevels[2]))
            onEntered:
            {
                grade3_image.source = pic2
            }
            onExited:
            {
                grade3_image.source = pic1
            }
            onPressed:
            {
                click_sound.play();
                grade3_image.source = pic3
                if(!SLogic.locklevels[2])
                    subLevelButtonClicked(3)
            }
            onReleased:
            {
                grade3_image.source = pic1
            }
        }
    }
    /*4*/
    Rectangle {
        id: grade4
        x: grade1.x
        y: grade1.y+grade1.height+grade1.height/4
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade4_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1
        }
        Image {
            id: grade4_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.locklevels[3])
        }
        Text{
            id:grade4_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            color: grade1_text.color
            text:"4"
            visible:(!(SLogic.locklevels[3]))
        }
        MouseArea {
            id: grade4_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.locklevels[3]))
            onEntered:
            {
                grade4_image.source = pic2
            }
            onExited:
            {
                grade4_image.source = pic1
            }
            onPressed:
            {
                click_sound.play();
                grade4_image.source = pic3
                if(!SLogic.locklevels[3])
                    subLevelButtonClicked(4)
            }
            onReleased:
            {
                grade4_image.source = pic1
            }
        }
    }
    /*5*/
    Rectangle {
        id: grade5
        x: grade1.x+grade1.width+grade1.width/4
        y: grade4.y
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade5_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1
        }
        Image {
            id: grade5_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.locklevels[4])
        }
        Text{
            id:grade5_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            color: grade1_text.color
            text:"5"
            visible:(!(SLogic.locklevels[4]))
        }
        MouseArea {
            id: grade5_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.locklevels[4]))
            onEntered:
            {
                grade5_image.source = pic2
            }
            onExited:
            {
                grade5_image.source = pic1
            }
            onPressed:
            {
                click_sound.play();
                grade5_image.source = pic3
                if(!SLogic.locklevels[4])
                    subLevelButtonClicked(5)
            }
            onReleased:
            {
                grade5_image.source = pic1
            }
        }
    }
    /*6*/
    Rectangle {
        id: grade6
        x: grade2.x+grade1.width+grade1.width/4
        y: grade4.y
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade6_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1
        }
        Image {
            id: grade6_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.locklevels[5])
        }
        Text{
            id:grade6_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            color: grade1_text.color
            text:"6"
            visible:(!(SLogic.locklevels[5]))
        }
        MouseArea {
            id: grade6_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.locklevels[5]))
            onEntered:
            {
                grade6_image.source = pic2
            }
            onExited:
            {
                grade6_image.source = pic1
            }
            onPressed:
            {
                click_sound.play();
                grade6_image.source = pic3
                if(!SLogic.locklevels[5])
                    subLevelButtonClicked(6)
            }
            onReleased:
            {
                grade6_image.source = pic1
            }
        }
    }
    /*7*/
    Rectangle {
        id: grade7
        x: grade1.x
        y: grade4.y + grade1.height + grade1.height/4
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade7_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1
        }
        Image {
            id: grade7_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.locklevels[6])
        }
        Text{
            id:grade7_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            color: grade1_text.color
            text:"7"
            visible:(!(SLogic.locklevels[6]))
        }
        MouseArea {
            id: grade7_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.locklevels[6]))
            onEntered:
            {
                grade7_image.source = pic2
            }
            onExited:
            {
                grade7_image.source = pic1
            }
            onPressed:
            {
                click_sound.play();
                grade7_image.source = pic3
                if(!SLogic.locklevels[6])
                    subLevelButtonClicked(7)
            }
            onReleased:
            {
                grade7_image.source = pic1
            }
        }
    }
    /*8*/
    Rectangle {
        id: grade8
        x: grade1.x + grade1.width + grade1.width/4
        y: grade7.y
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade8_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1
        }
        Image {
            id: grade8_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.locklevels[7])
        }
        Text{
            id:grade8_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            color: grade1_text.color
            text:"8"
            visible:(!(SLogic.locklevels[7]))
        }
        MouseArea {
            id: grade8_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.locklevels[7]))
            onEntered:
            {
                grade8_image.source = pic2
            }
            onExited:
            {
                grade8_image.source = pic1
            }
            onPressed:
            {
                click_sound.play();
                grade8_image.source = pic3
                if(!(SLogic.locklevels[7]))
                    subLevelButtonClicked(8)
            }
            onReleased:
            {
                grade8_image.source = pic1
            }
        }
    }
    /*9*/
    Rectangle {
        id: grade9
        x: grade2.x + grade1.width + grade1.width/4
        y: grade7.y
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade9_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1
        }
        Image {
            id: grade9_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.locklevels[8])
        }
        Text{
            id:grade9_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            color: grade1_text.color
            text:"9"
            visible:(!(SLogic.locklevels[8]))
        }
        MouseArea {
            id: grade9_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.locklevels[8]))
            onEntered:
            {
                grade9_image.source = pic2
            }
            onExited:
            {
                grade9_image.source = pic1
            }
            onPressed:
            {
                click_sound.play();
                grade9_image.source = pic3
                if(!SLogic.locklevels[8])
                    subLevelButtonClicked(9)
            }
            onReleased:
            {
                grade9_image.source = pic1
            }
        }
    }
}
