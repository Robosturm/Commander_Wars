#include "coreengine/Gamepad.h"
#include "coreengine/mainapp.h"
#include "coreengine/workerthread.h"
#include <QDateTime>

Gamepad::Gamepad(qint32 gamepadId)
    : m_timer(this),
      m_gamepadId(gamepadId)
{
    setObjectName("Gamepad");
}

void Gamepad::init()
{
    if (Settings::getGamepadEnabled())
    {
        for (auto & key : m_keys)
        {
            key.m_key = Settings::getKey_information();
        }
        m_keys[Buttons::A].m_key = Settings::getKey_confirm();
        m_keys[Buttons::B].m_key = Settings::getKey_cancel();
        m_keys[Buttons::L2].m_key = Settings::getKey_previous();
        m_keys[Buttons::R2].m_key = Settings::getKey_next();
        connect(&m_timer, &QTimer::timeout, this, &Gamepad::update);
        m_timer.setSingleShot(false);
        m_timer.start(1);
    }
}

void Gamepad::handleButtonPress(ButtonInformation & info, bool currentState)
{
    if (info.m_key != Qt::Key_unknown)
    {
        if (currentState)
        {
            oxygine::KeyEvent event(info.m_key, currentState == info.m_wasPressed);
            emit Mainapp::getInstance()->sigKeyDown(event);
        }
        else if (info.m_wasPressed)
        {
            oxygine::KeyEvent event(info.m_key, false);
            emit Mainapp::getInstance()->sigKeyUp(event);
        }
        info.m_wasPressed = currentState;
    }
}

float Gamepad::getAxisValue(qint32 value) const
{
    if (value < 0)
    {
        return static_cast<float>(value) / MaxNegativeAxisValue;
    }
    else
    {
        return static_cast<float>(value) / MaxAxisValue;
    }
}

void Gamepad::handleKeyCursorStick(float xValue, float yValue)
{
    if (xValue > MinCursorTilt ||
        yValue > MinCursorTilt ||
        xValue < -MinCursorTilt ||
        yValue < -MinCursorTilt)
    {
        qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();
        if (currentTimestamp - m_lastMouseMoveEvent >= mouseIntervall * Settings::getGamepadSensitivity() * m_mouseMoveSpeed)
        {
            Mainapp* pApp = Mainapp::getInstance();
            QPoint pos = pApp->cursor().pos();
            pos += QPoint(xValue * cursorSpeed, -yValue * cursorSpeed);
            pApp->cursor().setPos(pos);
            m_lastMouseMoveEvent = currentTimestamp;
            nextMoveSpeed(m_mouseMoveSpeed);
        }
    }
}

void Gamepad::handleMouseCursorStick(float xValue, float yValue)
{
    qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();
    bool timeGone = (currentTimestamp - m_lastMouseMapMoveEvent >= mouseKeyIntervall * Settings::getGamepadSensitivity() * m_mouseMapMoveSpeed);
    bool moved = false;
    if (xValue > MinCursorTilt && timeGone)
    {
        oxygine::KeyEvent event(Settings::getKey_right(), m_xCursorMoveAcitve > 0);
        emit Mainapp::getInstance()->sigKeyDown(event);
        m_xCursorMoveAcitve = 1;
        moved = true;
    }
    else if (xValue < -MinCursorTilt && timeGone)
    {
        oxygine::KeyEvent event(Settings::getKey_left(), m_xCursorMoveAcitve < 0);
        emit Mainapp::getInstance()->sigKeyDown(event);
        m_xCursorMoveAcitve = -1;
        moved = true;
    }
    else if (m_xCursorMoveAcitve > 0)
    {
        oxygine::KeyEvent event(Settings::getKey_right(), false);
        emit Mainapp::getInstance()->sigKeyUp(event);
        m_xCursorMoveAcitve = 0;
        m_lastMouseMapMoveEvent = currentTimestamp;
        m_mouseMapMoveSpeed = 1;
    }
    else if (m_xCursorMoveAcitve < 0)
    {
        oxygine::KeyEvent event(Settings::getKey_left(), false);
        emit Mainapp::getInstance()->sigKeyUp(event);
        m_xCursorMoveAcitve = 0;
        m_lastMouseMapMoveEvent = currentTimestamp;
        m_mouseMapMoveSpeed = 1;
    }

    if (yValue > MinCursorTilt && timeGone)
    {
        oxygine::KeyEvent event(Settings::getKey_up(), m_yCursorMoveAcitve > 0);
        emit Mainapp::getInstance()->sigKeyDown(event);
        m_yCursorMoveAcitve = 1;
        moved = true;
    }
    else if (yValue < -MinCursorTilt && timeGone)
    {
        oxygine::KeyEvent event(Settings::getKey_down(), m_yCursorMoveAcitve < 0);
        emit Mainapp::getInstance()->sigKeyDown(event);
        m_yCursorMoveAcitve = -1;
        moved = true;
    }
    else if (m_yCursorMoveAcitve > 0)
    {
        oxygine::KeyEvent event(Settings::getKey_up(), false);
        emit Mainapp::getInstance()->sigKeyUp(event);
        m_yCursorMoveAcitve = 0;
        m_lastMouseMapMoveEvent = currentTimestamp;
        m_mouseMapMoveSpeed = 1;
    }
    else if (m_yCursorMoveAcitve < 0)
    {
        oxygine::KeyEvent event(Settings::getKey_down(), false);
        emit Mainapp::getInstance()->sigKeyUp(event);
        m_yCursorMoveAcitve = 0;
        m_lastMouseMapMoveEvent = currentTimestamp;
        m_mouseMapMoveSpeed = 1;
    }
    if (moved)
    {
        nextMoveSpeed(m_mouseMapMoveSpeed);
        m_lastMouseMapMoveEvent = currentTimestamp;
    }
}

void Gamepad::handleWheelEvent(qint32 x, qint32 y)
{
    if (x != 0 || y != 0)
    {
        qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();
        if (currentTimestamp - m_lastWheelEvent >= mouseIntervall * Settings::getGamepadSensitivity())
        {
            m_lastWheelEvent = currentTimestamp;
            emit Mainapp::getInstance()->sigWheelEvent(x, y);
        }
    }
}

void Gamepad::handleThumbStickPress(bool left, bool right)
{
    Mainapp* pApp = Mainapp::getInstance();
    QPoint cursor = pApp->mapFromGlobal(pApp->cursor().pos());
    if (left)
    {
        emit pApp->sigMousePressEvent(oxygine::MouseButton_Left, cursor.x(), cursor.y());
        m_leftMouseSend = true;
    }
    else if (m_leftMouseSend)
    {
        emit pApp->sigMouseReleaseEvent(oxygine::MouseButton_Left, cursor.x(), cursor.y());
        m_leftMouseSend = false;
    }
    if (right)
    {
        emit pApp->sigMousePressEvent(oxygine::MouseButton_Right, cursor.x(), cursor.y());
        m_rightMouseSend = true;
    }
    else if (m_rightMouseSend)
    {
        emit pApp->sigMouseReleaseEvent(oxygine::MouseButton_Right, cursor.x(), cursor.y());
        m_rightMouseSend = false;
    }
}

void Gamepad::nextMoveSpeed(float & currentValue)
{
    currentValue *= 0.8f;
    if (currentValue < 0.5f)
    {
        currentValue = 0.5f;
    }
}

void Gamepad::nextMapMoveSpeed(float & currentValue)
{
    currentValue *= 0.6f;
    if (currentValue < 0.25f)
    {
        currentValue = 0.25f;
    }
}
