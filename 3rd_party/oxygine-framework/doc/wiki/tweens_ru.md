#Tweens

> [Inbetweening or tweening](https://en.wikipedia.org/wiki/Inbetweening) is the process of generating intermediate frames between two images to give the appearance that the first image evolves smoothly into the second image. Inbetweens are the drawings between the key frames which help to create the illusion of motion. Inbetweening is a key process in all types of animation, including computer animation." 

Oxygine provides a simple but powerful tweening interface based on template classes. It supports tweening of numeric properties, and allows you to chain tweens and actions together to create complex sequences.


##Создание и добавление

Actor has method **addTween**:

	template<class Prop>
	spTween addTween(const Prop &prop, timeMS duration, int loops = 1, bool twoSides = false, timeMS delay = 0, Tween::EASE ease = Tween::ease_linear)

* **duration** length of the animation in milliseconds
* **loops** amount of times to repeat the animation. Value of -1 will make the animation loop indefinitely.
* If **twoSides** is set to true, then the interpolation will be completed there and back: From A to B then back to A.
* **delay** the delay before the animation starts in milliseconds.
* **ease** function which controls the acceleration of the animation [function](http://easings.net/)

> see examples/Demo ->Tweens to check all available EASE functions.      

Пример создания Tween, который передвинет спрайт в точку (400, 300) за 500 миллисекунд:

	int duration = 500;//ms
	spTween tween = sprite->addTween(Actor::TweenPosition(400, 300), duration);

> В Oxygine есть большое количество уже созданных Tween для разных свойств. 


Все Tweens добавленные в Actor исполняются одновременно и параллельно. Одновременно можно добавить несколько разных Tween. Если нужна последовательность из нескольких анимаций, то используйте класс TweenQueue.

Чтобы поймать момент окончания анимации можно использовать Tween::setDoneCallback метод. Пример использования:

	void SomeClass::onTweenDone(Event *event)
	{
		log::messageln("tween done"); 
	}


	tween->setDoneCallback(CLOSURE(this, &SomeClass::onTweenDone));

Или подпишитесь на событие окончания TweenEvent::Done:

	tween->addEventListener(TweenEvent::DONE, CLOSURE(this, &SomeClass::onTweenDone));

> это работает несколько медленнее, чем setDoneCallback, т.к при первом вызове addEventListener происходит дополнительное выделение памяти.  


Если необходимо удалить (отсоеденить от родителя) актера по окончанию анимации можно воспользоваться методом Tween::setDetachActor

	tween->setDetachActor(true);

## Удаление

Для удаления уже добавленных Tween в Actor есть метод:

	void Actor::removeTween(spTween);

Удаление по имени, где имя устанавливается с помощью метода Tween::setName:
	 
	void Actor::removeTweensByName(const std::string &name);

> Если быть точным, то имя устанавливается через Object::setName, от которого унаследован Tween

Удаление всех Tween:
	
	void Actor::removeTweens(bool callComplete = false);

> если callComplete == false, то все Tween будут не просто удалены, но и завершены, с вызовом TweenEvent::DONE событий. 



## Спрайтовая анимация

Пример создания спрайтовой по кадровой анимации:

	ResAnim* resAnim = resources.getResAnim("run"); 
	spTween tween = sprite->addTween(TweenAnim(resAnim), duration);

Где анимация 'run' объявлена в xml файле с ресурсами:

	<image file="run.png" cols = "7" />

![](img/tween-run-anim.png)

В данном случае анимация 'run' состоит из 7 кадров.

## Dummy Tween

Иногда в актера нужно добавить твин, который ничего не делает:
	
	spTween tween = sprite->addTween(TweenDummy(), duration);

Но зато таким образом можно, например, удалить актера через duration миллисекунд:

	tween->setDetachActor(true);

Или вызвать произвольный callback по завершению:

	tween->setDoneCallback(...);



##TweenQueue

TweenQueue это наследник от класса Tween. TweenQueue используется для проигрывания анимаций последовательно.  

Пример:

	spTweenQueue tween = new TweenQueue;
	tween->add(Actor::TweenX(100), 500);
	tween->add(Actor::TweenY(100), 250);
	tween->add(Actor::TweenAlpha(0), 300);
	sprite->addTween(tween);

Более короткий способ:

	sprite->addTween(TweenQueue::create(
		createTween(Actor::TweenX(100), 500),
		createTween(Actor::TweenY(100), 500),
		createTween(tween->add(Actor::TweenAlpha(0), 300)),
	));


> TweenQueue::create поддерживает до 5ти аргументов
	

##Creating custom tweens

В Oxygine можно легко создавать и добавлять свои собственные Tween. Рассмотрим парочку уже существующих:

	typedef Property<float, float, Actor, &Actor::getX, &Actor::setX>										TweenX;
	typedef Property<float, float, Actor, &Actor::getY, &Actor::setY>										TweenY;
	typedef Property<float, float, Actor, &Actor::getWidth, &Actor::setWidth>								TweenWidth;
	typedef Property<float, float, Actor, &Actor::getHeight, &Actor::setHeight>								TweenHeight;
	typedef Property<float, float, Actor, &Actor::getRotation, &Actor::setRotation>							TweenRotation;
	typedef Property<unsigned char, unsigned char, Actor, &Actor::getAlpha, &Actor::setAlpha>				TweenAlpha;

Они создаются с помощью шаблонного класса Property, который использует уже существующие в актере методы Get и Set.

По этому же принципу можно создавать свои произвольные Tween для любого вашего актера:
	
	class MySprite: public Sprite
	{
	public:
		float getMyValue() const;
		void  setMyValue(float v);
	};

	typedef Property<float, float, MySprite, &MySprite::getMyValue, &Actor::setMyValue>			MyValueTween;

	mysprite->addTween(MyValueTween(123), 1000);

> Произвольные Tween можно создавать и без шаблонного класса Property, в качестве примера можете посмотреть TweenAnim.

	

##Creation of custom Tweens II
	
Существует другой, более продвинутый способ по созданию своего собственного Tween. В отличии от предыдущего примера он не использует шаблонный класс Property, а реализует весь Tween полностью.

В качестве примера реализуем MyTweenAnim для анимирования спрайта. MyTweenAnim будет похож на уже существующий TweenAnim, но в отличии от TweenAnim интервал из проигрываемых кадров, будет задаваться отдельно в xml файле с ресурсами (на основе произвольных атрибутов):

Обычная анимация в xml описывается так:

	<image file="ninja.png" />

Но мы добавим к ней пару произвольный атрибутов frames-run и frames-attack, которые будут означать, что с 0 по 10 кадр расположена анимация бега, а с 11 по 20 анимация атаки: 
 
	<image file="ninja" frames-run="0-10" frames-attack="11-20" />

В каждом Tween должны быть реализованы методы init, update и объявлен 'type' Актера, с которым будет работать Tween:

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



Пример использования:

	spSprite sprite = new Sprite;
	const ResAnim *rs = resources.getResAnim("ninja");
	sprite->addTween(MyTweenAnim(rs, "run"), 1000);

	