#pragma once

#include <QObject>
#include "awbwReplayReader/awbwmapdownloader.h"
#include "awbwReplayReader/awbwreplayerreader.h"
#include "awbwReplayReader/awbwactionparser.h"
#include "game/gamerecording/iReplayReader.h"

class ReplayMenu;

class AwbwReplayPlayer : public IReplayReader
{
    Q_OBJECT
public:
    explicit AwbwReplayPlayer(ReplayMenu * pReplayMenu, GameMap* pMap);

    virtual bool loadRecord(const QString & filename) override;
    virtual QStringList getMods() override;
    virtual spGameAction nextAction() override;
    virtual void onPostAction() override;
    virtual qint32 getRecordSize() override;
    virtual qint32 getProgess() override;
    virtual DayInfo getDayFromPosition(qint32 count) override;
    virtual void seekToDay(DayInfo dayInfo) override;
    virtual void requestReplayStart() override;
    qint32 getCurrentActionPos() const;
    const AwbwReplayerReader & getReplayReader() const;
    qint32 getCurrentTurnIndex(qint32 & actionIndex) const;
    ReplayMenu * getReplayMenu() const;

private slots:
    void onDownloadResult(bool success);
private:
    void startReplayInternal();
    void loadMap(bool withOutUnits, IReplayReader::DayInfo gameStateIndex);
    void loadBuildings(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex);
    void loadBuilding(const AwbwReplayerReader::BuildingInfo & building);
    void loadUnits(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex);
    void loadUnit(const AwbwReplayerReader::UnitInfo & unit, qint32 player, const QVector<AwbwReplayerReader::UnitInfo> & units);
    void loadLoadedUnit(qint32 unitId, Unit* pTransporter, const QVector<AwbwReplayerReader::UnitInfo> & units);
    void updateCapturePoints(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex);
    void loadPlayers(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex);
    void loadPlayer(const AwbwReplayerReader::PlayerInfo & player);
    void loadCo(const AwbwReplayerReader::CoInfo & coInfo, Player* pPlayer, qint32 coIdx);
    void loadGameRules(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex);
private:
    GameMap* m_pMap;
    ReplayMenu * m_pReplayMenu;
    AwbwMapDownloader m_mapDownloader;
    AwbwReplayerReader m_replayReader;
    AwbwActionParser m_actionParser;
    bool m_startReplay{false};
    qint32 m_currentActionPos{0};
};

