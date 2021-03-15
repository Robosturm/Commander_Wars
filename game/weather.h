#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/scriptvariables.h"
#include "coreengine/fileserializable.h"

#include "game/unit.h"
#include "game/terrain.h"

class Weather;
typedef oxygine::intrusive_ptr<Weather> spWeather;


class Weather : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit Weather();
    explicit Weather(QString weatherId);
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
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
public slots:
    QString getWeatherId() const;
    QString getWeatherName();
    QString getWeatherTerrainSprite();
    QString getWeatherSymbol();
    qint32 getOffensiveModifier();
    qint32 getDefensiveModifier();
    qint32 getMovementCostModifier(Unit* pUnit, Terrain* pTerrain);
    qint32 getMovementpointModifier(Unit* pUnit, Terrain* pTerrain);
    qint32 getFirerangeModifier();
    qint32 getMinFirerangeModifier();
    qint32 getVisionrangeModifier();
    qint32 getMovementFuelCostModifier(Unit* pUnit, qint32 fuelCost);
    void activate();
    void deactivate();
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
private:
    QString m_WeatherId;
    ScriptVariables m_Variables;
};

#endif // WEATHER_H

