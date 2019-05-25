#ifndef DIALOGRANDOMMAP_H
#define DIALOGRANDOMMAP_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "objects/textbox.h"

#include "objects/spinbox.h"

#include "objects/checkbox.h"

#include "objects/multislider.h"

class DialogRandomMap;
typedef oxygine::intrusive_ptr<DialogRandomMap> spDialogRandomMap;

class DialogRandomMap : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogRandomMap();
    virtual ~DialogRandomMap() = default;
signals:
    void sigFinished(QString mapName, QString author, QString description,
                     qint32 width,qint32 heigth, qint32 playerCount,
                     bool roadSupport, qint32 seed,
                     float forestchance, float mountainChance, float seachance, float buildingchance);
    void sigCancel();
public slots:

private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ExitButton;
    spTextbox m_MapName;
    spTextbox m_MapAuthor;
    spTextbox m_MapDescription;
    oxygine::spButton m_ScriptButton;
    spSpinBox m_MapWidth;
    spSpinBox m_MapHeigth;
    spSpinBox m_MapPlayerCount;
    spSpinBox m_Seed;
    spCheckbox m_CreateRoad;
    spMultislider m_TerrainChances;

};

#endif // DIALOGRANDOMMAP_H
