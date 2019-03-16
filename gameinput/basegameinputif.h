#ifndef BASEGAMEINPUTIF_H
#define BASEGAMEINPUTIF_H

#include <QObject>

#include "oxygine-framework.h"

class Player;

class BaseGameInputIF;
typedef oxygine::intrusive_ptr<BaseGameInputIF> spBaseGameInputIF;

class BaseGameInputIF : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit BaseGameInputIF();

    void setPlayer(Player* pPlayer);

    virtual void init() = 0;
signals:

public slots:

protected:
    Player* m_pPlayer{nullptr};
};

#endif // BASEGAMEINPUTIF_H
