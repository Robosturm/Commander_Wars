#HttpRequest
Oxygine supports GET and POST HTTP calls.

In order to make HTTP requests, you must first initialize the subsystem:
```cpp
	HttpRequestTask::init();
```
And release it upon exiting:
```cpp
	HttpRequestTask::release();
```
The request is made with the help of the following method:
```cpp
	spHttpRequestTask request = HttpRequestTask::create();
```
Class HttpRequestTask is an inheritor of the class EventDispatcher and supports 3 types of events:

* ERROR - called when a request error occurs
* PROGRESS - called every time upon receiving new data packets from the server
* COMPLETE - called upon a successful request

> all events are called in the main game thread

Example of a request:
```cpp
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
```


By default, the request is of the GET type. If you wish to make a POST request, then you must call the method *setPostData*, parsing into it the body of your POST request:
```cpp
	request->setPostData(mydata);
```

##File Downloads

HttpRequestTask can load resources straight into files. To do this, call the method *setFileName*.

> in this case getResponse will return an empty buffer

If a file with the same name already exists, then it will be deleted. If you are trying to point to a file within a folder, then you must manually create the required folder.


Example:
```cpp
	spHttpRequestTask task = HttpRequestTask::create();
	task->setUrl("http://oxygine.org/emscripten/MPHello.js.gz");
	task->setFileName("somefile.abc");
	task->run();
```
Upon completion of the request, the root folder will contain a file named *somefile.abc*



##Image Loading


The class WebImage, an heir of the class Sprite, can load images into itself using HTTP. The loading is asynchronous.
```cpp
	#include "WebImage.h"

	void TestHttp::someFunc()
	{
		spWebImage image = new WebImage;
		image->load("http://oxygine.org/img/madewith.png");		
		image->setPosition(300, 300);		
		image->attachTo(getStage());
	}
```

##Android

You need these permissions for Android:

```xml
    <uses-permission android:name="android.permission.INTERNET" /> <!--used for http requests demo-->    
    <uses-permission android:name="android.permission.WAKE_LOCK" /><!--used for http requests demo (downloading files)-->
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE"/><!--used for Proxy detection, could be disabled-->    
```