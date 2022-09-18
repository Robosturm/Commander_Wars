#pragma once
#include <QObject>
#include <QVector>
#include <QString>
#include <QTextStream>

#include "ai/decisiontree/decisionnode.h"
#include "ai/decisiontree/decisionquestion.h"

#include "coreengine/fileserializable.h"

class DecisionTree final : public QObject, public FileSerializable
{
	Q_OBJECT
public:
    DecisionTree(spDecisionNode & pRootNode);
    DecisionTree(std::vector<std::vector<float>>& trainingData, std::vector<std::vector<spDecisionQuestion>>& questions);
    DecisionTree(const QString & treeFile, const QString & trainingDataFile);
    ~DecisionTree() = default;

    spDecisionNode train(std::vector<std::vector<float>>& trainingData, std::vector<std::vector<spDecisionQuestion>>& questions);

    static std::vector<qint32> countClassItems(std::vector<std::vector<float>>& trainingData);

    virtual void serializeObject(QDataStream& pStream) const override;
    virtual void deserializeObject(QDataStream& pStream)  override;
    virtual qint32 getVersion() const override
    {
        return 1;
    }

    /**
     * @brief getTrainingData reads the training data from a training file for a pipeline either decision tree or neural network
     * @param file
     * @param trainingData
     * @param questions
     */
    static void getTrainingData(QString file, std::vector<std::vector<float>>& trainingData, std::vector<std::vector<spDecisionQuestion>>& questions);
    /**
     * @brief readTrainingFile
     * @param stream
     * @param questionsFound
     * @param types
     * @param readQuestions
     * @param trainingData
     * @param questions
     */
    static void readTrainingFile(QTextStream& stream, bool& questionsFound, QStringList& types,
                                 std::vector<spDecisionQuestion>& readQuestions,
                                 std::vector<std::vector<float>>& trainingData, std::vector<std::vector<spDecisionQuestion>>& questions);

public slots:
    float getDecision(std::vector<float>& input);
    void printTree(DecisionNode* pNode = nullptr, QString spacing = "");
protected:
    void seperateData(std::vector<std::vector<float>>& trainingData, spDecisionQuestion & question, std::vector<std::vector<std::vector<float>>>& splitData);
    float giniImpurity(std::vector<std::vector<float>>& trainingData);
    float infoGain(std::vector<std::vector<std::vector<float>>>& splitTrainingData, float currentUncertainty);
    spDecisionQuestion findBestSplit(std::vector<std::vector<float>>& trainingData, float& bestGain, std::vector<std::vector<spDecisionQuestion>>& questions);
private:
        spDecisionNode m_pRootNode;
};
