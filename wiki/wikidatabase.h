#ifndef WIKIDATABASE_H
#define WIKIDATABASE_H

#include <QObject>

#include <qvector.h>

#include "oxygine-framework.h"

#include "wiki/wikipage.h"

/**
 * @brief The WikiDatabase class
 */
class WikiDatabase : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief m_Entries
     * 0 = Name
     * 1 = ID
     * 2 = Tags
     */
    typedef std::tuple<QString, QString, QStringList> pageData;
    static WikiDatabase* getInstance();
    /**
     * @brief getPage
     * @param data
     * @return
     */
    spWikipage getPage(pageData data);
    /**
     * @brief getEntries
     * @param searchTerm
     * @return
     */
    QVector<pageData> getEntries(QString searchTerm);
    /**
     * @brief getEntries
     * @param entry
     * @return
     */
    pageData getEntry(qint32 entry);
    /**
     * @brief tagMatches
     * @param tags
     * @param searchTerm
     * @return
     */
    bool tagMatches(QStringList tags, QString searchTerm);
    /**
     * @brief getTags
     * @return
     */
    QVector<QString> getTags();
signals:

public slots:

private:
    explicit WikiDatabase();
    static WikiDatabase* m_pInstance;
    QVector<pageData> m_Entries;
};

#endif // WIKIDATABASE_H
