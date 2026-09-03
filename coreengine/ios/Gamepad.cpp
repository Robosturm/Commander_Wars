#include "coreengine/Gamepad.h"
#include "coreengine/mainapp.h"

#include <IOKit/hid/IOHIDLib.h>

namespace
{
constexpr qint32 GenericDesktopUsagePage = 0x01;
constexpr qint32 GamePadUsage = 0x05;
constexpr qint32 JoystickUsage = 0x04;
constexpr qint32 ButtonUsagePage = 0x09;
constexpr qint32 GenericDesktopX = 0x30;
constexpr qint32 GenericDesktopY = 0x31;
constexpr qint32 GenericDesktopZ = 0x32;
constexpr qint32 GenericDesktopRz = 0x35;
constexpr qint32 GenericDesktopHatSwitch = 0x39;

float axisValue(IOHIDValueRef value)
{
    const IOHIDElementRef element = IOHIDValueGetElement(value);
    const CFIndex minimum = IOHIDElementGetLogicalMin(element);
    const CFIndex maximum = IOHIDElementGetLogicalMax(element);
    if (maximum == minimum)
    {
        return 0.0f;
    }
    return static_cast<float>(IOHIDValueGetIntegerValue(value) - minimum) /
           static_cast<float>(maximum - minimum) * 2.0f - 1.0f;
}

void onInputValue(void* context, IOReturn, void*, IOHIDValueRef value)
{
    auto* gamepad = static_cast<Gamepad*>(context);
    const IOHIDElementRef element = IOHIDValueGetElement(value);
    const qint32 usagePage = IOHIDElementGetUsagePage(element);
    const qint32 usage = IOHIDElementGetUsage(element);
    const qint32 integerValue = IOHIDValueGetIntegerValue(value);

    if (usagePage == ButtonUsagePage)
    {
        gamepad->handleButton(usage - 1, integerValue != 0);
        return;
    }
    if (usagePage != GenericDesktopUsagePage)
    {
        return;
    }

    const float valueAsAxis = axisValue(value);
    switch (usage)
    {
        case GenericDesktopX:
        case GenericDesktopY:
        case GenericDesktopZ:
        case GenericDesktopRz:
        case GenericDesktopHatSwitch: 
        {
            
            
            gamepad->handleMacOsAxis(usage, valueAsAxis, integerValue); 

            switch (usage)
            {
                case 0x30: 
                {
                    m_inputValues.m_leftX = valueAsAxis; 
                    break;
                }
                case 0x31: 
                {
                    m_inputValues.m_leftY = valueAsAxis; 
                    break;
                }
                case 0x32: 
                {
                    m_inputValues.m_rightX = valueAsAxis; 
                    break;
                }
                case 0x35: 
                {
                    m_inputValues.m_rightY = valueAsAxis; 
                    break;
                }
                case 0x39:
                {
                    m_inputValues.m_hatX = integerValue == 2 || integerValue == 3 || integerValue == 4 ? 1.0f : integerValue == 6 || integerValue == 7 || integerValue == 8 ? -1.0f : 0.0f;
                    m_inputValues.m_hatY = integerValue == 4 || integerValue == 5 || integerValue == 6 ? 1.0f : integerValue == 0 || integerValue == 1 || integerValue == 8 ? -1.0f : 0.0f;
                    break;
                }
                default: 
                {
                    return;
                }
            }
            break;
        }
        default: 
        {
            break;
        }
    }
}
}

Gamepad::~Gamepad()
{
    if (m_nativeDeviceManager != nullptr)
    {
        IOHIDManagerClose(static_cast<IOHIDManagerRef>(m_nativeDeviceManager), kIOHIDOptionsTypeNone);
        CFRelease(static_cast<IOHIDManagerRef>(m_nativeDeviceManager));
    }
}

void Gamepad::update()
{
    if (!Mainapp::getInstance()->isActive())
    {
        return;
    }
    if (m_nativeDeviceManager == nullptr)
    {
        const IOHIDManagerRef manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
        if (manager == nullptr)
        {
            m_deviceState = DeviceState::Unavailable;
            return;
        }
        const qint32 usages[] = {GamePadUsage, JoystickUsage};
        CFMutableArrayRef matches = CFArrayCreateMutable(kCFAllocatorDefault, 2, &kCFTypeArrayCallBacks);
        for (const qint32 usage : usages)
        {
            CFMutableDictionaryRef match = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
            CFNumberRef usagePage = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &GenericDesktopUsagePage);
            CFNumberRef usageNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &usage);
            CFDictionarySetValue(match, CFSTR(kIOHIDDeviceUsagePageKey), usagePage);
            CFDictionarySetValue(match, CFSTR(kIOHIDDeviceUsageKey), usageNumber);
            CFArrayAppendValue(matches, match);
            CFRelease(usageNumber);
            CFRelease(usagePage);
            CFRelease(match);
        }
        IOHIDManagerSetDeviceMatchingMultiple(manager, matches);
        CFRelease(matches);
        IOHIDManagerRegisterInputValueCallback(manager, onInputValue, this);
        IOHIDManagerScheduleWithRunLoop(manager, CFRunLoopGetMain(), kCFRunLoopDefaultMode);
        if (IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone) != kIOReturnSuccess)
        {
            CFRelease(manager);
            m_deviceState = DeviceState::Unavailable;
            return;
        }
        m_nativeDeviceManager = manager;
    }
    const CFSetRef devices = IOHIDManagerCopyDevices(static_cast<IOHIDManagerRef>(m_nativeDeviceManager));
    m_deviceState = CFSetGetCount(devices) > 0 ? DeviceState::Available : DeviceState::Unavailable;
    CFRelease(devices);
    if (m_deviceState == DeviceState::Available)
    {
        handleAxes(m_inputValues);
    }
}

bool Gamepad::isSupported()
{
    return true;
}