#include <QDir>
#include <QFile>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "menue/generatormenu.h"
#include "menue/mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/audiomanager.h"

#include "ui_reader/uifactory.h"

GeneratorMenu::GeneratorMenu(const QString & generatorUi)
{
#ifdef GRAPHICSUPPORT
    setObjectName("GeneratorMenu");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/generatorMenu");
    pApp->getAudioManager()->playRandom();

    UiFactory::getInstance().createUi(generatorUi, this);
}

void GeneratorMenu::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "generatorMenu";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, GameConsole::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
        pInterpreter->doFunction(object, func, args);
    }
}

void GeneratorMenu::exitMenue(const QString & mainMenuXml)
{
    auto window = spMainwindow::create(mainMenuXml);
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

bool GeneratorMenu::loadVariablesFromFile(const QString & file)
{
    if (QFile::exists(file))
    {
        QFile fileObj(file);
        QDataStream stream(&fileObj);
        fileObj.open(QIODevice::ReadOnly);
        m_Variables.deserializeObject(stream);
        fileObj.close();
        return true;
    }
    return false;
}

void GeneratorMenu::writeVariablesToFile(const QString & file) const
{
    createDir(file);
    QFile fileObj(file);
    QDataStream stream(&fileObj);
    fileObj.open(QIODevice::WriteOnly);
    m_Variables.serializeObject(stream);
    fileObj.close();
}

void GeneratorMenu::writeDataToFile(const QString & file, const QString & data) const
{
    createDir(file);
    QFile fileObj(file);
    QDataStream stream(&fileObj);
    fileObj.open(QIODevice::WriteOnly);
    stream << data;
    fileObj.close();
}

void GeneratorMenu::createDir(const QString & path) const
{
    QFileInfo info(path);
    info.absoluteDir().mkpath(".");
}

void GeneratorMenu::copySourceToTarget(const QString & source, const QString & target)
{
    createDir(target);
    QFile::copy(source, target);
}
