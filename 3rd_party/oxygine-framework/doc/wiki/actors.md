#Scene Graph
Low level 2D frameworks provides you basic rendering functionality where you're required to draw images each frame manually.
 
Oxygine isn't low level 2D framework and in addition to this it provides own scene graph.

##Example
You could create Sprite, set position and image, attach it to Display (called Stage) and "forget". Sprite would be displayed and updated each frame automatically. You could say to this sprite: "rotate on 360 degrees for 2 seconds 5 times and then remove self".

```cpp
spSprite sprite = new ColorRectSprite;
sprite->setPosition(300, 300);
sprite->setSize(100, 100);
sprite->attachTo(getStage()); 
sprite->addTween(Sprite::TweenRotation(2*PI), 2000, 5)->setDetachActor(true);
```

You could create second *"child"* sprite attached to already created and they would be rotating both:

```cpp
spSprite child = new ColorRectSprite;
child->setSize(30, 30);
child->attachTo(sprite);
```

If you want to handle mouse click or touch on *"child"* subscribe to click event:

```cpp
void onSpriteClicked(Event*)
{
	printf("clicked to child");
}

EventCallback callback = onSpriteClicked;
child->addEventListener(TouchEvent::CLICK, callback );
```

If you don't want handle the click event on that sprite anymore remove the listener:

```cpp
child->removeEventListener(TouchEvent::CLICK, callback);
```
> it works only if you create callback with *CLOSURE* - CLOSURE(someptr, classmethod)

If you do not need *child* anymore just detach it:

```cpp
child->detach();
```

You don't need to call **delete** because Sprites (Actors) are managed by intrusive smart pointers. It has reference counter inside. Sprite will be freed automatically when you remove the last reference to it. 

Oxygine classes with prefix **sp** are smart pointers. For example **spSprite** is just typedef:

```cpp
typedef oxygine::intrusive_ptr<oxygine::Sprite> spSprite;
```

#Actors
 
![Actor inheritance](img/actor.png)
 
 

##Actor
[Actor](http://oxygine.org/doc/api/classoxygine_1_1_actor.html) is a base class in scene graph. 
It can be moved, rotated, scaled, animated. Actors can have other actors as children. When a parent is transformed, all its children are transformed as well.

Actor has properties:

- transformation (position, scale, rotation)
- anchor point
- size
- name
- transparency
- list of children
- list of added tweens 
- etc. 




##Sprite

You would widely use [Sprites](http://oxygine.org/doc/api/classoxygine_1_1_sprite.html) in your app.
Sprite is subclass of Actor. It is using for displaying images and animations.


![](img/sprite.png)

##Polygon
[Polygon](http://oxygine.org/doc/api/classoxygine_1_1_polygon.html) is subclass of Sprite. It is used for rendering custom vertices array.  

![](img/polygon.png)

##TextField
[TextField](http://oxygine.org/doc/api/classoxygine_1_1_text_field.html) is subclass of Actor. It is used for displaying text.

![](img/text.png)

##Progress Bar
[ProgressBar](http://oxygine.org/doc/api/classoxygine_1_1_progress_bar.html) is subclass of Sprite. It is used for displaying progress.

![](img/progressbar.png)

##ColorRectSprite
[ColorRectSprite](http://oxygine.org/doc/api/classoxygine_1_1_color_rect_sprite.html) is subclass of Sprite. It is rectangle filled with one color.

![](img/colorrect.png)

##ClipRectActor
[ClipRectActor](http://oxygine.org/doc/api/classoxygine_1_1_clip_rect_actor.html) is sublcass of Actor. It is used to clip area with children outside of actor's bound.

##Box9Sprite

[Box9Sprite](http://oxygine.org/doc/api/classoxygine_1_1_box9_sprite.html) is sublcass of Sprite. It is used to scale the image in such a way that the 4 corners remain unscaled, but the four edges are scaled(or tiled) in one axis and the middle is scaled(or tiled) in both axis. So the graphical representation of this scenario will be something like this:

![](img/box9sprite.jpg)


##MaskedSprite
[MaskedSprite](http://oxygine.org/doc/api/classoxygine_1_1_masked_sprite.html) is subclass of Sprite. It is using other sprite for masking own children.

![](img/mask.png)