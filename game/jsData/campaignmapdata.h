#pragma once

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QString>

class CampaignMapData : public QObject
{
    Q_OBJECT
public:
    explicit CampaignMapData();
    virtual ~CampaignMapData() = default;

public slots:
    const QString getMapBackground() const;
    void setMapBackground(const QString &newMapBackground);

    const QString getFolder() const;
    void setFolder(const QString &newFolder);

    qint32 getMapWidth() const;
    void setMapWidth(qint32 newMapWidth);

    qint32 getMapHeight() const;
    void setMapHeight(qint32 newMapHeight);

    const QStringList getMapFilenames() const;
    void setMapFilenames(const QStringList &newMapFilenames);

    const QVector<QPointF> getOpenMapPositions() const;
    void setOpenMapPositions(const QVector<QPointF> &newOpenMapPositions);

    const QVector<qint32> getNewMapPosition() const;
    void setNewMapPosition(const QVector<qint32> &newNewMapPosition);

    qint32 getNewlyWonMap() const;
    void setNewlyWonMap(qint32 newNewlyWonMap);

    const QVector<QPointF> getWonMapPositions() const;
    void setWonMapPositions(const QVector<QPointF> &newWonMapPositions);

private:
    QString m_mapBackground;
    QString m_folder;
    qint32 m_mapWidth{-1};
    qint32 m_mapHeight{-1};
    QStringList m_mapFilenames;
    QVector<QPointF> m_openMapPositions;
    QVector<qint32> m_newMapPosition;
    qint32 m_newlyWonMap{-1};
    QVector<QPointF> m_wonMapPositions;
};

