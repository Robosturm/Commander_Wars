#include "gameinput/cursordata.h"

#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"

CursorData::CursorData()
{
    setObjectName("CursorData");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

QString CursorData::getCursor() const
{
    return m_cursor;
}

void CursorData::setCursor(const QString &value)
{
    m_cursor = value;
}

qint32 CursorData::getXOffset() const
{
    return m_xOffset;
}

void CursorData::setXOffset(const qint32 &value)
{
    m_xOffset = value;
}

qint32 CursorData::getYOffset() const
{
    return m_yOffset;
}

void CursorData::setYOffset(const qint32 &value)
{
    m_yOffset = value;
}

float CursorData::getScale() const
{
    return m_scale;
}

void CursorData::setScale(const float &value)
{
    m_scale = value;
}
