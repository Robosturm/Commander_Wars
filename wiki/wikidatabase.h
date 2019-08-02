#ifndef WIKIDATABASE_H
#define WIKIDATABASE_H

#include <QObject>

#include <qvector.h>

#include "oxygine-framework.h"

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
    oxygine::spActor getPage(pageData& data);
signals:

public slots:

private:
    explicit WikiDatabase();
    static WikiDatabase* m_pInstance;
    QVector<pageData> m_Entries;
};

#endif // WIKIDATABASE_H
