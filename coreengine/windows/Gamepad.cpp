#include "coreengine/Gamepad.h"
#include <xinput.h>
#include <dinput.h>
#include "coreengine/mainapp.h"

Gamepad::~Gamepad() = default;

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
    if ((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0)
    {
        m_inputValues.m_hatY = 1;
    }
    else if ((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0)
    {
        m_inputValues.m_hatY = -1;
    }
    if ((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0)
    {
        m_inputValues.m_hatX = 1;
    }
    else if ((xstate.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0)
    {
        m_inputValues.m_hatX = -1;
    }
    m_inputValues.m_leftX = getAxisValue(xstate.Gamepad.sThumbLX);
    m_inputValues.m_leftY = getAxisValue(xstate.Gamepad.sThumbLY);
    m_inputValues.m_rightX = getAxisValue(xstate.Gamepad.sThumbRX);
    m_inputValues.m_rightY = getAxisValue(xstate.Gamepad.sThumbRY);
    handleButtonPress(m_keys[Buttons::A], (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
    handleButtonPress(m_keys[Buttons::B], (xstate.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
    handleAxes(inputValues);

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
    handleButtonPress(m_keys[Buttons::L1], xstate.Gamepad.bLeftTrigger != 0);
    handleButtonPress(m_keys[Buttons::R1], xstate.Gamepad.bRightTrigger != 0);
}

bool Gamepad::isSupported()
{
    return true;
}
