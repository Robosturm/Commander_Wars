#include "markedfielddata.h"

MarkedFieldData::MarkedFieldData() : QObject()
{

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

bool MarkedFieldData::getShowZData() const
{
    return showZData;
}

void MarkedFieldData::setShowZData(bool value)
{
    showZData = value;
}
