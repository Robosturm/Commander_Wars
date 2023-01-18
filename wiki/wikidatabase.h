#ifndef WIKIDATABASE_H
#define WIKIDATABASE_H

#include <QObject>
#include <QVector>

#include "resource_management/ressourcemanagement.h"
#include "wiki/wikipage.h"

class Player;
class GameMap;
class WikiDatabase;
using spWikiDatabase = oxygine::intrusive_ptr<WikiDatabase>;

/**
 * @brief The WikiDatabase class
 */
class WikiDatabase final : public QObject, public RessourceManagement<WikiDatabase>
{
    Q_OBJECT
public:
    struct PageData
    {
        explicit PageData() = default;
        PageData (QString name, QString id, QStringList tags, QString mainId = "", qint32 item = 0)
            : m_name(name),
              m_id(id),
              m_tags(tags),
              m_mainId(mainId),
              m_item(item)
        {
        }
        QString m_name;
        QString m_id;
        QStringList m_tags;
        QString m_mainId;
        qint32 m_item{0};
    };

    ~WikiDatabase() = default;
    /**
     * @brief load
     */
    void load();
    /**
     * @brief getPage
     * @param data
     * @return
     */
    spWikipage getPage(PageData data);
    /**
     * @brief getEntries
     * @param searchTerm
     * @return
     */
    QVector<PageData> getEntries(QString searchTerm, bool onlyTag);
    /**
     * @brief getEntries
     * @param entry
     * @return
     */
    PageData getEntry(qint32 entry);
    /**
     * @brief hasEntry
     * @param file1
     * @return
     */
    bool hasEntry(QString file1);
    /**
     * @brief getEntry
     * @param id
     * @return
     */
    WikiDatabase::PageData getEntry(QString id);
    /**
     * @brief tagMatches
     * @param tags
     * @param searchTerm
     * @return
     */
    bool tagMatches(const QStringList & tags, const QString & searchTerm);
    /**
     * @brief getTags
     * @return
     */
    QStringList getTags();
    /**
     * @brief getIcon
     * @param file
     * @return
     */
    oxygine::spSprite getIcon(GameMap* pMap, QString file, qint32 size, Player* pIconPlayer = nullptr);

private:
    friend RessourceManagement<WikiDatabase>;
    friend class oxygine::intrusive_ptr<WikiDatabase>;
    explicit WikiDatabase();

private:
    QVector<PageData> m_Entries;
};

#endif // WIKIDATABASE_H
