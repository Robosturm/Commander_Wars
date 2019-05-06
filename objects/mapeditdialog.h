#ifndef MAPEDITDIALOG_H
#define MAPEDITDIALOG_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "objects/textbox.h"

#include "objects/spinbox.h"

class MapEditDialog;
typedef oxygine::intrusive_ptr<MapEditDialog> spMapEditDialog;


class MapEditDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit MapEditDialog(QString mapName, QString author, QString description, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount);
    virtual ~MapEditDialog() = default;

signals:
    void editFinished(QString mapName, QString author, QString description, qint32 mapWidth, qint32 mapHeigth, qint32 playerCount);
public slots:

private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ExitButton;
    spTextbox m_MapName;
    spTextbox m_MapAuthor;
    spTextbox m_MapDescription;
    spSpinBox m_MapWidth;
    spSpinBox m_MapHeigth;
    spSpinBox m_MapPlayerCount;

};

#endif // MAPEDITDIALOG_H
