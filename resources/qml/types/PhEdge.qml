import QtQuick 2.15

QtObject {
    id: root

    required property PhVertex vertexA
    required property PhVertex vertexB
    readonly property real length: internal.length

    readonly property QtObject internal: QtObject {
        property real length: 0.0

        Component.onCompleted: {
            length = root.vertexB.pos.minus(root.vertexA.pos).length()
        }
    }
}
