#include "ai/coreai.h"
#include "ai/decisiontree.h"
#include "ai/leaf.h"

#include "coreengine/console.h"
#include <coreengine/settings.h>

#include <qcryptographichash.h>
#include <qfile.h>
#include <qdatastream.h>

DecisionTree::DecisionTree(spDecisionNode pRootNode)
	: m_pRootNode(pRootNode)
{
    setObjectName("DecisionTree");
}

DecisionTree::DecisionTree(QVector<QVector<float>>& trainingData, QVector<QVector<spDecisionQuestion>>& questions)
{
    setObjectName("DecisionTree");
	m_pRootNode = train(trainingData, questions);
}

/**
 * @brief DecisionTree::DecisionTree
 * @param file
 * @param trainingFile
 */
DecisionTree::DecisionTree(QString treeFile, QString trainingDataFile)
{
    setObjectName("DecisionTree");
    QFile file(treeFile);
    QFile trainingFile(trainingDataFile);

    trainingFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QCryptographicHash myHash(QCryptographicHash::Sha3_512);
    myHash.addData(&trainingFile);
    myHash.addData(Settings::getModConfigString().toUtf8());
    QByteArray hash = myHash.result();
    trainingFile.close();

    bool needsTraining = true;
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        QByteArray currentHash;
        for (qint32 i = 0; i < hash.size(); i++)
        {
            qint8 value = 0;
            stream >> value;
            currentHash.append(value);
        }
        // check if the training data has changed
        if (currentHash == hash)
        {
            DecisionTree::deserializeObject(stream);
            needsTraining = false;
        }
        file.close();
    }
    if (needsTraining)
    {
        QVector<QVector<float>> trainingData;
        QVector<QVector<spDecisionQuestion>> questions;
        CoreAI::getTrainingData(trainingDataFile, trainingData, questions);
        m_pRootNode = train(trainingData, questions);

        // store trained tree for next use.
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        for (qint32 i = 0; i < hash.size(); i++)
        {
            stream << static_cast<qint8>(hash[i]);
        }
        DecisionTree::serializeObject(stream);
    }
}

DecisionTree::~DecisionTree()
{
}

float DecisionTree::getDecision(QVector<float>& input)
{
    Console::print("getDecision() for decision tree()", Console::eDEBUG);
    float output = m_pRootNode->getDecision(input);
    Console::print("Result = " + QString::number(output), Console::eDEBUG);
    return output;
}

spDecisionNode DecisionTree::train(QVector<QVector<float>>& trainingData, QVector<QVector<spDecisionQuestion>>& questions)
{
    Console::print("training decision tree()", Console::eDEBUG);
	float gain = 0;
    spDecisionQuestion pQuestion = findBestSplit(trainingData, gain, questions);
	if (gain <= 0.0f)
	{
        return spLeaf::create(trainingData);
	}
    QVector<QVector<QVector<float>>> splitData;
    seperateData(trainingData, pQuestion, splitData);

    QVector<spDecisionNode> pNodes;
    for (qint32 i = 0; i < splitData.size(); i++)
    {
        pNodes.append(train(splitData[i], questions));
    }
    return spDecisionNode::create(pQuestion, pNodes);
}

QVector<qint32> DecisionTree::countClassItems(QVector<QVector<float>>& trainingData)
{
	QVector<qint32> ret;
	QVector<float> labels;

	for (qint32 i = 0; i < trainingData.size(); i++)
	{
		float label = trainingData[i][trainingData[i].size() - 1];
		if (labels.contains(label))
		{
			ret[labels.indexOf(label)]++;
		}
		else
		{
			ret.append(1);
			labels.append(label);
		}
	}
	return ret;
}

void DecisionTree::seperateData(QVector<QVector<float>>& trainingData, spDecisionQuestion question, QVector<QVector<QVector<float>>>& splitData)
{
    for (qint32 i = 0; i < question->getSize(); i++)
    {
        splitData.append(QVector<QVector<float>>());
    }

	for (qint32 i = 0; i < trainingData.size(); i++)
	{
        qint32 answer = question->matches(trainingData[i]);
        if (answer >= 0 && answer < splitData.size())
        {
            splitData[answer].append(trainingData[i]);
        }
	}
}

float DecisionTree::giniImpurity(QVector<QVector<float>>& trainingData)
{
	QVector<qint32> counts = countClassItems(trainingData);
	float impurity = 1;
	for (qint32 i = 0; i < counts.size(); i++)
	{
        float probability = static_cast<float>(counts[i]) / static_cast<float>(trainingData.size());
		impurity -= probability * probability;
	}
	return impurity;
}

float DecisionTree::infoGain(QVector<QVector<QVector<float>>>& splitTrainingData, float currentUncertainty)
{
    float count = 0;
    for (qint32 i = 0; i < splitTrainingData.size(); i++)
    {
        count += splitTrainingData[i].size();
    }
    float ret = currentUncertainty;
    for (qint32 i = 0; i < splitTrainingData.size(); i++)
    {
        float p = static_cast<float>(splitTrainingData[i].size()) / count;
        ret -= p * giniImpurity(splitTrainingData[i]);
    }
    return ret;
}

spDecisionQuestion DecisionTree::findBestSplit(QVector<QVector<float>>& trainingData, float& bestGain, QVector<QVector<spDecisionQuestion>>& questions)
{
    spDecisionQuestion bestQuestion = nullptr;
    if (trainingData.size() > 0)
    {
        float currentUncertainty = giniImpurity(trainingData);
        float n_features = trainingData[0].size() - 1;
        bestGain = 0;
        for (qint32 col = 0; col < n_features; col++)
        {
            for (qint32 i = 0; i < questions[col].size(); i++)
            {
                // try splitting the dataset
                QVector<QVector<QVector<float>>> splitData;
                seperateData(trainingData, questions[col][i], splitData);

                // Skip this split if it doesn't divide the dataset.
                qint32 count = 0;
                for (qint32 i = 0; i < splitData.size(); i++)
                {
                    if (splitData[i].size() > 0)
                    {
                        count++;
                    }
                }
                if (count == 0)
                {
                    continue;
                }
                // Calculate the information gain from this split
                float gain = infoGain(splitData, currentUncertainty);
                if (gain > bestGain)
                {
                    bestGain = gain;
                    bestQuestion = questions[col][i];
                }
            }
        }
    }
	return bestQuestion;
}

void DecisionTree::serializeObject(QDataStream& pStream) const
{
    pStream << DecisionTree::getVersion();
    m_pRootNode->serializeObject(pStream);
}

void DecisionTree::deserializeObject(QDataStream& pStream)
{
    qint32 version;
    pStream >> version;
    // never assume this as a leaf -> pointless
    bool isNode = false;
    pStream >> isNode;
    m_pRootNode = spDecisionNode::create();
    m_pRootNode->deserializeObject(pStream);
}

void DecisionTree::printTree(DecisionNode* pNode, QString spacing)
{
    if (pNode == nullptr)
    {
        pNode = m_pRootNode.get();
    }
    Leaf* pLeaf = dynamic_cast<Leaf*>(pNode);
    if (pLeaf != nullptr)
    {
        Console::print(spacing + "Predict " + pLeaf->print(), Console::eDEBUG);
        return;
    }
    if (pNode != nullptr)
    {
        auto * pQuestion = pNode->getQuestion();
        if (pQuestion != nullptr)
        {
            Console::print(spacing + pQuestion->print(), Console::eDEBUG);
        }
        for (qint32 i = 0; i < pNode->getNodeSize(); i++)
        {
            Console::print(spacing + "--> " + QString::number(i) + ":", Console::eDEBUG);
            printTree(pNode->getNode(i) , spacing + "    ");
        }
    }
}
