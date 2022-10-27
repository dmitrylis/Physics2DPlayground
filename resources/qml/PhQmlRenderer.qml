import QtQuick 2.15
import QtQuick.Controls 2.15

import "controls"
import "types"

import "PhObjectSpawner.js" as Spawner

Item {
    // some auxilary stuff
    readonly property vector2d gravity: Qt.vector2d(controlPanel.gravityXValue, controlPanel.gravityYValue)
    readonly property int numOfIterations: 10
    readonly property int epsilon: 20
    readonly property real deltaTime: 0.2

    // predefined containers for vertexes and edges
    property var vertexes: []
    property var edges: []

    function generateGrid(width, height, density) {
        for (let i = 0; i < height; ++i) {
            for (let j = 0; j < width; ++j) {
                const isLocked = (width === 1 || height === 1) ? (i === 0) && (j === 0) : (i === 0) && (j % 2 === 0)
                vertexes.push(Spawner.create(vertexes, "types/PhVertex", { pos: Qt.vector2d(j * density, i * density), locked: isLocked }))

                if (j > 0) {
                    edges.push(Spawner.create(edges, "types/PhEdge", { vertexA: vertexes[vertexes.length - 2], vertexB: vertexes[vertexes.length - 1] }))
                }

                if (i > 0) {
                    edges.push(Spawner.create(edges, "types/PhEdge", { vertexA: vertexes[(i - 1) * width + j], vertexB: vertexes[i * width + j] }))
                }
            }
        }
    }

    function clearGrid() {
        vertexes = []
        edges = []
    }

    function initGrid() {
        clearGrid()
        generateGrid(controlPanel.widthValue, controlPanel.heightValue, controlPanel.densityValue)

        canvas.smartRequestPaint()
    }

    // "verlet" simulation
    function simulate() {
        if (vertexes.length === 0 || edges.length === 0) {
            return
        }

        for (const vertex of vertexes) {
            if (!vertex.locked) {
                const posBefore = Qt.vector2d(vertex.pos.x, vertex.pos.y)

                vertex.pos = vertex.pos.plus(vertex.pos.minus(vertex.prevPos))
                vertex.pos = vertex.pos.plus(gravity.times(deltaTime))

                vertex.prevPos = posBefore
            }
        }

        for (let j = 0; j < numOfIterations; ++j) {
            for (const edge of edges) {
                const difference = edge.vertexB.pos.minus(edge.vertexA.pos)
                const direction = difference.normalized()
                const error = difference.length() - edge.length

                if (!edge.vertexA.locked) {
                    edge.vertexA.pos = edge.vertexA.pos.plus(direction.times(error * (edge.vertexB.locked ? 1.0 : 0.5)))
                }

                if (!edge.vertexB.locked) {
                    edge.vertexB.pos = edge.vertexB.pos.minus(direction.times(error * (edge.vertexA.locked ? 1.0 : 0.5)))
                }
            }
        }
    }

    Component.onCompleted: {
        initGrid()
    }

    Timer {
        id: timer

        running: true
        repeat: true
        interval: 33

        onTriggered: {
            simulate()

            canvas.requestPaint()
        }
    }

    Canvas {
        id: canvas

        function smartRequestPaint() {
            if (!timer.running) {
                canvas.requestPaint()
            }
        }

        anchors.fill: parent

        function drawLine(context, x1, y1, x2, y2, color, size) {
            context.beginPath()

            context.moveTo(x1, y1)
            context.lineTo(x2, y2)

            context.lineWidth = size
            context.lineCap = "round"
            context.strokeStyle = color
            context.stroke()
        }

        function drawPoint(context, x, y, color, size) {
            context.beginPath()
            context.fillStyle = color
            context.arc(x, y, size, 0 * Math.PI, 2 * Math.PI)
            context.fill()
        }

        onPaint: {
            if (vertexes.length === 0 || edges.length === 0) {
                return
            }

            const context = getContext("2d")
            context.clearRect(0, 0, width, height)

            context.save()
            context.translate(ma.offset.x, ma.offset.y)
            context.scale(ma.zoom, ma.zoom)

            for (const edge of edges) {
                drawLine(context, edge.vertexA.pos.x, edge.vertexA.pos.y, edge.vertexB.pos.x, edge.vertexB.pos.y, "black", 6)
            }

            for (const vertex of vertexes) {
                const color = vertex === ma.currentVertex ? "yellow" : (vertex.locked ? "red" : "grey")
                const size = vertex === ma.currentVertex ? 3.5 : 2.5
                drawPoint(context, vertex.pos.x, vertex.pos.y, color, size)
            }
            context.restore()
        }
    }

    MouseArea {
        id: ma

        // drag & drop properties
        property PhVertex currentVertex: null
        property bool wasLocked: false

        // drag space properties
        property vector2d offset: Qt.vector2d(100, 100)
        property vector2d pressOffset: Qt.vector2d(0.0, 0.0)

        // zoom properties
        readonly property real zoomRate: 0.0015
        readonly property real zoomMin: 0.1
        readonly property real zoomMax: 20.0
        property real zoom: 1.0

        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton

        onPressed: {
            if (mouse.button === Qt.LeftButton) {
                if (vertexes.length === 0) {
                    return
                }

                let minLength = Number.MAX_SAFE_INTEGER
                const mousePos = Qt.vector2d(mouse.x, mouse.y).minus(ma.offset).times(1.0 / ma.zoom)

                for (const vertex of vertexes) {
                    const currentLength = mousePos.minus(vertex.pos).length()
                    if (currentLength < minLength) {
                        minLength = currentLength

                        if (minLength < epsilon) {
                            currentVertex = vertex
                        }
                    }
                }

                if (currentVertex) {
                    wasLocked = currentVertex.locked
                    currentVertex.locked = true
                }
            }
            else if (mouse.button === Qt.RightButton) {
                vertexes.push(Spawner.create(vertexes, "types/PhVertex", { pos: Qt.vector2d(mouse.x, mouse.y).minus(ma.offset), locked: vertexes.length === 0 }))

                if (vertexes.length > 1) {
                    edges.push(Spawner.create(edges, "types/PhEdge", { vertexA: vertexes[vertexes.length - 2], vertexB: vertexes[vertexes.length - 1] }))
                }
            }
            else if (mouse.button === Qt.MiddleButton) {
                ma.pressOffset = Qt.vector2d(offset.x - mouse.x, offset.y - mouse.y)
            }

            canvas.smartRequestPaint()
        }

        onPositionChanged: {
            if (mouse.buttons === Qt.LeftButton) {
                if (currentVertex) {
                    currentVertex.prevPos = Qt.vector2d(currentVertex.pos.x, currentVertex.pos.y)
                    currentVertex.pos = Qt.vector2d(mouse.x, mouse.y).minus(ma.offset).times(1.0 / ma.zoom)

                    canvas.smartRequestPaint()
                }
            }
            else if (mouse.buttons === Qt.MiddleButton) {
                ma.offset = Qt.vector2d(mouse.x, mouse.y).plus(ma.pressOffset)
                canvas.smartRequestPaint()
            }
        }

        onReleased: {
            if (mouse.button === Qt.LeftButton) {
                if (currentVertex) {
                    currentVertex.locked = wasLocked
                    currentVertex = null
                    canvas.smartRequestPaint()
                }
            }
            else if (mouse.button === Qt.MiddleButton) {
                ma.pressOffset = Qt.vector2d(0.0, 0.0)
            }
        }

        onWheel: {
            ma.zoom = Math.max(Math.min(ma.zoom * Math.exp(wheel.angleDelta.y * ma.zoomRate), ma.zoomMax), ma.zoomMin)

            canvas.smartRequestPaint()
            wheel.accepted = true
        }
    }

    PhControlPanel {
        id: controlPanel

        anchors.right: parent.right

        button1 {
            text: timer.running ? qsTr("Pause") : qsTr("Play")

            onClicked: {
                timer.running = !timer.running
            }
        }

        onValueChanged: {
            initGrid()
        }
    }
}
