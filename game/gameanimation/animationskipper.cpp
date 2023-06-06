#include "game/gameanimation/animationskipper.h"
#include "coreengine/settings.h"
#include "coreengine/interpreter.h"

AnimationSkipper::AnimationSkipper()
{
    Interpreter::setCppOwnerShip(this);
}

void AnimationSkipper::startSeeking()
{
    storeAnimationSettings();
    disableAllAnimations();
}

void AnimationSkipper::disableAllAnimations()
{
    Settings::getInstance()->setOverworldAnimations(false);
    Settings::getInstance()->setBattleAnimationMode(GameEnums::BattleAnimationMode::BattleAnimationMode_None);
    Settings::getInstance()->setDialogAnimation(false);
    Settings::getInstance()->setCaptureAnimation(false);
    Settings::getInstance()->setMovementAnimations(false);
    Settings::getInstance()->setDay2dayScreen(false);
    Settings::getInstance()->setAnimationSpeed(100);
    Settings::getInstance()->setBattleAnimationSpeed(100);
    Settings::getInstance()->setDialogAnimationSpeed(100);
    Settings::getInstance()->setCaptureAnimationSpeed(100);
}

void AnimationSkipper::storeAnimationSettings()
{
    // store animation modes
    m_storedOverworldAnimations = Settings::getInstance()->getOverworldAnimations();
    m_storedBattleAnimMode = Settings::getInstance()->getBattleAnimationMode();
    m_storedBatteAnimType = Settings::getInstance()->getBattleAnimationType();
    m_storedDialog = Settings::getInstance()->getDialogAnimation();
    m_storedCaptureAnimation = Settings::getInstance()->getCaptureAnimation();
    m_storedMovementAnimation = Settings::getInstance()->getMovementAnimations();
    m_storedDay2DayAnimation = Settings::getInstance()->getDay2dayScreen();
    m_storedAnimationSpeed = Settings::getInstance()->getAnimationSpeedValue();
    m_storedBattleAnimationSpeed = Settings::getInstance()->getBattleAnimationSpeedValue();
    m_storedDialogAnimationSpeed = Settings::getInstance()->getDialogAnimationSpeedValue();
    m_storedCaptureAnimationSpeed = Settings::getInstance()->getCaptureAnimationSpeedValue();
}

void AnimationSkipper::restoreAnimationSettings()
{
    Settings::getInstance()->setOverworldAnimations(m_storedOverworldAnimations);
    Settings::getInstance()->setBattleAnimationMode(m_storedBattleAnimMode);
    Settings::getInstance()->setBattleAnimationType(m_storedBatteAnimType);
    Settings::getInstance()->setDialogAnimation(m_storedDialog);
    Settings::getInstance()->setCaptureAnimation(m_storedCaptureAnimation);
    Settings::getInstance()->setMovementAnimations(m_storedMovementAnimation);
    Settings::getInstance()->setDay2dayScreen(m_storedDay2DayAnimation);
    Settings::getInstance()->setAnimationSpeed(m_storedAnimationSpeed);
    Settings::getInstance()->setBattleAnimationSpeed(m_storedBattleAnimationSpeed);
    Settings::getInstance()->setDialogAnimationSpeed(m_storedDialogAnimationSpeed);
    Settings::getInstance()->setCaptureAnimationSpeed(m_storedCaptureAnimationSpeed);
}
