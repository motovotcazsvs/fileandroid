#include "FileOperations.h"
#include <QStandardPaths>
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
    //filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/testFile.txt";
    //filePath = "content://com.android.externalstorage.documents/tree/primary%3Ahey/testfile.txt";
    //filePath = "content://com.android.externalstorage.documents/tree/primary%3Ahey/testfile.txt";
    //filePath = "content://com.android.externalstorage.documents/document/primary%3Ahey%2Ftestfile.txt";
    //filePath = "content://com.android.externalstorage.documents/document/primary%3Ahey%2Ftestfile.txt";
    //filePath = "/storage/emulated/0/hey/testfile.txt";
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
        qDebug() << "Data read from file." << content;
    } else {
        qWarning() << "Unable to read file.";
    }
}




bool FileOperations::hasSingleUri() const
{
    return m_uri.isValid() && DocumentsContract::isDocumentUri(m_uri) && (!m_tree || m_uri != m_tree->uri());
}

void FileOperations::fileContent()
{
    openTreeItem(0);
    QByteArray arr;
    if(hasSingleUri()) arr = ContentResolver::instance().openUri(m_uri, QIODevice::ReadOnly)->readAll();
    else QByteArray{};
    qDebug() << arr;
}

void FileOperations::openDir()
{
    SharedStorage::instance().openDocumentTree(3,
       [this](int code, net::Uri uri){
           if (code != 3 || !uri.isValid())
               return;
           ContentResolver::instance().takePersistableRWUriPermissions(uri);
           openTree(DocumentFile::fromTreeUri(uri));
           openUri(uri);
       });
}

void FileOperations::saveContent(const QByteArray &content)
{
    openTreeItem(0);
    ContentResolver::instance().openUri(m_uri, QIODevice::WriteOnly | QIODevice::Truncate)->write(content);
    openTree(m_tree);
}

void FileOperations::newTreeFile(const QString &name)
{
    m_tree->createFile("application/octet-stream", name);
    openTree(m_tree);
}

void FileOperations::openTreeItem(int idx)
{
    if (idx < 0 || idx >= int(m_files.size()))
        return;
    const auto doc = m_files[idx];
    if (doc->isDirectory())
        openTree(doc);
    openUri(doc->uri());
}

void FileOperations::openTree(const DocumentFilePtr &tree)
{
    if ((m_tree = tree))
        m_files = m_tree->listFiles();
    else
        m_files.clear();
    std::sort(m_files.begin(), m_files.end(),[](const provider::DocumentFilePtr &a, const provider::DocumentFilePtr &b){
        if (a->isDirectory() == b->isDirectory())
            return a->name() < b->name();
        return a->isDirectory();
    });
}

void FileOperations::openUri(const net::Uri &uri)
{
    m_uri = uri;
    fileContent();
}

QString FileOperations::fileName() const
{
    return m_uri.isValid() ? hasSingleUri() ? DocumentFile::fromSingleUri(m_uri)->name()
                                            : DocumentFile::fromTreeUri(m_uri)->name()
                           : QString{};
}





QString FileOperations::url() const
{
    return m_uri.isValid() ? m_uri.toString() : QString{};
}

bool FileOperations::hasTreeUri() const
{
    return m_tree.get();
}

bool FileOperations::hasParent() const
{
    return m_tree && m_tree->parent();
}

void FileOperations::newTreeFolder(const QString &name)
{
    m_tree->createDirectory(name);
    openTree(m_tree);
}

void FileOperations::newFile(const QString &fileName)
{
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
    SharedStorage::instance().openDocument(2,
                                           [this](int code, net::Uri uri){
                                               if (code != 2 || !uri.isValid())
                                                   return;
                                               ContentResolver::instance().takePersistableRWUriPermissions(uri);
                                               openTree();
                                               openUri(uri);
                                           }, mimeTypes);
}
