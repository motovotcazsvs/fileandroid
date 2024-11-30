#include "FileOperations.h"
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDebug>


#include <QJniObject>
#include <QJniEnvironment>

FileOperations::FileOperations(QObject *parent) : QObject(parent)
{
    //filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/testFile.txt";
    filePath = "content://com.android.externalstorage.documents/tree/primary%3Ahey/testfile.txt";
    //uri = "content://com.android.externalstorage.documents/tree/primary%3Ahey/testfile.txt";
    uri = "content://com.android.externalstorage.documents/document/primary%3Ahey%2Ftestfile.txt";


}

// QFile file(filePath);
// if (file.open(QIODevice::WriteOnly)) {
//     file.close();
//     qDebug() << "File created at:" << filePath;
// } else {
//     qWarning() << "Unable to create file.";
// }

void FileOperations::createFile() {
    QJniEnvironment env;

    // Перевірка, чи середовище JNI створене коректно
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        qWarning() << "JNI environment has exceptions.";
        return;
    }

    // Створення URI за допомогою Uri.parse
    QJniObject jniUri = QJniObject::callStaticObjectMethod(
        "android/net/Uri",
        "parse",
        "(Ljava/lang/String;)Landroid/net/Uri;",
        QJniObject::fromString(uri).object<jstring>());

    if (!jniUri.isValid()) {
        qWarning() << "Invalid URI object!";
        return;
    }

    // Отримання Activity через системний клас `android.app.ActivityThread`
    QJniObject activityThread = QJniObject::callStaticObjectMethod(
        "android/app/ActivityThread",
        "currentActivityThread",
        "()Landroid/app/ActivityThread;");

    if (!activityThread.isValid()) {
        qWarning() << "Unable to get ActivityThread.";
        return;
    }

    QJniObject app = activityThread.callObjectMethod(
        "getApplication",
        "()Landroid/app/Application;");

    if (!app.isValid()) {
        qWarning() << "Unable to get Application instance.";
        return;
    }

    QJniObject contentResolver = app.callObjectMethod(
        "getContentResolver",
        "()Landroid/content/ContentResolver;");

    if (!contentResolver.isValid()) {
        qWarning() << "Unable to get content resolver.";
        return;
    }

    // Відкриття OutputStream для запису
    QJniObject outputStream = contentResolver.callObjectMethod(
        "openOutputStream",
        "(Landroid/net/Uri;)Ljava/io/OutputStream;",
        jniUri.object<jobject>());

    if (!outputStream.isValid()) {
        qWarning() << "Unable to open output stream.";
        return;
    }

    // Запис даних у файл
    const QByteArray data = "Hello, this is a test file!";
    QJniObject byteArray = QJniObject::fromString(data);

    outputStream.callMethod<void>(
        "write",
        "([B)V",
        byteArray.callObjectMethod("getBytes", "()[B").object<jbyteArray>());

    // Закриття потоку
    outputStream.callMethod<void>("close");

    qDebug() << "File created successfully at:" << uri;
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
        qDebug() << "Data read from file." << content;
    } else {
        qWarning() << "Unable to read file.";
    }
}


