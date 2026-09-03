#include "coreengine/Gamepad.h"
#include "coreengine/mainapp.h"

#include <cerrno>
#include <fcntl.h>
#include <linux/joystick.h>
#include <unistd.h>

namespace
{
constexpr qint32 AxisLeftX = 0;
constexpr qint32 AxisLeftY = 1;
constexpr qint32 AxisL2 = 2;
constexpr qint32 AxisRightX = 3;
constexpr qint32 AxisRightY = 4;
constexpr qint32 AxisR2 = 5;
constexpr qint32 AxisLeftRight = 6;
constexpr qint32 AxisUpDown = 7;
}

Gamepad::~Gamepad()
{
    if (m_deviceFileDescriptor >= 0)
    {
        close(m_deviceFileDescriptor);
    }
}

void Gamepad::update()
{
    if (!Mainapp::getInstance()->isActive())
    {
        return;
    }

    if (m_deviceFileDescriptor < 0)
    {
        const QString devicePath = QStringLiteral("/dev/input/js%1").arg(m_gamepadId);
        m_deviceFileDescriptor = open(devicePath.toLocal8Bit().constData(), O_RDONLY | O_NONBLOCK);
        if (m_deviceFileDescriptor < 0)
        {
            m_deviceState = DeviceState::Unavailable;
            return;
        }
    }

    m_deviceState = DeviceState::Available;
    js_event event{};
    ssize_t bytesRead = 0;
    while ((bytesRead = read(m_deviceFileDescriptor, &event, sizeof(event))) == sizeof(event))
    {
        const qint32 type = event.type & ~JS_EVENT_INIT;
        if (type == JS_EVENT_BUTTON)
        {
            handleButton(event.number, event.value != 0);
        }
        else if (type == JS_EVENT_AXIS)
        {
            const float value = getAxisValue(event.value);
            CONSOLE_PRINT_MODULE(QString("Axis %1 value: %2 Last Values: LeftX: %3, LeftY: %4, RightX: %5, RightY: %6, HatX: %7, HatY: %8")
                                 .arg(event.number)
                                 .arg(event.value)
                                 .arg(m_inputValues.m_leftX)
                                 .arg(m_inputValues.m_leftY)
                                 .arg(m_inputValues.m_rightX)
                                 .arg(m_inputValues.m_rightY)
                                 .arg(m_inputValues.m_hatX)
                                 .arg(m_inputValues.m_hatY),
                                 GameConsole::eDEBUG, GameConsole::eGamepad);
            switch (event.number)
            {
                case AxisLeftX: 
                {
                    m_inputValues.m_leftX = value; 
                    break;
                }
                case AxisLeftY:
                {
                    m_inputValues.m_leftY = value; 
                    break;
                }
                case AxisL2:
                {
                    handleMouseEvent(oxygine::MouseButton::MouseButton_Left, value > MinCursorTilt); 
                    break;
                }
                case AxisRightX:
                {
                    m_inputValues.m_rightX = value; 
                    break;
                }
                case AxisRightY:
                {
                    m_inputValues.m_rightY = value; 
                    break;
                }
                case AxisR2:
                {
                    handleMouseEvent(oxygine::MouseButton::MouseButton_Right, value > MinCursorTilt); 
                    break;
                }
                case AxisLeftRight:
                {
                    m_inputValues.m_hatX = value; 
                    break;
                }
                case AxisUpDown:
                {
                    m_inputValues.m_hatY = value; 
                    break;
                }
                
                default:
                {
                    break;
                }
            }
        }
    }
    if (bytesRead == 0 || (bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK))
    {
        close(m_deviceFileDescriptor);
        m_deviceFileDescriptor = -1;
        m_deviceState = DeviceState::Unavailable;
    }
    else if (m_deviceState == DeviceState::Available)
    {
        handleAxes(m_inputValues);
    }
}

bool Gamepad::isSupported()
{
    return true;
}