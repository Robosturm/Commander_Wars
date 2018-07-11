#include "oxygine-framework.h"
#include "oxygine-flow.h"

using namespace oxygine;

class MyDialog : public flow::Scene
{

public:
    MyDialog()
    {
        setName("MyDialog");

        _dialog = true;

        spBox9Sprite view = new Box9Sprite;
        _view = view;
        _view->setSize(400, 300);
        _view->attachTo(_holder);
        _view->setPosition(_holder->getSize() / 2 - _view->getSize() / 2);

        spButton btn = new Button;
        btn->setPosition(120, 200);
        btn->attachTo(_view);

        btn->addEventListener(TouchEvent::CLICK, [ = ](Event*)
        {
            finish();
        });

        flow::TransitionMove::assign(this);

        addEventListener(EVENT_PRE_SHOWING, [ = ](Event*)
        {
            view->setColor(Color(rand() % 255, rand() % 255, rand() % 255, 255));
        });

        addEventListener(EVENT_POST_SHOWING, [ = ](Event*)
        {
            view->addTween(Sprite::TweenColor(Color::White), 300);
        });

        addEventListener(EVENT_BACK, [ = ](Event*)
        {
            finish();
        });
    }

    spActor _view;
};

class MyScene : public flow::Scene
{
public:
    MyScene()
    {
        setName("MyScene");

        _view = new Box9Sprite;
        _view->setSize(_holder->getSize());
        _view->attachTo(_holder);

        spButton btn = new Button;
        btn->setPosition(_view->getSize() / 2 - btn->getSize() / 2);
        btn->attachTo(_view);

        btn->addEventListener(TouchEvent::CLICK, [ = ](Event*)
        {
            flow::show(new MyDialog, [ = ](Event*)
            {
                //finish();
                logs::messageln("dialog closed");
            });
        });
    }

    spActor _view;
};


void example_preinit()
{

}

void example_init()
{
    //initialize oxygine_flow
    flow::init();

    //create scene and display it
    flow::show(new MyScene, [](Event * event)
    {
        logs::messageln("scene closed");
    });
}

void example_update()
{
    //update oxygine-flow each frame
    flow::update();
}

void example_destroy()
{
    //free oxygine-flow
    flow::free();
}