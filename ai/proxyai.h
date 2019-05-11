#ifndef PROXYAI_H
#define PROXYAI_H

#include <QObject>
#include <QList>
#include <QMutex>
#include "game/gameaction.h"
#include "coreai.h"
#include "coreengine/mainapp.h"

class ProxyAi : public CoreAI
{
    Q_OBJECT
public:
    ProxyAi();

    virtual void init() override;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() override
    {
        return 1;
    }
    /**
     * @brief process empty impl
     */
    virtual void process() override {}
public slots:
    virtual void nextAction() override;
    void recieveData(QByteArray data, Mainapp::NetworkSerives service);
private:
    QList<GameAction*> m_ActionBuffer;
    bool actionRunning{false};
    QMutex m_ActionMutex;
};

#endif // PROXYAI_H
