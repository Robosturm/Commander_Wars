#include "markedfielddata.h"

#include "coreengine/interpreter.h"

MarkedFieldData::MarkedFieldData()
    : QObject()
{
    Interpreter::setCppOwnerShip(this);
}

bool MarkedFieldData::getAllFields() const
{
    return allFields;
}

void MarkedFieldData::setAllFields(bool value)
{
    allFields = value;
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
    return showZData;
}

void MarkedFieldData::setShowZData(bool value)
{
    showZData = value;
}
