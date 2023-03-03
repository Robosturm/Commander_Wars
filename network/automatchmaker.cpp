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
