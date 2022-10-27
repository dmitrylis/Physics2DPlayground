import QtQuick 2.15

import com.dln.ph 1.0

import "controls"

PhRenderer {
    id: root

    solver: PhSolver {
        gridWidth: controlPanel.widthValue
        gridHeight: controlPanel.heightValue
        gridDensity: controlPanel.densityValue
        gravityX: controlPanel.gravityXValue
        gravityY: controlPanel.gravityYValue

        Component.onCompleted: {
            initGrid()
        }
    }

    PhControlPanel {
        id: controlPanel

        anchors.right: parent.right
        button1 {
            text: root.timerActive ? qsTr("Pause") : qsTr("Play")

            onClicked: {
                root.timerActive = !root.timerActive
            }
        }
    }
}
