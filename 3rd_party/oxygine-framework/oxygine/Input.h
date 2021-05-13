#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"
#include "3rd_party/oxygine-framework/oxygine/closure/closure.h"

namespace oxygine
{
    const qint32 MAX_TOUCHES = 17;

    class Input
    {
    public:
        static Input instance;

        Input();
        ~Input();

        void cleanup();

        void multiTouchEnabled(bool en);

        /**index should be in range [1, MAX_TOUCHES]*/
        PointerState* getTouchByIndex(pointer_index index);

        qint32 touchID2index(qint64 id);
        PointerState* getTouchByID(qint64 id);
        void sendPointerButtonEvent(spStage stage, MouseButton button, float x, float y, float pressure, qint32 type, PointerState* ps);
        void sendPointerMotionEvent(spStage stage, float x, float y, float pressure, PointerState* ps);
        void sendPointerWheelEvent(spStage stage, const Vector2& dir, PointerState* ps);
        void sendPointerZoomEvent(spStage stage, const Vector2& dir, PointerState* ps);
        void sendPointerTouchScrollEvent(spStage stage, float x, float y, PointerState* ps);


    public:
        PointerState m_pointers[MAX_TOUCHES];
        PointerState m_pointerMouse;
        qint64 m_ids[MAX_TOUCHES + 1];
        bool m_multiTouch;
    };
}
