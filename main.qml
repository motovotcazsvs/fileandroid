import QtQuick 2.15
import QtQuick.Controls 2.15
import io.qt.examples.backend 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Column {
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "Create"
            onClicked: fileOperations.createFile();
        }

        Button {
            text: "Write"
            onClicked: fileOperations.writeFile("This is some test data.");
        }

        Button {
            text: "Read"
            onClicked: fileOperations.readFile();
        }


    }

    // Підключаємо до C++ частину
    FileOperations {
        id: fileOperations

    }
}


