#Movie

[**oxygine-movie**](https://github.com/oxygine/oxygine-movie) это расширение для oxygine, позволяющее проигрывать видео  с поддержкой альфаканала. Используется кодек [Theora](http://www.theora.org/).

> Theora is a free and open video compression format from the Xiph.org Foundation. Like all our multimedia technology it can be used to distribute film and video online and on disc without the licensing and royalty fees or vendor lock-in associated with other formats.

**oxygine-movie** зависит от **oxygine-sound**, потому он тоже должен быть установлен и подключен к приложению.

Основной класс, с которым вы будете иметь дело - это **MovieSprite**, унаследованный от **Sprite**. Вы можете иметь их неограниченное количество на сцене, но нужно учитывать нагрузку на CPU. Декодирование производится в отдельном потоке. 

**oxygine-movie** поддерживает альфаканал, который должен быть сохранет в видео таким образом:
![movie with alpha](img/movie_alpha.png)

[Готовый пример](http://www.youtube.com/watch?v=d-Wf0krGoRU) есть в папке: oxygine-movie\examples\Movie\

##Usage

Include headers:

	#include "MovieSprite.h"

Инициализируем подсистему один раз перед использованием:

	MovieSprite::init();

Создаем спрайт с видео и прицепляем к сцене:

	spMovieSprite movie = MovieSprite::create();
	movie->attachTo(getStage());
	
Устанавливаем путь к видеофайлу:

	movie->setMovie("path/to/movie.ogv");

А если в видео есть альфаканал, то:

	movie->setMovie("path/to/movie.ogv", true);

Если вы хотите зациклить видео, до делайте это до вызова Play:

	movie->setLooped(true);	

Проигрываем видео:

	movie->play();	

Видео можно приостановить:

	movie->pause();

Остановить полностью:	

	movie->stop();

Можно подписаться на событие о завершении видео:

	movie->addEventListener(MovieSprite::COMPLETE, CLOSURE(this, &SomeClass::movieDone));

	void SomeClass::movieDone(Event*)
	{
        log::messageln("movie done");
	} 


Перед выходом из приложение не забудьте удалить подсистему:

    MovieSprite::free();