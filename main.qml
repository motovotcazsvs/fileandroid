import QtQuick
import QtQuick.Controls
import io.qt.examples.backend 1.0
import QtQuick.Dialogs

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    FileOperations {
        id: fileOperations
    }

    Column {
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "Create"
            onClicked: {
                fileOperations.createFile("testhello.txt")
            }
        }

        Button {
            text: "Write"
            onClicked: {
                fileOperations.saveContent("This is some test data.");
            }
        }

        Button {
            text: "Read"
            onClicked: {
                fileOperations.readFile();
            }
        }

        Button {
            text: "Permission1"
            onClicked: {
                fileOperations.openDir();
            }
        }

        Button {
            text: "empty"
            onClicked: {

            }
        }
    }

}


