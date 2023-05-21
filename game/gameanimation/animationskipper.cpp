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
    Settings::setOverworldAnimations(false);
    Settings::setBattleAnimationMode(GameEnums::BattleAnimationMode::BattleAnimationMode_None);
    Settings::setDialogAnimation(false);
    Settings::setCaptureAnimation(false);
    Settings::setMovementAnimations(false);
    Settings::setDay2dayScreen(false);
    Settings::setAnimationSpeed(100);
    Settings::setBattleAnimationSpeed(100);
    Settings::setDialogAnimationSpeed(100);
    Settings::setCaptureAnimationSpeed(100);
}

void AnimationSkipper::storeAnimationSettings()
{
    // store animation modes
    m_storedOverworldAnimations = Settings::getOverworldAnimations();
    m_storedBattleAnimMode = Settings::getBattleAnimationMode();
    m_storedBatteAnimType = Settings::getBattleAnimationType();
    m_storedDialog = Settings::getDialogAnimation();
    m_storedCaptureAnimation = Settings::getCaptureAnimation();
    m_storedMovementAnimation = Settings::getMovementAnimations();
    m_storedDay2DayAnimation = Settings::getDay2dayScreen();
    m_storedAnimationSpeed = Settings::getAnimationSpeedValue();
    m_storedBattleAnimationSpeed = Settings::getBattleAnimationSpeedValue();
    m_storedDialogAnimationSpeed = Settings::getDialogAnimationSpeedValue();
    m_storedCaptureAnimationSpeed = Settings::getCaptureAnimationSpeedValue();
}

void AnimationSkipper::restoreAnimationSettings()
{
    Settings::setOverworldAnimations(m_storedOverworldAnimations);
    Settings::setBattleAnimationMode(m_storedBattleAnimMode);
    Settings::setBattleAnimationType(m_storedBatteAnimType);
    Settings::setDialogAnimation(m_storedDialog);
    Settings::setCaptureAnimation(m_storedCaptureAnimation);
    Settings::setMovementAnimations(m_storedMovementAnimation);
    Settings::setDay2dayScreen(m_storedDay2DayAnimation);
    Settings::setAnimationSpeed(m_storedAnimationSpeed);
    Settings::setBattleAnimationSpeed(m_storedBattleAnimationSpeed);
    Settings::setDialogAnimationSpeed(m_storedDialogAnimationSpeed);
    Settings::setCaptureAnimationSpeed(m_storedCaptureAnimationSpeed);
}
