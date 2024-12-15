#include "FileOperations.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <contentresolver.h>
#include <documentscontract.h>
#include <sharedstorage.h>

using namespace android::provider;
using namespace android::content;

FileOperations::FileOperations(QObject *parent) : QObject(parent)
{
    qDebug() << "FileOperations()";

}

void FileOperations::createFile(QString name)
{
    qDebug() << "createFile()";
    newTreeFile(name);

}

void FileOperations::writeFile(const QString &data)
{
    qDebug() << "writeFile()";
    openTreeItem(0);
    saveContent(data.toUtf8());
}

void FileOperations::readFile()
{
    qDebug() << "hasSingleUri()";
    openTreeItem(0);
    fileContent();
}


void FileOperations::sizeFile(int numfile)
{
    qDebug() << "sizeFile()";
    int size;

    qDebug() << "sizeFile" <<  size;
}

void FileOperations::countFiles()
{
    qDebug() << "countFiles()";
    int count;

    qDebug() << "countFiles" <<  count;
}
void FileOperations::checkFiles()
{
    qDebug() << "checkFiles()";
    bool exists;

    qDebug() << "checkFiles" <<  exists;
}

void FileOperations::getNameFile(int numfile)
{
    qDebug() << "getNameFile()";
    qDebug() << "getNameFile";
}

void FileOperations::getNameFolder(int numfolder)
{
    qDebug() << "getNameFolder()";
    qDebug() << "getNameFolder";
}

void FileOperations::getAllFiles()
{
    qDebug() << "getAllFiles()";
    qDebug() << "getAllFiles";
}

void FileOperations::getAllFolders()
{
    qDebug() << "getAllFolders()";
    qDebug() << "getAllFolders";
}

void FileOperations::geturl()
{
    qDebug() << "geturl()";
    qDebug() << "geturl";
}



bool FileOperations::hasSingleUri() const
{
    qDebug() << "hasSingleUri()";
    qDebug() << m_uri.isValid() << DocumentsContract::isDocumentUri(m_uri) << (!m_tree || m_uri != m_tree->uri());
    return m_uri.isValid() && DocumentsContract::isDocumentUri(m_uri) && (!m_tree || m_uri != m_tree->uri());
}

bool FileOperations::hasTreeUri() const
{
    qDebug() << "hasTreeUri()";
    return m_tree.get();
}

bool FileOperations::hasParent() const
{
    qDebug() << "hasParent()";
    return m_tree && m_tree->parent();
}

QByteArray FileOperations::fileContent() const
{
    qDebug() << "fileContent()";
    qDebug() << "hasSingleUri" << hasSingleUri();
    QByteArray a = ContentResolver::instance().openUri(m_uri, QIODevice::ReadOnly)->readAll();
    QByteArray b = hasSingleUri() ? ContentResolver::instance().openUri(m_uri, QIODevice::ReadOnly)->readAll()
                      : QByteArray{};

    qDebug() << "baba" << b << a;
    return b;
}


void FileOperations::newFile(const QString &fileName)
{
    qDebug() << "newFile()";
    SharedStorage::instance().createDocument(1,
        [this](int code, net::Uri uri){
            if (code != 1 || !uri.isValid())
                return;
            ContentResolver::instance().takePersistableRWUriPermissions(uri);
            openTree();
            openUri(uri);
        }, "application/octet-stream", fileName);
}

void FileOperations::openFile(const QStringList &mimeTypes)
{
    qDebug() << "openFile()";
    SharedStorage::instance().openDocument(2,
        [this](int code, net::Uri uri){
            if (code != 2 || !uri.isValid())
                return;
            ContentResolver::instance().takePersistableRWUriPermissions(uri);
            openTree();
            openUri(uri);
        }, mimeTypes);
}

void FileOperations::openDir()
{
    qDebug() << "openDir()";
    SharedStorage::instance().openDocumentTree(3,
       [this](int code, net::Uri uri){
           if (code != 3 || !uri.isValid())
               return;
           ContentResolver::instance().takePersistableRWUriPermissions(uri);
           openTree(DocumentFile::fromTreeUri(uri));
           openUri(uri);
       });

    qDebug() << "fileName" << fileName();
    qDebug() << "url" << url();
}

void FileOperations::remove()
{
    qDebug() << "remove()";
    DocumentsContract::removeDocument(m_uri);
    openUri({});
}

void FileOperations::saveContent(const QByteArray &content)
{
    qDebug() << "saveContent()";
    ContentResolver::instance().openUri(m_uri, QIODevice::WriteOnly | QIODevice::Append)->write(content);
    openTree(m_tree);
}

void FileOperations::cdUp()
{
    qDebug() << "cdUp()";
    if (!m_tree->parent())
        return;
    const auto doc = m_tree->parent();
    openTree(doc);
    openUri(doc->uri());
}

void FileOperations::newTreeFile(const QString &name)
{
    qDebug() << "newTreeFile()";
    m_tree->createFile("application/octet-stream", name);
    openTree(m_tree);
}

void FileOperations::newTreeFolder(const QString &name)
{
    qDebug() << "newTreeFolder()";
    m_tree->createDirectory(name);
    openTree(m_tree);
}

void FileOperations::openTreeItem(int idx)
{
    qDebug() << "openTreeItem()";
    if (idx < 0 || idx >= int(m_files.size()))
        return;
    const auto doc = m_files[idx];
    if (doc->isDirectory())
        openTree(doc);
    openUri(doc->uri());
}

void FileOperations::removeTreeItem(int idx)
{
    qDebug() << "removeTreeItem()";
    if (idx < 0 || idx >= int(m_files.size()))
        return;
    m_files[idx]->remove();
    openTree(m_tree);
    openUri(m_tree->uri());
}

void FileOperations::renameTreeItem(int idx, const QString &newName)
{
    qDebug() << "renameTreeItem()";
    if (idx < 0 || idx >= int(m_files.size()))
        return;
    m_files[idx]->rename(newName);
    openTree(m_tree);
    openUri(m_tree->uri());
}

void FileOperations::openTree(const DocumentFilePtr &tree)
{
    qDebug() << "openTree()";
    if ((m_tree = tree))
        m_files = m_tree->listFiles();
    else
        m_files.clear();
    std::sort(m_files.begin(), m_files.end(),[](const provider::DocumentFilePtr &a, const provider::DocumentFilePtr &b){
        if (a->isDirectory() == b->isDirectory())
            return a->name() < b->name();
        return a->isDirectory();
    });

    emit treeDocChanged();
}

void FileOperations::openUri(const net::Uri &uri)
{
    qDebug() << "openUri()";
    m_uri = uri;


    emit uriChanged();
}

QString FileOperations::fileName() const
{
    qDebug() << "fileName()";
    return m_uri.isValid() ? hasSingleUri() ? DocumentFile::fromSingleUri(m_uri)->name()
                                            : DocumentFile::fromTreeUri(m_uri)->name()
                           : QString{};
}

QString FileOperations::url() const
{
    qDebug() << "url()";
    return m_uri.isValid() ? m_uri.toString() : QString{};
}
