#include "ai/decisiontree/decisiontree.h"
#include "ai/decisiontree/leaf.h"

#include "coreengine/gameconsole.h"
#include <coreengine/settings.h>

#include <QCryptographicHash>
#include <QFile>
#include <QDataStream>

#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/buildingspritemanager.h"

DecisionTree::DecisionTree(spDecisionNode & pRootNode)
	: m_pRootNode(pRootNode)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DecisionTree");
#endif
    Interpreter::setCppOwnerShip(this);
}

DecisionTree::DecisionTree(std::vector<std::vector<float>>& trainingData, std::vector<std::vector<spDecisionQuestion>>& questions)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DecisionTree");
#endif
    Interpreter::setCppOwnerShip(this);
	m_pRootNode = train(trainingData, questions);
}

/**
 * @brief DecisionTree::DecisionTree
 * @param file
 * @param trainingFile
 */
DecisionTree::DecisionTree(const QString & treeFile, const QString & trainingDataFile)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DecisionTree");
#endif
    Interpreter::setCppOwnerShip(this);
    QFile file(treeFile);
    QFile trainingFile(trainingDataFile);
    if (!trainingFile.exists())
    {
        trainingFile.setFileName(QString(oxygine::Resources::RCC_PREFIX_PATH) + trainingDataFile);
    }
    QByteArray hash;
    if (trainingFile.exists())
    {
        trainingFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
        QCryptographicHash myHash(QCryptographicHash::Sha512);
        while (!trainingFile.atEnd())
        {
            myHash.addData(trainingFile.readLine().trimmed());
        }
        myHash.addData(Settings::getInstance()->getModString().toUtf8());
        hash = myHash.result();
        trainingFile.close();
        bool needsTraining = true;
        if (file.exists())
        {
            file.open(QIODevice::ReadOnly | QIODevice::Truncate);
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Version::Qt_6_5);
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
            std::vector<std::vector<float>> trainingData;
            std::vector<std::vector<spDecisionQuestion>> questions;
            getTrainingData(trainingDataFile, trainingData, questions);
            m_pRootNode = train(trainingData, questions);
            CONSOLE_PRINT("Storing tree: " + treeFile, GameConsole::eDEBUG);
            // store trained tree for next use.
            file.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Version::Qt_6_5);
            for (qint32 i = 0; i < hash.size(); i++)
            {
                stream << static_cast<qint8>(hash[i]);
            }
            DecisionTree::serializeObject(stream);
        }
    }
}

float DecisionTree::getDecision(std::vector<float>& input)
{
    CONSOLE_PRINT("getDecision() for decision tree()", GameConsole::eDEBUG);
    float output = m_pRootNode->getDecision(input);
    CONSOLE_PRINT("Result = " + QString::number(output), GameConsole::eDEBUG);
    return output;
}

spDecisionNode DecisionTree::train(std::vector<std::vector<float>>& trainingData, std::vector<std::vector<spDecisionQuestion>>& questions)
{
    CONSOLE_PRINT("training decision tree()", GameConsole::eDEBUG);
	float gain = 0;
    spDecisionQuestion pQuestion = findBestSplit(trainingData, gain, questions);
	if (gain <= 0.0f)
	{
        return MemoryManagement::create<Leaf>(trainingData);
	}
    std::vector<std::vector<std::vector<float>>> splitData;
    seperateData(trainingData, pQuestion, splitData);

    std::vector<spDecisionNode> pNodes;
    for (qint32 i = 0; i < splitData.size(); i++)
    {
        pNodes.push_back(train(splitData[i], questions));
    }
    return MemoryManagement::create<DecisionNode>(pQuestion, pNodes);
}

std::vector<qint32> DecisionTree::countClassItems(std::vector<std::vector<float>>& trainingData)
{
    std::vector<qint32> ret;
    std::vector<float> labels;

	for (qint32 i = 0; i < trainingData.size(); i++)
	{
		float label = trainingData[i][trainingData[i].size() - 1];
        bool found = false;
        for (qint32 i = 0; i < labels.size(); ++i)
        {
            if (labels[i] == label)
            {
                ret[i]++;
                found = true;
                break;
            }
        }
        if (!found)
		{
            ret.push_back(1);
            labels.push_back(label);
		}
	}
	return ret;
}

void DecisionTree::seperateData(std::vector<std::vector<float>>& trainingData, spDecisionQuestion & question, std::vector<std::vector<std::vector<float>>>& splitData)
{
    for (qint32 i = 0; i < question->getSize(); i++)
    {
        splitData.push_back(std::vector<std::vector<float>>());
    }

	for (qint32 i = 0; i < trainingData.size(); i++)
	{
        qint32 answer = question->matches(trainingData[i]);
        if (answer >= 0 && answer < splitData.size())
        {
            splitData[answer].push_back(trainingData[i]);
        }
	}
}

float DecisionTree::giniImpurity(std::vector<std::vector<float>>& trainingData)
{
    std::vector<qint32> counts = countClassItems(trainingData);
	float impurity = 1;
	for (qint32 i = 0; i < counts.size(); i++)
	{
        float probability = static_cast<float>(counts[i]) / static_cast<float>(trainingData.size());
		impurity -= probability * probability;
	}
	return impurity;
}

float DecisionTree::infoGain(std::vector<std::vector<std::vector<float>>>& splitTrainingData, float currentUncertainty)
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

spDecisionQuestion DecisionTree::findBestSplit(std::vector<std::vector<float>>& trainingData, float& bestGain, std::vector<std::vector<spDecisionQuestion>>& questions)
{
    spDecisionQuestion bestQuestion;
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
                std::vector<std::vector<std::vector<float>>> splitData;
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
    m_pRootNode = MemoryManagement::create<DecisionNode>();
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
        CONSOLE_PRINT(spacing + "Predict " + pLeaf->print(), GameConsole::eDEBUG);
        return;
    }
    if (pNode != nullptr)
    {
        auto * pQuestion = pNode->getQuestion();
        if (pQuestion != nullptr)
        {
            CONSOLE_PRINT(spacing + pQuestion->print(), GameConsole::eDEBUG);
        }
        for (qint32 i = 0; i < pNode->getNodeSize(); i++)
        {
            CONSOLE_PRINT(spacing + "--> " + QString::number(i) + ":", GameConsole::eDEBUG);
            printTree(pNode->getNode(i) , spacing + "    ");
        }
    }
}

void DecisionTree::getTrainingData(QString file, std::vector<std::vector<float>>& trainingData, std::vector<std::vector<spDecisionQuestion>>& questions)
{
    QFile trainingFile(file);
    if (!trainingFile.exists())
    {
        trainingFile.setFileName(oxygine::Resource::RCC_PREFIX_PATH + file);
    }
    trainingFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QTextStream stream(&trainingFile);
    bool questionsFound = false;
    QStringList types;
    std::vector<spDecisionQuestion> readQuestions;

    readTrainingFile(stream, questionsFound, types, readQuestions, trainingData, questions);

    QStringList mods = Settings::getInstance()->getMods();
    QStringList fullMods;
    for(const QString & mod : std::as_const(mods))
    {
        fullMods.append(oxygine::Resource::RCC_PREFIX_PATH + mod);
        fullMods.append(Settings::getInstance()->getUserPath() + mod);
    }
    for (qint32 i = 0; i < fullMods.size(); i++)
    {
        QString modFilename = file;
        QFile modFile(modFilename.replace("resources/", fullMods[i] + "/"));
        if (modFile.exists())
        {
            modFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
            QTextStream modStream(&modFile);
            readTrainingFile(modStream, questionsFound, types, readQuestions, trainingData, questions);
        }
    }
}

void DecisionTree::readTrainingFile(QTextStream& stream, bool& questionsFound, QStringList& types,
                              std::vector<spDecisionQuestion>& readQuestions,
                              std::vector<std::vector<float>>& trainingData, std::vector<std::vector<spDecisionQuestion>>& questions)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        line = line.simplified();
        if (line.startsWith("//"))
        {
            // skip comments
        }
        else
        {
            if (line.startsWith("type") && !questionsFound )
            {
                questionsFound = true;
                QStringList items = line.split(" ");
                for (qint32 i = 1; i < items.size(); i++)
                {
                    readQuestions.push_back(MemoryManagement::create<DecisionQuestion>());
                    qint32 index = types.size();
                    QString typeLine = items[i];
                    if (typeLine.startsWith("NUMBER:"))
                    {
                        if (typeLine.startsWith("NUMBERFILE:"))
                        {
                            QFile numberFile(typeLine.split(":")[1]);
                            numberFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
                            QTextStream stream(&numberFile);
                            typeLine = stream.readLine();
                        }
                        QStringList questionString = typeLine.split(":")[1].split("|");
                        for (qint32 i2 = 0; i2 < questionString.size(); i2++)
                        {
                            QStringList questionData = questionString[i2].split("_");
                            if (questionData.size() == 2)
                            {
                                float value = questionData[0].toFloat();
                                if (questionData[1] == "<")
                                {
                                    readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(value, index, GameEnums::AIQuestionType_Greater));
                                }
                                else if (questionData[1] == ">")
                                {
                                    readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(value, index, GameEnums::AIQuestionType_Smaler));
                                }
                                else if (questionData[1] == "=")
                                {
                                    readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(value, index, GameEnums::AIQuestionType_Equal));
                                }
                            }
                            else if (questionData.size() == 3)
                            {
                                float value1 = questionData[0].toFloat();
                                float value2 = questionData[2].toFloat();
                                readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(value1, value2, index, GameEnums::AIQuestionType_Between));
                            }
                        }
                        types.append("NUMBER");
                    }
                    else
                    {
                        if (items[i] == "CO")
                        {
                            for (qint32 i2 = 0; i2 < pCOSpriteManager->getCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                            readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(-1, index, GameEnums::AIQuestionType_Equal));
                        }
                        else if (items[i] == "BUILDING")
                        {
                            for (qint32 i2 = 0; i2 < pBuildingSpriteManager->getCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                            readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(-1, index, GameEnums::AIQuestionType_Equal));
                        }
                        else if (items[i] == "UNIT")
                        {
                            for (qint32 i2 = 0; i2 < pUnitSpriteManager->getCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                            readQuestions[i - 1]->appendQuestion(MemoryManagement::create<Question>(-1, index, GameEnums::AIQuestionType_Equal));
                        }
                        types.append(items[i]);
                    }
                }
            }
            else if (questionsFound)
            {
                QStringList items = line.split(" ");
                // check for identic match here
                if (items.size() == types.size())
                {
                    trainingData.push_back(std::vector<float>());
                    questions.push_back(std::vector<spDecisionQuestion>());
                    qint32 item = trainingData.size() - 1;
                    for (qint32 i = 0; i < types.size(); i++)
                    {
                        // convert all data to numbers
                        if (types[i] == "CO")
                        {
                            qint32 index = pCOSpriteManager->getIndex(items[i]);
                            trainingData[item].push_back(index);
                            if (i < types.size() - 1)
                            {
                                questions[item].push_back(readQuestions[i]);
                            }
                        }
                        else if (types[i] == "BUILDING")
                        {
                            qint32 index = pBuildingSpriteManager->getIndex(items[i]);
                            trainingData[item].push_back(index);
                            if (i < types.size() - 1)
                            {
                                questions[item].push_back(readQuestions[i]);
                            }
                        }
                        else if (types[i] == "UNIT")
                        {
                            qint32 index = pUnitSpriteManager->getIndex(items[i]);
                            trainingData[item].push_back(index);
                            if (i < types.size() - 1)
                            {
                                questions[item].push_back(readQuestions[i]);
                            }
                        }
                        else if (types[i] == "NUMBER")
                        {
                            float value = items[i].toFloat();
                            trainingData[item].push_back(value);
                            if (i < types.size() - 1)
                            {
                                questions[item].push_back(readQuestions[i]);
                            }
                        }
                    }
                }
            }
        }
    }
}
