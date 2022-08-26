#pragma once
#include "3rd_party/oxygine-framework/oxygine/core/closure.h"
#include <chrono>
#include <QtGlobal>

#ifdef GAMEDEBUG
#define OXY_ASSERT(check) Q_ASSERT(check)
#else
#define OXY_ASSERT(check)
#endif

namespace oxygine
{
    using timeMS = std::chrono::milliseconds;
    using pointer_index = unsigned char;

    class AnimationFrame;
    class EventDispatcher;
    class UpdateState;
    class Actor;
    class ClipRectActor;
    class Sprite;
    class TextField;
    class ColorRectSprite;
    class SlidingActor;
    class SlidingActorNoClipRect;
    class Button;
    class Box9Sprite;
    class Stage;
    class RenderState;
    class RenderDelegate;
    class VertexDeclaration;
    class ShaderProgram;
    class UberShaderProgram;
    class Clock;
    class Texture;
    class Tween;
    class ResAnim;
    class ResFont;
    class TweenOptions;
    class ResourcesLoadOptions;
    class Resources;
    class Image;
    class VideoDriver;
    class Font;
    class CreateResourceContext;
    class Event;

    template <class T>
    class intrusive_ptr;
    using spClock = intrusive_ptr<Clock>;
    using spTexture = intrusive_ptr<Texture>;
    using spActor = intrusive_ptr<Actor>;
    using spSprite = intrusive_ptr<Sprite>;
    using spTextField = intrusive_ptr<TextField>;
    using spStage = intrusive_ptr<Stage>;
    using spClipRectActor = intrusive_ptr<ClipRectActor>;
    using spButton = intrusive_ptr<Button>;
    using spBox9Sprite = intrusive_ptr<Box9Sprite>;
    using spColorRectSprite = intrusive_ptr<ColorRectSprite>;
    using spEventDispatcher = intrusive_ptr<EventDispatcher>;
    using spSlidingActor = intrusive_ptr<SlidingActor>;
    using spSlidingActorNoClipRect = intrusive_ptr<SlidingActorNoClipRect>;
    using spResAnim = intrusive_ptr<ResAnim>;
    using spTween = intrusive_ptr<Tween>;
    using spResFont = intrusive_ptr<ResFont>;

    using EventCallback = OwnedClosure<void, Event*>;
    using eventType = qint32;
}
