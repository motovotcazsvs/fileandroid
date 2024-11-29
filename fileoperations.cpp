#include "FileOperations.h"
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDebug>


#include <QtCore/private/qandroidextras_p.h>


FileOperations::FileOperations(QObject *parent) : QObject(parent)
{
    //filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/testFile.txt";
    filePath = "content://com.android.externalstorage.documents/tree/primary%3Ahey/testfile.txt";
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
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString content = in.readAll();
        file.close();
        qDebug() << "Data read from file." << content;
    } else {
        qWarning() << "Unable to read file.";
    }
}


