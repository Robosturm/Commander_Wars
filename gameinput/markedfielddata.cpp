#include "gameinput/markedfielddata.h"

#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"

MarkedFieldData::MarkedFieldData()
    : QObject()
{
    setObjectName("MarkedFieldData");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

bool MarkedFieldData::getAllFields() const
{
    return m_allFields;
}

void MarkedFieldData::setAllFields(bool value)
{
    m_allFields = value;
}

QColor MarkedFieldData::getColor() const
{
    return m_Color;
}

void MarkedFieldData::setColor(const QColor &Color)
{
    m_Color = Color;
}

QColor MarkedFieldData::getZLabelColor() const
{
    return m_ZLabelColor;
}

void MarkedFieldData::setZLabelColor(const QColor &ZLabelColor)
{
    m_ZLabelColor = ZLabelColor;
}

QString MarkedFieldData::getZLabelText() const
{
    return m_ZLabelText;
}

void MarkedFieldData::setZLabelText(const QString &ZLabelText)
{
    m_ZLabelText = ZLabelText;
}

bool MarkedFieldData::getShowZData() const
{
    return m_showZData;
}

void MarkedFieldData::setShowZData(bool value)
{
    m_showZData = value;
}
