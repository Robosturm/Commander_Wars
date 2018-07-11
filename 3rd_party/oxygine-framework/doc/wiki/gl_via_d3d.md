#OpenGLES emulation through Direct3D

In order to enable the necessary context creation OpenGLES you must set the flag **force_gles** in the struture **oxygine::core::init_desc** to **true** before Oxygine initialization.
```cpp	
    //initialize Oxygine's internal stuff
    core::init_desc desc;
	desc.title = "My Application";
	desc.w = 960;
	desc.h = 640;

	desc.force_gles = true;

    core::init(&desc);
```
The flag **force_gles** is intended to be used in conjunction with the library **Angle**. 

##Angle

The goal of [ANGLE](https://code.google.com/p/angleproject/) is to allow Windows users to seamlessly run WebGL and other OpenGL ES 2.0 content by translating OpenGL ES 2.0 API calls to DirectX 9 or DirectX 11 API calls.


First off you must compile **Angle** and you will receive 4 important files:

* libGLESv2.lib
* libGLESv2.dll
* libEGL.dll
* d3dcompiler_47.dll


Copy the **dll** files into the folder **data** and **libGLESv2.lib** into **oxygine-framework/libs/**

If you are compiling the libraries manually, then you must edit a little bit of code in the file
**SDL/src/video/windows/SDL_windowsopengl.c**.
Find the setting:
	
	_this->gl_data->HAS_WGL_EXT_create_context_es2_profile = SDL_TRUE;

and comment it out:
	
	//_this->gl_data->HAS_WGL_EXT_create_context_es2_profile = SDL_TRUE;


###Visual Studio
In Linker settings you must set the Angle library **libGLESv2.lib** instead of **OpenGL32.lib**.

![](img/gles_vs.png)

###Visual Studio + CMake
If the project was made with the help of CMake, then set the flag **FORCE_GLES=1**. Example:
	

	cmake -G"Visual Studio 12 2013" -DFORCE_GLES=1



	
