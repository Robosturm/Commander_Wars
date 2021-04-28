#pragma  once
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"
#include "3rd_party/oxygine-framework/oxygine/closure/closure.h"
#include <chrono>
#include <qglobal.h>

#define DECLARE_SMART(class_name, spname) \
    class class_name;\
    typedef oxygine::intrusive_ptr<class_name> spname;

namespace pugi
{
    class xml_node;
}


namespace oxygine
{
    enum error_policy
    {
        ep_show_error,//shows assert and prints error to log
        ep_show_warning,//prints warning to log
        ep_ignore_error//doesn't show any errors
    };


    void handleErrorPolicy(error_policy ep, QString message);

    typedef std::chrono::milliseconds timeMS;
    typedef unsigned char pointer_index;

    namespace text
    {
        class Node;
        struct Symbol;
    }

    template <class T>
    class intrusive_ptr;

    DECLARE_SMART(Actor, spActor);
    DECLARE_SMART(AsyncTask, spAsyncTask);
    DECLARE_SMART(Box9Sprite, spBox9Sprite);
    DECLARE_SMART(Button, spButton);
    DECLARE_SMART(ClipRectActor, spClipRectActor);
    DECLARE_SMART(Clock, spClock);
    DECLARE_SMART(ColorRectSprite, spColorRectSprite);
    DECLARE_SMART(DebugActor, spDebugActor);
    DECLARE_SMART(DragHandler, spDragHandler);
    DECLARE_SMART(EventDispatcher, spEventDispatcher);
    DECLARE_SMART(HttpRequestTask, spHttpRequestTask);
    DECLARE_SMART(InputText, spInputText);
    DECLARE_SMART(MaskedSprite, spMaskedSprite);
    DECLARE_SMART(NativeTexture, spNativeTexture);
    DECLARE_SMART(Object, spObject);
    DECLARE_SMART(Polygon, spPolygon);
    DECLARE_SMART(ProgressBar, spProgressBar);
    DECLARE_SMART(RenderTexture, spRenderTexture);
    DECLARE_SMART(ResAnim, spResAnim);
    DECLARE_SMART(ResBuffer, spResBuffer);
    DECLARE_SMART(ResFont, spResFont);
    DECLARE_SMART(Resource, spResource);
    DECLARE_SMART(STDRenderDelegate, spSTDRenderDelegate);
    DECLARE_SMART(SlidingActor, spSlidingActor);
    DECLARE_SMART(Sprite, spSprite);
    DECLARE_SMART(Stage, spStage);
    DECLARE_SMART(TextField, spTextField);
    DECLARE_SMART(Texture, spTexture);
    DECLARE_SMART(ThreadLoader, spThreadLoader);
    DECLARE_SMART(Tween, spTween);
    DECLARE_SMART(TweenQueue, spTweenQueue);
    DECLARE_SMART(Material, spMaterialX);

    class Actor;
    class AnimationFrame;
    class ClipRectActor;
    class Color;
    class ColorRectSprite;
    class CreateResourceContext;
    class DebugActor;
    class Event;
    class EventState;
    class Font;
    class IVideoDriver;
    class Image;
    class ImageData;
    class LoadResourcesContext;
    class MaskedSprite;
    class RenderDelegate;
    class Mem2Native;
    class Mutex;
    class PointerState;
    class PostProcessOptions;
    class ProgressBar;
    class RenderState;
    class ResAnim;
    class ResAtlas;
    class ResFont;
    class ResFontBM;
    class Resource;
    class Resources;
    class ResourcesLoadOptions;
    class Restorable;
    class ShaderProgram;
    class SingleResAnim;
    class Sprite;
    class TextField;
    class ThreadDispatcher;
    class TouchEvent;
    class Tween;
    class TweenOptions;
    class UberShaderProgram;
    class UpdateState;
    class VertexDeclaration;
    class XmlWalker;
    struct deserializeLinkData;
    struct deserializedata;
    struct serializedata;

    const qint32 cloneOptionsDoNotCloneClildren = 0x01;
    const qint32 cloneOptionsResetTransform = 0x02;

    using EventCallback = OwnedClosure<void, Event*>;

    typedef qint32 cloneOptions;
    typedef qint32 copyOptions;//deprecated typedef
    typedef qint32 eventType;
    typedef qint32 glyphOptions;
}
