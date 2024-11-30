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

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0];
        onAccepted: {
            if (fileDialog.selectedFile) {
                console.log("Обраний файл: " + fileDialog.selectedFile);
            } else {
                console.log("Не вибраний файл");
            }
        }
        onRejected: {
            console.log("Вибір файла скасовано");
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
            text: "Permission1"
            onClicked: {
                folderDialog.open();
            }
        }

        Button {
            text: "Permission2"
            onClicked: {
                fileDialog.open();
            }
        }
    }

}


