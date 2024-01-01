#pragma once

#include <QObject>

/**
 * @brief This class is meant to read the content of an awbw replay file and create map states based on them in order to train an ai
 */
class AwbwReplayerReader : public QObject
{
    Q_OBJECT
public:
    explicit AwbwReplayerReader(const QString & awbwReplayZip);

signals:

private:
    // QList<QByteArray>
};

