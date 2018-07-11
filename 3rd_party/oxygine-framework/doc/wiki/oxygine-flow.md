#Oxygine-Flow

repository: https://github.com/oxygine/oxygine-flow

**oxygine-flow** is an extension for oxygine which provides a neat system for dialogue/scene organization based on an asynchronous model of events.

The general functionality of oxygine-flow will be explained using one example that will start simple and get more complex. To start off, we will use an empty project with the HelloWorld template from the oxygine-framework. We will be working only in example.cpp.

> Fully completed example is located in **oxygine-flow\examples\HelloFlow**

> Video demonstration of this example: https://www.youtube.com/watch?v=Z4p1ol1-cLM&feature=youtu.be

> Web version: http://oxygine.org/online_demo.php button *Run Oxygine-Flow example*

First off we will add all of the required functions and includes in **example.cpp**:

```cpp
	#include "oxygine-framework.h"
	#include "flow/flow.h"
	#include "flow/Scene.h"
	#include "flow/Transition.h"
	using namespace oxygine;


	void example_preinit()
	{

	}

	void example_init()
	{
		//initialize oxygine-flow
		flow::init();
	}

	void example_update()
	{
		//update oxygine-flow each frame
	    flow::update();
	}

	void example_destroy()
	{
		//free oxygine-flow
	    flow::free();
	}
```


##Scene
The main class with which you will be working with is **Scene**.
It is required of you to inherit from that class and attach all interface elements to actor **Scene::_holder**.


Let's create the class MyScene for our simple scene with a button in the center:

```cpp
	class MyScene : public oxygine::flow::Scene
	{
	public:
		MyScene()
		{
			setName("MyScene");

			spBox9Sprite view = new Box9Sprite;
			_view = view;
			_view->setSize(_holder->getSize());		
			_view->attachTo(_holder);

			spButton btn = new Button;
			btn->setPosition(_view->getSize() / 2 - btn->getSize() / 2);
			btn->attachTo(_view);
		}

		spActor _view;
	};
```


Then show MyScene upon the start of the application:

```cpp
	void example_init()
	{
		//initialize oxygine_flow
		flow::init();

		//create scene and display it
	    flow::show(new MyScene);
	}
```

![](img/flow1.png)

For now the button does not do anything, so let's add the closure of the scene (inside the constructor of MyScene) when the button is clicked by calling the method **Scene::finish()**:

```cpp
	btn->addEventListener(TouchEvent::CLICK, [=](Event*){
		finish();
		});
```

> to make things easier, we use lambda
> keep in mind: finish() is called for the class MyScene

Now the scene MyScene closes when we press the button, but how do we know that? A way of checking is to pass Callback into the method **flow::show**:

```cpp
	flow::show(new MyScene, [](Event* event){
		log::messageln("scene closed");
		});
```

Let's make the example a bit more complex by adding another class for dialogue which will differ from the scene by the flag **Scene::_dialog**, which will tell the system that it needs to be shown on top:


```cpp
	class MyDialog: public flow::Scene
	{

	public:
		MyDialog()
		{
			setName("MyDialog");
			
			_dialog = true;

			spBox9Sprite view = new Box9Sprite;
			_view = view;
			_view->setSize(400, 300);
			_view->attachTo(_holder);
			_view->setPosition(_holder->getSize() / 2 - _view->getSize() / 2);

			spButton btn = new Button;
			btn->setPosition(120, 200);
			btn->attachTo(_view);

			btn->addEventListener(TouchEvent::CLICK, [=](Event*) {
				finish();
			});
		}

		spActor _view;
	};
```

Now we will add the ability to open the dialogue from class MyScene by rewriting the handling of the press of the button:


```cpp
	btn->addEventListener(TouchEvent::CLICK, [=](Event*){
			flow::show(new MyDialog, [=](Event*){
				//finish();
				log::messageln("dialog closed");
				});
		});		
```

Now, every time the button in MyScene is clicked, MyDialog will open. If you uncomment *//finish();* above then MyScene will also close upon the press of the button.

For now the appearance of MyDialog seems to be very simple. Let's make it a bit nicer by adding the following to the MyDialog constructor:

```cpp
	flow::TransitionMove::assign(this);
```

Now MyDialog will appear smoothly from the top of the screen, slightly shading MyScene behind it. 

![](img/flow2.png)

##Special Events

Let's learn how to use some key Events which class Scene can call.
For example, before MyDialog shows up we can make it a random color. Add the following to MyDialog's constructor:

```cpp
	addEventListener(EVENT_PRE_SHOWING, [=](Event*) {
			view->setColor(Color(rand() % 255, rand() % 255, rand() % 255, 255));
		});
```

When it is finally open, smoothly change its color to white, like it was before by adding this to MyDialog's constructor:


```cpp
	addEventListener(EVENT_POST_SHOWING, [=](Event*) {
			view->addTween(Sprite::TweenColor(Color::White), 300);
		});
```

Analogous events exist for the closing of the scene:


- **EVENT_PRE_HIDING**
- **EVENT_POST_HIDING**


And also for pressing Esc/Back (Back for Android):

- **EVENT_BACK**

Let's add the handling of EVENT_BACK into MyDialog's constructor the same way we did it for the event CLICK in *btn*:


```cpp
	addEventListener(EVENT_BACK, [=](Event*) {
			finish();
		});
```


Events which notify of a new scene that was opened on top of an already existing one:

- **EVENT_SCENE_SHOWN**
- **EVENT_SCENE_HIDDEN**

Events for entering/exiting a scene:

- **EVENT_ENTERING**
- **EVENT_LEAVING**


##Important
- All open scenes are located on *Stack*, which work based on the principle *Last In First Out*, and the opened Scene always waits until its on the top of the stack. The list of opened scenes can be seen in the DebugActor ![](img/flow3.png)
- If a scene is already open (ie. it is seen in the *stack of open scenes*), it cannot be shown another time before it is closed (removed from the Stack), even if it is not currently visible.
- If you need to remove a scene that is open but invisible, use *Scene::remove*.
- You can see the stack of open scenes in the DebugActor.
- You cannot (!!!) make a new Scene appear over a currently open Dialog. You must first wait for the dialog to close and show your scene in the Callback to flow::show.
- All EVENTS above are logged, so you can always trace them in the logs.



##Transitions

Oxygine-flow comes with a small selection of different animations for transitions between dialogs and scenes:

- TransitionMove - appears from the top of the screen and dims the background
- TransitionFade - smooth appearance out of nowhere (fade)
- TransitionShutters - appears like a rolling shutter
- TransitionQuads