import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

import "content/SettingLogic.js" as SLogic

Item{
    id:coverFlow
    width: SLogic.applyWScale(Screen,SLogic.refWidth); height: SLogic.applyHScale(Screen,SLogic.refHeight)

    property ListModel model

    Rectangle{
        anchors.fill:parent
        color:"black"
    }
    PathView{
       id:pathView

       model:coverFlow.model
       //delegate:MyDelegate{}
       delegate: Item {
                   id:delegateItem
                   width: coverFlow.width*0.8//400
                   height: coverFlow.height*0.8//800
                   z:PathView.iconZ
                   scale:PathView.iconScale

                   Image{
                       id:image
                       y:coverFlow.height*0.1
                       source: url
                       width: delegateItem.width
                       height: delegateItem.height
                   }
                   ShaderEffect {
                       anchors.top: image.bottom
                       width: image.width
                       height: image.height;
                       anchors.left: image.left
                       property variant source: image;
                       property size sourceSize: Qt.size(0.5 / image.width, 0.5 / image.height);
                       fragmentShader:
                               "varying highp vec2 qt_TexCoord0;
                               uniform lowp sampler2D source;
                               uniform lowp vec2 sourceSize;
                               uniform lowp float qt_Opacity;
                               void main() {

                                   lowp vec2 tc = qt_TexCoord0 * vec2(1, -1) + vec2(0, 1);
                                   lowp vec4 col = 0.25 * (texture2D(source, tc + sourceSize) + texture2D(source, tc- sourceSize)
                                   + texture2D(source, tc + sourceSize * vec2(1, -1))
                                   + texture2D(source, tc + sourceSize * vec2(-1, 1)));
                                   gl_FragColor = col * qt_Opacity * (1.0 - qt_TexCoord0.y) * 0.2;
                               }"
                   }



                   transform: Rotation{
                       origin.x:image.width/2.0
                       origin.y:image.height/2.0
                       axis{x:0;y:1;z:0}
                       angle: delegateItem.PathView.iconAngle
                   }
               }
       path:coverFlowPath
       pathItemCount:coverFlow.model.count//coverFlow.itemCount
       anchors.fill:parent
       preferredHighlightBegin:0.5
       preferredHighlightEnd:0.5
    }

    /*Path{
       id:coverFlowPath
       startX:0
       startY:coverFlow.height/3

       PathLine{x:coverFlow.width*0.5;y:coverFlow.height/3;  }
       PathPercent{value:0.50}

       PathLine{x:coverFlow.width;y:coverFlow.height/3;}
       PathPercent{value:1.0}
    }*/
    Path{
        id:coverFlowPath
        startX:0
        startY:coverFlow.height/3
        PathAttribute{name:"iconZ";value:0}
        PathAttribute{name:"iconAngle";value:70}
        PathAttribute{name:"iconScale";value:0.6}

        PathLine{x:coverFlow.width*0.5;y:coverFlow.height/3;  }
        PathAttribute{name:"iconZ";value:100}
        PathAttribute{name:"iconAngle";value:0}
        PathAttribute{name:"iconScale";value:1.0}

        PathLine{x:coverFlow.width;y:coverFlow.height/3;}
        PathAttribute{name:"iconZ";value:0}
        PathAttribute{name:"iconAngle";value:-70}
        PathAttribute{name:"iconScale";value:0.6}
        PathPercent{value:1.0}
    }
}


