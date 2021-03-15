#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"
#include "3rd_party/oxygine-framework/oxygine/closure/closure.h"

namespace oxygine
{
    const int MAX_TOUCHES = 17;

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

        int touchID2index(qint64 id);
        PointerState* getTouchByID(qint64 id);


        //internal:
        PointerState _pointers[MAX_TOUCHES];
        PointerState _pointerMouse;

        qint64 _ids[MAX_TOUCHES + 1];


        void sendPointerButtonEvent(spStage, MouseButton button, float x, float y, float pressure, int type, PointerState*);
        void sendPointerMotionEvent(spStage, float x, float y, float pressure, PointerState*);
        void sendPointerWheelEvent(spStage, const Vector2& dir, PointerState*);

        bool _multiTouch;
    };
}
