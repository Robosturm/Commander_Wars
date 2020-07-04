#ifndef NETWORKGAMEDATA_H
#define NETWORKGAMEDATA_H

#include <QObject>

class NetworkGameData : public QObject
{
    Q_OBJECT
public:
    explicit NetworkGameData(QObject *parent = nullptr);

signals:

};

#endif // NETWORKGAMEDATA_H
