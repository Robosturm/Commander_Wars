#ifndef BASEMENU_H
#define BASEMENU_H

#include <QObject>

class Basemenu : public QObject
{
    Q_OBJECT
public:
    explicit Basemenu();

signals:
    void sigOnUpdate();
};

#endif // BASEMENU_H
