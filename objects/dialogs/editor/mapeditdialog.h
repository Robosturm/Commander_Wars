#ifndef MAPEDITDIALOG_H
#define MAPEDITDIALOG_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"

#include "objects/base/textbox.h"

#include "objects/base/spinbox.h"

class MapEditDialog;
typedef oxygine::intrusive_ptr<MapEditDialog> spMapEditDialog;


class MapEditDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit MapEditDialog(QString mapName, QString author, QString description, QString scriptFile,
                           qint32 mapWidth, qint32 mapHeigth, qint32 playerCount,
                           qint32 turnLimit, quint32 buildLimit);
    virtual ~MapEditDialog() = default;

signals:
    void editFinished(QString mapName, QString author, QString description, QString scriptFile,
                      qint32 mapWidth, qint32 mapHeigth, qint32 playerCount,
                      qint32 turnLimit, quint32 buildLimit);
    void sigCanceled();
    void sigshowSelectScript();
public slots:
    void scriptFileChanged(QString file);
    void showSelectScript();
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ExitButton;
    spTextbox m_MapName;
    spTextbox m_MapAuthor;
    spTextbox m_MapDescription;
    spTextbox m_MapScriptFile;
    oxygine::spButton m_ScriptButton;
    spSpinBox m_MapWidth;
    spSpinBox m_MapHeigth;
    spSpinBox m_MapPlayerCount;
    spSpinBox m_MapTurnLimit;
    spSpinBox m_UnitBuildLimit;

};

#endif // MAPEDITDIALOG_H
