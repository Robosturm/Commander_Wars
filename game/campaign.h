#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include <QObject>
#include <QVector>
#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

class GameMap;
class CampaignMapData;
class Campaign;
using spCampaign = oxygine::intrusive_ptr<Campaign>;

class Campaign final : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    struct CampaignMapInfo
    {
        CampaignMapInfo(const QString & folder, const QStringList & mapFilenames)
            : m_folder(folder),
              m_mapFilenames(mapFilenames)
        {
        }
        QString m_folder;
        QStringList m_mapFilenames;
    };
    static const char* const scriptName;
    explicit Campaign(QString file);
    explicit Campaign();
    ~Campaign();
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 1;
    }
    /**
     * @brief init loads the script and stores the script in the script variable
     */
    void init();
    /**
     * @brief getCampaignMaps
     * @return
     */
    CampaignMapInfo getCampaignMaps();
    /**
     * @brief getSelectableCOs
     * @return
     */
    QStringList getSelectableCOs(GameMap* pMap, qint32 player, quint8 coIdx);
    /**
     * @brief addDeveloperMaps
     * @param folder
     * @param files
     */
    void addDeveloperMaps(QString prefix, QString folder, QStringList & files);
signals:

public slots:
    /**
     * @brief getAllowArmyCustomization if true you can change the army style and color for each player
     * @return
     */
    bool getAllowArmyCustomization(GameMap* pMap);
    /**
     * @brief mapFiniished
     */
    void mapFinished(GameMap* pMap, bool result);
    /**
     * @brief getCampaignFinished
     * @return
     */
    bool getCampaignFinished();
    /**
     * @brief getAuthor
     * @return
     */
    QString getAuthor();
    /**
     * @brief getName
     * @return
     */
    QString getName();
    /**
     * @brief getDescription
     * @return
     */
    QString getDescription();
    /**
     * @brief getVariables returns the enviroment variables of this game script
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief getAutoSelectPlayerColors
     * @return
     */
    bool getAutoSelectPlayerColors(GameMap* pMap);
    /**
     * @brief getUsesCampaignMap
     * @return
     */
    bool getUsesCampaignMap();
    /**
     * @brief getCampaignMapData
     * @param pCampaignMapData
     */
    void getCampaignMapData(CampaignMapData & pCampaignMapData);
private:
    QString m_script;
    QString m_scriptFile;
    ScriptVariables m_Variables;
    bool m_loaded{false};
};

Q_DECLARE_INTERFACE(Campaign, "Campaign");

#endif // CAMPAIGN_H
