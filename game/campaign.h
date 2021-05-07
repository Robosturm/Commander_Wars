#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include <QObject>
#include <QVector>
#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class GameMap;

class Campaign;
typedef oxygine::intrusive_ptr<Campaign> spCampaign;

class Campaign : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    static const QString scriptName;
    explicit Campaign(QString file);
    explicit Campaign();
    virtual ~Campaign() override;
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
    std::tuple<QString, QStringList> getCampaignMaps();
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
    void addDeveloperMaps(QString & folder, QStringList & files);
signals:

public slots:
    /**
     * @brief mapFiniished
     */
    void mapFinished(bool result);
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
private:
    QString m_script;
    QString m_scriptFile;
    ScriptVariables m_Variables;
    bool m_loaded{false};

};

#endif // CAMPAIGN_H
