#ifndef FILELOADER_H
#define FILELOADER_H

#include <QObject>

class FileLoader : public QObject
{
    Q_OBJECT
public:
    explicit FileLoader(QObject *parent = nullptr);
    Q_INVOKABLE QString readFile(const QString& path);
};

#endif // FILELOADER_H
