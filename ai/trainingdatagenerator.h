#pragma once

#include <QObject>
#include "coreengine/fileserializable.h"

class GameMap;
class GameAction;
class SituationEvaluator;
using spSituationEvaluator = std::shared_ptr<SituationEvaluator>;

class TrainingDataGenerator : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    explicit TrainingDataGenerator(GameMap* pMap);
    void init();
    Q_INVOKABLE void onActionDone(GameAction* pAction);
    Q_INVOKABLE void saveDataToFile(const QString & filepath);
    void saveDataToFile();
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
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 1;
    }
private:
    GameMap* m_pMap{nullptr};
    std::vector<spSituationEvaluator> m_evaluators;
    std::vector<QStringList> m_data;
};

Q_DECLARE_INTERFACE(TrainingDataGenerator, "TrainingDataGenerator");
