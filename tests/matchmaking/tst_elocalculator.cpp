#include <QtTest/QtTest>

#include "network/elocalculator.h"

class EloCalculatorTest final : public QObject
{
    Q_OBJECT
private slots:
    void expectedScoreForEqualRatingsIsEven()
    {
        QCOMPARE(EloCalculator::expectedScore(1200, 1200), 0.5);
    }

    void higherRatedPlayerHasHigherExpectedScore()
    {
        QVERIFY(EloCalculator::expectedScore(1400, 1200) > 0.5);
        QVERIFY(EloCalculator::expectedScore(1200, 1400) < 0.5);
    }

    void equalRatingWinAndLossAreSymmetric()
    {
        QCOMPARE(EloCalculator::updatedRating(1200, 1200, 30, 1.0).value(), 1215);
        QCOMPARE(EloCalculator::updatedRating(1200, 1200, 30, 0.0).value(), 1185);
        QCOMPARE(EloCalculator::updatedRating(1200, 1200, 30, 0.5).value(), 1200);
    }

    void invalidInputsAreRejected()
    {
        QVERIFY(!EloCalculator::updatedRating(-1, 1200, 30, 1.0).has_value());
        QVERIFY(!EloCalculator::updatedRating(1200, 1200, -1, 1.0).has_value());
        QVERIFY(!EloCalculator::updatedRating(1200, 1200, 30, 2.0).has_value());
    }
};

QTEST_GUILESS_MAIN(EloCalculatorTest)
#include "tst_elocalculator.moc"
