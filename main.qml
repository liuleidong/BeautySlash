import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.LocalStorage 2.0

import "content/SettingLogic.js" as SLogic


Window {
    visible: true
    id: mainWindow
    title: qsTr("BeautySlash")
    width: SLogic.applyWScale(Screen,SLogic.refWidth)
    height: SLogic.applyHScale(Screen,SLogic.refHeight)

    Item{
        id: root
        anchors.fill: parent

        property var gameState

        Column {
            id: view
            y: -(height - mainWindow.height)
            width: parent.width

            GameCanvas {
                id: gameCanvas
                onBackButtonClicked: {
                    root.state = "gameSubLevel"
                    gameSubLevel.setLevelLocks(SLogic.grade);
                }
            }

            GameSubLevel{
                id: gameSubLevel
                onBackButtonClicked: {
                    gameLevel.setGradeLocks();
                    root.state = "gameLevel"
                }
                onSubLevelButtonClicked: {
                    SLogic.level = level;
                    var tmpPath = SLogic.levelPath + SLogic.grade + SLogic.level + ".qml";
                    gameCanvas._initGameZone(tmpPath);
                    root.state = "gameOn"
                }
            }

            GameLevel{
                id: gameLevel
                onBackButtonClicked: {
                    root.state = "startView"
                }
                onLevelButtonClicked:{
                    SLogic.grade = grade;
                    gameSubLevel.setLevelLocks(grade);
                    root.state = "gameSubLevel"
                }
            }
            NewGameScreen {
                id: newGameScreen
                onStartButtonClicked: {
                    root.state = "gameLevel";
                    gameLevel.setGradeLocks();
                }
                onAchievementButtonClicked:{
                    picViewScreen.setPics();
                    root.state = "picView";
                }
            }
            PicViewScreen {
                id:picViewScreen
                onBackButtonClicked: {
                    root.state = "startView"
                }
            }
        }
        states: [
            State {
                name: "picView"
                PropertyChanges {
                    target: view
                    y: -(height - mainWindow.height * 1)
                }
            },
            State {
                name: "startView"
                PropertyChanges {
                    target: view
                    y: -(height - mainWindow.height * 2)
                }
            },
            State {
                name: "gameLevel"
                PropertyChanges {
                    target: view
                    y: -(height - mainWindow.height * 3)
                }
            },
            State {
                name: "gameSubLevel"
                PropertyChanges {
                    target: view
                    y: -(height - mainWindow.height * 4)
                }
            },
            State {
                name: "gameOn"
                PropertyChanges {
                    target: view
                    y: -(height - mainWindow.height * 5)
                }
            }
        ]
    }

    Connections {
       target: qGuiApp
       onApplicationStateChanged: {
         console.log("State change", state);
           if(state == 0 || state == 1 || state == 2)
               newGameScreen._backMusicOff();
           else if(state == 4)
               newGameScreen._backMusicOn();
       }
    }

    onClosing:{
        if(root.state === "startView")
        {
            close.accepted = true;
        }
        else if(root.state === "gameOn")
        {
            close.accepted = false;
            if(gameCanvas.game_status == 0)
                gameCanvas._showPauseDlg();
            else if(gameCanvas.game_status == 1)
            {
                gameCanvas._deInitGameZone();
                root.state = "gameSubLevel";
                gameSubLevel.setLevelLocks(SLogic.grade);
            }
            else if(gameCanvas.game_status == 2)
            {
                gameCanvas._deInitGameZone();
                root.state = "gameSubLevel";
                gameSubLevel.setLevelLocks(SLogic.grade);
            }
        }
        else if(root.state === "gameSubLevel")
        {
            close.accepted = false;
            gameLevel.setGradeLocks();
            root.state = "gameLevel";
        }
        else if(root.state === "gameLevel")
        {
            close.accepted = false;
            root.state = "startView";
        }
        else if(root.state === "picView")
        {
            close.accepted = false;
            root.state = "startView";
        }
    }

    Component.onCompleted: {
            SLogic.initDB();
            newGameScreen._initImageSource();
//            SLogic.grade = 6
//            SLogic.level = 9;
//            var tempPath = SLogic.levelPath + SLogic.grade + SLogic.level + ".qml";
//            gameCanvas._initGameZone(tempPath);
            root.state = "startView";
        }
}

