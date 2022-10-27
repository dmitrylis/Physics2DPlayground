import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property alias textItem: textItem
    property alias sliderItem: sliderItem

    implicitHeight: Math.max(textItem.height, sliderItem.height)
    implicitWidth: textItem.implicitWidth + sliderItem.implicitWidth

    Text {
        id: textItem

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: sliderItem.left
        }
        elide: Text.ElideRight
    }

    Slider {
        id: sliderItem

        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
        }
    }
}
