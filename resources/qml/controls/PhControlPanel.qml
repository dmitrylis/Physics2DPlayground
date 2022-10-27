import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    property alias widthValue: widthSlider.sliderItem.value
    property alias heightValue: heightSlider.sliderItem.value
    property alias densityValue: densitySlider.sliderItem.value
    property alias gravityXValue: gravityXSlider.sliderItem.value
    property alias gravityYValue: gravityYSlider.sliderItem.value

    property alias button1: button1

    signal valueChanged()

    width: column.width
    height: column.height
    color: Qt.rgba(0, 0, 0, 0.35)

    Column {
        id: column

        width: 320
        padding: 10

        PhSlider {
            id: widthSlider

            width: parent.width - parent.padding * 2
            textItem.text: qsTr("Width: ") + sliderItem.value

            sliderItem {
                from: 1
                to: 61
                stepSize: 1
                value: 21

                onValueChanged: {
                    root.valueChanged()
                }
            }
        }

        PhSlider {
            id: heightSlider

            width: parent.width - parent.padding * 2
            textItem.text: qsTr("Height: ") + sliderItem.value

            sliderItem {
                from: 1
                to: 61
                stepSize: 1
                value: 21

                onValueChanged: {
                    root.valueChanged()
                }
            }
        }

        PhSlider {
            id: densitySlider

            width: parent.width - parent.padding * 2
            textItem.text: qsTr("Density: ") + sliderItem.value

            sliderItem {
                from: 1
                to: 100
                stepSize: 1
                value: 25

                onValueChanged: {
                    root.valueChanged()
                }
            }
        }

        PhSlider {
            id: gravityXSlider

            width: parent.width - parent.padding * 2
            textItem.text: qsTr("Gravity X: ") + sliderItem.value

            sliderItem {
                from: -50.0
                to: 50.0
                stepSize: 1.0
                value: 0.0
            }
        }

        PhSlider {
            id: gravityYSlider

            width: parent.width - parent.padding * 2
            textItem.text: qsTr("Gravity Y: ") + sliderItem.value

            sliderItem {
                from: -50.0
                to: 50.0
                stepSize: 1.0
                value: 9.8
            }
        }

        Button {
            id: button1

            width: parent.width - parent.padding * 2
        }
    }
}
