#pragma once

#include <QFile>
#include <QTextStream>
#include "ai/heavyai/heavyai.h"

class HeavyAiTrainingDataGenerator;
using spHeavyAiTrainingDataGenerator = std::shared_ptr<HeavyAiTrainingDataGenerator>;

class HeavyAiTrainingDataGenerator : public HeavyAi
{
    Q_OBJECT
public:
    HeavyAiTrainingDataGenerator(GameMap* pMap, const QString & type, const QString & trainingFile);

    virtual void init(BaseGamemenu* pMenu) override;
    void addCurrentSituationTrainingData();
    qint32 getWinnerTeam() const;
    void setWinnerTeam(qint32 newWinnerTeam);

private:
    qint32 m_winnerTeam{-1};
    QFile m_trainingDataFile;
    QTextStream m_stream;
};
