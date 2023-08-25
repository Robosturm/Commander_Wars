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
    class TweenOptions;
    class ResourcesLoadOptions;
    class Resources;
    class Image;
    class VideoDriver;
    class Font;
    class CreateResourceContext;
    class Event;
    class VStyleActor;

    using spClock = std::shared_ptr<Clock>;
    using spTexture = std::shared_ptr<Texture>;
    using spActor = std::shared_ptr<Actor>;
    using spSprite = std::shared_ptr<Sprite>;
    using spTextField = std::shared_ptr<TextField>;
    using spStage = std::shared_ptr<Stage>;
    using spClipRectActor = std::shared_ptr<ClipRectActor>;
    using spButton = std::shared_ptr<Button>;
    using spBox9Sprite = std::shared_ptr<Box9Sprite>;
    using spColorRectSprite = std::shared_ptr<ColorRectSprite>;
    using spEventDispatcher = std::shared_ptr<EventDispatcher>;
    using spSlidingActor = std::shared_ptr<SlidingActor>;
    using spSlidingActorNoClipRect = std::shared_ptr<SlidingActorNoClipRect>;
    using spResAnim = std::shared_ptr<ResAnim>;
    using spTween = std::shared_ptr<Tween>;
    using spVStyleActor = std::shared_ptr<VStyleActor>;

    using spWeakStage = std::weak_ptr<Stage>;
    using spWeakActor = std::weak_ptr<Actor>;

    using EventCallback = OwnedClosure<void, Event*>;
    using eventType = qint32;
}
