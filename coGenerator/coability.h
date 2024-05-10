#pragma once

#include "objects/dialogs/customdialog.h"
#include "coreengine/scriptvariables.h"
#include "coreengine/fileserializable.h"
#include "game/GameEnums.h"

class CoGeneratorMenu;
class CoAbility;
using spCoAbility = std::shared_ptr<CoAbility>;

class CoAbility : public CustomDialog
{
    Q_OBJECT
public:
    CoAbility(CoGeneratorMenu * pParent);

    CoAbility(CoGeneratorMenu * pParent, const QString & coAbilityId);
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
    /**
     * @brief getActiveCondition
     * @return
     */
    GameEnums::PowerMode getActiveCondition() const;
    /**
     * @brief setActiveCondition
     * @param newActiveCondition
     */
    void setActiveCondition(const GameEnums::PowerMode & newActiveCondition);
    /**
     * @brief getVariables
     * @return
     */
    Q_INVOKABLE inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief getFunctionNameContext
     * @return
     */
    Q_INVOKABLE QString getFunctionNameContext() const;
    Q_INVOKABLE bool getIncoRange() const;
    Q_INVOKABLE void setIncoRange(bool newIncoRange);
    Q_INVOKABLE void showCoAbility();
    /**
     * @brief writeFuncPrologue
     */
    void writeFuncPrologue(QTextStream & stream);
    /**
     * @brief writeCoAbilityContent
     */
    void writeCoAbilityContent(QTextStream & stream);
    /**
     * @brief writeFuncEpilogue
     */
    void writeFuncEpilogue(QTextStream & stream);

private:
    CoGeneratorMenu * m_pParent{nullptr};
    QVector<spCoAbility> m_coAbilities;
    bool m_incoRange{false};
    GameEnums::PowerMode m_activeCondition;
    ScriptVariables m_Variables;
    QString m_coAbilityId;
};

Q_DECLARE_INTERFACE(CoAbility, "CoAbility");
