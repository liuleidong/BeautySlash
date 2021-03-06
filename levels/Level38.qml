import QtQuick 2.0
import "../content"

TemplateBase{
    scoreTarget: 80

    //球基本数据-x,y位置、半径、密度、摩擦力、恢复系数、速度（x、y）
    ballData: [
        [80,250,10,2.5,0,1,12,16],
        [150,400,15,3.5,0,1,22,36],
        [200,300,10,2.5,0,1,13,17],
        [260,80,15,3.5,0,1,23,31],
        [300,360,10,2.5,0,1,13,17],
        [400,240,15,3.5,0,1,22,24],
        [420,300,15,3.5,0,1,20,32]
    ]
    wallData: [
        120,30,360,30,420,160,450,200,450,300,420,340,360,470,120,470,60,340,30,300,30,200,60,160
    ]
    wallRigid: [
        1,0,0,1,0,0,1,0,0,1,0,0
    ]
}
