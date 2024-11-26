#include "FileOperations.h"
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDebug>

FileOperations::FileOperations(QObject *parent) : QObject(parent)
{
    // Визначаємо шлях до файлу в Documents каталозі
    filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/testFile.txt";
    //filePath = content://com.android.externalstorage.documents/tree/primary%3Aimgg%2F6038586442907648-1024x576~2.png;
}

void FileOperations::createFile()
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
        qDebug() << "File created at:" << filePath;
    } else {
        qWarning() << "Unable to create file.";
    }
}

void FileOperations::writeFile(const QString &data)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;
        file.close();
        qDebug() << "Data written to file.";
    } else {
        qWarning() << "Unable to write to file.";
    }
}

void FileOperations::readFile()
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        file.close();
        emit readFinished(content);
        qDebug() << "Data read from file.";
    } else {
        qWarning() << "Unable to read file.";
    }
}
