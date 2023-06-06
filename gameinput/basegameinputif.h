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
    void setPlayer(Player* pPlayer);

    virtual void init(GameMenue* pMenu) = 0;
    virtual void onGameStart();
    static void serializeInterface(QDataStream& pStream, BaseGameInputIF* input, GameEnums::AiTypes aiType);

    static spBaseGameInputIF deserializeInterface(GameMap* pMap, QDataStream& pStream, qint32 version);

    static spBaseGameInputIF createAi(GameMap* pMap, GameEnums::AiTypes type);

    virtual void centerCameraOnAction(GameAction* pAction);

    bool getProcessing() const;
    void setProcessing(bool newProcessing);

    /**
     * @brief getAiType
     * @return
     */
    Q_INVOKABLE GameEnums::AiTypes getAiType() const;
    /**
     * @brief getEnableNeutralTerrainAttack
     * @return
     */
    Q_INVOKABLE bool getEnableNeutralTerrainAttack() const;
    /**
     * @brief setEnableNeutralTerrainAttack
     * @param value
     */
    Q_INVOKABLE void setEnableNeutralTerrainAttack(bool value);
    /**
     * @brief setUnitBuildValue only applies to Normal AI and better
     * @param unitID unit to alter the building score
     * @param value score modification value. value <= 0 means won't be produced. value means 1.0 normal chance. value 2.0 means doubled score mostlikely AI will only build this unit
     */
    Q_INVOKABLE void setUnitBuildValue(QString unitID, float value);
    /**
     * @brief getUnitBuildValue only applies to Normal AI and better
     * @param unitID unit to alter the building score
     * @return value score modification value. value <= 0 means won't be produced. value means 1.0 normal chance. value 2.0 means doubled score mostlikely AI will only build this unit
     */
    Q_INVOKABLE float getUnitBuildValue(QString unitID);
    /**
     * @brief setMoveCostMapValue
     * @param x
     * @param y
     */
    Q_INVOKABLE void setMoveCostMapValue(qint32 x, qint32 y, qint32 value);
    /**
     * @brief getMoveCostMapValue
     * @param x
     * @param y
     * @return
     */
    Q_INVOKABLE qint32 getMoveCostMapValue(qint32 x, qint32 y);
    Q_INVOKABLE Player* getPlayer()
    {
        return m_pPlayer;
    }
    Q_INVOKABLE GameMap* getMap()
    {
        return m_pMap;
    }
protected:
    explicit BaseGameInputIF(GameMap* pMap, GameEnums::AiTypes aiType);
    virtual ~BaseGameInputIF() = default;

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
    bool m_processing{false};
};

Q_DECLARE_INTERFACE(BaseGameInputIF, "BaseGameInputIF");

#endif // BASEGAMEINPUTIF_H
