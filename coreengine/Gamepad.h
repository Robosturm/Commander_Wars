#pragma once

#include <QObject>
#include <QTimer>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Gamepad : public QObject
{
    Q_OBJECT
public:
    static constexpr float MaxAxisValue = 32767.0f;
    static constexpr float MaxNegativeAxisValue = 32768.0f;
    static constexpr float MinCursorTilt = 0.6f;
    static constexpr qint32 wheelSpeed = 15;
    static constexpr qint32 cursorSpeed = 20;
    static constexpr qint64 mouseIntervall = 150;

    enum class DeviceState
    {
        Available,
        Unavailable
    };
    enum Buttons
    {
        A,
        B,
        X,
        Y,
        Start,
        Select,
        L2,
        R2,
        Max,
    };

    struct ButtonInformation
    {
        bool m_wasPressed{false};
        Qt::Key m_key{Qt::Key_unknown};
    };

    explicit Gamepad(qint32 gamepadId);
    virtual ~Gamepad() = default;

    void init();
protected slots:
    void update();
protected:
    void handleButtonPress(ButtonInformation & info, bool currentState);
    float getAxisValue(qint32 value) const;
    void handleKeyCursorStick(float xValue, float yValue);
    void handleMouseCursorStick(float xValue, float yValue);
    void handleWheelEvent(qint32 x, qint32 y);
    void handleThumbStickPress(bool left, bool right);
private:
    QTimer m_timer;
    qint32 m_gamepadId{0};
    DeviceState m_deviceState{DeviceState::Available};
    qint32 m_updateCounter{-1};
    ButtonInformation m_keys[Buttons::Max];
    qint32 m_xCursorMoveAcitve{0};
    qint32 m_yCursorMoveAcitve{0};
    bool m_leftMouseSend{false};
    bool m_rightMouseSend{false};
    qint64 m_lastWheelEvent{0};
    qint64 m_lastMouseMoveEvent{0};
};
