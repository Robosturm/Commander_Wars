#Клавиатура

Oxygine не имеет своего собственного API для обработки клавиш. Если вам необходим доступ к клавиатуре, то необходимо использовать функции из SDL2 (или Marmalade).
 
Есть несколько способов обрабатывать нажатия кнопок в SDL:

* Event Based
* опрос клавиш


##Event Based 

Когда пользователь нажимает любую клавишу, то срабатывает событие SDL_KEYDOWN и SDL_KEYUP. 

Пример:

	#include "Input.h"
	#include "SDL.h"
	#include "SDL_keyboard.h"
	

	void example_init()
	{
		Input::instance.addEventListener(Input::event_platform, onEvent);
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

> с помощь Input::event_platform можно перехватывать все SDL события. 

##Опрос клавиш

Опрос состояния нужных клавиш. Обычно применяется в цикле и подходит для мягкого и точного управления движением объекта.

В данном примере перегружается метод **Actor::doUpdate**, который вызывается каждый кадр и опрашивается состояние клавиш *A, S, D, W*. В следствии чего изменяется позиция спрайта на экране:  

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
	
  