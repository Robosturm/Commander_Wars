#pragma once

#include <QVector>
#include "menue/generatormenu.h"
#include "coGenerator/coability.h"
#include "coreengine/fileserializable.h"

class CoGeneratorMenu;
using spCoGeneratorMenu = std::shared_ptr<CoGeneratorMenu>;

class CoGeneratorMenu final : public GeneratorMenu
{
    Q_OBJECT
public:
    static const char* const CO_GENERATOR_MENU_JSNAME;
    static const char* const CO_GENERATOR_MENU_BASEPATH;
    static const char* const CO_GENERATOR_MENU_XML;

    CoGeneratorMenu();

    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& stream) const;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& stream);
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const
    {
        return 1;
    }

    Q_INVOKABLE void addCoAbility(const QString & coAbilityId, GameEnums::PowerMode powerMode);
    Q_INVOKABLE void removeCoAbility(qint32 index);

    Q_INVOKABLE void generateCo(const QString & modDir, const QString & coid);

    Q_INVOKABLE void storeCoData(const QString & filename) const;

    Q_INVOKABLE void loadCoData(const QString & filename);
    Q_INVOKABLE void reloadGeneratorUi();

    Q_INVOKABLE QString getCoId() const;
    Q_INVOKABLE void setCoId(const QString & newCoId);

    Q_INVOKABLE QString getCoName() const;
    Q_INVOKABLE void setCoName(const QString & newCoName);

    Q_INVOKABLE qint32 getPowerStars() const;
    Q_INVOKABLE void setPowerStars(qint32 newPowerStars);

    Q_INVOKABLE QString getCoPowerDescription() const;
    Q_INVOKABLE void setCoPowerDescription(const QString & newCoPowerDescription);

    Q_INVOKABLE QString getCoPowerName() const;
    Q_INVOKABLE void setCoPowerName(const QString & newCoPowerName);

    Q_INVOKABLE qint32 getSuperpowerStars() const;
    Q_INVOKABLE void setSuperpowerStars(qint32 newSuperpowerStars);

    Q_INVOKABLE QString getCoSuperpowerDescription() const;
    Q_INVOKABLE void setCoSuperpowerDescription(const QString & newCoSuperpowerDescription);

    Q_INVOKABLE QString getCoSuperpowerName() const;
    Q_INVOKABLE void setCoSuperpowerName(const QString & newCoSuperpowerName);

    Q_INVOKABLE QString getCoBio() const;
    Q_INVOKABLE void setCoBio(const QString & newCoBio);

    Q_INVOKABLE QString getCoHits() const;
    Q_INVOKABLE void setCoHits(const QString & newCoHits);

    Q_INVOKABLE QString getCoMiss() const;
    Q_INVOKABLE void setCoMiss(const QString & newCoMiss);

    Q_INVOKABLE QString getCoDescription() const;
    Q_INVOKABLE void setCoDescription(const QString & newCoDescription);

    Q_INVOKABLE QString getCoLongDescription() const;
    Q_INVOKABLE void setCoLongDescription(const QString & newCoLongDescription);

    Q_INVOKABLE QString getCoArmy() const;
    Q_INVOKABLE void setCoArmy(const QString & newCoArmy);

    Q_INVOKABLE qint32 getCoUnitRange() const;
    Q_INVOKABLE void setCoUnitRange(qint32 newCoUnitRange);

    Q_INVOKABLE QStringList getCoPowerSentences() const;
    Q_INVOKABLE void setCoPowerSentences(const QStringList & newCoPowerSentences);
    Q_INVOKABLE void setCoPowerSentence(const QString & newCoPowerSentence, qint32 idx);

    Q_INVOKABLE QStringList getCoVictorySentences() const;
    Q_INVOKABLE void setCoVictorySentences(const QStringList & newCoVictorySentences);
    Q_INVOKABLE void setCoVictorySentence(const QString & newCoVictorySentence, qint32 idx);

    Q_INVOKABLE QStringList getCoDefeatSentences() const;
    Q_INVOKABLE void setCoDefeatSentences(const QStringList & newCoDefeatSentences);
    Q_INVOKABLE void setCoDefeatSentence(const QString & newCoDefeatSentence, qint32 idx);

    Q_INVOKABLE qint32 getCoAbilityCount() const;
    Q_INVOKABLE CoAbility* getCoAbility(qint32 index) const;
    /**
     * @brief writeFuncPrologue
     */
    void writeCoPrologue(QTextStream & stream);
    /**
     * @brief writeCoAbilityContent
     */
    void writeCoContent(QTextStream & stream);
    /**
     * @brief writeFuncEpilogue
     */
    void writeCoEpilogue(QTextStream & stream);

private:
    QVector<spCoAbility> m_coAbilities;

    QString m_coId{"Unknown"};
    QString m_coName{"Unknown"};
    qint32 m_powerStars{3};
    QString m_coPowerDescription{"Unknown"};
    QString m_coPowerName{"Unknown"};
    qint32 m_superpowerStars{3};
    QString m_coSuperpowerDescription{"Unknown"};
    QString m_coSuperpowerName{"Unknown"};
    QString m_coBio{"Unknown"};
    QString m_coHits{"Unknown"};
    QString m_coMiss{"Unknown"};
    QString m_coDescription{"Unknown"};
    QString m_coLongDescription{"Unknown"};
    QString m_coArmy{"OS"};
    qint32 m_coUnitRange{2};
    QStringList m_coPowerSentences;
    QStringList m_coVictorySentences;
    QStringList m_coDefeatSentences;
};

Q_DECLARE_INTERFACE(CoGeneratorMenu, "CoGeneratorMenu");
