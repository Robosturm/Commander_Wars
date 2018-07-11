#include "oxygine-framework.h"
#include <functional>
#include "ResFontFT.h"
#include "test.h"

using namespace oxygine;

//it is our resources
//in real project you would have more than one Resources declarations.
//It is important on mobile devices with limited memory and you would load/unload them
Resources gameResources;

enum TextMode
{
    tm_no_shadow = 0,
    tm_shadow = 1,
    tm_shadow2 = 2,
};

void example_preinit() {}

void myShadowsFilter(ResFontFT::postProcessData& data)
{
    Image& destIm = *data.dest;
    ImageData& src = *data.src;

    ImageData rc;


    Image tempImage;
    tempImage.init(src.w, src.h, TF_R8G8B8A8);

    rc = tempImage.lock();
    operations::blitPremultiply(src, rc);

    src = rc;

    if (data.opt == tm_no_shadow)
    {
        //if shadows disabled
        destIm.swap(tempImage);
        return;
    }


    const int xoffset = 4;
    const int yoffset = 3;

    //initialize destination Image with increased size
    destIm.init(src.w + xoffset, src.h + yoffset, TF_R8G8B8A8);
    //clear it
    destIm.fillZero();


    //copy black image as shadow
    rc = destIm.lock(Rect(xoffset, yoffset, src.w, src.h));

    Color shadowColor = data.opt == tm_shadow ? Color(0, 0, 0, 255) : Color(255, 0, 0, 255);
    operations::blitColored(src, rc, shadowColor);

    //copy original image
    operations::op_blend_one_invSrcAlpha op;
    rc = destIm.lock(Rect(0, 0, src.w, src.h));
    operations::applyOperation(op, src, rc);
}



class TestActor : public Test
{
public:

    spTextField _text;
    TestActor()
    {

        addButton("scale+", "scale+");
        addButton("scale-", "scale-");

        //create TextField Actor
        _text = new TextField();
        //attach it as child to button
        _text->attachTo(content);
        //centered
        //_text->setX(content->getWidth() / 4);
        _text->setY(content->getHeight() / 2);

        //initialize text style
        TextStyle style;
        style.font = gameResources.getResFont("main");
        style.fontSize = 80;

        style.color = Color::Crimson;
        style.vAlign = TextStyle::VALIGN_BOTTOM;
        style.hAlign = TextStyle::HALIGN_LEFT;
        style.baselineScale = 0.7f;

        //apply our custom option
        style.options = tm_shadow;

        _text->setStyle(style);
    }

    void doUpdate(const UpdateState& us)
    {
        char txt[255];
        safe_sprintf(txt, " <div opt='2'>tm:</div> %d", getTimeMS());
        _text->setHtmlText(txt);
    }
    void clicked(string id)
    {
        if (id == "scale+")
        {
            _text->setScale(_text->getScaleX() + 0.05f);
        }
        if (id == "scale-")
        {
            _text->setScale(_text->getScaleX() - 0.05f);
        }

    }
};


//called from entry_point.cpp
void example_init()
{
    ResFontFT::initLibrary();

    //use it for adding shadows
    ResFontFT::setGlyphPostProcessor(myShadowsFilter);

    //load xml file with resources definition
    gameResources.loadXML("res.xml");


    Test::init();

    Test::instance = new TestActor;
    getStage()->addChild(Test::instance);
}


//called each frame from entry_point.cpp
void example_update()
{
}

//called each frame from entry_point.cpp
void example_destroy()
{
    //free previously loaded resources
    gameResources.free();
    ResFontFT::freeLibrary();
    Test::free();
}
