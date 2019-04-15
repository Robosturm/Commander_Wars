#include "coreai.h"

#include "coreengine/mainapp.h"

#include "game/gameanimationfactory.h"

#include "game/gamemap.h"

#include "menue/gamemenue.h"

CoreAI::CoreAI()
{
    Interpreter::setCppOwnerShip(this);
    // move signals and slots to Audio Thread
    this->moveToThread(this);

}

CoreAI::~CoreAI()
{
    terminate();
    wait();
}

void CoreAI::init()
{
    connect(GameAnimationFactory::getInstance(), &GameAnimationFactory::animationsFinished, this, &CoreAI::nextAction, Qt::QueuedConnection);
    connect(this, &CoreAI::performAction, GameMenue::getInstance(), &GameMenue::performAction, Qt::QueuedConnection);
    start();
}

void CoreAI::nextAction()
{
    // check if it's our turn
    if (m_pPlayer == GameMap::getInstance()->getCurrentPlayer())
    {
        // if so execute next action
        process();
    }
}

void CoreAI::run()
{
    while (!finish)
    {
        exec();
    }
}
