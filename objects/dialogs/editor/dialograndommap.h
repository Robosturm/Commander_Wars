#ifndef DIALOGRANDOMMAP_H
#define DIALOGRANDOMMAP_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"
#include "objects/base/spinbox.h"
#include "objects/base/checkbox.h"
#include "objects/base/multislider.h"
#include "objects/base/label.h"
#include "objects/base/dropdownmenu.h"

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
                     qint32 width, qint32 heigth, qint32 playerCount,
                     bool roadSupport, qint32 seed,
                     QVector<std::tuple<QString, float>> terrains,
                     QVector<std::tuple<QString, float>> buildings,
                     QVector<float> ownedBaseSize,
                     float startBaseSize,
                     QVector<std::tuple<QString, float>> units,
                     qint32 unitCount,
                     float startBaseUnitSize,
                     QVector<float> unitDistribution,
                     bool unitsDistributed);
    void sigCancel();
    /**
     * @brief sigShowGeneratorSelection
     */
    void sigShowGeneratorSelection();
public slots:
    void generatorChanged(QString filename);
    void showGeneratorSelection();
    void playerChanged(qreal);
private:
    void createUnitChances();
private:
    spTextbox m_GeneratorFile;
    oxygine::spButton m_Generator;
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
    spSlider m_BaseSize;

    QStringList m_TerrainIDs;
    oxygine::spTextField m_TerrainChanceLabel;
    spMultislider m_TerrainChances;
    QStringList m_BuildingIDs;
    oxygine::spTextField m_BuildingChanceLabel;
    spMultislider m_BuildingChances;
    oxygine::spTextField m_OwnerDistributionLabel;
    spMultislider m_OwnerDistribution;

    spLabel m_unitCountLabel;
    spSpinBox m_unitCount;
    spLabel m_unitsNearHqLabel;
    spSpinBox m_unitsNearHq;
    spLabel m_unitDistributionLabel;
    spDropDownmenu m_unitDistributionSelection;
    oxygine::spTextField m_UnitDistributionLabel;
    spMultislider m_unitDistribution;
    oxygine::spTextField m_UnitChanceLabel;
    spMultislider m_UnitChances;
    QStringList m_UnitIDs;
    QVector<qint32> m_UnitChanceValues;


    spPanel m_pPanel;
};

#endif // DIALOGRANDOMMAP_H
