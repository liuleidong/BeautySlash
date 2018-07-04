import QtQuick 2.0
import QtGraphicalEffects 1.0
/*Item{
    id:delegateItem
    width:320
    height:450

    Image{
       id:dlgImg
       source:url
    }
}*/
/*Item{
    id:delegateItem
    width:320
    height:450
    z:PathView.iconZ
    scale:PathView.iconScale

    Image{
       id:dlgImg
       source:url
       width:delegateItem.width
       height:delegateItem.height
    }

    transform:Rotation{
       origin.x:dlgImg.width/2
       origin.y:dlgImg.height/2
        axis{x:0;y:1;z:0}
       angle:delegateItem.PathView.iconAngle
    }
}*/
Column  {
    id:delegate
    y:0
    width:320
    height:450
    spacing:10

    Image{
       id:dlgImg
       source:url
       width:delegate.width
       height:delegate.height
    }

    /*Image{
       id:reflection
       width:delegateItem.width
       height:delegateItem.height
       source:dlgImg.source
       opacity:0.4
       transform:Scale{
           yScale:-1
           origin.y:delegateItem.height/2
       }
    }*/
    /*Item{
        width:delegate.width
        height:delegate.height

        Image{
           id:reflection
           source:dlgImg.source
           transform:Scale{
               yScale:-1
               origin.y:delegateItem.height/2
           }
        }

        LinearGradient{
           width:delegate.width
           height:delegate.height
           gradient:Gradient{
               GradientStop{position:0.0;color:  Qt.rgba(0,0,0,0.1)}
               GradientStop{position:0.4;color:  Qt.rgba(0,0,0,1)}
           }
        }
    }*/
}
