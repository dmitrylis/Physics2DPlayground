const qmlPostfix = ".qml"

function create(parent, name, properties) {
    const url = (name.slice(-4) === qmlPostfix) ? name : name + qmlPostfix
    const component = Qt.createComponent(url)

    if (component.status === Component.Ready) {
        return component.createObject(parent, properties)
    }

    console.warn("The next problem has been occured during %1 creation:".arg(url), component.errorString())
    return null
}
