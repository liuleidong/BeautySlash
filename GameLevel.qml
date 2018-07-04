import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "content/SettingLogic.js" as SLogic

Item {
    id: gameLevelScreen
    width: SLogic.applyWScale(Screen,SLogic.refWidth); height: SLogic.applyHScale(Screen,SLogic.refHeight)

    signal backButtonClicked
    signal levelButtonClicked(int grade)
    //字体
    FontLoader { id: font1; source: "font/old_evils.ttf" }
    //变量
    property int fontsize: 55
    property string fontname: font1.name
    property string color1: "#744596"
    property int space: 40
    property int ww: (gameLevelScreen.width - space*3)/2
    property int hh: (gameLevelScreen.height - space*5)/3
    property string png1: "pics/grade/2.png"

    property string pic0_0: "pics/grade/return_0.png"
    property string pic0_1: "pics/grade/return_1.png"
    property string pic0_2: "pics/grade/return_2.png"

    property string pic1_0: "pics/grade/pic1_0.png"
    property string pic1_1: "pics/grade/pic1_1.png"
    property string pic1_2: "pics/grade/pic1_2.png"

    property string pic2_0: "pics/grade/pic2_0.png"
    property string pic2_1: "pics/grade/pic2_1.png"
    property string pic2_2: "pics/grade/pic2_2.png"

    property string pic3_0: "pics/grade/pic3_0.png"
    property string pic3_1: "pics/grade/pic3_1.png"
    property string pic3_2: "pics/grade/pic3_2.png"

    property string pic4_0: "pics/grade/pic4_0.png"
    property string pic4_1: "pics/grade/pic4_1.png"
    property string pic4_2: "pics/grade/pic4_2.png"

    property string pic5_0: "pics/grade/pic5_0.png"
    property string pic5_1: "pics/grade/pic5_1.png"
    property string pic5_2: "pics/grade/pic5_2.png"

    property string pic6_0: "pics/grade/pic6_0.png"
    property string pic6_1: "pics/grade/pic6_1.png"
    property string pic6_2: "pics/grade/pic6_2.png"

    property string pic_return: "pics/return.png"
    property string pic_lock: "pics/grade/pic1_lock.png"

    function setGradeStatus(lock,text,mouseArea)
    {
        lock.visible = false;
        text.visible = true;
        mouseArea.hoverEnabled = true;
    }

    function setGradeLocks()
    {
        for(var i = 0;i < SLogic.highestGrade;i++)
        {
            SLogic.lockgrades[i] = false;

            switch(i)
            {
            case 0:
                setGradeStatus(grade1_lock,grade1_text,grade1_mouseArea);
                break;
            case 1:
                setGradeStatus(grade2_lock,grade2_text,grade2_mouseArea);
                break;
            case 2:
                setGradeStatus(grade3_lock,grade3_text,grade3_mouseArea);
                break;
            case 3:
                setGradeStatus(grade4_lock,grade4_text,grade4_mouseArea);
                break;
            case 4:
                setGradeStatus(grade5_lock,grade5_text,grade5_mouseArea);
                break;
            case 5:
                setGradeStatus(grade6_lock,grade6_text,grade6_mouseArea);
                break;
            }
        }
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

    /*蝴蝶*/
    property string gif2: "pics/gif/2/"
    property int bbb:1
    Component.onCompleted: {
        timer_gif.start()
    }
    Timer{
        id:timer_gif
        interval: 125
        repeat: true
        running: false
        triggeredOnStart: false
        onTriggered: {
            bbb = (bbb+1)>8?1:bbb+1;
            butterfly_image.source = gif2 + bbb + ".png"
        }
    }

    Rectangle {
        id: butterfly
        x: parent.width/10
        y: parent.height - parent.height/8
        width: parent.width/3
        height: parent.height/8
        color: "#00ffffff"

        Image {
            id: butterfly_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: ""
        }
    }

    LLSoundEffect {
        id: click_sound
        source: "./sound/click.wav"
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
                //timer_gif.stop()
                backButtonClicked();
            }
            onReleased:
            {
                return_image.source = pic0_0
            }
        }
    }
    /*1*/
    Rectangle {
        id: grade1
        x: parent.width/9
        y: parent.height/5
        width: parent.width/3
        height: parent.height/6
        color: "#00ffffff"

        Image {
            id: grade1_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic1_0
        }
        Image {
            id: grade1_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.lockgrades[0])
        }
        Text{
            id:grade1_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            //font.bold: true
            text:qsTr("1")
            visible:(!(SLogic.lockgrades[0]))
        }
        MouseArea {
            id: grade1_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.lockgrades[0]))
            onEntered:
            {
                grade1_image.source = pic1_1
            }
            onExited:
            {
                grade1_image.source = pic1_0
            }
            onPressed:
            {
                click_sound.play();
                grade1_image.source = pic1_2
                if(!(SLogic.lockgrades[0]))
                {
                    levelButtonClicked(1)
                }
            }
            onReleased:
            {
                grade1_image.source = pic1_0
            }
        }
    }
    /*2*/
    Rectangle {
        id: grade2
        x: grade1.x + grade1.width + parent.width/9
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
            source: pic2_0
        }
        Image {
            id: grade2_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.lockgrades[1])
        }
        Text{
            id:grade2_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            //font.bold: true
            text:qsTr("2")
            visible:(!(SLogic.lockgrades[1]))
        }
        MouseArea {
            id: grade2_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.lockgrades[1]))
            onEntered:
            {
                grade2_image.source = pic2_1
            }
            onExited:
            {
                grade2_image.source = pic2_0
            }
            onPressed:
            {
                click_sound.play();
                grade2_image.source = pic2_2
                if(!(SLogic.lockgrades[1]))
                {
                    levelButtonClicked(2)
                }
            }
            onReleased:
            {
                grade2_image.source = pic2_0
            }
        }
    }
    /*3*/
    Rectangle {
        id: grade3
        x: grade1.x
        y: grade1.y + grade1.height
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade3_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic3_0
        }
        Image {
            id: grade3_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.lockgrades[2])
        }
        Text{
            id:grade3_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            //font.bold: true
            text:qsTr("3")
            visible:(!(SLogic.lockgrades[2]))
        }
        MouseArea {
            id: grade3_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.lockgrades[2]))
            onEntered:
            {
                grade3_image.source = pic3_1
            }
            onExited:
            {
                grade3_image.source = pic3_0
            }
            onPressed:
            {
                click_sound.play();
                grade3_image.source = pic3_2
                if(!(SLogic.lockgrades[2]))
                {
                    levelButtonClicked(3)
                }
            }
            onReleased:
            {
                grade3_image.source = pic3_0
            }
        }
    }
    /*4*/
    Rectangle {
        id: grade4
        x: grade2.x
        y: grade2.y + grade2.height
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade4_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic4_0
        }
        Image {
            id: grade4_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.lockgrades[3])
        }
        Text{
            id:grade4_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            //font.bold: true
            text:qsTr("4")
            visible:((SLogic.lockgrades[3]))
        }
        MouseArea {
            id: grade4_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.lockgrades[3]))
            onEntered:
            {
                grade4_image.source = pic4_1
            }
            onExited:
            {
                grade4_image.source = pic4_0
            }
            onPressed:
            {
                click_sound.play();
                grade4_image.source = pic4_2
                if(!(SLogic.lockgrades[3]))
                {
                    levelButtonClicked(4)
                }
            }
            onReleased:
            {
                grade4_image.source = pic4_0
            }
        }
    }
    /*5*/
    Rectangle {
        id: grade5
        x: grade1.x
        y: grade3.y + grade3.height
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade5_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic5_0
        }
        Image {
            id: grade5_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.lockgrades[4])
        }
        Text{
            id:grade5_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            //font.bold: true
            text:qsTr("5")
            visible:((SLogic.lockgrades[4]))
        }
        MouseArea {
            id: grade5_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.lockgrades[4]))
            onEntered:
            {
                grade5_image.source = pic5_1
            }
            onExited:
            {
                grade5_image.source = pic5_0
            }
            onPressed:
            {
                click_sound.play();
                grade5_image.source = pic5_2
                if(!(SLogic.lockgrades[4]))
                {
                    levelButtonClicked(5)
                }
            }
            onReleased:
            {
                grade5_image.source = pic5_0
            }
        }
    }
    /*6*/
    Rectangle {
        id: grade6
        x: grade2.x
        y: grade4.y + grade4.height
        width: grade1.width
        height: grade1.height
        color: "#00ffffff"

        Image {
            id: grade6_image
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            source: pic6_0
        }
        Image {
            id: grade6_lock
            x: 0; y: 0; width: parent.width; height: parent.height;
            source: pic_lock
            visible:(SLogic.lockgrades[5])
        }
        Text{
            id:grade6_text
            anchors.centerIn : parent
            font.family: fontname
            font.pixelSize: fontsize
            //font.bold: true
            text:qsTr("6")
            visible:(!(SLogic.lockgrades[5]))
        }
        MouseArea {
            id: grade6_mouseArea
            anchors.fill:parent
            hoverEnabled: (!(SLogic.lockgrades[5]))
            onEntered:
            {
                grade6_image.source = pic6_1
            }
            onExited:
            {
                grade6_image.source = pic6_0
            }
            onPressed:
            {
                click_sound.play();
                grade6_image.source = pic6_2
                if(!(SLogic.lockgrades[5]))
                {
                    levelButtonClicked(6)
                }
            }
            onReleased:
            {
                grade6_image.source = pic6_0
            }
        }
    }
}
