#ifndef VICTORYMENUE_H
#define VICTORYMENUE_H

#include <QVector>
#include <QTimer>
#include <QColor>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"

#include "objects/unitstatisticview.h"

#include "network/NetworkInterface.h"

#include "coreengine/LUPDATE_MACROS.h"
#include "menue/basemenu.h"

class VictoryMenue;
using spVictoryMenue = oxygine::intrusive_ptr<VictoryMenue>;

class VictoryMenue : public Basemenu
{
    Q_OBJECT
public:
    ENUM_CLASS GraphModes
    {
        Funds,
        Income,
        Buildings,
        Units,
        PlayerStrength,
        Max,
        PlayerStatistics,
        VictoryRanking
    };

    ENUM_CLASS StatisticModes
    {
        Produced,
        Destroyed,
        Lost
    };

    explicit VictoryMenue(spNetworkInterface pNetworkInterface);
    virtual ~VictoryMenue() = default;

    /**
     * @brief createLine creates a nice sweet polygon line for drawing our graphs
     * @param end
     * @param width
     * @param color
     * @return
     */
    oxygine::spActor createLine(QPointF end, qint32 width, QColor color);
signals:
    void sigExitMenue();
    void sigShowGraph(VictoryMenue::GraphModes mode);
    void sigFinishCurrentGraph();
    void sigOnEnter();
public slots:
    void exitMenue();
    /**
     * @brief showGraph
     * @param mode
     */
    void showGraph(VictoryMenue::GraphModes mode);
    /**
     * @brief updateGraph
     */
    void updateGraph();
    /**
     * @brief finishGraph
     */
    void finishGraph();
    /**
     * @brief onEnter
     */
    void onEnter();
protected slots:
    /**
     * @brief onProgressTimerStart
     */
    void onProgressTimerStart();
    /**
     * @brief showPlayerStatistic
     * @param player
     * @param mode
     */
    void showPlayerStatistic(qint32 player);
protected:
    /**
     * @brief drawGraphStep
     */
    void drawGraphStep(qint32 progress);
    /**
     * @brief getStepTime
     * @return
     */
    qint32 getStepTime();
    /**
     * @brief AddScoreToUserdata
     */
    void AddScoreToUserdata();
    /**
     * @brief addShopMoney
     */
    void addShopMoney();
    /**
     * @brief createStatisticsView
     */
    void createStatisticsView();
private:
    GraphModes m_CurrentGraphMode{GraphModes::Funds};
    /**
     * @brief m_PlayerGraphs first dimensions holds the graph modes, second dimension the graphs for each player
     */
    QVector<QVector<oxygine::spActor>> m_PlayerGraphs{static_cast<qint32>(GraphModes::Max)};
    /**
     * @brief m_GraphProgress how far we got with drawing each graph
     */
    QVector<qint32> m_GraphProgress = QVector<qint32>(static_cast<qint32>(GraphModes::Max), 0);
    /**
     * @brief m_GraphMaxValues maximum values for each graph :)
     */
    QVector<qint32> m_GraphMaxValues = QVector<qint32>(static_cast<qint32>(GraphModes::Max), 0);

    QVector<oxygine::spColorRectSprite> m_GraphsProgessLine = QVector<oxygine::spColorRectSprite>(static_cast<qint32>(GraphModes::Max), nullptr);
    /**
     * @brief m_VisiblePlayers holds the graphs of which players are drawn (by default all)
     */
    QVector<bool> m_VisiblePlayers;
    /**
     * @brief m_ProgressTimer tick timer for updating the graph
     */
    QTimer m_ProgressTimer;

    float m_lineLength{0};

    oxygine::spBox9Sprite m_pGraphBackground;

    oxygine::spTextField m_Textfield;

    spPanel m_PlayerSelectPanel;

    QVector<oxygine::spTextField> m_YGraphItems;

    QVector<QVector3D> m_VictoryScores;
    QVector<QVector<oxygine::spTextField>> m_VictoryTexts;
    spPanel m_VictoryPanel;

    oxygine::spBox9Sprite m_statisticsBox;
    spUnitStatisticView m_statisticsView;
    spDropDownmenu m_pStatisticPlayer;

    qint32 m_progress = 0;

    spNetworkInterface m_pNetworkInterface;

};

#endif // VICTORYMENUE_H

