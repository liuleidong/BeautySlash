import QtQuick 2.0
import Box2D 2.0
import "shared"

PhysicsItem {
    id: wall

    property alias vertices: edge.vertices

    property Body body: EdgeBody{
        id: edge
        target: wall
        sleepingAllowed: false
        bodyType: Body.Static
        restitution: 1
    }
}

