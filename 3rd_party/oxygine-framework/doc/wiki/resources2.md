#Resources

All resources which you are planning to use in your application should be predefined in a special xml file.

You may create a single large xml file for the entire application or break your resources up into smaller files for easier use.

Example: [resources.xml](https://github.com/oxygine/oxygine-framework/blob/master/examples/HelloWorld/data/res.xml) from HelloWorld:
```xml
<?xml version="1.0"?>
<resources>
	<set path = "images" />
	<atlas>
		<image file="anim.png" cols = "7" />
		<image file="button.png"/>		
	</atlas>
		
	<set path = "fonts" />
	<font file="main.fnt" />	
</resources>
```
In this file, there are 3 resources that could be accessed by id: *anim, button and main*.

##Resource loading

Example of loading resources.xml and creation of a sprite with an image "button":
```cpp
Resources menu;

void example_init()
{
	menu.loadXML("resources.xml");

	ResAnim *resButton = menu.getResAnim("button");
	spSprite close = new Sprite;
	close->setResAnim(resButton);
	close->attachTo(getStage());
}
```
A single Resources instance may contain multiple different xmls simultaneously, given that their ids are not intersecting:
```cpp
Resources game;
game.loadXML("monsters.xml");
game.loadXML("icons.xml");
```

##Resource ID

* Usually, the ID represents the name of the file and its extension without the folder path
* ID is not case sensitive
* Initializing an arbitrary ID:

```xml
<image id="std_btn" file="button.png"/>
```
> ID can be set for any type of resource, not only < image >.

##Resource location

You may specify the path to a resource in different ways:

* full path:
```xml
<image file="images/menu/button.png"/>
```
* using < set path />:
```xml
<set path = "images/menu" />
<image file="button.png"/>
     or
<set path = "images" />
<image file="menu/button.png"/>
```

* by default, all paths are relative to the working directory of the application (in this case, it is the data folder)
* in order to specify the path relative to the xml file in which they are described, use the prefix **"./"**:
```xml
<image file="./button.png" />
	or
<set path = "./" />
<image file="button.png"/>
```

##< Image >

The main type of resources with which you will be working in Oxygine are images. Images are described with the **< image >** tag. An image cannot exist independently, because you must enclose it in **< atlas >**:
```xml
<atlas>
	<image file="anim.png" />
	<image file="button.png"/>		
</atlas>
```

>A texture atlas provides a way to improve the performance of your apps by combining all of the app’s image assets into one or more large images. This provides you with a way to improve the performance of your app by drawing multiple images with a single draw call.

###Cols and Rows attributes

For animated images or images with multiple states, multiple image frames may be created in the same image file.


* **cols** allows you to define the amount of columns in the image
* **rows** allows you to define the amount of rows in the image
```xml
<image .... cols = "3" rows = "2" />
```
For example, the following image contains 4 columns:
  
![](img/image_cols.png)

and must be defined like this:
```xml
<image file="flags.png" cols="4" /> 
```
The pseudocode below demonstrates how it will be used in-game:
```cpp
enum state
{
	state_new = 0, 
	state_fight = 1, 
	state_refresh = 2,
	state_pvp = 3
}
	
state state = getLocationState();
int column = state; 
spSprite locSprite = new Sprite;
locSprite->setResAnim(resources.getResAnim("flags"), column);
```
###Trim attribute
Default value is true. Set it to false if you don't want to trim image by transparency.
```xml
<image file="image.png" trim="false or true" /> 
```

###Box9Sprite attributes

Stretchable (aka nine-patch) images used in *Box9Sprite* class can have additional attributes:

* **guideX1** Left side of stretchable image region
* **guideX2** Right side of stretchable image region
* **guideY1** Top of stretchable image region
* **guideY2** Bottom of stretchable image region
* **vertical** Vertical resize mode: 0 - tiling, 1 - full tiling, 2 - stretching
* **horizontal** Horizontal resize mode: 0 - tiling, 1 - full tiling, 2 - stretching
```xml
<image .... guideX1 = "10" guideX2 = "20" horizontal = "2" />
```
##< Atlas >

All images must be encapsulated in < atlas >. You may have multiple < atlas > blocks in a single xml file.

By default, the atlas texture generates upon the loading of an xml file. Oxygine has the utility **[oxyresbuild](atlasses)** for generation of atlases upon the compilation of the application.

### Format 

For video memory optimization, you may define a special pixel format for a generated atlas, for example:
```xml
<atlas format="565">
```
Supported formats:

* **8888**, 32-bit format, default
* **565**, 16-bit format with no alpha channel where R=5, G=6, B=5 bits
* **5551**, 16-bit format with one alpha channel where R=5, G=5, B=5, A=1 bits
* **4444**, 16-bit format with 4 bits per channel where R=4, G=4, B=4, A=4 bits

###linearFilter
Boolean option. Default value is true.
Disables or Enables Bilinear filtering mode atlas texture.
```xml
<atlas linearFilter="true|false">
```

###clamp2edge
Boolean option. Default value is true.
Disables or Enables Clamp to Edge filtering mode for atlas texture.
```xml
<atlas clamp2edge="true|false">
```
##User data attributes

Resource may have arbitrary attributes. These attributes may have any name (with the exception of the ones included with Oxygine).

Example: 
```xml
<image file="button.png" anim-delay = "800" offset-x = "200"/>
```
Access to the attribute:
```cpp
ResAnim *rs = resources.getResAnim("button");
int time = rs->getAttribute("anim-delay").as_int(0);
int offset = rs->getAttribute("offset-x").as_int(0);
```
> You could set user data to any type of resource (not only image).


##< Font >

More [tutorial](fonts)

##< Starling > (Sparrow) atlas format

Oxygine supports atlas format exported for Starling/Sparrow framework. 

Declare it in the resources xml:
```xml
<starling file="atlas.xml" />
```

Then use as usual resource:
```cpp
ResAnim *rs = resources.getResAnim("image");
```