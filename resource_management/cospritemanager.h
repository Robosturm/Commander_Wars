#ifndef COSPRITEMANAGER_H
#define COSPRITEMANAGER_H

#include "QString"
#include <QStringList>

#include "oxygine-framework.h"

class COSpriteManager : public oxygine::Resources
{
public:
    /**
     * @brief getInstance this object
     * @return
     */
    static COSpriteManager* getInstance();
    /**
     * @brief loadAll loads all co's data
     */
    void loadAll();
    /**
     * @brief loadUnit
     * @param unitID the co that should be loaded.
     * @return true if all is fine
     */
    bool loadCO(const QString& coID);
    /**
     * @brief getCOID
     * @param position the position in the loaded co list
     * @return the id of the co at the given index
     */
    inline QString getCOID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedCOs.size()))
        {
            return m_loadedCOs.at(position);
        }
        return "";
    }
    /**
     * @brief getCOIndex
     * @param id
     * @return
     */
    qint32 getCOIndex(QString id);
    /**
     * @brief getCOCount
     * @return the amount of units loaded at the current time
     */
    inline qint32 getCOCount()
    {
        return m_loadedCOs.size();
    }
    /**
     * @brief reset deletes all data
     */
    void reset();
    /**
     * @brief existsCO
     * @param coID
     */
    bool existsCO(QString coID);
private:
    explicit COSpriteManager();
    virtual ~COSpriteManager() = default;
    QStringList m_loadedCOs;
    static COSpriteManager* m_pInstance;
};

#endif // UNITSPRITEMANAGER_H
