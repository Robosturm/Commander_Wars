#ifndef COSPRITEMANAGER_H
#define COSPRITEMANAGER_H

#include "resource_management/ressourcemanagement.h"

class COSpriteManager : public QObject, public RessourceManagement<COSpriteManager>
{
    Q_OBJECT
public:
    QVector<QString> getSpriteCOIDs();
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
    virtual oxygine::ResAnim* getResAnim(QString id, oxygine::error_policy ep = oxygine::ep_show_error) const override;
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
protected:
    friend RessourceManagement<COSpriteManager>;
    COSpriteManager();
private:
    virtual ~COSpriteManager() = default;
    QVector<std::tuple<QString, oxygine::spResAnim>> m_Ressources;
};


#endif // COSPRITEMANAGER_H
