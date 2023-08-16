#ifndef COSPRITEMANAGER_H
#define COSPRITEMANAGER_H

#include "resource_management/ressourcemanagement.h"

class COSpriteManager final : public QObject, public RessourceManagement<COSpriteManager>
{
    Q_OBJECT
public:
    struct CoGroup
    {
        QString name;
        QStringList cos;
    };
    /**
     * @brief getCOStyles
     * @param id
     * @return
     */
    Q_INVOKABLE QStringList getCOStyles(const QString id);
    /**
     * @brief getCOStyles
     * @param position
     * @return
     */
    Q_INVOKABLE QStringList getCOStyles(qint32 position);
    /**
     * @brief getResAnim
     * @param id
     * @param ep
     * @return
     */
    virtual oxygine::ResAnim* getResAnim(const QString & id, oxygine::error_policy ep = oxygine::ep_ignore_error) const override;
    /**
     * @brief release
     */
    void release();
    /**
     * @brief getCoGroups
     * @return
     */
    QVector<CoGroup> getCoGroups(QStringList & coids);
    /**
     * @brief getSpriteCOIDs
     * @return
     */
    Q_INVOKABLE QStringList getSpriteCOIDs();
    /**
     * @brief getCoIds
     * @return
     */
    Q_INVOKABLE QStringList getCoIds()
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
    Q_INVOKABLE void loadResAnim(const QString coid, const QString file, QImage colorTable, QImage maskTable, bool useColorBox);
    /**
     * @brief removeRessource
     * @param id
     */
    Q_INVOKABLE void removeRessource(const QString id);
    /**
     * @brief getArmyList
     * @param coids
     * @return
     */
    Q_INVOKABLE QStringList getArmyList(const QStringList coids) const;

signals:
    void sigLoadResAnim(const QString coid, const QString file, QImage colorTable, QImage maskTable, bool useColorBox);

protected:
    friend RessourceManagement<COSpriteManager>;
    COSpriteManager();
    ~COSpriteManager() = default;
private:
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

Q_DECLARE_INTERFACE(COSpriteManager, "COSpriteManager");

#endif // COSPRITEMANAGER_H
