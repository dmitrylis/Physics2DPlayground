import QtQuick 2.15

QtObject {
    property vector2d pos: Qt.vector2d(0.0, 0.0)
    property vector2d prevPos: Qt.vector2d(pos.x, pos.y)
    property bool locked: false
}
