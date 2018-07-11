#Using DisplayLink on iOS

For better integration with UIKit you may want to use CADisplayLink for driving display updates.
In game's main.cpp add:

```cpp

#if __APPLE__
#include <TargetConditionals.h>
#endif

#include "core/oxygine.h"

#ifdef OXYGINE_SDL
extern "C"
{
#include "SDL.h"
};
#endif
```

In run() method search for "#ifdef EMSCRIPTEN" and add a similar section for iOS to prevent busy-wait gameloop from running:

```cpp

// ADD:
#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
	// On iPhone mainloop is called automatically by CADisplayLink
	return;
#endif

#ifdef EMSCRIPTEN
    /*
    if you build for Emscripten mainloop would be called automatically outside.
    see emscripten_set_main_loop below
    */
    return;
#endif
```

In main() register DisplayLink callback, SDL already supports this:

```cpp
#ifdef OXYGINE_SDL

#include "SDL_main.h"
extern "C"
{
    void one(void *param) { mainloop(); }
	
    int main(int argc, char* argv[])
    {
        run();
#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
        // If parameter 2 is set to 1, refresh rate will be 60 fps, 2 - 30 fps, 3 - 15 fps.
        SDL_iPhoneSetAnimationCallback(core::getWindow(), 1, one, nullptr);
#endif
        return 0;
    }
};
#endif
```

Done!
