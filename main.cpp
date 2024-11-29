#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "FileOperations.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Реєструємо FileOperations як об'єкт QML
    qmlRegisterType<FileOperations>("io.qt.examples.backend", 1, 0, "FileOperations");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
