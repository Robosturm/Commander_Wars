#pragma once

#include <QObject>
#include <QTimer>

class Gamepad final : public QObject
{
    Q_OBJECT
public:
    static constexpr float MaxAxisValue = 32767.0f;
    static constexpr float MaxNegativeAxisValue = 32768.0f;
    static constexpr float MinCursorTilt = 0.3f;
    static constexpr qint32 wheelSpeed = 7;
    static constexpr qint32 cursorSpeed = 25;
    static constexpr qint64 mouseIntervall = 50;
    static constexpr qint64 mouseKeyIntervall = 200;

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
    DeviceState getDeviceState() const;

    static bool isSupported();
protected slots:
    void update();
protected:
    void handleButtonPress(ButtonInformation & info, bool currentState);
    float getAxisValue(qint32 value) const;
    void handleKeyCursorStick(float xValue, float yValue);
    void handleMouseCursorStick(float xValue, float yValue);
    void handleWheelEvent(qint32 x, qint32 y);
    void handleThumbStickPress(bool left, bool right);
    void nextMoveSpeed(float & currentValue);
    void nextMapMoveSpeed(float & currentValue);
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
    qint64 m_lastMouseMapMoveEvent{0};
    float m_mouseMapMoveSpeed{1};
    qint64 m_lastMouseMoveEvent{0};
    float m_mouseMoveSpeed{1};
};
