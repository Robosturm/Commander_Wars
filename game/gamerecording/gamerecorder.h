#ifndef GAMERECORDER_H
#define GAMERECORDER_H

#include <QObject>

class GameRecorder : public QObject
{
    Q_OBJECT
public:
    explicit GameRecorder(QObject *parent = nullptr);

signals:

public slots:
};

#endif // GAMERECORDER_H