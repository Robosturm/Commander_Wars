#pragma once
#ifndef OXYGINE_FRAMEWORK
#define OXYGINE_FRAMEWORK
//oxygine single include

//
#include "oxygine/oxygine-include.h"
#include "oxygine/actor/Actor.h"
#include "oxygine/AnimationFrame.h"
#include "oxygine/actor/Box9Sprite.h"
#include "oxygine/actor/Button.h"
#include "oxygine/actor/ClipRectActor.h"
#include "oxygine/actor/slidingsprite.h"
#include "oxygine/Clock.h"
#include "oxygine/actor/ColorRectSprite.h"
#include "oxygine/Draggable.h"
#include "oxygine/Event.h"
#include "oxygine/EventDispatcher.h"
#include "oxygine/Font.h"
#include "oxygine/initActor.h"
#include "oxygine/Input.h"
#include "oxygine/Image.h"
#include "oxygine/PointerState.h"
#include "oxygine/actor/ProgressBar.h"
#include "oxygine/RenderState.h"
#include "oxygine/actor/Stage.h"
#include "oxygine/actor/SlidingActor.h"
#include "oxygine/actor/Sprite.h"
#include "oxygine/actor/MaskedSprite.h"
#include "oxygine/actor/TextField.h"
#include "oxygine/TextStyle.h"
#include "oxygine/tween/Tween.h"
#include "oxygine/tween/TweenQueue.h"
#include "oxygine/TouchEvent.h"
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"
#include "oxygine/UpdateState.h"
#include "oxygine/VisualStyle.h"
#include "oxygine/STDRenderer.h"
#include "oxygine/actor/Polygon.h"
#include "oxygine/tween/TweenOutline.h"
#include "oxygine/tween/TweenGlow.h"
#include "oxygine/tween/TweenAlphaFade.h"
#include "oxygine/Material.h"
#include "oxygine/MaterialCache.h"

//closure/
#include "oxygine/closure/closure.h"

#include <functional>

//core/
#include "oxygine/core/ImageData.h"
#include "oxygine/core/ImageDataOperations.h"
#include "oxygine/core/intrusive_ptr.h"
#include "oxygine/core/NativeTexture.h"
#include "oxygine/core/Object.h"
#include "oxygine/core/oxygine.h"
#include "oxygine/core/pixel.h"
#include "oxygine/core/ref_counter.h"
#include "oxygine/core/Renderer.h"
#include "oxygine/core/ShaderProgram.h"
#include "oxygine/core/Texture.h"
#include "oxygine/core/vertex.h"
#include "oxygine/core/VertexDeclaration.h"
#include "oxygine/core/VideoDriver.h"
#include "oxygine/core/UberShaderProgram.h"

//core/gl/
#include "oxygine/core/gl/NativeTextureGLES.h"
#include "oxygine/core/gl/VertexDeclarationGL.h"
#include "oxygine/core/gl/VideoDriverGLES20.h"
#include "oxygine/core/gl/ShaderProgramGL.h"

//math/
#include "oxygine/math/AffineTransform.h"
#include "oxygine/math/Matrix.h"
#include "oxygine/math/Rect.h"
#include "oxygine/math/ScalarMath.h"
#include "oxygine/math/Vector2.h"
#include "oxygine/math/Vector3.h"
#include "oxygine/math/Vector4.h"

//res/
#include "oxygine/res/CreateResourceContext.h"
#include "oxygine/res/ResAnim.h"
#include "oxygine/res/ResAtlas.h"
#include "oxygine/res/ResBuffer.h"
#include "oxygine/res/ResFont.h"
#include "oxygine/res/ResFontBM.h"
#include "oxygine/res/Resource.h"
#include "oxygine/res/Resources.h"
#include "oxygine/res/SingleResAnim.h"

//text_utils/
#include "oxygine/text_utils/Aligner.h"
#include "oxygine/text_utils/Node.h"
#include "oxygine/text_utils/TextBuilder.h"

//tree_inspector/

//tree_inspector/internal/

//utils/
#include "oxygine/utils/AtlasBuilder.h"
#include "oxygine/utils/intrusive_list.h"

#endif
