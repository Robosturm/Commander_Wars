#pragma once
#include "oxygine-framework.h"
using namespace oxygine;
using namespace std;

void example_init();
void example_preinit();
void example_destroy();
void example_update();

extern Resources gameResources;

const int VirtualWidth = 960;
const int VirtualHeight = 640;

DECLARE_SMART(MySceneA, spMySceneA);
DECLARE_SMART(MySceneB, spMySceneB);
DECLARE_SMART(MyDialog, spMyDialog);

extern spMySceneA SceneA;
extern spMySceneB SceneB;
extern spMyDialog dialogA;

spButton createButton(const string& name, const string& txt);