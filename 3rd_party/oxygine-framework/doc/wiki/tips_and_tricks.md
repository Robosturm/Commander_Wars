#Oxygine. Tips and Tricks

##InitActor

The template function initActor provides an elegant and short syntax for initialization of actors, as well as optimizes the size of the code and increases its readability. The function supports up to 9 arguments which can be written in any order. All supported arguments can be found in the header file ‘initActor.h’.


Regular Method:

```cpp
toolTip = new TextActor();
toolTip->setStyle(styleTooltip);
toolTip->setPosition(700, 850);
toolTip->setInputEnabled(false);
toolTip->setText("Some Text");
toolTip->attachTo(getStage());
```

Using initActor:
```cpp
toolTip = initActor(new TextActor(),
      arg_style = styleTooltip,
      arg_pos = Vector2(700, 850),
      arg_input = false,
      arg_text = "Some Text",     
      arg_attachTo = getStage());
```

Regular Method:
```cpp
spSprite spr = new Sprite;
spr->setResAnim(resAnim);
spr->setAnchor(0.5f, 0.5f);
spr->setPosition(getStage()->getSize()/2);
spr->setScale(1.0f / getStage()->getScaleX());
spr->attachTo(getStage());
```
Using initActor:
```cpp
spSprite spr = initActor(new Sprite,
	arg_resAnim = resAnim,
	arg_anchor = Vector2(0.5f, 0.5f),
	arg_pos = getStage()->getSize()/2,
	arg_scale = 1.0f / getStage()->getScaleX(),
	arg_attachTo = getStage());
```

##Tracking Leakages

All objects in Oxygine inherit from the class ObjectBase. This allows for accurate tracking of leakages in the DEBUG mode. Upon the creation of ObjectBase, it is registered in the list of created object, and in the destructor deletes itself from said list. Upon the closure of the application, when all previously created objects are destroyed, it is possible to find objects that have yet to be deleted. The method ObjectBase::dumpCreatedObjects() prints them in the log:

  allocated objects:
  id = 3, name = '!renderer. white', typeid = 'class oxygine::NativeTextureGLES', refs = 1
  id = 4, name = '', typeid = 'class oxygine::Stage', refs = 1
  id = 5, name = '', typeid = 'class oxygine::Clock', refs = 1
  total: 3 -----------------------------

Each object has a unique ID, which allows for tracking of the moment it is created and destroyed (assuming its ID is not changed) with the help of the following methods:

```cpp
ObjectBase::showAssertInCtor(int id); 
ObjectBase::showAssertInDtor(int id); 
```

For example, if we call the following method before the start of the application:
```cpp
  ObjectBase::showAssertInCtor(4); 
```
then an assertion occurs upon the creation of the object Stage.

Your own classes’ creation and deletion can also be tracked in this way. 


##UserData and Objects

Sometimes it is necessary to attach some arbitrary data to an object, for example to Actor. For this the class **ObjectBase**, from which Actor inherits, has a field “userData” and methods used to perform operations on it.
```cpp
void ObjectBase::setUserData(void *data);
void *ObjectBase::getUserData();
```
This method is used in the example examples/DemoBox2D when attaching the class Body to Sprite.

This way we can attach a pointer to any Actor:

```cpp
spActor actor = new Actor;
actor->setUserData(data);
```

Example:
```cpp
struct data
{
  data(int v):value(v){}
  int value;
};

void SomeClass::init()
{
  for (int i = 0; i < 10; ++i)
  {
    spActor actor = new Actor();
    data *dta = new data(i);//don’t forget to delete allocated memory!
    actor->setUserData(dta);
    actor->addEventListener(TouchEvent::CLICK, CLOSURE(this, &SomeClass::&actorClicked));
    actor->attachTo(getStage());
  }
}

void SomeClass::actorClicked(Event *event)
{
        data *dta = (data*)(event->currentTarget->getUserData());
        log::messageln("clicked to actor with value: %d", dta->value);
}
```

The same method can be used to attach the string to the Object:
```cpp
actor->setName(“name”);
string name = actor->getName();	
```


##UserData for resources in XML

Any resource that is described in the xml file can have custom attributes:
```xml
<image file="bg.jpg" data-custom = "custom user data per xml resource defined in xml"/>. 
```
Example of use:

Suppose you have a few images, all of which are moving from left to right on the screen, but each has its own unique color and speed. This data has to be set elsewhere, and XML UserData works perfectly to solve this. The easiest way to do it is to set custom attributes to the images in the XML resources like so:
```xml
<image file="anim1.png" duration = "1000" color=”FFA0BC”/>
<image file="anim2.png" duration = "1400"/>
<image file="anim3.png" duration = "800"/>
```
After that is done, you can extract this custom data in **pugi::xml_node** in which these attributes are saved from the xml.
```cpp
string name = “anim1”;//or anim2 or anim3
ResAnim *rs = resources.getResAnim(name);
 
spSprite sprite = new Sprite;
sprite->setResAnim(rs);
sprite->attachTo(getStage());
Color color = hex2color(rs->getNode().attribute(“color”).as_string(“ffffff”));//0xffffffff - use white color if it not set
sprite->setColor(color);
 
int duration = rs->getNode().attribute(“duration”).as_int();
sprite->addTween(Actor::TweenX(1024), duration);
```

##Automatic Pause

With the help of the Clock class it is possible to control time and the update cycle of actors. It is possible to slow or speed time up, pause the update cycle or change its speed. Clock works on the actor itself and all of its children, including any of its Tweens.

Example of use:

Suppose that in your game the player presses the Menu key and opens the game menu, during which the game itself is paused. The scene must have everything paused - all animations, update cycles and Tweens. With the help of Clock you can do this very easily:
```cpp
void init()
{   
    _clock = new Clock;
   
    spActor gameField = getGameField();
    gameField->setClock(_clock); 
}
void onMenuClicked(Event *)
{
    _clock->pause();
}
void onMenuClosed(Event *)
{
    _clock->resume();
}
```


##What to do with lost/unfound resources and other objects by ID

When we try to find some resource by ID and it is not found, what is the best course of action? Do we print the error in the log, show assert or simply ignore it? This depends on the situation. Many methods in Oxygine which search for things by ID have a special argument called error_policy:

```cpp
enum error_policy
{
	ep_show_error,//shows assert and prints error to log
	ep_show_warning,//prints warning to log
	ep_ignore_error//do nothing, returns null
};


spActor Actor::getChild(const string &name, error_policy ep = ep_show_error) const;
ResAnim *Resources::getResAnim(const string &id, error_policy ep = ep_show_error);
handle file::open(const char *file, const char *mode, error_policy ep = ep_show_error);
```

With the help of **error_policy** we can choose how the engine behaves in case of an error. Usually, by default, this argument causes the most strict behaviour = ep_show_error.

Example of use, which prints a friendly warning into the log:

```cpp
ResAnim *rs = resources.getResAnim(“anim1”, ep_show_warning);
if (rs)
   sprite->setResAnim(rs);
else
   sprite->setResAnim(resources.getResAnim(“temp_anim”));
```

You may add error_policy into your own code as well. This is the key function:
```cpp
void handleErrorPolicy(error_policy ep, const char *format, ...);
```
Example:
```cpp
Property *StatSet::getProperty(const string &stat, error_policy ep)
{
	statList::iterator i = find(_stats, stat);
	if(i == _stats.end()) 
	{
		handleErrorPolicy(ep, "StatSet. can't find property %s", stat.c_str());
		return 0;
	}

	return &i->val;
}
```




##The easiest way to cull unused/unseen actors

In some situations it is useful to ‘cull’ certain actors because they are out of range of your game, for example when said actors are not seen and therefore have no reason to be rendered. This is done with the following method:

```cpp
 void Actor::setCull(bool enable) 
```

This is called for every actor which needs to be checked for visibility on screen. The actor must have a correct size, but rotation is not taken into account.
In the example Demo -> SlidingActor, setCull(true) is used. You may notice how the amount of rendered triangles changes during scrolling.

##Unique ID for custom events

If it is necessary to generate a unique ID for a custom event, then defining it with **eventID** will make it possible:

```cpp
#define eventID(a,b,c,d) ( ((unsigned int)a) | (((unsigned int)b)<< 8) | (((unsigned int)c)<<16) | (((unsigned int)d)<<24))
```

```cpp
class MySprite: public Sprite
{
public:
  enum
  {
    event_SOMETHING = eventID('M', 'S', ‘S', 'T'),
  };

  void init()
  {
    addEventListener(event_SOMETHING, CLOSURE(this, &MySprite::onSomething));
  }

  void onSomething(Event *event)
  {
    log::messageln("something happened!");    
  }

  void fireSomething()
  {
    Event event(event_SOMETHING);
    dispatchEvent(&event);
  }
};
```

where ('M', 'S', ‘S', 'T') - are codes you wrote, which must form a unique int32, in this case this short for “MySpriteSomeThing”.



##Automatic loading/unloading of atlases during use 
```cpp
Sprite::setManageResAnim(bool manage):
```

If set to true, then the Sprite will control the loading/unloading of the atlas, and upon change in animation the old atlas will be unloaded and a new one loaded. This is useful for unique images which meet only a few times on the screen.


It is also possible to implement it alongside the following:
```cpp
void Resources::loadXML(const string &file, LoadResourcesContext *load_context = 0, bool load_completely = true, bool use_load_counter = false, const string &prebuilt_folder = "");
```
the arugmnet - **use_load_counter** is set to false by default, and any call resAnim->getAtlas()->load()/unload() will load/unload the resource. 
However, if use_load_counter is true, then calls resAnim->getAtlas()->load()/unload() will use an internal counter which will count the amount of load()/unload() calls.




##A little tip

Do not forget to turn off the input processing for actors which do not need it, especially if there are many such actors. This will significantly lower the load when processing, for example, the movement of a finger on the touchscreen.

```cpp
/**Enables/Disables input events(touch, mouse) for Actor.*/
void Actor::setTouchEnabled(bool enabled) {_flags &= ~flag_inputEnabled; if (enabled) _flags |= flag_inputEnabled;}
/**Enables/Disables input events(touch, mouse) for children of Actor.*/
void Actor::setTouchChildrenEnabled(bool enabled) {_flags &= ~flag_inputChildrenEnabled; if (enabled) _flags |= flag_inputChildrenEnabled;}
```


##Draw debug line
```cpp
void drawLine(spActor root, const Vector2 &a, const Vector2 &b, const Color &color = Color(Color::White))
{
	spColorRectSprite sp = new ColorRectSprite;
	sp->attachTo(root);
	sp->setPosition(a);
	Vector2 dir = b - a;
	float len = (b - a).length();
	sp->setSize(len, 1);
	float angle = atan2(dir.y, dir.x);
	sp->setRotation(angle);
	sp->setColor(color);
	sp->addTween(TweenDummy(), 1)->setDetachActor(true);
}


void example_update()
{
	drawLine(getStage(), Vector2(100, 100), getStage()->getSize() / 2);
...
}

```