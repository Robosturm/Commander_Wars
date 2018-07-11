#Resources

Все ресурсы, которые вы собираетесь использовать в приложении желательно заранее   описывать в специальном xml файле. 

Вы можете создать 1 большой xml файл на все приложение или разделить ваши ресурсы логически по нескольким меньшим файлам.

Resources.xml из примера HelloWorld:

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

В данном файле описано 3 ресурса доступные по id: *anim, button и main*.

##Загрузка ресурсов

Пример загрузки resources.xml и создание спрайта с изображением "button":

	Resources menu;

	void initMenu()
	{
		menu.loadXML("resources.xml");

		ResAnim *resButton = menu.getResAnim("button");
		spSprite close = new Sprite;
		close->setResAnim(resClose);
	}

В один instance Resources может быть загружено несколько разных xml одновременно, при условии что их id не пересекаются:

	Resources game;
	game.loadXML("monsters.xml");
	game.loadXML("icons.xml");


##ID ресурса

* Обычно ID получается из удаления расширения и имени папки из имени файла.
* ID не чувствителен к регистру 
* Можно указывать произвольный ID:

		<image id="std_btn" file="button.png"/>
  
> ID можно указывать для любого типа ресурса, а не только < image >.

##Расположение ресурса

Можно задавать путь к ресурсу разными способами:

* указывать полный путь:

		<image file="images/menu/button.png"/>

* использовать < set path />:
		
		<set path = "images/menu" />
		<image file="button.png"/>
		или
		<set path = "images" />
		<image file="menu/button.png"/>

* по умолчанию все пути указываются относительно рабочей директории приложения (в примерах это папка data)
* чтобы указать путь относительно xml файла в котором они описаны нужно использовать префикс **"./"**:

		<image file="./button.png" />
		или 
		<set path = "./" />
		<image file="button.png"/>




##< Image >

Основной тип ресурсов с которыми вы будете иметь дело в Oxygine - это изображения. Изображение описывается с тегом **< image >**. Картинка не может существовать сама по себе, потому ее обязательно нужно заключить внутрь **< atlas >**:

	<atlas>
			<image file="anim.png" />
			<image file="button.png"/>		
	</atlas>


>A texture atlas provides a way to improve the performance of your apps by combining all of the app’s image assets into one or more large images. This provides you with a way to improve the performance of your app by drawing multiple images with a single draw call.

###Атрибуты Cols and Rows

Иногда для удобства в одном изображении можно хранить сразу несколько разных кадров, это может быть анимация или различные состояния объекта.


* **cols** позволяет задать число столбцов в изображении 
* **rows** позволяет задать число строк в изображении

	<image .... cols = "3" rows = "2" />

Например, данная картинка содержит в себе 4 столбца:
  
![](img/image_cols.png)

и должна быть описана таким образом:

	<image file="flags.png" cols="4" /> 

Псевдокод ниже демонстрирует ее использование в игре:

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
	
	
##< Atlas >

Как было уже написано выше все изображения обязательно должны быть заключены в < atlas >. Вы можете иметь много разных блоков < atlas > в одном xml файле.

По умолчанию текстура атласа генерируется при загрузке xml файла. В Oxygine входит утилита **[oxyresbuild](atlasses)** для генерации атласов еще на этапе сборки приложения.  

### Format 

Для экономии видеопамяти можно указать особый формат пикселя для генерируемого атласа, например:

	<atlas format="565">

Поддерживаемые форматы:

* **8888**, 32 битный формат, который установлен по умолчанию
* **565**, 16 битный формат без альфа канала, где R=5, G=6, B=5 bits
* **5551**, 16 битный формат, с 1 битом на альфаканал, где R=5, G=5, B=5, A=1 bits
* **4444**, 16 битный формат, по 4 бита на каждый канал, где R=4, G=4, B=4, A=4 bits



###User data attributes

Ресурсы могут иметь произвольные свойства - атрибуты. Произвольные атрибуты могут иметь любое имя (за исключением зарезервированных).

Пример: 

	<image file="button.png" anim-delay = "800" offset-x = "200"/>

Доступ к атрибуту:

	ResAnim *rs = resources.getResAnim("button");
	int time = rs->getAttribute("anim-delay").as_int(0);
	int offset = rs->getAttribute("offset-x").as_int(0);

> You could set user data to any type of resource (not only image).


##< Starling > (Sparrow) atlas format

Oxygine supports atlas format exported for Starling/Sparrow framework. 

Declare it in the resources xml:
 
	<starling file="atlas.xml" />


Then use as usual resource:

	ResAnim *rs = resources.getResAnim("image");

