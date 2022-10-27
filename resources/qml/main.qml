import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    width: 1024
    height: 768
    visible: true
    title: qsTr("Physics 2D Playground")

    Component {
        id: cppComponent

        PhCppRenderer {}
    }

    Component {
        id: qmlComponent

        PhQmlRenderer {}
    }

    Loader {
        id: implLoader

        anchors.fill: parent
        sourceComponent: cppComponent
    }

    Button {
        text: implLoader.sourceComponent == cppComponent ? "C++" : "Qml"

        onClicked: {
            if (implLoader.sourceComponent == cppComponent) {
                implLoader.sourceComponent = qmlComponent
            }
            else {
                implLoader.sourceComponent = cppComponent
            }
        }
    }
}
