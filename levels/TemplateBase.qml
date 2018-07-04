import QtQuick 2.0

Item {
    id: levelConfig
    property int scoreTarget: -1    //pass condition
    property string backPicUrl: ""  //background pic url
    property var ballData           //ball config
    property var wallData           //wall config
    property var wallRigid          //is wall rigid

}
