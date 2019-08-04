import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 800
    height: 480
    color: "#000000"
    title: qsTr("Hello World")

    Text {
        id: temp
        objectName: "temp"
        x: 80
        y: 73
        width: 560
        height: 289
        color: "#fb6300"
        text: qsTr("10.0")
        font.letterSpacing: 0
        style: Text.Sunken
        font.weight: Font.Bold
        font.family: "Arial"
        font.italic: false
        font.bold: false
        renderType: Text.NativeRendering
        font.pixelSize: 280
    }

    Text {
        id: hum
        objectName: "hum"
        x: 587
        y: 385
        width: 128
        height: 75
        color: "#fb6300"
        text: qsTr("10.0")
        renderType: Text.NativeRendering
        font.pixelSize: 60
    }

    Text {
        id: temp1
        objectName: "temp1"
        x: 659
        y: 123
        width: 65
        height: 65
        color: "#fb6300"
        text: qsTr("°C")
        renderType: Text.NativeRendering
        font.bold: false
        font.weight: Font.Bold
        font.pixelSize: 50
        style: Text.Sunken
        font.italic: false
        font.family: "Arial"
    }

    Text {
        id: temp2
        x: 714
        y: 390
        width: 56
        height: 65
        color: "#fb6300"
        text: qsTr("%")
        renderType: Text.NativeRendering
        font.italic: false
        font.weight: Font.Bold
        style: Text.Sunken
        objectName: "temp1"
        font.bold: false
        font.pixelSize: 50
        font.family: "Arial"
    }
}
