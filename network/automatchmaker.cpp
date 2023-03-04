#include <QJsonObject>
#include <QJsonArray>

#include "network/automatchmaker.h"
#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "coreengine/interpreter.h"

AutoMatchMaker::AutoMatchMaker(const QString & matchName, MainServer & mainServer)
    : m_matchName(matchName),
      m_mainServer(mainServer)
{

}

QString AutoMatchMaker::getMatchName() const
{
    return m_matchName;
}

void AutoMatchMaker::onNewMatchResultData(const QJsonObject & objData)
{
    QJsonArray winnerInfo  = objData.value(JsonKeys::JSONKEY_GAMERESULTARRAY).toArray();
    QStringList usernames;
    QVector<qint32> results;
    for (const auto & player : winnerInfo)
    {
        auto data = player.toObject();
        usernames.append(data.value(JsonKeys::JSONKEY_PLAYER).toString());
        results.append(data.value(JsonKeys::JSONKEY_GAMERESULT).toInt());
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->arraytoJSValue(usernames),
                       pInterpreter->arraytoJSValue(results),
                       pInterpreter->newQObject(&m_mainServer)});
    pInterpreter->doFunction(m_matchName, args);
}

void AutoMatchMaker::updateInGameInfo(const QJsonObject & objData)
{

}

void AutoMatchMaker::createNewGames()
{

}

void AutoMatchMaker::updateMmr(const QString & player1, const QString & player2, qint32 maxEloChange, GameEnums::GameResult gameResultForPlayer1)
{

    qint32 mmr1 = 750;
    qint32 mmr2 = 750;
    // todo read mmr
    float diff = static_cast<float>(mmr1 - mmr2);
    float ea1 = 1.0f / (1.0f + qPow(10.0f, diff / 400.0f));
    float ea2 = (1.0f - ea1);
    switch (gameResultForPlayer1)
    {
        case GameEnums::GameResult_Won:
        {
            mmr1 = mmr1 + maxEloChange * (1.0f - ea1);
            mmr2 = mmr2 + maxEloChange * (0.0f - ea2);
            break;
        }
        case GameEnums::GameResult_Lost:
        {
            mmr1 = mmr1 + maxEloChange * (0.0f - ea1);
            mmr2 = mmr2 + maxEloChange * (1.0f - ea2);
            break;
        }
        case GameEnums::GameResult_Draw:
        {
            mmr1 = mmr1 + maxEloChange * (0.5f - ea1);
            mmr2 = mmr2 + maxEloChange * (0.5f - ea2);
            break;
        }
    }
    // todo write mmr
}
