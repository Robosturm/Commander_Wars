#include "decisiontree.h"

#include "leaf.h"

DecisionTree::DecisionTree(spDecisionNode pRootNode)
	: m_pRootNode(pRootNode)
{
}

DecisionTree::DecisionTree(QVector<QVector<float>>& trainingData, QVector<QVector<spQuestion>>& questions)
{
	m_pRootNode = train(trainingData, questions);
}

/**
 * @brief DecisionTree::DecisionTree
 * @param file
 * @param trainingFile
 */
DecisionTree::DecisionTree(QString treeFile, QString trainingData)
{

}

DecisionTree::~DecisionTree()
{
}

float DecisionTree::getDecision(QVector<float>& input)
{
	return m_pRootNode->getDecision(input);
}

spDecisionNode DecisionTree::train(QVector<QVector<float>>& trainingData, QVector<QVector<spQuestion>>& questions)
{
	float gain = 0;
    spQuestion pQuestion = findBestSplit(trainingData, gain, questions);
	if (gain <= 0.0f)
	{
		return new Leaf(trainingData);
	}
	QVector<QVector<float>> trueData;
	QVector<QVector<float>> falseData;
	seperateTrueFalse(trainingData, pQuestion, trueData, falseData);

    spDecisionNode pNodeTrue = train(trueData, questions);
    spDecisionNode pNodeFalse = train(falseData, questions);

	return new DecisionNode(pQuestion, pNodeTrue, pNodeFalse);
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

void DecisionTree::seperateTrueFalse(QVector<QVector<float>>& trainingData, spQuestion question, QVector<QVector<float>>& trueData, QVector<QVector<float>>& falseData)
{
	for (qint32 i = 0; i < trainingData.size(); i++)
	{
		if (question->matches(trainingData[i]))
		{
			trueData.append(trainingData[i]);
		}
		else
		{
			falseData.append(trainingData[i]);
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

float DecisionTree::infoGain(QVector<QVector<float>>& trainingDataLeft, QVector<QVector<float>>& trainingDataRight, float currentUncertainty)
{
	float p = static_cast<float>(trainingDataLeft.size()) / (static_cast<float>(trainingDataLeft.size() + static_cast<float>(trainingDataRight.size())));
	return currentUncertainty - p * giniImpurity(trainingDataLeft) - (1 - p) * giniImpurity(trainingDataRight);
}

spQuestion DecisionTree::findBestSplit(QVector<QVector<float>>& trainingData, float& bestGain, QVector<QVector<spQuestion>>& questions)
{
    spQuestion bestQuestion = nullptr;
	float currentUncertainty = giniImpurity(trainingData);
	float n_features = trainingData[0].size() - 1;
	bestGain = 0;
	for (qint32 col = 0; col < n_features; col++)
	{
		for (qint32 i = 0; i < questions[col].size(); i++)
		{
			// try splitting the dataset
			QVector<QVector<float>> trueData;
			QVector<QVector<float>> falseData;
			seperateTrueFalse(trainingData, questions[col][i], trueData, falseData);
			// Skip this split if it doesn't divide the dataset.
			if ((trueData.size() == 0) || 
				(falseData.size() == 0))
			{
				continue;
			}
			// Calculate the information gain from this split
            float gain = infoGain(trueData, falseData, currentUncertainty);
			if (gain > bestGain)
			{
				bestGain = gain;
				bestQuestion = questions[col][i];
			}
		}
	}
	return bestQuestion;
}

void DecisionTree::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    m_pRootNode->serialize(pStream);
}

void DecisionTree::deserialize(QDataStream& pStream)
{
    qint32 version;
    pStream >> version;
    // never assume this as a leaf -> pointless
    bool isNode = false;
    pStream >> isNode;
    m_pRootNode = new DecisionNode(nullptr, nullptr, nullptr);
    m_pRootNode->deserialize(pStream);
}
