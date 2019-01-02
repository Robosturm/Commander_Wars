#ifndef BASEGAMEINPUTIF_H
#define BASEGAMEINPUTIF_H

#include <QObject>

class Player;

class BaseGameInputIF : public QObject
{
    Q_OBJECT
public:
    explicit BaseGameInputIF();

    void setPlayer(Player* pPlayer);
signals:

public slots:

protected:
    Player* m_pPlayer{nullptr};
};

#endif // BASEGAMEINPUTIF_H
