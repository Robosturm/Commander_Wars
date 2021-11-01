#ifndef COSPRITEMANAGER_H
#define COSPRITEMANAGER_H

#include "resource_management/ressourcemanagement.h"

class COSpriteManager : public QObject, public RessourceManagement<COSpriteManager>
{
    Q_OBJECT
public:
    QStringList getSpriteCOIDs();
    /**
     * @brief getCOStyles
     * @param id
     * @return
     */
    QStringList getCOStyles(QString id);
    /**
     * @brief getCOStyles
     * @param position
     * @return
     */
    QStringList getCOStyles(qint32 position);
    /**
     * @brief getResAnim
     * @param id
     * @param ep
     * @return
     */
    virtual oxygine::ResAnim* getResAnim(const QString & id, oxygine::error_policy ep = oxygine::ep_show_error) const override;
    /**
     * @brief release
     */
    void release();
signals:
    void sigLoadResAnim(QString coid, QString file, QImage colorTable, QImage maskTable, bool useColorBox);
public slots:
    QStringList getCoIds()
    {
        return m_loadedRessources;
    }
    /**
     * @brief loadResAnim this function is not callable from js
     * @param coid
     * @param file
     * @param colorTable
     * @param maskTable
     */
    void loadResAnim(QString coid, QString file, QImage colorTable, QImage maskTable, bool useColorBox);
    /**
     * @brief removeRessource
     * @param id
     */
    void removeRessource(QString id);
    /**
     * @brief getArmyList
     * @param coids
     * @return
     */
    QStringList getArmyList(const QStringList & coids) const;
protected:
    friend RessourceManagement<COSpriteManager>;
    COSpriteManager();
private:
    virtual ~COSpriteManager() = default;
    struct CoSprite
    {
        CoSprite(QString spriteId, oxygine::spResAnim sprite)
            : m_spriteId(spriteId),
              m_sprite(sprite)
        {
        }
        QString m_spriteId;
        oxygine::spResAnim m_sprite;
    };

    QVector<CoSprite> m_Ressources;
};


#endif // COSPRITEMANAGER_H
