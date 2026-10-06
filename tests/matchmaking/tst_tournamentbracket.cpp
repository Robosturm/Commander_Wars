#include <QtTest/QtTest>

#include <QJsonArray>
#include <QJsonDocument>
#include <QSet>

#include "network/tournamentbracket.h"
#include "network/tournamentbracketcontroller.h"

class TournamentBracketTest final : public QObject
{
    Q_OBJECT

    static QVector<TournamentBracket::Entrant> entrants(int count)
    {
        QVector<TournamentBracket::Entrant> result;
        for (int seed = 1; seed <= count; ++seed)
        {
            result.append({QStringLiteral("Seed%1").arg(seed), 2000 - seed});
        }
        return result;
    }

private slots:
    void emptyBracket()
    {
        TournamentBracket bracket;
        QVERIFY(!bracket.isComplete());
        QVERIFY(bracket.readyMatches().isEmpty());
        QVERIFY(bracket.placements().isEmpty());
        QCOMPARE(bracket.roundCount(), 0);
        QString error;
        QVERIFY(!bracket.reportResult(0, QStringLiteral("Seed1"), error));
        QVERIFY(!error.isEmpty());
    }

    void seedingAndRounds()
    {
        TournamentBracket bracket;
        QString error;
        const QVector<TournamentBracket::Entrant> players{
            {QStringLiteral("Low"), 500}, {QStringLiteral("FirstTie"), 1000},
            {QStringLiteral("Top"), 2000}, {QStringLiteral("SecondTie"), 1000}};
        QVERIFY2(bracket.setup(players, false, error), qPrintable(error));
        QVERIFY(error.isEmpty());
        QCOMPARE(bracket.entrants()[0].name, QStringLiteral("Top"));
        QCOMPARE(bracket.entrants()[1].name, QStringLiteral("FirstTie"));
        QCOMPARE(bracket.entrants()[2].name, QStringLiteral("SecondTie"));
        QCOMPARE(bracket.entrants()[3].name, QStringLiteral("Low"));
        QCOMPARE(bracket.roundCount(), 2);
        QCOMPARE(bracket.bracketSize(), 4);
        QCOMPARE(bracket.matches().size(), 3);
        QCOMPARE(bracket.readyMatches().size(), 2);
        QCOMPARE(bracket.matches()[0].player1, 0);
        QCOMPARE(bracket.matches()[0].player2, 3);
        QCOMPARE(bracket.matches()[1].player1, 1);
        QCOMPARE(bracket.matches()[1].player2, 2);
        QCOMPARE(bracket.matches()[2].round, 1);
        QCOMPARE(bracket.matches()[2].slot, 0);
        QVERIFY(!bracket.matches()[2].completed);
    }

    void explicitSeedOrderPreservesRatings()
    {
        TournamentBracket bracket;
        QString error;
        const QVector<TournamentBracket::Entrant> players{
            {QStringLiteral("Top"), 2000}, {QStringLiteral("Second"), 1900},
            {QStringLiteral("RandomLow2"), 500}, {QStringLiteral("RandomLow1"), 900}};
        QVERIFY2(bracket.setupSeededOrder(players, false, error), qPrintable(error));
        QCOMPARE(bracket.entrants()[0].name, QStringLiteral("Top"));
        QCOMPARE(bracket.entrants()[1].name, QStringLiteral("Second"));
        QCOMPARE(bracket.entrants()[2].name, QStringLiteral("RandomLow2"));
        QCOMPARE(bracket.entrants()[2].rating, 500);
        QCOMPARE(bracket.matches()[0].player1, 0);
        QCOMPARE(bracket.matches()[0].player2, 3);
        QCOMPARE(bracket.matches()[1].player1, 1);
        QCOMPARE(bracket.matches()[1].player2, 2);
        const auto restored = TournamentBracket::fromJson(bracket.toJson(), error);
        QVERIFY2(restored.has_value(), qPrintable(error));
        QCOMPARE(restored->entrants()[2].name, QStringLiteral("RandomLow2"));
        QCOMPARE(restored->matches()[1].player2, 2);
    }

    void byesAndPendingFeeders()
    {
        TournamentBracket bracket;
        QString error;
        QVERIFY2(bracket.setup(entrants(5), false, error), qPrintable(error));
        QCOMPARE(bracket.bracketSize(), 8);
        QCOMPARE(bracket.roundCount(), 3);
        QCOMPARE(bracket.matches().size(), 7);
        QCOMPARE(bracket.readyMatches().size(), 2);
        const auto &matches = bracket.matches();
        // Seeds 1, 2 and 3 get the byes; seed 4 plays seed 5.
        QVERIFY(matches[0].automatic);
        QCOMPARE(matches[0].winner, 0);
        QCOMPARE(matches[1].player1, 3);
        QCOMPARE(matches[1].player2, 4);
        QVERIFY(matches[2].automatic);
        QVERIFY(matches[3].automatic);
        QCOMPARE(matches[4].player1, 0);
        QCOMPARE(matches[4].player2, -1);
        QVERIFY(!matches[4].completed);
        QCOMPARE(matches[5].player1, 1);
        QCOMPARE(matches[5].player2, 2);
        QCOMPARE(matches[6].player1, -1);
        QVERIFY(!bracket.reportResult(4, QStringLiteral("Seed1"), error));
        QVERIFY(!error.isEmpty());
        QVERIFY(bracket.reportResult(1, QStringLiteral("Seed5"), error));
        QCOMPARE(matches[4].player2, 4);
        QVERIFY(bracket.reportResult(5, QStringLiteral("Seed3"), error));
        QCOMPARE(matches[6].player2, 2);
        QVERIFY(!matches[6].completed);
        QVERIFY(bracket.reportResult(4, QStringLiteral("Seed5"), error));
        QCOMPARE(matches[6].player1, 4);
        QVERIFY(bracket.reportResult(6, QStringLiteral("Seed3"), error));
        QVERIFY(bracket.isComplete());
        const auto places = bracket.placements();
        QCOMPARE(places.size(), 5);
        QCOMPARE(places[0].name, QStringLiteral("Seed3"));
        QCOMPARE(places[0].place, 1);
        QCOMPARE(places[1].name, QStringLiteral("Seed5"));
        QCOMPARE(places[1].place, 2);
        QCOMPARE(places[2].place, 3);
        QCOMPARE(places[3].place, 3);
        QCOMPARE(places[4].name, QStringLiteral("Seed4"));
        QCOMPARE(places[4].place, 5);
    }

    void thirdPlaceProgression()
    {
        TournamentBracket bracket;
        QString error;
        QVERIFY(bracket.setup(entrants(4), true, error));
        QVERIFY(bracket.hasThirdPlaceMatch());
        QCOMPARE(bracket.matches().size(), 4);
        QVERIFY(bracket.matches()[3].thirdPlace);
        QVERIFY(bracket.reportResult(0, QStringLiteral("Seed4"), error));
        QCOMPARE(bracket.matches()[2].player1, 3);
        QCOMPARE(bracket.matches()[3].player1, 0);
        QCOMPARE(bracket.readyMatches().size(), 1);
        QVERIFY(bracket.placements().isEmpty());
        QVERIFY(bracket.reportResult(1, QStringLiteral("Seed2"), error));
        QCOMPARE(bracket.readyMatches().size(), 2);
        QCOMPARE(bracket.matches()[3].player2, 2);
        // The consolation match can be played before the final.
        QVERIFY(bracket.reportResult(3, QStringLiteral("Seed3"), error));
        QVERIFY(!bracket.isComplete());
        QCOMPARE(bracket.placements().size(), 2);
        QCOMPARE(bracket.placements()[0].place, 3);
        QCOMPARE(bracket.placements()[0].name, QStringLiteral("Seed3"));
        QVERIFY(bracket.reportResult(2, QStringLiteral("Seed4"), error));
        QVERIFY(bracket.isComplete());
        const auto places = bracket.placements();
        QCOMPARE(places.size(), 4);
        QCOMPARE(places[0].name, QStringLiteral("Seed4"));
        QCOMPARE(places[1].name, QStringLiteral("Seed2"));
        QCOMPARE(places[2].name, QStringLiteral("Seed3"));
        QCOMPARE(places[3].name, QStringLiteral("Seed1"));
        for (int index = 0; index < 4; ++index)
        {
            QCOMPARE(places[index].place, index + 1);
        }
    }

    void finalDoesNotCompletePendingThirdPlace()
    {
        TournamentBracket bracket;
        QString error;
        QVERIFY(bracket.setup(entrants(4), true, error));
        QVERIFY(bracket.reportResult(0, QStringLiteral("Seed1"), error));
        QVERIFY(bracket.reportResult(1, QStringLiteral("Seed2"), error));
        QVERIFY(bracket.reportResult(2, QStringLiteral("Seed1"), error));
        QVERIFY(!bracket.isComplete());
        QCOMPARE(bracket.placements().size(), 2);
        QCOMPARE(bracket.readyMatches().size(), 1);
        QVERIFY(bracket.readyMatches()[0].thirdPlace);
        QVERIFY(bracket.reportResult(3, QStringLiteral("Seed4"), error));
        QVERIFY(bracket.isComplete());
    }

    void invalidSetupIsAtomic()
    {
        TournamentBracket bracket;
        QString error;
        QVERIFY(bracket.setup(entrants(4), false, error));
        QVERIFY(bracket.reportResult(0, QStringLiteral("Seed1"), error));
        const auto before = bracket.toJson();
        const QVector<QVector<TournamentBracket::Entrant>> invalid{
            {}, entrants(1),
            {{QStringLiteral("Same"), 1}, {QStringLiteral("Same"), 2}},
            {{QStringLiteral(" \t"), 1}, {QStringLiteral("Other"), 2}},
            {{QStringLiteral("Negative"), -1}, {QStringLiteral("Other"), 2}}};
        for (const auto &players : invalid)
        {
            QVERIFY(!bracket.setup(players, false, error));
            QVERIFY(!error.isEmpty());
            QCOMPARE(bracket.toJson(), before);
        }
        QVERIFY(!bracket.setup(entrants(3), true, error));
        QVERIFY(!error.isEmpty());
        QCOMPARE(bracket.toJson(), before);
        QVERIFY(bracket.setup(entrants(2), false, error));
        QVERIFY(error.isEmpty());
        QCOMPARE(bracket.matches().size(), 1);
        QVERIFY(bracket.placements().isEmpty());
    }

    void invalidResultsAreAtomic()
    {
        TournamentBracket bracket;
        QString error;
        QVERIFY(bracket.setup(entrants(3), false, error));
        const auto before = bracket.toJson();
        for (int id : {-1, 3, 0, 2})
        {
            QVERIFY(!bracket.reportResult(id, QStringLiteral("Seed1"), error));
            QVERIFY(!error.isEmpty());
            QCOMPARE(bracket.toJson(), before);
        }
        QVERIFY(!bracket.reportResult(1, QStringLiteral("Seed1"), error));
        QVERIFY(!error.isEmpty());
        QCOMPARE(bracket.toJson(), before);
        QVERIFY(bracket.reportResult(1, QStringLiteral("Seed3"), error));
        QVERIFY(error.isEmpty());
        const auto reported = bracket.toJson();
        QVERIFY(!bracket.reportResult(1, QStringLiteral("Seed2"), error));
        QVERIFY(!error.isEmpty());
        QCOMPARE(bracket.toJson(), reported);
    }

    void completionAndRoundTrip_data()
    {
        QTest::addColumn<int>("count");
        QTest::addColumn<bool>("thirdPlace");
        for (int count = 2; count <= 33; ++count)
        {
            QTest::newRow(qPrintable(QStringLiteral("%1-no-third").arg(count))) << count << false;
            if (count >= 4)
            {
                QTest::newRow(qPrintable(QStringLiteral("%1-third").arg(count))) << count << true;
            }
        }
    }

    void completionAndRoundTrip()
    {
        QFETCH(int, count);
        QFETCH(bool, thirdPlace);
        TournamentBracket bracket;
        QString error;
        QVERIFY2(bracket.setup(entrants(count), thirdPlace, error), qPrintable(error));
        int automaticCount = 0;
        for (const auto &match : bracket.matches())
        {
            automaticCount += match.automatic ? 1 : 0;
        }
        QCOMPARE(automaticCount, bracket.bracketSize() - count);
        int played = 0;
        while (!bracket.isComplete())
        {
            const auto ready = bracket.readyMatches();
            QVERIFY(!ready.isEmpty());
            for (const auto &match : ready)
            {
                QVERIFY2(bracket.reportResult(match.id, bracket.entrants()[match.player1].name, error),
                         qPrintable(error));
                ++played;
            }
            // Exercise actual JSON bytes, not only an in-memory object.
            const auto json = QJsonDocument::fromJson(QJsonDocument(bracket.toJson()).toJson()).object();
            auto restored = TournamentBracket::fromJson(json, error);
            QVERIFY2(restored.has_value(), qPrintable(error));
            QVERIFY(error.isEmpty());
            QCOMPARE(restored->toJson(), bracket.toJson());
            QCOMPARE(restored->isComplete(), bracket.isComplete());
            QCOMPARE(restored->readyMatches().size(), bracket.readyMatches().size());
            for (int index = 0; index < bracket.matches().size(); ++index)
            {
                const auto &original = bracket.matches()[index];
                const auto &loaded = restored->matches()[index];
                QCOMPARE(loaded.id, original.id);
                QCOMPARE(loaded.round, original.round);
                QCOMPARE(loaded.slot, original.slot);
                QCOMPARE(loaded.thirdPlace, original.thirdPlace);
                QCOMPARE(loaded.player1, original.player1);
                QCOMPARE(loaded.player2, original.player2);
                QCOMPARE(loaded.winner, original.winner);
                QCOMPARE(loaded.completed, original.completed);
                QCOMPARE(loaded.automatic, original.automatic);
            }
            bracket = std::move(*restored);
        }
        QCOMPARE(played, count - 1 + (thirdPlace ? 1 : 0));
        QVERIFY(bracket.readyMatches().isEmpty());
        const auto places = bracket.placements();
        QCOMPARE(places.size(), count);
        QCOMPARE(places[0].place, 1);
        QSet<QString> placed;
        for (const auto &place : places)
        {
            QVERIFY(!placed.contains(place.name));
            placed.insert(place.name);
            QVERIFY(place.place >= 1 && place.place <= bracket.bracketSize());
            const auto seed = place.name.mid(4).toInt();
            QCOMPARE(place.rating, 2000 - seed);
        }
        if (thirdPlace)
        {
            QCOMPARE(places[2].place, 3);
            QCOMPARE(places[3].place, 4);
        }
    }

    void jsonBeforeResultsAndUnorderedResults()
    {
        TournamentBracket bracket;
        QString error;
        QVERIFY(bracket.setup(entrants(5), true, error));
        auto restored = TournamentBracket::fromJson(bracket.toJson(), error);
        QVERIFY2(restored.has_value(), qPrintable(error));
        QCOMPARE(restored->toJson(), bracket.toJson());
        while (!bracket.isComplete())
        {
            const auto ready = bracket.readyMatches();
            QVERIFY(!ready.isEmpty());
            for (const auto &match : ready)
            {
                QVERIFY(bracket.reportResult(match.id, bracket.entrants()[match.player2].name, error));
            }
        }
        auto json = bracket.toJson();
        const auto results = json.value(QStringLiteral("results")).toArray();
        QJsonArray reversed;
        for (qsizetype index = results.size(); index > 0; --index)
        {
            reversed.append(results[index - 1]);
        }
        json.insert(QStringLiteral("results"), reversed);
        restored = TournamentBracket::fromJson(json, error);
        QVERIFY2(restored.has_value(), qPrintable(error));
        QCOMPARE(restored->toJson(), bracket.toJson());
        QVERIFY(restored->isComplete());
    }

    void malformedJson()
    {
        TournamentBracket bracket;
        QString error;
        QVERIFY(bracket.setup(entrants(3), false, error));
        const auto valid = bracket.toJson();
        QVector<QJsonObject> invalid;
        invalid.append(QJsonObject{});
        for (const auto &key : {QStringLiteral("version"), QStringLiteral("entrants"),
                                QStringLiteral("thirdPlace"), QStringLiteral("seedOrderProvided"),
                                QStringLiteral("results")})
        {
            auto json = valid;
            json.remove(key);
            invalid.append(json);
        }
        for (const auto &version : {QJsonValue(3), QJsonValue(1.5), QJsonValue("1")})
        {
            auto json = valid;
            json.insert(QStringLiteral("version"), version);
            invalid.append(json);
        }
        auto json = valid;
        json.insert(QStringLiteral("thirdPlace"), 1);
        invalid.append(json);
        json = valid;
        json.insert(QStringLiteral("entrants"), QJsonArray{false});
        invalid.append(json);
        for (const auto &rating : {QJsonValue(-1), QJsonValue(1.5), QJsonValue(2147483648.0),
                                   QJsonValue("1000"), QJsonValue()})
        {
            auto players = valid.value(QStringLiteral("entrants")).toArray();
            auto player = players[0].toObject();
            player.insert(QStringLiteral("rating"), rating);
            players[0] = player;
            json = valid;
            json.insert(QStringLiteral("entrants"), players);
            invalid.append(json);
        }
        const auto result = [](QJsonValue id, QJsonValue winner) {
            return QJsonObject{{QStringLiteral("matchId"), id}, {QStringLiteral("winner"), winner}};
        };
        const QVector<QJsonArray> badResults{
            {false},
            {result(1.5, QStringLiteral("Seed2"))},
            {result(-1, QStringLiteral("Seed2"))},
            {result(99, QStringLiteral("Seed2"))},
            {result(0, QStringLiteral("Seed1"))}, // automatic bye
            {result(2, QStringLiteral("Seed1"))}, // missing semifinal result
            {result(1, QStringLiteral("Seed1"))}, // not a participant
            {result(1, 2)},
            {result(1, QStringLiteral("Seed2")), result(1, QStringLiteral("Seed2"))}};
        for (const auto &results : badResults)
        {
            json = valid;
            json.insert(QStringLiteral("results"), results);
            invalid.append(json);
        }
        for (const auto &object : invalid)
        {
            QVERIFY(!TournamentBracket::fromJson(object, error).has_value());
            QVERIFY(!error.isEmpty());
        }
        QVERIFY(TournamentBracket::fromJson(valid, error).has_value());
        QVERIFY(error.isEmpty());
        auto versionOne = valid;
        versionOne.insert(QStringLiteral("version"), 1);
        versionOne.remove(QStringLiteral("seedOrderProvided"));
        QVERIFY(TournamentBracket::fromJson(versionOne, error).has_value());
        QVERIFY(error.isEmpty());
    }

    void scriptControllerRoundTrip()
    {
        TournamentBracketController controller;
        QVERIFY(controller.setup({QStringLiteral("A"), QStringLiteral("B"),
                                  QStringLiteral("C"), QStringLiteral("D")},
                                 {2000, 1800, 1600, 1400}, true));
        QVERIFY(controller.getLastError().isEmpty());
        auto ready = QJsonDocument::fromJson(controller.readyMatchesJson().toUtf8()).array();
        QCOMPARE(ready.size(), 2);
        const auto first = ready[0].toObject();
        QVERIFY(controller.reportResult(first.value(QStringLiteral("matchId")).toInt(),
                                        first.value(QStringLiteral("player1")).toString()));
        QVERIFY(controller.getLastError().isEmpty());

        TournamentBracketController restored;
        QVERIFY(restored.load(controller.save()));
        QVERIFY(restored.getLastError().isEmpty());
        QCOMPARE(restored.readyMatchesJson(), controller.readyMatchesJson());
        QCOMPARE(restored.placementsJson(), controller.placementsJson());
        const QString originalState = restored.save();
        QVERIFY(!restored.setup({QStringLiteral("Bad")}, {QVariant(1.5)}, false));
        QVERIFY(!restored.getLastError().isEmpty());
        QCOMPARE(restored.save(), originalState);
        QVERIFY(!restored.load(QStringLiteral("{")));
        QVERIFY(!restored.getLastError().isEmpty());
    }

    void scriptControllerSupportsExplicitSeedOrder()
    {
        TournamentBracketController controller;
        QVERIFY(controller.setupSeededOrder(
            {QStringLiteral("Top"), QStringLiteral("Second"), QStringLiteral("RandomLow"),
             QStringLiteral("Low")},
            {2000, 1900, 100, 50}, false));
        const auto ready = QJsonDocument::fromJson(controller.readyMatchesJson().toUtf8()).array();
        QCOMPARE(ready.size(), 2);
        const auto match = ready.first().toObject();
        QCOMPARE(match.value(QStringLiteral("player1")).toString(), QStringLiteral("Top"));
        QCOMPARE(match.value(QStringLiteral("player2")).toString(), QStringLiteral("Low"));
    }
};

QTEST_GUILESS_MAIN(TournamentBracketTest)
#include "tst_tournamentbracket.moc"
