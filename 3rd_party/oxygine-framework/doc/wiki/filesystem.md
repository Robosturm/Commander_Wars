#File System
Oxygine provides cross-platform API for working with files: creating, reading, writing. It is recommended to use it instead of std API (fopen/fwrite/CreateFile etc).

##Typical usage example 
Read file, modify it and save back:
```cpp	
#include "core/file.h" 

void test()
{
	oxygine::file::buffer data;

	//read entire file into buffer with stdio flags "rb" (Read Binary)
	oxygine::file::read("user", data);
	
	//modify buffer
	data[0] += 1
	data[1] += 2
	data.push_back(3);
	
	//write file back from buffer with stdio flags "wb" (Write Binary)
	oxygine::file::write("user", data);
}
```	

**oxygine::file::buffer** is intermediate buffer for storing data when working with files. It uses *std::vector < unsigned char > * internally.

 
##Low level
Namespace **oxygine::file** has also low level functions to working with files.
	
```cpp

oxygine::file::handle h = oxygine::file::open("user", "wb");
oxygine::file::write(h, "Hello World", strlen("Hello World"));	
oxygine::file::close(h);
```
> **Oxygine supports only binary file operations, you always should use 'b' flag when opening file**

##Working with missing files

Imagine you want to read file and work with loaded data. But you not sure that file is exits:
```cpp
file::read("user", data);
//user function
load_user(data);
```
If file "user" is missing you would see error:

	>> error: can't open file: user
	>> error: Assert! 941 d:\oxygine-framework\oxygine\src\core\oxygine.cpp
	>> Assertion failed!

To avoid error you should pass additional argument:
```cpp
if(file::read("user", data, ep_ignore_error))
	load_user(data);
```
This code will show warning if file is missing:
```cpp
if(file::read("user", data, ep_show_warning))
	load_user(data);
```

>> warning: can't open file: user
	

In examples above third argument is **error_policy** enum. It has 3 values:

* **ep_show_error**  is default and most strict value. Shows assert and prints error to log.
* **ep_show_warning** prints warning to log.
* **ep_ignore_error** does nothing.


Or you could check it with **oxygine::file::exists** function. It is slower than solutions above:
```cpp
if (oxygine::file::exists("user"))
{
	file::read("user", data);
	//user function
	load_user(data);
}
```


##Mounting zip archieves to filesytem

ZipFileSystem is limited and works in a read-only mode.

First step is mount ZipFileSystem. More than one archive could be mounted at once:
```cpp
#include "core/ZipFileSystem.h"

oxygine::file::ZipFileSystem zp;

void mount()
{		
	zp.add("pack1.zip")
	zp.add("pack2.zip")
	zp.add("pack3.zip")
	
	oxygine::file::mount(&zp);		
}
```
Read any data from file as usual:
```cpp
void read_packs()
{
	oxygine::file::buffer data;
	oxygine::file::read("anyfilefromzip", data);
}
```

> ZipFileSystem is read only.

ZipFileSystem can work with files in a streaming/random access mode and seek, but with a few limitations:

- The file which you wish to open inside the zip archive must be packed without compression
- You must open the file in "srb" mode:

```cpp
void open_file_if_you_need_random_access_or_streaming()
{
	oxygine::file::handle h = oxygine::file::open("streamingfilefromzip", "srb");

	oxygine::file::seek(h, 0, SEEK_END);
	unsigned int size = oxygine::file::tell(h);
	oxygine::file::seek(h, 0, SEEK_SET);
	....
	oxygine::file::close(h);
}
```

- If you open the file in "rb" mode instead, you will get an error when you attempt to call file::seek or upon opening multiple files simultaneously.
- Because of this, it is recommended that you instead read files using file::read directly into file::buffer.


##Working with folders

Create folder:

	oxygine::file::makeDirectory("name");

Delete:

	oxygine::file::deleteDirectory("name");


##XML

[Pugixml](https://github.com/zeux/pugixml) is included to Oxygine.
```cpp
	#include "pugixml/pugixml.hpp"

	//load file to buffer
	file::buffer bf;
	file::read("file.xml", bf);

	pugi::xml_document doc;

	//parse xml document from buffer
	doc.load_buffer(&bf.data[0], bf.size());
```

##JSON

[JsonCpp](https://github.com/open-source-parsers/jsoncpp) is included to Oxygine.
```cpp
	#include "json/json.h"

	//load file to buffer
	file::buffer bf;
	file::read("file.json", bf);

	//parse
	Json::Reader reader;
	Json::Value value;
	reader.parse((char*)&bf.front(), (char*)&bf.front() + bf.size(), value, false);

	//add some data
	value["score"] = 123;

	//save it back to file
	Json::FastWriter writer;
	string s = writer.write(value);
	file::write("file.json", s.c_str(), s.size());
```

##Some notes
You should access to filesystem after Oxygine core initialization:
```cpp
oxygine::core::init(..)
```

If you require access to the file system before the oxygine::core::init call (for example, if you wish to read a config file), then use the following:
```cpp
oxygine::file::init(...);
```