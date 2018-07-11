##0. Download NanoVG and copy its source files to your project directory
##1. Include NanoVG:
```cpp
#include "core/gl/oxgl.h"

#include "nanovg/nanovg.h"

#define NANOVG_GLES2_IMPLEMENTATION

#define glLinkProgram oxglLinkProgram
#define glActiveTexture oxglActiveTexture
#define glBindBuffer oxglBindBuffer
#define glDeleteBuffers oxglDeleteBuffers
#define glGenBuffers oxglGenBuffers
#define glBufferData oxglBufferData
#define glStencilOpSeparate oxglStencilOpSeparate
#define glAttachShader oxglAttachShader
#define glBindAttribLocation oxglBindAttribLocation
#define glCompileShader oxglCompileShader
#define glCreateProgram oxglCreateProgram
#define glCreateShader oxglCreateShader
#define glDeleteProgram oxglDeleteProgram
#define glDeleteShader oxglDeleteShader
#define glDisableVertexAttribArray oxglDisableVertexAttribArray
#define glEnableVertexAttribArray oxglEnableVertexAttribArray
#define glGetProgramiv oxglGetProgramiv
#define glGetProgramInfoLog oxglGetProgramInfoLog
#define glGetShaderiv oxglGetShaderiv
#define glGetShaderInfoLog oxglGetShaderInfoLog
#define glGetUniformLocation oxglGetUniformLocation
#define glShaderSource oxglShaderSource
#define glUseProgram oxglUseProgram
#define glUniform1i oxglUniform1i
#define glUniform2fv oxglUniform2fv
#define glUniform4fv oxglUniform4fv
#define glVertexAttribPointer oxglVertexAttribPointer
#define glBindRenderbuffer oxglBindRenderbuffer
#define glDeleteRenderbuffers oxglDeleteRenderbuffers
#define glGenRenderbuffers oxglGenRenderbuffers
#define glRenderbufferStorage oxglRenderbufferStorage
#define glBindFramebuffer oxglBindFramebuffer
#define glDeleteFramebuffers oxglDeleteFramebuffers
#define glGenFramebuffers oxglGenFramebuffers
#define glCheckFramebufferStatus oxglCheckFramebufferStatus
#define glFramebufferTexture2D oxglFramebufferTexture2D
#define glFramebufferRenderbuffer oxglFramebufferRenderbuffer
#define glGenerateMipmap oxglGenerateMipmap


#include "nanovg/nanovg_gl.h"
#include "nanovg/nanovg_gl_utils.h"
```

##2. Initialize NanoVG context:

```cpp
NVGcontext* vg = 0;
vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
```

##3. Create your own Sprite and overload the rendering method:

```cpp
class NVGSprite : public Sprite
{
public:
	NVGSprite()
	{
	}
	
	void doRender(const RenderState& rs)
	{
		Material::setCurrent(0);

		nvgBeginFrame(vg, getStage()->getWidth(), getStage()->getHeight(), 1);
		nvgTransform(_vg, rs.transform.a, rs.transform.b, rs.transform.c, rs.transform.d, rs.transform.x, rs.transform.y);


		nvgBeginPath(vg);
		nvgEllipse(vg, 500, 500, 200, 100);
		nvgFillColor(vg, nvgRGBA(255, 192, 0, 255));
		nvgFill(vg);

		nvgEndFrame(vg);
	}
};
```