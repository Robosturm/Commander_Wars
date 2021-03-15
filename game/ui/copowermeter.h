#ifndef COPOWERMETER_H
#define COPOWERMETER_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "game/co.h"

class CoPowermeter;
typedef oxygine::intrusive_ptr<CoPowermeter> spCoPowermeter;

class CoPowermeter : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit CoPowermeter(CO* pCO);

    /**
     * @brief drawPowerMeter
     */
    void drawPowerMeter();
    /**
     * @brief getCO
     * @return
     */
    CO *getCO() const;
    /**
     * @brief setCO
     * @param pCO
     */
    void setCO(CO *pCO);
    /**
     * @brief setFlippedX
     * @param value
     */
    void setFlippedX(bool value);
    /**
     * @brief getFlippedX
     * @return
     */
    bool getFlippedX() const;
signals:

private:
    CO* _pCO;
    bool m_flippedX{false};

};

#endif // COPOWERMETER_H
