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

    FolderDialog {
        id: folderDialog
        title: "Оберіть папку для синхронізації"
        onAccepted: {
            // Використовуємо selectedFolder замість fileUrl
            if (folderDialog.selectedFolder) {
                console.log("Обрана папка: " + folderDialog.selectedFolder);
            } else {
                console.log("Не вибрана папка");
            }
        }
        onRejected: {
            console.log("Вибір папки скасовано");
        }
    }

    Column {
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "Create"
            onClicked: {
                fileOperations.createFile();
            }
        }

        Button {
            text: "Write"

            onClicked: {
                fileOperations.writeFile("This is some test data.");
            }
        }

        Button {
            text: "Read"
            onClicked: fileOperations.readFile();
        }

        Button {
            text: "Permission"
            onClicked: {
                folderDialog.open();
            }
        }


    }

}


