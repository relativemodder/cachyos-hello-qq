#ifndef AUTOSTARTCONTROLLER_H
#define AUTOSTARTCONTROLLER_H

#include <QObject>

class AutostartController : public QObject
{
    Q_OBJECT
public:
    explicit AutostartController(QObject *parent = nullptr);

    Q_INVOKABLE bool isAutostart() const;
    Q_INVOKABLE void setAutostart(bool enabled);
};

#endif // AUTOSTARTCONTROLLER_H
