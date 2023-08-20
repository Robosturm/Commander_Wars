#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>

#include "coreengine/scriptvariables.h"
#include "coreengine/fileserializable.h"

#include "game/unit.h"
#include "game/terrain.h"

class GameMap;
class Weather;
using spWeather = std::shared_ptr<Weather>;

class Weather final : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    explicit Weather(GameMap* pMap);
    explicit Weather(QString weatherId, GameMap* pMap);
    ~Weather() = default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief serializeObject
     * @param pStream
     * @param forHash
     */
    void serializeObject(QDataStream& pStream, bool forHash) const;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 1;
    }
    Q_INVOKABLE QString getWeatherId() const;
    Q_INVOKABLE QString getWeatherName();
    Q_INVOKABLE QString getWeatherTerrainSprite();
    Q_INVOKABLE QString getWeatherSymbol();
    Q_INVOKABLE qint32 getOffensiveModifier();
    Q_INVOKABLE qint32 getDefensiveModifier();
    Q_INVOKABLE qint32 getMovementCostModifier(Unit* pUnit, Terrain* pTerrain);
    Q_INVOKABLE qint32 getMovementpointModifier(Unit* pUnit, Terrain* pTerrain);
    Q_INVOKABLE qint32 getFirerangeModifier();
    Q_INVOKABLE qint32 getMinFirerangeModifier();
    Q_INVOKABLE qint32 getVisionrangeModifier();
    Q_INVOKABLE qint32 getMovementFuelCostModifier(Unit* pUnit, qint32 fuelCost);
    Q_INVOKABLE void activate();
    Q_INVOKABLE void deactivate();
    Q_INVOKABLE inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
private:
    QString m_WeatherId;
    ScriptVariables m_Variables;
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(Weather, "Weather");

#endif // WEATHER_H

