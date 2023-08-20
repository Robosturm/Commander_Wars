#ifndef CURSOR_H
#define CURSOR_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

class GameMap;
class Cursor;
using spCursor = std::shared_ptr<Cursor>;

/**
 * @brief The Cursor class is the ingame cursor for the map
 */
class Cursor final : public QObject, public oxygine::Actor
{
   Q_OBJECT
public:
    explicit Cursor(GameMap* pMap);
    ~Cursor() = default;
    void changeCursor(const QString & spriteID, qint32 xOffset = 0, qint32 yOffset = 0, float scale = 1.0f);
    /**
     * @brief addCursorRangeOutline
     * @param range
     */
    void addCursorRangeOutline(qint32 range, QColor color);
    /**
     * @brief resetCursorRangeOutline
     */
    void resetCursorRangeOutline();
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
    QPoint getMapPoint()
    {
        return QPoint(m_MapPointX, m_MapPointY);
    }
    void setMapPoint(qint32 x, qint32 y);

signals:
    void sigCursorMoved(qint32 x, qint32 y);
public:
    /**
     * @brief getMap
     * @return
     */
    Q_INVOKABLE GameMap *getMap() const;
    /**
     * @brief updatePosition calculates the cursor point based on the mouse x and y coordinates
     * @param mousePosX
     * @param mousePosY
     */
    Q_INVOKABLE void updatePosition(qint32 mousePosX, qint32 mousePosY);

private:
    void createOuterLeftRightOutline(qint32 range, QColor color);
    void createOuterTopBottomOutline(qint32 range, QColor color);
    void createOutline(qint32 i, qint32 range, QColor color);
private:
    oxygine::spSprite m_CurrentCursor;
    oxygine::spActor m_cursorRangeOutline;
    bool m_onMap{false};
    GameMap* m_pMap{nullptr};

    qint32 m_MapPointX{0};
    qint32 m_MapPointY{0};
};

#endif // CURSOR_H
