#HttpRequest
в Oxygine есть базовая поддержка GET и POST HTTP запросов.

Перед выполнением HTTP запросов необходимо инициализировать подсистему:

	HttpRequestTask::init();

А при выходе освободить:

	HttpRequestTask::release();

Запрос создается с помощью метода:

	spHttpRequestTask request = HttpRequestTask::create();

Класс HttpRequestTask является наследником класса EventDispatcher и поддерживает 3 типа событий: 

* ERROR - срабатывает при ошибке выполнения запроса
* PROGRESS - вызывается каждый раз, при получении новой порции данных с сервера
* COMPLETE - срабатывает при успешном завершении запроса 

> все события вызываются в основном игровом потоке

Пример выполнения запроса:

	#include "HttpRequestTask.h"

	void TestHttp::someFunc()
	{
		spHttpRequestTask task = HttpRequestTask::create();
		task->setUrl("http://nist.time.gov/actualtime.cgi");
		task->addEventListener(HttpRequestTask::COMPLETE, CLOSURE(this, &TestHttp::dateTimeLoaded));
		task->run();
	}

	void TestHttp::dateTimeLoaded(Event*)
	{
		HttpRequestTask* task = safeCast<HttpRequestTask*>(event->currentTarget.get());

		const std::vector<unsigned char> &response = task->getResponse();

		//convert it to string and print
		string resp;
		resp.assign(response.begin(), response.end());
		
		log::messageln("server response: %s", resp.c_str()); 
	}



По умолчанию, запрос имеет тип GET, если вы хотите сделать POST запрос, то необходимо вызвать метод *setPostData*, передав в него тело своего POST запроса:

	request->setPostData(mydata);


##Скачивание файлов

HttpRequestTask умеет загружать данные напрямую в файл, для этого надо вызвать метод *setFileName*. 

> в таком случае getResponse будет возвращать пустой буфер

Если файл с таким же именем уже существует, то он будет удален при запуске запроса. Если вы указываете файл, вложенный в папку, то данную папку необходимо создать самостоятельно.


Пример:

	spHttpRequestTask task = HttpRequestTask::create();
	task->setUrl("http://oxygine.org/emscripten/MPHello.js.gz");
	task->setFileName("somefile.abc");
	task->run();

По завершению запроса в корне появится файл с именем *somefile.abc* 



##Загрузка изображений

Класс WebImage унаследован от Sprite и умеет напрямую загружать в себя картинки доступные по HTTP. Загрузка выполняется асинхронно.

	#include "WebImage.h"

	void TestHttp::someFunc()
	{
		spWebImage image = new WebImage;
		image->load("http://oxygine.org/img/madewith.png");		
		image->setPosition(300, 300);		
		image->attachTo(getStage());
	}
