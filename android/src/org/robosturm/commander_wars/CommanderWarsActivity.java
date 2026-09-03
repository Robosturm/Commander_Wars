package org.robosturm.commander_wars;

import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import org.qtproject.qt.android.bindings.QtActivity;

public class CommanderWarsActivity extends QtActivity {
    private static native void nativeGamepadButton(int button, boolean pressed);
    private static native void nativeGamepadAxes(float leftX, float leftY, float rightX, float rightY, float leftTrigger, float rightTrigger, float hatX, float hatY);

    private static boolean isGamepad(InputDevice device) {
        if (device == null) {
            return false;
        }
        int sources = device.getSources();
        return (sources & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD
            || (sources & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK;
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if (isGamepad(event.getDevice())) {
            nativeGamepadButton(event.getKeyCode(), event.getAction() != KeyEvent.ACTION_UP);
        }
        return super.dispatchKeyEvent(event);
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_MOVE && isGamepad(event.getDevice())) {
            nativeGamepadAxes(event.getAxisValue(MotionEvent.AXIS_X), event.getAxisValue(MotionEvent.AXIS_Y),
                              event.getAxisValue(MotionEvent.AXIS_Z), event.getAxisValue(MotionEvent.AXIS_RZ),
                              event.getAxisValue(MotionEvent.AXIS_LTRIGGER), event.getAxisValue(MotionEvent.AXIS_RTRIGGER),
                              event.getAxisValue(MotionEvent.AXIS_HAT_X), event.getAxisValue(MotionEvent.AXIS_HAT_Y));
        }
        return super.onGenericMotionEvent(event);
    }
}