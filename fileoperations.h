#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QObject>

#include <documentfile.h>

class FileOperations : public QObject
{
    Q_OBJECT
public:
    explicit FileOperations(QObject *parent = nullptr);
    Q_INVOKABLE void createFile();
    Q_INVOKABLE void writeFile(const QString &data);
    Q_INVOKABLE void readFile();


signals:

private:
    QString filePath;
    QString uri;
};

#endif // FILEOPERATIONS_H
