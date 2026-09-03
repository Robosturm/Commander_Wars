#include "coreengine/Gamepad.h"
#include "coreengine/mainapp.h"

#include <jni.h>

namespace
{
qint32 mapAndroidButton(jint keyCode)
{
    switch (keyCode)
    {
        case 96: return 0;
        case 97: return 1;
        case 99: return 2;
        case 100: return 3;
        case 104: return 4;
        case 105: return 5;
        case 109: return 6;
        case 108: return 7;
        case 106:
        case 102: return 9;
        case 107:
        case 103: return 10;
        default: return -1;
    }
}
}

extern "C" JNIEXPORT void JNICALL
Java_org_robosturm_commander_1wars_CommanderWarsActivity_nativeGamepadButton(JNIEnv*, jclass, jint keyCode, jboolean pressed)
{
    const qint32 button = mapAndroidButton(keyCode);
    if (button >= 0)
    {
        Mainapp::getInstance()->getGamepad().handleButton(button, pressed == JNI_TRUE);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_org_robosturm_commander_1wars_CommanderWarsActivity_nativeGamepadAxes(JNIEnv*, jclass, jfloat leftX, jfloat leftY, jfloat rightX, jfloat rightY, jfloat hatX, jfloat hatY)
{
    auto & inputValues = Mainapp::getInstance()->getGamepad().getInputValues();
    inputValues.m_leftX = leftX;
    inputValues.m_leftY = leftY;
    inputValues.m_rightX = rightX;
    inputValues.m_rightY = rightY;
    inputValues.m_hatX = hatX;
    inputValues.m_hatY = hatY;
}

Gamepad::~Gamepad() = default;

void Gamepad::update()
{
    if (Mainapp::getInstance()->isActive())
    {
        handleAxes(m_inputValues);
    }
}

bool Gamepad::isSupported()
{
    return true;
}