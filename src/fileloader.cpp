#include "fileloader.h"
#include <QFile>
#include <QDirIterator>
#include <iostream>

FileLoader::FileLoader(QObject *parent)
    : QObject{parent}
{}

QString FileLoader::readFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Failed to open file " << path.toStdString() << std::endl;
        return QString();
    }
    return QString::fromUtf8(file.readAll());
}
