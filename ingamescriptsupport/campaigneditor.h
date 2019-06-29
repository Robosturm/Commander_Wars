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
    static const QString campaign;
    static const QString campaignName;
    static const QString campaignDescription;
    static const QString campaignAuthor;
    static const QString campaignMaps;
    static const QString campaignMapsFolder;
    static const QString campainMapFinished;
    static const QString campaignFinished;

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
    /**
     * @brief sigShowLoadCampaign
     */
    void sigShowLoadCampaign();
    /**
     * @brief sigShowSaveCampaign
     */
    void sigShowSaveCampaign();
public slots:
    /**
     * @brief showSaveCampaign
     */
    void showSaveCampaign();
    /**
     * @brief saveCampaign
     */
    void saveCampaign(QString filename);
    /**
     * @brief showLoadCampaign
     */
    void showLoadCampaign();
    /**
     * @brief loadCampaign
     * @param filename
     */
    void loadCampaign(QString filename);
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

    spTextbox m_Description;
    spTextbox m_Author;
    spTextbox m_Name;
};

#endif // CAMPAIGNEDITOR_H
