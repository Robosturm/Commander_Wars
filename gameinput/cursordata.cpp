#include "cursordata.h"

#include "coreengine/interpreter.h"

#include "coreengine/mainapp.h"

CursorData::CursorData()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

QString CursorData::getCursor() const
{
    return cursor;
}

void CursorData::setCursor(const QString &value)
{
    cursor = value;
}

qint32 CursorData::getXOffset() const
{
    return xOffset;
}

void CursorData::setXOffset(const qint32 &value)
{
    xOffset = value;
}

qint32 CursorData::getYOffset() const
{
    return yOffset;
}

void CursorData::setYOffset(const qint32 &value)
{
    yOffset = value;
}

float CursorData::getScale() const
{
    return scale;
}

void CursorData::setScale(const float &value)
{
    scale = value;
}
