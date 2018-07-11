#Sounds

[**oxygine-sound**](https://github.com/oxygine/oxygine-sound) is extension for oxygine-framework. It is based on OpenAL library and could be used for playing ogg files.

It supports streaming for long sound tracks and using separate thread for decoding and reading ogg files.
 
##Usage

Include headers:
```cpp
#include "SoundPlayer.h"
#include "SoundSystem.h"
#include "SoundInstance.h"
```

Initialize sound system with 16 channels:
```cpp	
SoundSystem::create();
SoundSystem::get()->init(16);

SoundPlayer::initialize();
```
*SoundPlayer::initialize* registers new type of resource "sound".


Add your ogg tracks to resources file, lets call it "sounds.xml":

```xml
<?xml version="1.0"?>
<resources>
	<set path = "sounds" />	
	
 	<sound file="sfx1.ogg" />
	<sound file="sfx2.ogg" />
	<sound file="voice1.ogg"/>
	<sound file="music.ogg" streaming="true"/>
	
</resources>
```
> you could	have multiple resources files

* If sound is too short if would be automaticaly decoded to buffer when loading. 
* If sound is longer than about 5 seconds it would be decoded step by step from memory when playing.
* **streaming=true** attribute means that sound would be loaded from file and decoded on the fly when playing.   


Load resources:
```cpp
Resources resources;
resources.loadXML("sounds.xml");
```
Next step is declaring SoundPlayer instance:
```cpp
SoundPlayer sfxPlayer;
```
SoundPlayer is a container for group of sounds playing right now. You could declare multiple instances of SoundPlayer and control them individually:
```cpp
SoundPlayer sfxPlayer;
SoundPlayer musicPlayer;

...

sfxPlayer.setVolume(1.0f);
musicPlayer.setVolume(0.5f);
```

Update SoundSystem and SoundPlayer instances each frame:
```cpp
void example_update()
{
	SoundSystem::instance->update();
	sfxPlayer.update();
	musicPlayer.update();
}
```

Play single sound:
```cpp	
sfxPlayer.play(resources.get("sfx1"));
```
Play looped music with fade in 1.5 seconds:
```cpp
musicPlayer.play(resources.get("music"), PlayOptions().loop().fade(1500));
```
You could save handle to playing sound instance and use it for control:
```cpp
spSoundInstance music = musicPlayer.play(resources.get("music"));	

void levelCompleted()
{
	music->fadeOut();
}
```
Or you could control group of sounds belonging to SoundPlayer:
```cpp
sfxPlayer.pause();
...
sfxPlayer.stop();
```

You could subscribe to events:
```cpp
spSoundInstance music = musicPlayer.play(resources.get("music"));
music->addEventListener(SoundInstance::SoundEvent::EVENT_DONE, CLOSURE(this, &SomeClass::musicDone));

void SomeClass::musicDone(Event*)
{
	musicPlayer.play(resources.get("music2"));
} 
```


Don't forget to stop sounds and release SoundSystem when exit:
```cpp
sfxPlayer.stop();
musicPlayer.stop();

SoundPlayer::free();
SoundSystem::free();
```