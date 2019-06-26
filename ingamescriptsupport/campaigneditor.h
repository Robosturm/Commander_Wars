#ifndef CAMPAIGNEDITOR_H
#define CAMPAIGNEDITOR_H

#include <QObject>

#include <QVector>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "objects/textbox.h"

class CampaignEditor;
typedef oxygine::intrusive_ptr<CampaignEditor> spCampaignEditor;

class CampaignEditor : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    struct MapData
    {
        QString map;
        QString mapName;
        QStringList previousMaps;
        QStringList disableMaps;
        bool lastMap{false};
    };
    explicit CampaignEditor();
    virtual ~CampaignEditor() = default;
signals:
    /**
     * @brief sigFinished
     */
    void sigFinished();
    /**
     * @brief sigShowAddCampaign
     */
    void sigShowAddCampaign();
    /**
     * @brief sigShowSelectFolder
     */
    void sigShowSelectFolder();
    /**
     * @brief sigupdateCampaignData
     */
    void sigUpdateCampaignData();
public slots:
    /**
     * @brief showAddCampaign
     */
    void showAddCampaign();
    /**
     * @brief addCampaign
     */
    void addCampaign(QString filename);
    /**
     * @brief showSelectFolder
     */
    void showSelectFolder();
    /**
     * @brief selectFolder
     * @param folder
     */
    void selectFolder(QString folder);
    /**
     * @brief clearCampaignData
     */
    void clearCampaignData();
    /**
     * @brief updateCampaignData
     */
    void updateCampaignData();
private:
    spPanel m_Panel;
    /**
     * @brief m_CampaignFolder
     */
    spTextbox m_CampaignFolder;
    /**
     * @brief mapData
     */
    QVector<MapData> mapDatas;
};

#endif // CAMPAIGNEDITOR_H
