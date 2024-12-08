import QtQuick
import QtQuick.Controls
import io.qt.examples.backend 1.0


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
                fileOperations.newTreeFile("testhello.txt")
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
            onClicked: fileOperations.fileContent();
        }

        Button {
            text: "Permission1"
            onClicked: {
                fileOperations.openDir();
            }
        }

        Button {
            text: "Permission2"
            onClicked: {

            }
        }
    }

}


