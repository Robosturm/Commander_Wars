#ifndef COSELECTIONDIALOG_H
#define COSELECTIONDIALOG_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "objects/textbox.h"

#include "objects/spinbox.h"

#include "objects/coselection.h"

class COSelectionDialog;
typedef oxygine::intrusive_ptr<COSelectionDialog> spCOSelectionDialog;


class COSelectionDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit COSelectionDialog(QString coid, QColor color, qint32 player);
    virtual ~COSelectionDialog() = default;
signals:
    void editFinished(QString coid, qint32 player);
    void canceled();
public slots:
    void selectedCOIDChanged(QString coid);
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ExitButton;
    spCOSelection m_COSelection;
    oxygine::spSprite m_pCurrentCO;
    QString m_currentCOID;
    qint32 m_player;
};

#endif // COSELECTIONDIALOG_H
