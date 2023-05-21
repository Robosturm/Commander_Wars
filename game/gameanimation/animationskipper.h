#pragma once

#include <QObject>

#include "game/GameEnums.h"

class AnimationSkipper final : public QObject
{
    Q_OBJECT
public:
    explicit AnimationSkipper();
    ~AnimationSkipper() = default;
    void startSeeking();
    static void disableAllAnimations();
    void storeAnimationSettings();
    void restoreAnimationSettings();

private:
    bool m_storedOverworldAnimations = false;
    GameEnums::BattleAnimationMode m_storedBattleAnimMode;
    GameEnums::BattleAnimationType m_storedBatteAnimType;
    bool m_storedDialog = false;
    bool m_storedCaptureAnimation = false;
    bool m_storedMovementAnimation = false;
    bool m_storedDay2DayAnimation = false;
    quint32 m_storedDialogAnimationSpeed = 1;
    quint32 m_storedAnimationSpeed = 1;
    quint32 m_storedBattleAnimationSpeed = 1;
    quint32 m_storedCaptureAnimationSpeed = 1;
};
