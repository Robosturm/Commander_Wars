#ifndef SCRIPTEVENTCHANGEWEATHER_H
#define SCRIPTEVENTCHANGEWEATHER_H

#include "ingamescriptsupport/events/scriptevent.h"

class ScriptEventChangeWeather;
using spScriptEventChangeWeather = oxygine::intrusive_ptr<ScriptEventChangeWeather>;

class ScriptEventChangeWeather : public ScriptEvent
{
    Q_OBJECT
public:
    ScriptEventChangeWeather();

    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream, QString line) override;
    /**
     * @brief writeEvent
     * @param rStream
     */
    virtual void writeEvent(QTextStream& rStream) override;

    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Change Weather");
    }
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) override;
    /**
     * @brief getTurns
     * @return
     */
    qint32 getTurns() const;
    /**
     * @brief setTurns
     * @param value
     */
    void setTurns(const qint32 &value);
    /**
     * @brief getWeatherID
     * @return
     */
    qint32 getWeatherID() const;
    /**
     * @brief setWeatherID
     * @param value
     */
    void setWeatherID(const qint32 &value);
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    qint32 weatherID{0};
    qint32 turns{1};
};

#endif // SCRIPTEVENTCHANGEWEATHER_H
