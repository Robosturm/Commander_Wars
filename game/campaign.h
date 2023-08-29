#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include <QObject>
#include <QVector>
#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"
#include "coreengine/jsthis.h"

class GameMap;
class CampaignMapData;
class Campaign;
using spCampaign = std::shared_ptr<Campaign>;

class Campaign final : public QObject, public FileSerializable, public JsThis
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
    ~Campaign() = default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief serializeObject
     * @param pStream
     * @param forHash
     */
    void serializeObject(QDataStream& pStream, bool forHash) const;
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
    void addDeveloperMaps(const QString & prefix, const QString & folder, QStringList & files);
    /**
     * @brief getCampaignMapData
     * @param pCampaignMapData
     */
    void getCampaignMapData(CampaignMapData & pCampaignMapData);
    /**
     * @brief getAllowArmyCustomization if true you can change the army style and color for each player
     * @return
     */
    Q_INVOKABLE bool getAllowArmyCustomization(GameMap* pMap);
    /**
     * @brief mapFiniished
     */
    Q_INVOKABLE void mapFinished(GameMap* pMap, bool result);
    /**
     * @brief getCampaignFinished
     * @return
     */
    Q_INVOKABLE bool getCampaignFinished();
    /**
     * @brief getAuthor
     * @return
     */
    Q_INVOKABLE QString getAuthor();
    /**
     * @brief getName
     * @return
     */
    Q_INVOKABLE QString getName();
    /**
     * @brief getDescription
     * @return
     */
    Q_INVOKABLE QString getDescription();
    /**
     * @brief getVariables returns the enviroment variables of this game script
     * @return
     */
    Q_INVOKABLE inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief getAutoSelectPlayerColors
     * @return
     */
    Q_INVOKABLE bool getAutoSelectPlayerColors(GameMap* pMap);
    /**
     * @brief getUsesCampaignMap
     * @return
     */
    Q_INVOKABLE bool getUsesCampaignMap();
    /**
     * @brief onCampaignMapSelected
     */
    Q_INVOKABLE void onCampaignMapSelected(GameMap* pMap, const QString & filePath);

private:
    QString m_script;
    QString m_scriptFile;
    ScriptVariables m_Variables;
    bool m_loaded{false};
};

Q_DECLARE_INTERFACE(Campaign, "Campaign");

#endif // CAMPAIGN_H
