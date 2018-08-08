#ifndef MOVEMENTTABLEMANAGER_H
#define MOVEMENTTABLEMANAGER_H

#include <QString>
#include <QObject>

class MovementTableManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief getInstance this object
     * @return
     */
    static MovementTableManager* getInstance();
    /**
     * @brief loadAll loads all movement table data
     */
    void loadAll();
    /**
     * @brief loadTable
     * @param tableID the movement table that should be loaded.
     * @return true if all is fine
     */
    bool loadTable(const QString& tableID);
    /**
     * @brief getMovementID
     * @param position the position in the loaded movement table list
     * @return the id of the movement table at the given index
     */
    inline QString getMovementID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedTables.size()))
        {
            return m_loadedTables.at(position);
        }
    }
    /**
     * @brief getMovementTableCount
     * @return the amount of movement table loaded at the current time
     */
    inline qint32 getMovementTableCount()
    {
        return m_loadedTables.size();
    }
    /**
     * @brief reset deletes all data
     */
    void reset();
public slots:
    qint8 getBaseMovementPoints(QString movementID, QString terrainID);
private:
    explicit MovementTableManager();
    virtual ~MovementTableManager() = default;
    QStringList m_loadedTables;
    static MovementTableManager* m_pInstance;
};

#endif // MOVEMENTTABLEMANAGER_H
