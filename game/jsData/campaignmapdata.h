#pragma once

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QString>

class CampaignMapData final : public QObject
{
    Q_OBJECT
public:
    explicit CampaignMapData();
   virtual ~CampaignMapData() = default;
    Q_INVOKABLE const QString getMapBackground() const;
    Q_INVOKABLE void setMapBackground(const QString newMapBackground);
    Q_INVOKABLE const QString getFolder() const;
    Q_INVOKABLE void setFolder(const QString newFolder);
    Q_INVOKABLE qint32 getMapWidth() const;
    Q_INVOKABLE void setMapWidth(qint32 newMapWidth);
    Q_INVOKABLE qint32 getMapHeight() const;
    Q_INVOKABLE void setMapHeight(qint32 newMapHeight);
    Q_INVOKABLE const QStringList getMapFilenames() const;
    Q_INVOKABLE void setMapFilenames(const QStringList newMapFilenames);
    Q_INVOKABLE const QVector<QPointF> getOpenMapPositions() const;
    Q_INVOKABLE void setOpenMapPositions(const QVector<QPointF> newOpenMapPositions);
    Q_INVOKABLE const QVector<qint32> getNewMapPosition() const;
    Q_INVOKABLE void setNewMapPosition(const QVector<qint32> newNewMapPosition);
    Q_INVOKABLE qint32 getNewlyWonMap() const;
    Q_INVOKABLE void setNewlyWonMap(qint32 newNewlyWonMap);
    Q_INVOKABLE const QVector<QPointF> getWonMapPositions() const;
    Q_INVOKABLE void setWonMapPositions(const QVector<QPointF> newWonMapPositions);

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

Q_DECLARE_INTERFACE(CampaignMapData, "CampaignMapData");
