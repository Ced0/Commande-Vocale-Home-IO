import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Command vocal interface")

    Button {
        objectName: "btnRecord"
        text: "Start record command"
        x: parent.width/2-width/2
        y: parent.height/2-height/2
    }
}
