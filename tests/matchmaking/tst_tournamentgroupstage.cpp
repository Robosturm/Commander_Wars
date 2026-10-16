#include <QtTest/QtTest>

#include <QJsonDocument>
#include <QSet>

#include "network/tournamentbracketcontroller.h"
#include "network/tournamentgroupstage.h"

class TournamentGroupStageTest final : public QObject
{
    Q_OBJECT

private slots:
    void snakeSeedingAndRoundRobin()
    {
        TournamentGroupStage stage;
        QString error;
        const QVector<TournamentGroupStage::Entrant> entrants{
            {QStringLiteral("A"), 2000}, {QStringLiteral("B"), 1900},
            {QStringLiteral("C"), 1800}, {QStringLiteral("D"), 1700},
            {QStringLiteral("E"), 1600}, {QStringLiteral("F"), 1500}};
        QVERIFY2(stage.setup(entrants, 3, 1, error), qPrintable(error));
        QCOMPARE(stage.groupCount(), 2);
        QCOMPARE(stage.matches().size(), 6);
        QCOMPARE(stage.readyMatches().size(), 2);
        QCOMPARE(stage.matches()[0].group, 0);
        QCOMPARE(stage.matches()[1].group, 0);
        QCOMPARE(stage.matches()[2].group, 0);
        QCOMPARE(stage.matches()[3].group, 1);
        QSet<QPair<int, int>> fixtures;
        for (const auto &match : stage.matches())
        {
            const auto pair = qMakePair(std::min(match.player1, match.player2),
                                        std::max(match.player1, match.player2));
            QVERIFY(!fixtures.contains(pair));
            fixtures.insert(pair);
        }
        QCOMPARE(fixtures.size(), 6);
    }

    void reportingStandingsAndAdvancement()
    {
        TournamentGroupStage stage;
        QString error;
        const QVector<TournamentGroupStage::Entrant> entrants{
            {QStringLiteral("A"), 2000}, {QStringLiteral("B"), 1900},
            {QStringLiteral("C"), 1800}, {QStringLiteral("D"), 1700}};
        QVERIFY(stage.setup(entrants, 2, 1, error));
        QVERIFY(stage.reportResult(0, 2, 1, error));
        QVERIFY(stage.reportResult(1, 0, 0, error));
        QVERIFY(stage.isComplete());
        const auto standings = stage.standings();
        QCOMPARE(standings.size(), 4);
        QCOMPARE(standings[0].name, QStringLiteral("A"));
        QCOMPARE(standings[0].points, 3);
        QCOMPARE(standings[1].name, QStringLiteral("D"));
        QCOMPARE(standings[1].points, 0);
        QCOMPARE(standings[2].name, QStringLiteral("B"));
        QCOMPARE(standings[3].name, QStringLiteral("C"));
        const auto advancers = stage.advancers();
        QCOMPARE(advancers.size(), 2);
        QCOMPARE(advancers[0].name, QStringLiteral("A"));
        QCOMPARE(advancers[1].name, QStringLiteral("B"));
        QVERIFY(!stage.reportResult(0, 0, 0, error));
    }

    void tiedPointsUseHeadToHeadThenRating()
    {
        TournamentGroupStage stage;
        QString error;
        const QVector<TournamentGroupStage::Entrant> entrants{
            {QStringLiteral("A"), 900}, {QStringLiteral("B"), 1200},
            {QStringLiteral("C"), 800}, {QStringLiteral("D"), 700}};
        QVERIFY(stage.setup(entrants, 4, 1, error));
        const auto matches = stage.matches();
        for (const auto &match : matches)
        {
            const QString first = stage.entrants()[match.player1].name;
            const QString second = stage.entrants()[match.player2].name;
            if ((first == QStringLiteral("A") && second == QStringLiteral("B")) ||
                (first == QStringLiteral("B") && second == QStringLiteral("A")))
            {
                const int firstResult = first == QStringLiteral("A") ? 2 : 1;
                QVERIFY(stage.reportResult(match.id, firstResult, 3 - firstResult, error));
            }
            else if ((first == QStringLiteral("A") && second == QStringLiteral("C")) ||
                     (first == QStringLiteral("C") && second == QStringLiteral("A")))
            {
                const int firstResult = first == QStringLiteral("A") ? 1 : 2;
                QVERIFY(stage.reportResult(match.id, firstResult, 3 - firstResult, error));
            }
            else if ((first == QStringLiteral("A") && second == QStringLiteral("D")) ||
                     (first == QStringLiteral("D") && second == QStringLiteral("A")))
            {
                const int firstResult = first == QStringLiteral("A") ? 2 : 1;
                QVERIFY(stage.reportResult(match.id, firstResult, 3 - firstResult, error));
            }
            else if ((first == QStringLiteral("B") && second == QStringLiteral("C")) ||
                     (first == QStringLiteral("C") && second == QStringLiteral("B")))
            {
                const int firstResult = first == QStringLiteral("B") ? 2 : 1;
                QVERIFY(stage.reportResult(match.id, firstResult, 3 - firstResult, error));
            }
            else if ((first == QStringLiteral("B") && second == QStringLiteral("D")) ||
                     (first == QStringLiteral("D") && second == QStringLiteral("B")))
            {
                const int firstResult = first == QStringLiteral("B") ? 2 : 1;
                QVERIFY(stage.reportResult(match.id, firstResult, 3 - firstResult, error));
            }
            else
            {
                QVERIFY(stage.reportResult(match.id, 0, 0, error));
            }
        }
        const auto standings = stage.standings();
        QCOMPARE(standings[0].name, QStringLiteral("A"));
        QCOMPARE(standings[0].points, 6);
        QCOMPARE(standings[1].name, QStringLiteral("B"));
        QCOMPARE(standings[1].points, 6);
    }

    void persistenceAndController()
    {
        TournamentGroupStage stage;
        QString error;
        const QVector<TournamentGroupStage::Entrant> entrants{
            {QStringLiteral("A"), 1500}, {QStringLiteral("B"), 1400},
            {QStringLiteral("C"), 1300}, {QStringLiteral("D"), 1200}};
        QVERIFY(stage.setup(entrants, 2, 1, error));
        QVERIFY(stage.reportResult(0, 2, 1, error));
        const auto restored = TournamentGroupStage::fromJson(stage.toJson(), error);
        QVERIFY2(restored.has_value(), qPrintable(error));
        QCOMPARE(restored->matches()[0].result1, 2);
        QCOMPARE(restored->readyMatches().size(), 1);

        TournamentBracketController controller;
        QVERIFY(controller.setupGroupStage(
            {QStringLiteral("A"), QStringLiteral("B"), QStringLiteral("C"), QStringLiteral("D")},
            {1500, 1400, 1300, 1200}, 2, 1));
        QCOMPARE(QJsonDocument::fromJson(controller.readyGroupMatchesJson().toUtf8())
                     .array().size(), 2);
        QVERIFY(controller.reportGroupResult(0, 2, 1));
        QVERIFY(controller.loadGroupStage(controller.saveGroupStage()));
        QVERIFY(!controller.isGroupStageComplete());
        QVERIFY(controller.getLastError().isEmpty());
    }

    void invalidSetupsAreAtomic()
    {
        TournamentGroupStage stage;
        QString error;
        const QVector<TournamentGroupStage::Entrant> entrants{
            {QStringLiteral("A"), 1000}, {QStringLiteral("B"), 900}};
        QVERIFY(stage.setup(entrants, 2, 1, error));
        const auto before = stage.toJson();
        QVERIFY(!stage.setup({{QStringLiteral("A"), 1000}, {QStringLiteral("A"), 900}},
                             2, 1, error));
        QCOMPARE(stage.toJson(), before);
        QVERIFY(!stage.setup(entrants, 2, 2, error));
        QCOMPARE(stage.toJson(), before);
    }
};

QTEST_GUILESS_MAIN(TournamentGroupStageTest)
#include "tst_tournamentgroupstage.moc"
