#pragma once
#include "test.h"
#include "oxygine-sound.h"


extern SoundPlayer splayer;

class TestSoundInstance : public Test
{
public:

    spSprite volumeSprite;
    spNativeTexture volumeTexture;
    Image volumeImage;

    spSprite posSprite;
    spNativeTexture posTexture;
    Image posImage;

    spSoundInstance snd;
    spTextField txtState;




    TestSoundInstance()
    {
        //snd = splayer.play("track_48000_stereo");


        //snd = splayer.play("track_44100_mono", PlayOptions().loop(false));
        addButton("set_streaming", "set streaming sound");
        addButton("set_static", "set static sound");

        addButton("loop=1", "loop = true");
        //addButton("loop=0", "loop = false");
        addButton("pitch=2", "pitch = 2");
        //addButton("pitch=1", "pitch = 1");
        //addButton("set_not_looped", )

        addButton("play", "play");
        addButton("pause", "pause");
        addButton("resume", "resume");
        addButton("stop", "stop");

        addButton("fadeIn500", "fade in 500");
        addButton("fadeIn1000", "fade in 1000");
        addButton("fadeOutPause500", "fade out pause 500");
        addButton("fadeOutPause1000", "fade out pause 1000");
        addButton("fadeOut", "fade out");



        volumeImage.init((int)getStage()->getWidth(), 50, TF_R8G8B8A8);
        volumeImage.fill(Color(Color::Gray).rgba());

        volumeTexture = IVideoDriver::instance->createTexture();
        volumeTexture->init(volumeImage.lock());


        spTextField label;
        label = new TextField;
        label->setText("Volume:");
        label->setFont(Test::_resources.getResFont("main"));
        label->setX(10);
        label->setY(_content->getHeight() / 2 - 80);
        label->attachTo(_content);
        volumeSprite = new Sprite;
        volumeSprite->setAnimFrame(AnimationFrame(volumeTexture));
        volumeSprite->attachTo(_content);
        volumeSprite->setY(label->getY() + 10);



        posImage.init((int)getStage()->getWidth(), 100, TF_R8G8B8A8);
        posImage.fill(Color(Color::Gray).rgba());

        posTexture = IVideoDriver::instance->createTexture();
        posTexture->init(posImage.lock());

        label = new TextField;
        label->setText("Position:");
        label->setFont(Test::_resources.getResFont("main"));
        label->setX(10);
        label->setY(_content->getHeight() / 2);
        label->attachTo(_content);
        posSprite = new Sprite;
        posSprite->setAnimFrame(AnimationFrame(posTexture));
        posSprite->attachTo(_content);
        posSprite->setY(label->getY() + 10);


        label = new TextField;
        label->setFont(Test::_resources.getResFont("main"));
        label->setX(10);
        label->setY(_content->getHeight() / 2 + 110);
        label->attachTo(_content);
        txtState = label;

    }

    void soundEvent(Event*)
    {
        notify("done");
        // snd = 0;
    }

    void set(spSoundInstance s)
    {
        if (snd)
            snd->stop();
        snd = s;
        s->setDoneCallback(CLOSURE(this, &TestSoundInstance::soundEvent));
        s->seek(10000);
    }

    void clicked(string id)
    {
        if (id == "set_streaming")
            set(splayer.play("wave", PlayOptions().pause()));

        if (id == "set_static")
            set(splayer.play("win_round", PlayOptions().pause()));

        if (!snd)
            return;

        if (id == "loop=1")
            snd->setLoop(true);

        if (id == "loop=0")
            snd->setLoop(false);

        if (id == "pitch=2")
            snd->setPitch(2);

        if (id == "pitch=1")
            snd->setPitch(1);

        if (id == "play")
            snd->play();

        if (id == "pause")
            snd->pause();

        if (id == "resume")
            snd->resume();

        if (id == "stop")
            snd->stop();

        if (id == "fadeIn500")
            snd->fadeIn(500);

        if (id == "fadeIn1000")
            snd->fadeIn(1000);

        if (id == "fadeOut")
            snd->fadeOut(500);

        if (id == "fadeOutPause500")
            snd->fadeOutPause(500);
        if (id == "fadeOutPause1000")
            snd->fadeOutPause(1000);
    }


    void updateTexture(Image& image, spNativeTexture t)
    {
        ImageData im = image.lock();

        ImageData src = im.getRect(Rect(1, 0, im.w - 1, im.h));
        ImageData dest = im.getRect(Rect(0, 0, im.w - 1, im.h));

        operations::move(src, dest);

        t->updateRegion(0, 0, im);
    }

    void doUpdate(const UpdateState& us)
    {
        if (snd)
        {
            ImageData im;
            int y;

            unsigned char* ptr;
            PixelR8G8B8A8 p;


            im = volumeImage.lock();
            float vol = snd->getCurrentVolume();
            y = int(im.h - vol * (im.h - 5) - 2);
            ptr = im.getPixelPtr(im.w / 2, y);
            p.setPixel(ptr, initPixel(Color(Color::Green).rgba()));



            im = posImage.lock();
            float progress = snd->getPosition() / (float)snd->getDuration();
            y = int(im.h - progress * (im.h - 5) - 2);
            ptr = im.getPixelPtr(im.w / 2, y);
            p.setPixel(ptr, initPixel(Color(Color::Yellow).rgba()));

            updateTexture(volumeImage, volumeTexture);
            updateTexture(posImage, posTexture);

            SoundInstance::State s = snd->getState();
            string st;
            switch (s)
            {
                case SoundInstance::FadingIn:
                    st = "FadingIn";
                    break;
                case SoundInstance::FadingOutStop:
                    st = "FadingOutStop";
                    break;
                case SoundInstance::FadingOutPause:
                    st = "FadingOutPause";
                    break;
                case SoundInstance::Normal:
                    st = "Normal";
                    break;
                case SoundInstance::Paused:
                    st = "Paused";
                    break;
                case SoundInstance::Stopped:
                    st = "Stopped";
                    break;
                case SoundInstance::Ended:
                    st = "Ended";
                    break;
                default:
                    break;
            }

            txtState->setText(st);
        }
        else
            txtState->setText("");
    }
};
