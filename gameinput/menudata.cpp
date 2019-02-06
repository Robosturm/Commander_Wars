#include "menudata.h"

MenuData::MenuData()
    : QObject()
{

}

void MenuData::addData(QString text, QString actionID, qint32 costs, bool enabled)
{
    texts.append(text);
    actionIDs.append(actionID);
    costList.append(costs);
    enabledList.append(enabled);
}
