TEMPLATE = app

QT += qml quick

include($$PWD/lib/shared_storage/shared_storage.pri)

CONFIG += c++11

SOURCES += main.cpp \
    fileoperations.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    fileoperations.h


