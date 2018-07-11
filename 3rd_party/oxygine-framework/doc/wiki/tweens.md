#Tweens

> [Inbetweening or tweening](https://en.wikipedia.org/wiki/Inbetweening) is the process of generating intermediate frames between two images to give the appearance that the first image evolves smoothly into the second image. Inbetweens are the drawings between the key frames which help to create the illusion of motion. Inbetweening is a key process in all types of animation, including computer animation." 

Oxygine provides a simple but powerful tweening interface based on template classes. It supports tweening of numeric properties, and allows you to chain tweens and actions together to create complex sequences.


##Creation and addition

Actor has method **addTween**:
```cpp
template<class Prop>
spTween addTween(const Prop &prop, timeMS duration, int loops = 1, bool twoSides = false, timeMS delay = 0, Tween::EASE ease = Tween::ease_linear)
```

* **duration** length of the animation in milliseconds
* **loops** amount of times to repeat the animation. Value of -1 will make the animation loop indefinitely.
* If **twoSides** is set to true, then the interpolation will be completed there and back: From A to B then back to A.
* **delay** the delay before the animation starts in milliseconds.
* **ease** function which controls the acceleration of the animation [function](http://easings.net/)

> see examples/Demo ->Tweens to check all available EASE functions.      

Example of Tween creation code which will move a sprite to position (400, 300) over 500 milliseconds:
```cpp
int duration = 500;//ms
spTween tween = sprite->addTween(Actor::TweenPosition(400, 300), duration);
```
There is also alternative addTween method based on simple *TweenOptions* class:
```cpp
spTween tween = sprite->addTween(Actor::TweenAlpha(0), TweenOptions(duration).delay(100).loops(3));
```
> Oxygine has a large assortment of premade Tween animations for different purposes


All Tweens added to an Actor will happen simultaneously. Many different Tweens can be added to an Actor at the same time. If you want to queue Tween animations, use the **TweenQueue** class.

If you wish to catch the moment a Tween finishes, you can use the Tween::setDoneCallback method. Example below:
```cpp
void SomeClass::onTweenDone(Event *event)
{
	log::messageln("tween done"); 
}

tween->setDoneCallback(CLOSURE(this, &SomeClass::onTweenDone));
```

Or this way:
```cpp
tween->addEventListener(TweenEvent::DONE, CLOSURE(this, &SomeClass::onTweenDone));
```
> This works slower than setDoneCallback, as addEventListener reserves extra memory.

C++11 Lambdas supported:
```cpp
tween->addEventListener(TweenEvent::DONE, [](Event*){
	log::messageln("tween done"); 
});
```

If you want to delete (detach child from parent) an actor upon the completion of an animation you can use the method **Tween::setDetachActor**:
```cpp
	tween->setDetachActor(true);
```

##Deletion
For deletion of already added actor tweens you may use the following method:
```cpp
void Actor::removeTween(spTween);
```

Deletion by name where the name is set using method **Tween::setName**:
```cpp
void Actor::removeTweensByName(const std::string &name);
```
> To be percise, the name is set using Object::SetName from which Tween inherits.

Deletion of all Tweens:
```cpp
void Actor::removeTweens(bool callComplete = false);
```
> If callComplete == true, then all Tweens will not only be deleted but also completed with calls to TweenEvent::DONE.



##Sprite Animations

Example of creation of frame-by-frame sprite animations:
```cpp
ResAnim* resAnim = resources.getResAnim("run"); 
spTween tween = sprite->addTween(TweenAnim(resAnim), duration);
```
where 'run' is an animation declared in the xml file with resources:
```xml
<image file="run.png" cols = "7" />
```
![](img/tween-run-anim.png)

In the given case the animation 'run' is made up of 7 frames.

##Dummy Tween

Sometimes a 'dummy' Tween needs to be added to an Actor (the tween does nothing):
```cpp
spTween tween = sprite->addTween(TweenDummy(), duration);
```
This will allow you to delete the Actor after a specified delay (in milliseconds):
```cpp
tween->setDetachActor(true);
```
You can also call the callback upon the animation finishes:
```cpp
tween->setDoneCallback(...);
```


##TweenQueue

**TweenQueue** inherits from Tween class. TweenQueue is used for the playback of animations in order (like a queue).

For example:
```cpp
spTweenQueue tween = new TweenQueue;
tween->add(Actor::TweenX(100), 500);
tween->add(Actor::TweenY(100), 250);
tween->add(Actor::TweenAlpha(0), 300);
sprite->addTween(tween);
```

Shorter method:
```cpp
sprite->addTween(TweenQueue::create(
		createTween(Actor::TweenX(100), 500),
		createTween(Actor::TweenY(100), 500),
		createTween(tween->add(Actor::TweenAlpha(0), 300)),
));
```
>TweenQueue::create supports up to 7 arguments

##Creation of custom Tweens

Oxygine allows easy creation and addition of custom Tweens. Let us examine a pair of ready-made Tweens:
```cpp
	typedef Property<float, float, Actor, &Actor::getX, &Actor::setX>										TweenX;
	typedef Property<float, float, Actor, &Actor::getY, &Actor::setY>										TweenY;
	typedef Property<float, float, Actor, &Actor::getWidth, &Actor::setWidth>								TweenWidth;
	typedef Property<float, float, Actor, &Actor::getHeight, &Actor::setHeight>								TweenHeight;
	typedef Property<float, float, Actor, &Actor::getRotation, &Actor::setRotation>							TweenRotation;
	typedef Property<unsigned char, unsigned char, Actor, &Actor::getAlpha, &Actor::setAlpha>				TweenAlpha;
```
These are created with the help of Property class which uses ready-made Actor methods Get and Set.

Using this we can create custom Tweens for any Actor:
```cpp
class MySprite: public Sprite
{
public:
	float getMyValue() const;
	void  setMyValue(float v);
};

typedef Property<float, float, MySprite, &MySprite::getMyValue, &MySprite::setMyValue>			MyValueTween;

mysprite->addTween(MyValueTween(123), 1000);
```
>Any Tweens can be created without the Property class. You can check TweenAnim for more details.


##Creation of custom Tweens II

There exists a more advanced method of creating custom Tweens. Unlike the other method, it does not use the template class **Property**, but rather allows the creation of your own Tweens.

For example, let's make MyTweenAnim for animating the sprite. MyTweenAnim will be similar to the already made TweenAnim, but different in the way that it will use the xml file with resources to distinguish frame intervals of images as separate animations:

A regular animation in xml is written like so:

	<image file="ninja.png" />

Now we will add attributes for two different frame animations, frames-run and frames-attack. The designated frames of the image are assigned to the animation name, run animation occurs from frames 0-10 and attack animation occurs between frames 11-20:
 
	<image file="ninja" frames-run="0-10" frames-attack="11-20" />

Each Tween must have an init method, update, done and the typedef 'type' of Actor on which the Tween will work initialized:
```cpp
class MyTweenAnim
{
public:
	typedef Sprite type;
	
	MyTweenAnim(const ResAnim *rs, const char *name) :_resAnim(rs), _start(0), _end(0)
	{
		char str[255];
		safe_sprintf(str, "frames-%s", name);
		const char *data = rs->getAttribute(str).as_string(0);
		if (data)
			sscanf(data, "%d-%d", &_start, &_end);
	}


	void init(Sprite &actor){}
	void done(Sprite &actor){}
	void update(Sprite &actor, float p, const UpdateState &us)
	{
		int frame = interpolate<int>(_start, _end, p);
		actor.setAnimFrame(_resAnim->getFrame(frame));	
	}
	
private:
	const ResAnim *_resAnim;
	int _start;
	int _end;
};
```


Usage example:
```cpp
spSprite sprite = new Sprite;
const ResAnim *rs = resources.getResAnim("ninja");
sprite->addTween(MyTweenAnim(rs, "run"), 1000);
```