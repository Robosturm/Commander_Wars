#ifndef BASEGAMEINPUTIF_H
#define BASEGAMEINPUTIF_H

#include <QObject>
#include <QVector>
#include <QMap>

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

#include "coreengine/fileserializable.h"

#include "game/GameEnums.h"

class GameMap;
class Player;
class GameAction;
class GameMenue;
class BaseGameInputIF;
using spBaseGameInputIF = oxygine::intrusive_ptr<BaseGameInputIF>;

class BaseGameInputIF : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit BaseGameInputIF(GameMap* pMap, GameEnums::AiTypes aiType);
    virtual ~BaseGameInputIF() = default;
    void setPlayer(Player* pPlayer);

    virtual void init(GameMenue* pMenu) = 0;

    static void serializeInterface(QDataStream& pStream, BaseGameInputIF* input);

    static spBaseGameInputIF deserializeInterface(GameMap* pMap, QDataStream& pStream, qint32 version);

    static spBaseGameInputIF createAi(GameMap* pMap, GameEnums::AiTypes type);

    virtual void centerCameraOnAction(GameAction* pAction);

public slots:
    /**
     * @brief getAiType
     * @return
     */
    GameEnums::AiTypes getAiType() const;
    /**
     * @brief getEnableNeutralTerrainAttack
     * @return
     */
    bool getEnableNeutralTerrainAttack() const;
    /**
     * @brief setEnableNeutralTerrainAttack
     * @param value
     */
    void setEnableNeutralTerrainAttack(bool value);
    /**
     * @brief setUnitBuildValue only applies to Normal AI and better
     * @param unitID unit to alter the building score
     * @param value score modification value. value <= 0 means won't be produced. value means 1.0 normal chance. value 2.0 means doubled score mostlikely AI will only build this unit
     */
    void setUnitBuildValue(QString unitID, float value);
    /**
     * @brief getUnitBuildValue only applies to Normal AI and better
     * @param unitID unit to alter the building score
     * @return value score modification value. value <= 0 means won't be produced. value means 1.0 normal chance. value 2.0 means doubled score mostlikely AI will only build this unit
     */
    float getUnitBuildValue(QString unitID);
    /**
     * @brief setMoveCostMapValue
     * @param x
     * @param y
     */
    void setMoveCostMapValue(qint32 x, qint32 y, qint32 value);
    /**
     * @brief getMoveCostMapValue
     * @param x
     * @param y
     * @return
     */
    qint32 getMoveCostMapValue(qint32 x, qint32 y);
protected:
    Player* m_pPlayer{nullptr};
    GameEnums::AiTypes m_AiType{GameEnums::AiTypes_Human};
    bool m_enableNeutralTerrainAttack{true};
    QMap<QString, float> m_BuildingChanceModifier;
    /**
     * @brief m_MoveCostMap move cost modifier map for the ai
     */
    std::vector<std::vector<std::tuple<qint32, bool>>> m_MoveCostMap;
    GameMap* m_pMap{nullptr};
    GameMenue* m_pMenu{nullptr};
    bool m_initDone{false};
};

Q_DECLARE_INTERFACE(BaseGameInputIF, "BaseGameInputIF");

#endif // BASEGAMEINPUTIF_H
