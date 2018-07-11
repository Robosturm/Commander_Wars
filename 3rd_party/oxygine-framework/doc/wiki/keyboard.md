#Keyboard

Oxygine does not have its own API for working with keys. If you need access to the keyboard, then you must use functions from SDL2 or Marmalade.
 
There exist multiple ways of dealing with keystrokes in SDL:

* Event-based
* Key Scanning


##Event-based 

When the user presses any key, then the event SDL_KEYDOWN and SDL_KEYUP is fired.

Example:
```cpp
#include "core/oxygine.h"
#include "SDL.h"
#include "SDL_keyboard.h"

void example_init()
{
	oxygine::core::getDispatcher()->addEventListener(oxygine::core::EVENT_SYSTEM, onEvent);
}


void onEvent(Event* ev)
{
	SDL_Event *event = (SDL_Event*)ev->userData;

	if (event->type != SDL_KEYDOWN)
		return;

		
	switch (event->key.keysym.sym)
	{
		case SDLK_RETURN:			
			//ENTER key was pressed, handle it
		break;
		case SDLK_SPACE:			
			//SPACE key was pressed, handle it
		break;
	}
}
```
> with the help of core::EVENT_SYSTEM, you may catch all SDL events.

##Key Scanning

Scans the status of specific keys. This method is usually used in a cycle for smooth and accurate control of an object's movement.

In the following example, the overloaded method **Actor::doUpdate** is called every frame and requests the status of the keys *A, S, D, W*. Then, the sprite's position on the screen changes accordingly:
```cpp
#include "SDL.h"
#include "SDL_keyboard.h"

class MySprite: public Sprite
{
protected:
	void doUpdate(const UpdateState &us);
};

void MySprite::doUpdate(const UpdateState &us)
{
	const Uint8* data  = SDL_GetKeyboardState(0);
     
	//calculate speed using delta time
	float speed = 1.0f * (us.dt / 1000.0f);

	Vector2 pos = getPosition();
	     
	if (data[SDL_SCANCODE_A]) pos.x -= speed;
	if (data[SDL_SCANCODE_D]) pos.x += speed;
	if (data[SDL_SCANCODE_W]) pos.y -= speed;
	if (data[SDL_SCANCODE_S]) pos.y += speed;

	setPosition(pos);
}
```
  

##oxygine::key api

These functions tell you whether a key was pressed or not (scans every frame):
```cpp
bool wasPressed(keycode);
bool wasReleased(keycode);
```

before using them, you must initialize the system

```cpp
#include "key.h"
#include "SDL_keyboard.h"

....
oxygine::key::init();
```

usage:

```cpp
if (oxygine::key::wasPressed(SDL_SCANCODE_SPACE))
   jump();
```