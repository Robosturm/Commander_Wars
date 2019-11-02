#ifndef COSPRITEMANAGER_H
#define COSPRITEMANAGER_H

#include "QString"
#include <QStringList>
#include <qvector.h>
#include <qimage.h>

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
    bool loadCO(QString coID);
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
     * @brief getCOName
     * @param position the position in the loaded co list
     * @return the id of the co at the given index
     */
    QString getCOName(qint32 position);
    /**
     * @brief getCOName
     * @param coid
     * @return
     */
    QString getCOName(QString coid);
    /**
     * @brief getCOIDs
     * @return
     */
    QVector<QString> getSpriteCOIDs();
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
    /**
     * @brief getResAnim
     * @param id
     * @param ep
     * @return
     */
    virtual oxygine::ResAnim* getResAnim(QString id, oxygine::error_policy ep = oxygine::ep_show_error) const override;
    /**
     * @brief loadResAnim
     * @param coid
     * @param file
     * @param colorTable
     * @param maskTable
     */
    void loadResAnim(QString coid, QString file, QImage& colorTable, QImage& maskTable, bool useColorBox);
    /**
     * @brief getCOStyles
     * @param id
     * @return
     */
    QStringList getCOStyles(QString id);
    /**
     * @brief getCOStyles
     * @param id
     * @return
     */
    QStringList getCOStyles(qint32 id);
private:
    explicit COSpriteManager();
    virtual ~COSpriteManager() = default;
    QStringList m_loadedCOs;
    QVector<std::tuple<QString, oxygine::spResAnim>> m_Ressources;
    static COSpriteManager* m_pInstance;
};

#endif // UNITSPRITEMANAGER_H
