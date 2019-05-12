#ifndef HUMANPLAYERINPUT_H
#define HUMANPLAYERINPUT_H

#include <QVector>

#include <QVector3D>

#include <QPoint>

#include "gameinput/basegameinputif.h"

#include "menue/gamemenue.h"

#include "oxygine-framework.h"

#include "gameinput/humanplayerinputmenu.h"

class GameAction;
class UnitPathFindingSystem;

class HumanPlayerInput : public BaseGameInputIF
{
    Q_OBJECT
public:
    enum class Arrows
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

    HumanPlayerInput();
    ~HumanPlayerInput();

    virtual void init() override;
    /**
     * @brief deleteArrow deletes the current unit path arrow
     */
    void deleteArrow();
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
    void createMarkedField(QPoint point, QColor color, Terrain::DrawPriority drawPriority);
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
    void createActionMenu(QStringList actionIDs, qint32 x, qint32 y);
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
     * @brief clearMarkedFields
     */
    void clearMarkedFields();
    /**
     * @brief clearMenu
     */
    void clearMenu();
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() override
    {
        return 1;
    }
signals:
    /**
     * @brief performAction signal with an action to be performed the action has to be deleted by the reciever of this slot. Only one slot can be connected to this signal
     * @param pAction
     */
    void performAction(GameAction* pAction);
public slots:
    void rightClick(qint32 x, qint32 y);
    void leftClick(qint32 x, qint32 y);
    void cursorMoved(qint32 x, qint32 y);
    void keyInput(SDL_Event event);
    void menuItemSelected(QString itemID, qint32 cost);
    void markedFieldSelected(QPoint point);
    QStringList getEmptyActionList();
    void autoEndTurn();
protected:
    void nextMarkedField();
    void previousMarkedField();
    void nextSelectOption();
    void previousSelectOption();
private:
    GameAction* m_pGameAction{nullptr};
    UnitPathFindingSystem* m_pUnitPathFindingSystem{nullptr};

    QVector<oxygine::spActor> m_Fields;
    QVector<QVector3D> m_FieldPoints;
    MarkedFieldData* m_pMarkedFieldData{nullptr};

    QVector<oxygine::spActor> m_Arrows;
    QVector<QPoint> m_ArrowPoints;

    oxygine::spActor m_ZInformationLabel;

    spHumanPlayerInputMenu m_CurrentMenu{nullptr};
};

#endif // HUMANPLAYERINPUT_H
