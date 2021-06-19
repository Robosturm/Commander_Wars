#ifndef SCRIPTEVENTCHANGECOBAR_H
#define SCRIPTEVENTCHANGECOBAR_H

#include "ingamescriptsupport/events/scriptevent.h"

class ScriptEventChangeCOBar;
using spScriptEventChangeCOBar = oxygine::intrusive_ptr<ScriptEventChangeCOBar>;

class ScriptEventChangeCOBar : public ScriptEvent
{
    Q_OBJECT
public:
    ScriptEventChangeCOBar();
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
     * @brief getPlayer
     * @return
     */
    qint32 getPlayer() const;
    /**
     * @brief setPlayer
     * @param value
     */
    void setPlayer(const qint32 &value);
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Gain ") + QString::number(stars, 'f', 2) + tr(" Stars: ") + QString::number(player + 1) + " CO: " + QString::number(co + 1) ;
    }
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) override;
    /**
     * @brief getCo
     * @return
     */
    qint32 getCo() const;
    /**
     * @brief setCo
     * @param value
     */
    void setCo(const qint32 &value);
    /**
     * @brief getStars
     * @return
     */
    float getStars() const;
    /**
     * @brief setStars
     * @param value
     */
    void setStars(float value);
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    qint32 player{0};
    qint32 co{0};
    float stars{0};

};

#endif // SCRIPTEVENTCHANGECOBAR_H
