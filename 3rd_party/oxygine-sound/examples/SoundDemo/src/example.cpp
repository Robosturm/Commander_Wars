#include "oxygine-framework.h"
#include "oxygine-sound.h"

#include "test.h"
#include "TestSoundFormats.h"
#include "TestStress.h"
#include "TestCoord.h"
#include "TestSoundInstance.h"

using namespace oxygine;


//it is our resources
//in real project you would have more than one Resources declarations. It is important on mobile devices with limited memory and you would load/unload them
Resources resources;
SoundPlayer splayer;

class TestActor: public Test
{
public:

    TestActor()
    {
        _x = getStage()->getWidth() / 2.0f;
        _y = 0;


        addButton("sound_formats", "sound formats");
        addButton("stress_test", "stress test");
        addButton("test_coord", "test coord");
        addButton("test_si", "test sound instance");
        addButton("exit", "exit");

        getStage()->addEventListener(Stage::ACTIVATE, CLOSURE(this, &TestActor::resume));
        getStage()->addEventListener(Stage::DEACTIVATE, CLOSURE(this, &TestActor::pause));

        //clicked("test_si");
    }

    void pause(Event*)
    {
        SoundSystem::get()->pause();
    }

    void resume(Event*)
    {
        SoundSystem::get()->resume();
    }

    void clicked(string id)
    {
        setVisible(false);

        if (id == "sound_formats")
        {
            getStage()->addChild(new TestSoundFormats);
        }

        if (id == "stress_test")
        {
            getStage()->addChild(new TestStress);
        }

        if (id == "test_coord")
        {
            getStage()->addChild(new TestCoord);
        }

        if (id == "test_si")
        {
            getStage()->addChild(new TestSoundInstance);
        }

        if (id == "exit")
        {
            core::requestQuit();
        }
    }
};

void example_preinit()
{

}

void example_init()
{
    //initialize our sound system with 16 channels

    SoundSystem::create()->init(16);

    //initialize SoundPlayer
    SoundPlayer::initialize();
    splayer.setResources(&resources);
    splayer.setVolume(0.5f);

    //load xml file with resources definition
    resources.loadXML("xmls/res.xml");

    Test::init();

    Test::instance = new TestActor;
    getStage()->addChild(Test::instance);


#ifdef EMSCRIPTEN
    emscStartSoundsPreloading(resources);
#endif
}

void example_update()
{
    SoundSystem::get()->update();
    splayer.update();
}

void example_destroy()
{
    splayer.stop();
    resources.free();

    SoundPlayer::free();
    SoundSystem::free();

    Test::free();
}


