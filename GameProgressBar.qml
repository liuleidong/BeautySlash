import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    property alias value: progressBar.value
    property double rate:height/35
    ProgressBar{
      id: progressBar
      width: parent.width
      height: parent.height
      from: 0
      to: 1
      visible:false
      contentItem: Item{
      }
    }

    //自定义进度条-背景
    Rectangle{
        id: bar_progress_back
        color: "transparent"
        x:5*rate
        y:5*rate
        radius: 10*rate
        width: parent.width - x*2
        height: 20*rate
        Image {
            id: bar_progress_back_img
            anchors.fill:parent
            source: "pics/progress/progress_back.png"
        }
        //进度
        Rectangle {
          id:contentItem_rec
          x:bar_progress_back.x
          y:0
          radius: bar_progress_back.radius
          width: bar_progress_back.width*progressBar.value
          height: bar_progress_back.height
          //radius: bar_progress_back.radius
          color: "#17a81a"
        }
    }
    //边界
    Rectangle {
        id: bar_border
        color: "transparent"
        border.color: "#c8b154"
        border.width:5*rate
        x:bar_progress_back.x
        y:bar_progress_back.y
        radius: bar_progress_back.radius
        width: bar_progress_back.width
        height: bar_progress_back.height
    }
    //标志
    Rectangle {
        id: bar_flag
        x:bar_progress_back.x + bar_progress_back.width*8/10
        y:bar_progress_back.y
        width: 10*rate
        height: bar_progress_back.height+10*rate
        color: "transparent"
        Image {
            id: bar_progress_flag
            anchors.fill:parent
            source: "pics/progress/progress_flag.png"
        }
    }
}
