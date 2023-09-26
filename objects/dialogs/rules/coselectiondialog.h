#ifndef COSELECTIONDIALOG_H
#define COSELECTIONDIALOG_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

#include "objects/coselection.h"

#include "game/gamemap.h"

class GameMap;
class COSelectionDialog;
using spCOSelectionDialog = std::shared_ptr<COSelectionDialog>;


class COSelectionDialog final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit COSelectionDialog(GameMap* pMap, QString coid, QColor color, qint32 player, QStringList coids = {});
    virtual ~COSelectionDialog() = default;
signals:
    void editFinished(QString coid, qint32 player);
    void canceled();
    void sigShowCOInfo();
    void sigFinished();
public slots:
    void selectedCOIDChanged(QString coid);
    void showCOInfo();
private slots:
    void remove();
private:
    void filterAndSortCoIds(const QStringList & coids);
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ShowCOInfoButton;
    oxygine::spButton m_ExitButton;
    spCOSelection m_COSelection;
    QString m_currentCOID;
    QStringList m_coids;
    qint32 m_player;
    GameMap* m_pMap{nullptr};
};

#endif // COSELECTIONDIALOG_H
