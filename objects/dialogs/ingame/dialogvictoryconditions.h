#ifndef DIALOGVICTORYCONDITIONS_H
#define DIALOGVICTORYCONDITIONS_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

class GameMap;
class DialogVictoryConditions;
using spDialogVictoryConditions = std::shared_ptr<DialogVictoryConditions>;

class DialogVictoryConditions final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogVictoryConditions(GameMap* pMap);
    virtual ~DialogVictoryConditions() = default;

signals:
    void sigFinished();
    void sigShowPopup(QString rule);
protected slots:
    void showPopup(QString rule);
    void keyInput(oxygine::KeyEvent event);
    void remove();
private:
    oxygine::spButton m_OkButton;
    GameMap* m_pMap{nullptr};
};

#endif // DIALOGVICTORYCONDITIONS_H
