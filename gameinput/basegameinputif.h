#ifndef BASEGAMEINPUTIF_H
#define BASEGAMEINPUTIF_H

#include <QObject>

#include "oxygine-framework.h"

#include "coreengine/fileserializable.h"

class Player;

class BaseGameInputIF;
typedef oxygine::intrusive_ptr<BaseGameInputIF> spBaseGameInputIF;

class BaseGameInputIF : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    enum class AiTypes
    {
        Unkown = -1,
        Human,
    };

    explicit BaseGameInputIF();

    void setPlayer(Player* pPlayer);

    virtual void init() = 0;

    static void serializeInterface(QDataStream& pStream, BaseGameInputIF* input);
    static BaseGameInputIF* deserializeInterface(QDataStream& pStream);
signals:

public slots:

protected:
    Player* m_pPlayer{nullptr};
};

#endif // BASEGAMEINPUTIF_H
