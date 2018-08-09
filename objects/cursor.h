#ifndef CURSOR_H
#define CURSOR_H

#include <QObject>

#include "oxygine-framework.h"

class Cursor;
typedef oxygine::intrusive_ptr<Cursor> spCursor;

/**
 * @brief The Cursor class is the ingame cursor for the map
 */
class Cursor : public QObject, public oxygine::Actor
{
   Q_OBJECT
public:
    Cursor();
    /**
     * @brief updatePosition calculates the new cursor point based on the mouse x and y coordinates
     * @param mousePosX
     * @param mousePosY
     */
    void updatePosition(qint32 mousePosX, qint32 mousePosY);
    void changeCursor(const QString& spriteID);
    /**
     * @brief getMapPointX changes our cursor sprite
     * @return
     */
    inline qint32 getMapPointX()
    {
        return m_MapPointX;
    }
    inline qint32 getMapPointY()
    {
        return m_MapPointY;
    }
signals:
    void sigCursorMoved();
private:
    oxygine::spSprite m_CurrentCursor;
    bool onMap{false};

    qint32 m_MapPointX{-1};
    qint32 m_MapPointY{-1};
};

#endif // CURSOR_H
