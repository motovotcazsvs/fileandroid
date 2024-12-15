#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QObject>

#include <documentfile.h>

using namespace android;

class FileOperations : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasSingleUri READ hasSingleUri NOTIFY uriChanged)
    Q_PROPERTY(bool hasTreeUri READ hasTreeUri NOTIFY uriChanged)
    Q_PROPERTY(bool hasParent READ hasParent NOTIFY treeDocChanged)
    Q_PROPERTY(QByteArray fileContent READ fileContent NOTIFY uriChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY uriChanged)
    Q_PROPERTY(QString url READ url NOTIFY uriChanged)

public:
    explicit FileOperations(QObject *parent = nullptr);
    Q_INVOKABLE void createFile(QString);
    Q_INVOKABLE void writeFile(const QString &data);
    Q_INVOKABLE void readFile();
    Q_INVOKABLE void sizeFile();
    Q_INVOKABLE void countFiles();
    Q_INVOKABLE void checkFiles();
    Q_INVOKABLE void getNameFile();
    Q_INVOKABLE void getNameFolder();
    Q_INVOKABLE void getAllFiles();
    Q_INVOKABLE void getAllFolders();
    Q_INVOKABLE void geturl();

    bool hasSingleUri() const;
    bool hasTreeUri() const;
    bool hasParent() const;
    QByteArray fileContent() const;
    QString fileName() const;
    QString url() const;


public slots:
    void newFile(const QString &fileName);
    void openFile(const QStringList &mimeTypes = {"*/*"});
    void openDir();
    void remove();
    void saveContent(const QByteArray &content);

    void cdUp();
    void newTreeFile(const QString &name);
    void newTreeFolder(const QString &name);

    void openTreeItem(int idx);
    void removeTreeItem(int idx);
    void renameTreeItem(int idx, const QString &newName);

signals:
    void uriChanged();
    void treeDocChanged();

private:
    void openTree(const provider::DocumentFilePtr &tree = {});
    void openUri(const net::Uri &uri);

private:
    net::Uri m_uri;
    provider::DocumentFilePtr m_tree;
    std::vector<provider::DocumentFilePtr> m_files;
};

#endif // FILEOPERATIONS_H
