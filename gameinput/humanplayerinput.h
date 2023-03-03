#ifndef HUMANPLAYERINPUT_H
#define HUMANPLAYERINPUT_H

#include <QVector>
#include <QVector3D>
#include <QPoint>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "gameinput/basegameinputif.h"
#include "gameinput/humanplayerinputmenu.h"

#include "game/unitpathfindingsystem.h"

#include "coreengine/LUPDATE_MACROS.h"

#include "menue/gamemenue.h"

class GameMap;
class GameAction;
class UnitPathFindingSystem;

class HumanPlayerInput;
using spHumanPlayerInput = oxygine::intrusive_ptr<HumanPlayerInput>;


class HumanPlayerInput : public BaseGameInputIF
{
    Q_OBJECT
public:
    ENUM_CLASS Arrows
    {
        LeftRight = 0,
        UpDown,
        UpRight,
        UpLeft,
        DownRight,
        DownLeft,
        Right,
        Left,
        Up,
        Down
    };

    explicit HumanPlayerInput(GameMap* pMap);
    virtual ~HumanPlayerInput();

    virtual void init(GameMenue* pMenu) override;
    /**
     * @brief deleteArrow deletes the current unit path arrow
     */
    void deleteArrow();
    /**
     * @brief createArrow
     * @param points
     */
    void createArrow(std::vector<QPoint>& points);
    /**
     * @brief selectUnit selects the given target unit as active
     * @param x
     * @param y
     */
    void selectUnit(qint32 x, qint32 y);
    /**
     * @brief createMarkedMoveFields createsthe fields marked for moving
     */
    void createMarkedMoveFields();
    /**
     * @brief createMarkedField
     * @param x field
     * @param y field
     * @param color of the marked field
     */
    void createMarkedField(QPoint point, QColor color);
    /**
     * @brief createCursorPath creates the arrow showing the current unit path
     * @param x
     * @param y
     */
    void createCursorPath(qint32 x, qint32 y);
    /**
     * @brief createActionMenu creates the menu for selecting the next action
     * @param actionIDs
     */
    void createActionMenu(const QStringList & actionIDs, qint32 x, qint32 y);
    /**
     * @brief attachActionMenu
     * @param x
     * @param y
     */
    void attachActionMenu(qint32 x, qint32 y);
    /**
     * @brief finishAction finishes input and performs the action
     */
    void finishAction();
    /**
     * @brief getNextStepData
     */
    void getNextStepData();
    /**
     * @brief cleanUpInput resets all internal data
     */
    void cleanUpInput();
    /**
     * @brief HumanPlayerInput::showAttackableFields
     * @param x
     * @param y
     */
    void showAttackableFields(qint32 x, qint32 y);
    /**
     * @brief showVisionFields
     * @param x
     * @param y
     */
    void showVisionFields(qint32 x, qint32 y);
    /**
     * @brief clearMarkedFields
     */
    void clearMarkedFields();
    /**
     * @brief clearMenu
     */
    void clearMenu();
    /**
     * @brief cancelActionInput
     */
    void cancelActionInput();
    /**
     * @brief cancelSelection
     */
    void cancelSelection(qint32 x, qint32 y);
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 3;
    }

    virtual void centerCameraOnAction(GameAction* pAction) override;
    /**
     * @brief isCurrentPlayer
     * @return
     */
    bool isCurrentPlayer(Player* pPlayer) const;
    /**
     * @brief getPerformingPlayer
     * @return
     */
    virtual Player* getPerformingPlayer(Player*) const;
signals:
    /**
     * @brief performAction signal with an action to be performed the action has to be deleted by the reciever of this slot. Only one slot can be connected to this signal
     * @param pAction
     */
    void performAction(spGameAction pAction, bool fromAiPipe = false);
    void sigNextTurn();
public slots:
    void rightClickDown(qint32 x, qint32 y);
    void rightClickUp(qint32 x, qint32 y);
    virtual void leftClick(qint32 x, qint32 y);
    void cursorMoved(qint32 x, qint32 y);
    void keyDown(oxygine::KeyEvent event);
    void menuItemSelected(const QString & itemID, qint32 cost);
    void markedFieldSelected(QPoint point);
    virtual QStringList getEmptyActionList();
    QStringList getViewplayerActionList();
    virtual void autoEndTurn();
    void syncMarkedFields();
    void nextTurn();
    bool inputAllowed();
    void gotoNext();
    void gotoPrevious();
    void performBasicAction(QString action);
    void showInfoMenu(qint32 x, qint32 y);
protected slots:
    void zoomChanged(float zoom);
protected:
    void nextMarkedField();
    void previousMarkedField();
    void nextSelectOption();
    void previousSelectOption();
    void showSelectedUnitAttackableFields(bool all);
    void showUnitAttackFields(Unit* pUnit, std::vector<QPoint> & usedFields);
    /**
     * @brief HumanPlayerInput::createMarkedFieldActor
     * @param point
     * @param color
     * @param drawPriority
     * @return
     */
    oxygine::spSprite createMarkedFieldActor(QPoint point, QColor color);
    /**
     * @brief createSimpleZInformation
     * @param pData
     */
    void createSimpleZInformation(qint32 x, qint32 y, const MarkedFieldData::ZInformation* pData);
    /**
     * @brief createComplexZInformation
     * @param pData
     */
    void createComplexZInformation(qint32 x, qint32 y, const MarkedFieldData::ZInformation* pData);
private:
    spGameAction m_pGameAction{nullptr};
    spUnitPathFindingSystem m_pUnitPathFindingSystem{nullptr};

    static oxygine::spActor m_ZInformationLabel;
    static spHumanPlayerInputMenu m_CurrentMenu;
    static spMarkedFieldData m_pMarkedFieldData;
    static std::vector<oxygine::spActor> m_Fields;
    static std::vector<QPoint> m_FieldPoints;
    static std::vector<oxygine::spActor> m_InfoFields;

    std::vector<oxygine::spActor> m_Arrows;
    std::vector<QPoint> m_ArrowPoints;


    QPoint m_lastMapView{std::numeric_limits<qint32>::min(),
                std::numeric_limits<qint32>::min()};
    QElapsedTimer m_doubleClickTime;

    QPoint m_lastClickPoint;
    QPoint m_lastCursorPosition;
    bool m_showVisionFields;
};

#endif // HUMANPLAYERINPUT_H
