import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

import LL.BPolygon 1.0
import Box2D 2.0


import "levels"
import "content/SettingLogic.js" as SLogic


Item {
    id: mainCanvas

    width: SLogic.applyWScale(Screen,SLogic.refWidth); height: SLogic.applyHScale(Screen,SLogic.refHeight)

    signal backButtonClicked;

    property int tool_space: 10
    property int tool_num:7
    property int game_status: 0

    function _initGameZone(path)
    {
        if(SLogic.isFirstRun === 1)
        {
            tutorial.visible = true;
            SLogic.isFirstRun = 0;
        }
        pauseDialog.visible = false;
        backGround.source = "pics/beautys/" + SLogic.grade + SLogic.level + ".jpg";
        physicsWorld.running = true;
        game_status = 0;
        winDialog.finish();
        SLogic.initGameZone(gameZone,path);
    }
    function _deInitGameZone()
    {
        SLogic.deInitGameZone(gameZone,progressBar);
    }

    function _showPauseDlg()
    {
        tutorial.visible = false;
        physicsWorld.running = false;
        pauseDialog.visible = true;
        game_status = 1;
        showPauseDialog.restart();
    }

    Image {
        id: backGround
        width: mainCanvas.width
        height: mainCanvas.height
        source: {
            var temp = "pics/beautys/" + SLogic.grade + SLogic.level + ".jpg";
            return temp;
        }
    }

    LLSoundEffect {
        id: click_sound
        source: "./sound/click.wav"
    }

    Rectangle{
        id: tutorial
        z: 1501
        visible: false
        color: "transparent"
        width: mainCanvas.width
        height: mainCanvas.height

        Image {
            id: turorialImage
            width: parent.width
            height: parent.height
            source: "qrc:/pics/toturial.png"
            fillMode: Image.PreserveAspectFit
        }
    }

    Rectangle{
        id: winDialog
        z: 1500
        visible: false

        property bool shown: false

        function finish() {
            shown = false
        }

        function open(){
            zoomPic.source = "qrc:/pics/beautys/" + SLogic.grade + SLogic.level + ".jpg";
            oriPic.source = zoomPic.source;
            shown = true
            game_status = 2;
        }

        color: "transparent"
        width: parent.width
        height: parent.height

        states: State{
            name: "shown"; when: winDialog.shown
            PropertyChanges { target: winDialog; visible:true }
        }

        Rectangle{
            id: zoomView
            visible: false
            anchors.fill: parent
            z: 1500

            property bool shown: false

            function finish() {
                shown = false
            }

            function open(){
                shown = true
            }

            Flickable {
                  id: flickView
                  anchors.fill: parent
                  contentWidth: zoomPic.width*3; contentHeight: zoomPic.height*3
                  Image {
                      id: zoomPic;
                      source: {
                          var temp = "qrc:/pics/beautys/" + SLogic.grade + SLogic.level + ".jpg";
                          return temp;
                      }
                      transform: Scale { xScale:3;yScale:3 }
                  }
                  MouseArea{
                      anchors.fill: parent
                      propagateComposedEvents: true
                      onDoubleClicked: {
                          console.log("double flick clicked")
                          zoomView.finish();
                      }
                  }
            }

            states: State{
                name: "shown"; when: zoomView.shown
                PropertyChanges { target: zoomView; visible:true }
            }
        }

        Rectangle{
            id: normalView
            anchors.fill: parent

            Image{
                id: oriPic
                anchors.fill: parent
                source: {
                    var temp = "qrc:/pics/beautys/" + SLogic.grade + SLogic.level + ".jpg";
                    return temp;
                }
            }
        }


        SLButton{
            id: retBtn
            width: parent.width / 4
            height: SLogic.applyHScale(Screen,60)
            x:parent.width / 16
            y:parent.height - height*1.1
            margin: 0.4
            fontsize: 55
            text: qsTr("返回")
            source: "qrc:/pics/success_back.png"
            MouseArea{
                anchors.fill:parent
                onClicked: {
                    click_sound.play();
                    SLogic.deInitGameZone(gameZone,progressBar);
                    winDialog.finish();
                    backButtonClicked();
                }
            }
        }

        SLButton{
            id: retryBtn
            width: parent.width / 4
            height: SLogic.applyHScale(Screen,60)
            x:retBtn.x + retBtn.width + parent.width / 16
            y:retBtn.y
            margin: 0.3
            fontsize: 55
            text: qsTr("重试")
            source: "qrc:/pics/success_retry.png"
            MouseArea{
                anchors.fill:parent
                onClicked: {
                    click_sound.play();
                    SLogic.gradeLevelDecrease();
                    var tmpPath = SLogic.levelPath + SLogic.grade + SLogic.level + ".qml";
                    SLogic.deInitGameZone(gameZone,progressBar);
                    SLogic.initGameZone(gameZone,tmpPath);
                    winDialog.finish();
                }
            }
        }

        SLButton{
            id: nextBtn
            width: parent.width / 4
            height: SLogic.applyHScale(Screen,60)
            x:retryBtn.x + retBtn.width + parent.width / 16
            y:retBtn.y
            margin: 0.15
            fontsize: 55
            text: qsTr("下一关")
            source: "qrc:/pics/success_next.png"
            MouseArea{
                anchors.fill:parent
                onClicked: {
                    click_sound.play();
                    var tmpPath = SLogic.levelPath + SLogic.grade + SLogic.level + ".qml";
                    SLogic.deInitGameZone(gameZone,progressBar);
                    backGround.source = "pics/beautys/" + SLogic.grade + SLogic.level + ".jpg";
                    SLogic.initGameZone(gameZone,tmpPath);
                    winDialog.finish();
                }
                onDoubleClicked: {
                    viewer.zoomViewOn();
                }
            }
        }

        MouseArea{
            anchors.fill: parent
            propagateComposedEvents: true
            onDoubleClicked: {

                var ws = oriPic.sourceSize.width * 3 / oriPic.width;
                var hs = oriPic.sourceSize.height * 3 / oriPic.height;

                var rx = mouse.x * ws;
                var ry = mouse.y * hs;
                flickView.contentX = rx - mouse.x;
                flickView.contentY = ry - mouse.y;

                if(zoomView.shown)
                    zoomView.finish();
                else
                    zoomView.open();
            }
        }
    }

    Rectangle {
      id: progressZone
      color: "transparent"
      width: mainCanvas.width
      height: SLogic.applyHScale(Screen,35)
      anchors.top: parent.top
      GameProgressBar{
          id: progressBar
          width: parent.width
          height: parent.height
      }
    }

    Rectangle {
      id: gameZone
      color: "transparent"
      width: mainCanvas.width
      height: SLogic.applyHScale(Screen,555)
      anchors.top: progressZone.bottom

      function _playSound(type)
      {
          if(SLogic.sound_switch === false)
              return;
          if(type === 1)
              sound_splash.play();
          else if(type === 2)
              sound_splash_fail.play();
          else if(type === 3)
              sound_splash_steel.play();
          else if(type === 4)
              sound_splash_die.play();
      }

      function _playSparkAnim(x,y)
      {
          spark.x = x - spark.width / 2;
          spark.y = y - spark.height / 2;
          spark.visible = true;
          sparkAnimation.start();
      }

      World {
          id: physicsWorld
          gravity: "0.0,0.0"
      }

      LLSoundEffect {
          id: sound_splash
          source: "./sound/slash.wav"
      }
      LLSoundEffect {
          id: sound_splash_fail
          source: "./sound/slash_fail.wav"
      }
      LLSoundEffect {
          id: sound_splash_steel
          source: "./sound/steel.wav"
      }
      LLSoundEffect {
          id: sound_splash_die
          source: "qrc:/sound/splash_die.wav"
      }

      Image {
          id: spark
          z: 1500
          width: SLogic.applyWScale(Screen,64)
          height: SLogic.applyHScale(Screen,64)
          fillMode: Image.PreserveAspectFit
          source: "./pics/spark.png"
          visible: false

          PropertyAnimation {
            id: sparkAnimation;
            target: spark;
            property: "visible";
            to: false;
            duration: 500
          }
      }

      MouseArea{
          id: gameArea
          anchors.fill: parent
          onPressed: {
              if(mouse.button == Qt.LeftButton){
                  tutorial.visible = false;
                  SLogic.x1 = mouse.x;
                  SLogic.y1 = mouse.y;
                  SLogic.isMouseClicked = true;
              }

          }

          onPositionChanged:{
                SLogic.isDrawLine(backGround,winDialog,gameZone,progressBar,mouse.x,mouse.y);
          }

          onReleased: {
                //调用js方法判断是否和多边形相交
                SLogic.isCutPolygon();
          }
      }

      GameToolBar{
          id: toolBar
          width: parent.width
          height: SLogic.applyHScale(Screen,50)
          anchors.bottom: parent.bottom
          onGamePause: {
            click_sound.play();
            physicsWorld.running = false;
            pauseDialog.visible = true;
            tutorial.visible = false;
            showPauseDialog.restart()
          }
      }
    }

/*
    Rectangle {
      id: toolZone
      color: "transparent"
      width: mainCanvas.width
      height: SLogic.applyHScale(Screen,10)
      anchors.top: gameZone.bottom

      GameToolBar{
          id: toolBar
          width: parent.width
          height: parent.height
          onGamePause: {
            physicsWorld.running = false;
            pauseDialog.visible = true
            showPauseDialog.restart()
          }
      }
    }
*/
    //TODO::弹出暂停窗口，屏蔽暂停后面的窗体接收事件
    Rectangle{
        id:pauseDialog
        color:"transparent"
        x:parent.width/3
        y:0//parent.height/4
        width:parent.width/3
        height:parent.width/3*1.109489
        visible: false
        Image{
            anchors.fill: parent
            source: "pics/pause/pause.png"
        }
        //PropertyAnimation {id: showPauseDialog; target: pauseDialog; properties: "y";from:0; to: parent.parent.height/4; duration: 300}
        SpringAnimation{id:showPauseDialog;from:0;to:(parent.parent.height-pauseDialog.height)/2;target: pauseDialog;property: "y";damping: 0.1;epsilon: 0.005;spring: 3}
        Rectangle{
            id:returnRec
            y:parent.height*0.21
            width:parent.width
            height:parent.height*0.3289
            color:"transparent"//"red"
            MouseArea{
                anchors.fill:parent
                onPressed: {
                    click_sound.play();
                    pauseDialog.visible = false
                    //TODO:返回
                    SLogic.deInitGameZone(gameZone,progressBar);
                    //root.state = "gameSubLevel";
                    backButtonClicked();
                }
            }
        }
        Rectangle{
            id:continueRec
            y:parent.height*0.6381
            width:parent.width
            height:parent.height*0.3618
            color:"transparent"//"blue"
            MouseArea{
                anchors.fill:parent
                onPressed: {
                    click_sound.play();
                    pauseDialog.visible = false
                    physicsWorld.running = true;
                    //TODO:继续
                }
            }
        }
    }

    Component.onCompleted: {
        pauseDialog.visible = false;
        //winDialog.open();
        //SLogic.addSound(gameZone);
    }
}
