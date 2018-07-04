import QtQuick 2.0
import Box2D 2.0
import "shared"

PhysicsItem {
    id: ball

    property bool isBall: true
    property alias radius: circle.radius
    property alias color: circle.color

    Rectangle{
        id: circle

        color: "yellow"
        width: parent.width
        height: parent.height
        radius: width / 2
        anchors.fill: parent
    }

    property Body body: CircleBody{
        target: ball
        sleepingAllowed: false
        bodyType: Body.Dynamic

        radius: circle.radius

        density: 1
        friction: 0
        restitution: 1
    }
}
