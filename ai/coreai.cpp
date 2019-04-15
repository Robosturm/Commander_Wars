#include "coreai.h"

#include "coreengine/mainapp.h"

#include "game/gameanimationfactory.h"

#include "game/gamemap.h"

#include "menue/gamemenue.h"

const QString CoreAI::ACTION_CAPTURE = "ACTION_CAPTURE";
const QString CoreAI::ACTION_FIRE = "ACTION_FIRE";
const QString CoreAI::ACTION_NEXT_PLAYER = "ACTION_NEXT_PLAYER";

CoreAI::CoreAI()
{
    Interpreter::setCppOwnerShip(this);
}

CoreAI::~CoreAI()
{
}

void CoreAI::init()
{
    connect(GameAnimationFactory::getInstance(), &GameAnimationFactory::animationsFinished, this, &CoreAI::nextAction, Qt::QueuedConnection);
    connect(this, &CoreAI::performAction, GameMenue::getInstance(), &GameMenue::performAction, Qt::QueuedConnection);
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
