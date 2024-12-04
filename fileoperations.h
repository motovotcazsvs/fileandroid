#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QObject>

#include <documentfile.h>

using namespace android;

class FileOperations : public QObject
{
    Q_OBJECT
public:
    explicit FileOperations(QObject *parent = nullptr);
    Q_INVOKABLE void createFile();
    Q_INVOKABLE void writeFile(const QString &data);
    Q_INVOKABLE void readFile();

    bool hasSingleUri() const;
    bool hasTreeUri() const;
    bool hasParent() const;
    QString fileName() const;
    QString url() const;


public slots:
    void newFile(const QString &fileName);
    void openFile(const QStringList &mimeTypes = {"*/*"});
    void openDir();
    void saveContent(const QByteArray &content);
    void newTreeFile(const QString &name);
    void newTreeFolder(const QString &name);
    void openTreeItem(int idx);
    void fileContent();

signals:

private:
    void openTree(const provider::DocumentFilePtr &tree = {});
    void openUri(const net::Uri &uri);

private:
    QString filePath;

    net::Uri m_uri;
    provider::DocumentFilePtr m_tree;
    std::vector<provider::DocumentFilePtr> m_files;
};

#endif // FILEOPERATIONS_H
