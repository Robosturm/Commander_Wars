#Dealing with high CPU usage on OS X when the app is idle

## The problem ##
On OS X game updates are bound to display refresh rate. But when the window is hidden or entirely covered by another window, CPU usage suddenly jumps to 100%.

## Fix by patching SDL (optional) ##

I've compared the code of SwapWindow with GLFW implementation which cocos2d-x uses internally, and it seems GLFW doesn't call *updateIfNeeded*, it only calls *flushBuffer*.

In *SDL/src/video/cocoa/SDL_cocoaopengl.m* comment out *updateIfNeeded*:

```cpp

Cocoa_GL_SwapWindow(_THIS, SDL_Window * window)
{ @autoreleasepool
{
    SDLOpenGLContext* nscontext = (SDLOpenGLContext*)SDL_GL_GetCurrentContext();
    [nscontext flushBuffer];
    //[nscontext updateIfNeeded]; // <--- COMMENT OUT
}}
```

Commenting out *updateIfNeeded* brought CPU usage from 100% to ~5%. Yet this is not a complete fix. The function is called a lot when the window is inactive:

```
Cocoa_GL_SwapWindow: 60 fps
Cocoa_GL_SwapWindow: 60 fps
Cocoa_GL_SwapWindow: 60 fps
Cocoa_GL_SwapWindow: 60 fps
[Window hidden]
Cocoa_GL_SwapWindow: 2964 fps
Cocoa_GL_SwapWindow: 3045 fps
Cocoa_GL_SwapWindow: 3095 fps
Cocoa_GL_SwapWindow: 2999 fps
```

## Add sleeps to game loop ##

This is the approeach used by Cocos2d-x on Mac OS X. Below is an example of modified game loop in main.cpp:

```cpp

    //here is main game loop
    while (1)
    {
#ifdef TARGET_OS_MAC
        Uint32 start = SDL_GetTicks();
#endif
			
        int done = mainloop();
        if (done)
            break;

#ifdef TARGET_OS_MAC
        Uint32 duration = SDL_GetTicks() - start;
        if (duration < 1000 / 60)
            usleep((1000 / 60 - duration) * 1000);
#endif
	}
```

Done! CPU usage ~4.5%.

The downside is that usleep sometimes leads to frame drops (59-60 fps instead of steady 60). For fast-paces games you may want to lower threshold to avoid *usleep* as much as possible, but not too low, so high-HZ monitors are still throttled, but 75 hz plays full speed:

```cpp
        if (duration < 1000 / 90) // sleep only if framerate is ~1.5x the normal or more
            usleep((1000 / 60 - duration) * 1000);
```

If you'll experiment further, this code for measuring FPS might come in handy:

```cpp
Cocoa_GL_SwapWindow(_THIS, SDL_Window * window)
{ @autoreleasepool
{
    static int t = 0, n = 0;
    if (t != time(0)) {
        printf("Cocoa_GL_SwapWindow: %d fps\n", n);
        n = 0;
    }
    t = time(0);
    ++n;

    SDLOpenGLContext* nscontext = (SDLOpenGLContext*)SDL_GL_GetCurrentContext();
    [nscontext flushBuffer];
    //[nscontext updateIfNeeded];
}}
```

## TODO: Another solutions to consider ##

* NSTimer of 1 msec + waiting for vsync;
* CVDisplayLink.