import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtMultimedia 5.8

import "content/SettingLogic.js" as SLogic

Item {
    id: newGameScreen
    width: SLogic.applyWScale(Screen,SLogic.refWidth); height: SLogic.applyHScale(Screen,SLogic.refHeight)

    signal startButtonClicked
    signal achievementButtonClicked
    signal soundButtonClicked
    signal musicButtonClicked

    //字体
    FontLoader { id: font1; source: "font/huawencaiyun.ttf" }
    property string fontname: font1.name
    property int fontsize: 40
    property bool setMenu: false

    function _initImageSource()
    {
        menuImg1.source = "pics/setting/sound_1.png";
        menuImg2.source = "pics/setting/music_1.png";
        if(SLogic.sound_switch === true)
            menuImg1.source = "pics/setting/sound.png";
        if(SLogic.music_switch === true)
        {
            menuImg2.source = "pics/setting/music.png";
            playMusic.play();
        }
    }
    function _backMusicOn()
    {
        if(SLogic.music_switch === true)
            playMusic.play();
    }
    function _backMusicOff()
    {
        playMusic.stop();
    }

    //背景图
    Image {
        source: "pics/background.png"
        anchors.fill: parent
    }

    LLSoundEffect {
        id: click_sound
        source: "./sound/click.wav"
    }
    Audio {
              id: playMusic
              source: "./sound/bgm.mp3"
              loops: Audio.Infinite
    }

    //logo todo - 换一个合适的
    Image {
        id: logo
        source: "pics/logo.png"
        anchors.top: parent.top
        anchors.topMargin: SLogic.applyHScale(Screen,60)
        anchors.left: parent.left
        anchors.leftMargin: SLogic.applyWScale(Screen,90)
        visible:false
    }

    //左下角设置
    Rectangle {
        id:loopRect
        width:parent.width//0
        height:parent.width//0
        x:0-width/2
        y:parent.height-height/2
        color:"transparent"
        visible: false
        //PropertyAnimation {id: showLoop; target: loopRect; properties: "width,height";from:0; to: parent.parent.width; duration: 100}
        SpringAnimation{id:showLoop;from:0;to:parent.parent.width;target: loopRect;properties: "width,height";damping: 0.3;epsilon: 0.0005;spring: 3}
        Image {
            id: loopImg
            anchors.fill:parent
            source: "pics/setting/loop.png"
        }
    }
    Rectangle {
        id:menu1
        width:parent.width*0.258819
        height:parent.width
        x:0-width/2
        y:parent.height - height/2
        color:"transparent"
        visible: false
        states:State{name:"rotated";PropertyChanges { target: menu1; rotation:0}}
        transitions: Transition {RotationAnimation{duration: 300;direction: RotationAnimation.Shortest}}

        Rectangle {
            id:menu1Rect
            width:parent.width/2.5
            height:parent.width/2.5
            x:parent.width/2+parent.width/5
            y:parent.height/2/4
            color:"transparent"
            SpringAnimation{id:menu1Animation;from:0;to:(parent.parent.width/2.5*0.258819);target: menu1Rect;properties: "width,height";damping: 0.13;epsilon: 0.0005;spring: 3}
            Image {
                id: menuImg1
                anchors.fill:parent
                source: "pics/setting/sound.png"
            }
            MouseArea{
                anchors.fill:parent
                onPressed: {
                    click_sound.play();
                    SLogic.sound_switch = !SLogic.sound_switch;
                    console.log(SLogic.sound_switch);
                    SLogic.updateSoundSetting();
                    menuImg1.source = (SLogic.sound_switch===true)?"pics/setting/sound.png":"pics/setting/sound_1.png"
                }
            }
        }
    }
    Rectangle {
        id:menu2
        width:parent.width*0.258819
        height:parent.width
        x:0-width/2
        y:parent.height - height/2
        color:"transparent"
        visible: false
        states:State{name:"rotated";PropertyChanges { target: menu2; rotation:30}}
        transitions: Transition {RotationAnimation{duration: 200;direction: RotationAnimation.Shortest}}
        Rectangle {
            id:menu2Rect
            width:parent.width/2.5
            height:parent.width/2.5
            x:parent.width/2+parent.width/5
            y:parent.height/2/4
            color:"transparent"
            SpringAnimation{id:menu2Animation;from:0;to:(parent.parent.width/2.5*0.258819);target: menu2Rect;properties: "width,height";damping: 0.13;epsilon: 0.0005;spring: 3}
            Image {
                id: menuImg2
                anchors.fill:parent
                source: "pics/setting/music.png"
            }
            MouseArea{
                anchors.fill:parent
                onPressed: {
                    click_sound.play();
                    SLogic.music_switch = !SLogic.music_switch;
                    if(SLogic.music_switch === false)
                        playMusic.stop();
                    else
                        playMusic.play();
                    SLogic.updateSoundSetting();
                    menuImg2.source = (SLogic.music_switch===true)?"pics/setting/music.png":"pics/setting/music_1.png"
                }
            }
        }
    }
    Rectangle {
        id:menu3
        width:parent.width*0.258819
        height:parent.width
        x:0-width/2
        y:parent.height - height/2
        color:"transparent"
        visible: false
        states:State{name:"rotated";PropertyChanges { target: menu3; rotation:60}}
        transitions: Transition {RotationAnimation{duration: 200;direction: RotationAnimation.Shortest}}
        Rectangle {
            id:menu3Rect
            width:parent.width/2.5
            height:parent.width/2.5
            x:parent.width/2+parent.width/5
            y:parent.height/2/4
            color:"transparent"
            SpringAnimation{id:menu3Animation;from:0;to:(parent.parent.width/2.5*0.258819);target: menu3Rect;properties: "width,height";damping: 0.13;epsilon: 0.0005;spring: 3}
            Image {
                id: menuImg3
                anchors.fill:parent
                source: "pics/setting/quit.png"
            }
            MouseArea{
                anchors.fill:parent
                onPressed: {
                    //TODO::退出游戏
                    click_sound.play();
                    Qt.quit();
                }
            }
        }
    }
    Rectangle {
        id:settingRect
        width:parent.width/3
        height:parent.width/3
        x:0-width/2
        y:parent.height-height/2
        color:"transparent"
        Image {
            id: settingImg
            anchors.fill:parent
            source: "pics/setting/setting.png"
        }
        MouseArea {
            anchors.fill:parent
            onPressed: {
                showLoop.restart()
                menu1Animation.restart()
                menu2Animation.restart()
                menu3Animation.restart()
                setMenu=(setMenu==false)?true:false
                loopRect.visible=setMenu
                menu1.visible=setMenu
                menu2.visible=setMenu
                menu3.visible=setMenu
                menu1.state = (setMenu==true)?"rotated":""
                menu2.state = (setMenu==true)?"rotated":""
                menu3.state = (setMenu==true)?"rotated":""
            }
        }
    }
    function closeSetting()
    {
        loopRect.visible=false
        setMenu=false
        menu1.visible=false
        menu2.visible=false
        menu3.visible=false
        menu1.state = ""
        menu2.state = ""
        menu3.state = ""
    }
    //其他区域点击后关闭设置菜单
    Rectangle {
        id:loopCloseRect1
        width:parent.width/2
        height:parent.width/2
        x:width
        y:parent.height-height
        color:"transparent"
        visible: true
        MouseArea{
            anchors.fill:parent
            onPressed: {
                closeSetting()
            }
        }
    }
    Rectangle {
        id:loopCloseRect2
        width:parent.width
        height:parent.height - parent.width/2
        x:0
        y:0
        color:"transparent"
        visible: true
        MouseArea{
            anchors.fill:parent
            onPressed: {
                closeSetting()
            }
        }
        //开始游戏
        SLButton{
            id: newGameBtn
            width: parent.width/3
            height: width*0.37
            x:(parent.width - width)/2
            y:(parent.height+parent.width/2)/2 - height/2
            text: qsTr("开始")
            source: "qrc:/pics/button-play.png"
            MouseArea{
                anchors.fill: parent
                onClicked:{
                    click_sound.play();
                    closeSetting();
                    newGameScreen.startButtonClicked()
                }
            }
        }
        //成就
        SLButton{
            id: achievementBtn
            width:newGameBtn.width
            height:newGameBtn.height
            x:newGameBtn.x
            y:newGameBtn.y + height*1.5
            text: qsTr("成就")
            source: "qrc:/pics/button-achievement.png"
            MouseArea{
                anchors.fill: parent
                onClicked:{
                    click_sound.play();
                    closeSetting();
                    newGameScreen.achievementButtonClicked();
                }
            }
        }

        /*
        Image {
               id: newGameBtn
               source: "pics/button-play.png"
               width:parent.width/3
               height:width*0.37
               x:(parent.width - width)/2
               y:(parent.height+parent.width/2)/2 - height/2
               MouseArea {
                   anchors.fill: parent
                   onClicked:{
                       click_sound.play();
                       closeSetting();
                       newGameScreen.startButtonClicked()
                   }
               }
         }

        //成就
        Image {
               id: achievementBtn
               source: "pics/button-achievement.png"
               width:newGameBtn.width
               height:newGameBtn.height
               x:newGameBtn.x
               y:newGameBtn.y + height*1.5
               MouseArea {
                   anchors.fill: parent
                   onClicked:{
                       click_sound.play();
                       closeSetting();
                       newGameScreen.achievementButtonClicked()
                   }
               }
         }
        */
    }
}
