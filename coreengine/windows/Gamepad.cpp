#include "coreengine/Gamepad.h"
#include <xinput.h>
#include <dinput.h>
#include "coreengine/mainapp.h"

void Gamepad::update()
{
    if (!Mainapp::getInstance()->isActive())
    {
        return;
    }
    XINPUT_STATE xstate;
    qint32 result = XInputGetState(m_gamepadId, &xstate);
    if (result != ERROR_SUCCESS)
    {
        m_deviceState = DeviceState::Unavailable;
        return;
    }
    m_deviceState = DeviceState::Available;
    qint32 wheelX = 0;
    qint32 wheelY = 0;
    if ((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0)
    {
        wheelY = wheelSpeed;
    }
    else if ((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0)
    {
        wheelY = -wheelSpeed;
    }
    if ((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0)
    {
        wheelX = wheelSpeed;
    }
    else if ((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0)
    {
        wheelX = -wheelSpeed;
    }
    handleWheelEvent(wheelX, wheelY);
    handleButtonPress(m_keys[Buttons::A], (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
    handleButtonPress(m_keys[Buttons::B], (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
    handleMouseCursorStick(getAxisValue(xstate.Gamepad.sThumbLX), getAxisValue(xstate.Gamepad.sThumbLY));
    handleKeyCursorStick(getAxisValue(xstate.Gamepad.sThumbRX), getAxisValue(xstate.Gamepad.sThumbRY));

    if (m_updateCounter == xstate.dwPacketNumber)
    {
        return;
    }
    m_updateCounter = xstate.dwPacketNumber;

    handleThumbStickPress((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0, (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);
    handleThumbStickPress((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0, (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);

    handleButtonPress(m_keys[Buttons::X], (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
    handleButtonPress(m_keys[Buttons::Y], (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
    handleButtonPress(m_keys[Buttons::Start], (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
    handleButtonPress(m_keys[Buttons::Select], (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0);
    handleButtonPress(m_keys[Buttons::L2], xstate.Gamepad.bLeftTrigger != 0);
    handleButtonPress(m_keys[Buttons::R2], xstate.Gamepad.bRightTrigger != 0);
}

Gamepad::DeviceState Gamepad::getDeviceState() const
{
    return m_deviceState;
}

bool Gamepad::isSupported()
{
    return true;
}
