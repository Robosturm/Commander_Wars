#ifndef DIALOGRANDOMMAP_H
#define DIALOGRANDOMMAP_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"
#include "objects/base/spinbox.h"
#include "objects/base/checkbox.h"
#include "objects/base/multislider.h"
#include "objects/base/label.h"
#include "objects/base/dropdownmenu.h"

class DialogRandomMap;
using spDialogRandomMap = oxygine::intrusive_ptr<DialogRandomMap>;

class DialogRandomMap final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogRandomMap(const QString & confirmMessage);
    ~DialogRandomMap() = default;
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
                     bool unitsDistributed,
                     bool mirrored);
    void sigCancel();
    /**
     * @brief sigShowGeneratorSelection
     */
    void sigShowGeneratorSelection();
    void sigClose();
    void sigFinish();
public slots:
    void generatorChanged(QString filename);
    void showGeneratorSelection();
    void playerChanged(qreal);
    void remove();
    void generatorFinished();
    void finished();
private:
    void createUnitChances();
    void checkIfGenerationIsAllowed();
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
    spCheckbox m_mirrored;
    spSpinBox m_Seed;
    spCheckbox m_CreateRoad;
    spSlider m_BaseSize;
    QStringList m_TerrainIDs;
    spLabel m_TerrainChanceLabel;
    spMultislider m_TerrainChances;
    QStringList m_BuildingIDs;
    spLabel m_BuildingChanceLabel;
    spMultislider m_BuildingChances;
    spLabel m_OwnerDistributionLabel;
    spMultislider m_OwnerDistribution;
    spLabel m_unitCountLabel;
    spSpinBox m_unitCount;
    spLabel m_unitsNearHqLabel;
    spSpinBox m_unitsNearHq;
    spLabel m_unitDistributionLabel;
    spDropDownmenu m_unitDistributionSelection;
    spLabel m_UnitDistributionLabel;
    spMultislider m_unitDistribution;
    spLabel m_UnitChanceLabel;
    spMultislider m_UnitChances;
    QStringList m_UnitIDs;
    QVector<qint32> m_UnitChanceValues;
    spPanel m_pPanel;
    QString m_confirmMessage;
};

#endif // DIALOGRANDOMMAP_H
