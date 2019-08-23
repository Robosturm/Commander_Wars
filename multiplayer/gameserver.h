#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>

/**
 * @brief The GameServer class
 */
class GameServer : public QObject
{
    Q_OBJECT
public:
    static GameServer* getInstance();

signals:
    void sigNewGame();
    void sigUpdateGame();
    void sigRemoveGame();
public slots:

private:
    explicit GameServer();

    static GameServer* m_pInstance;
};

#endif // GAMESERVER_H
