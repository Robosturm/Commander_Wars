#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include <QDir>
#include <QClipboard>
#include <QGuiApplication>
#include <qlogging.h>

#include "coreengine/console.h"
#include "coreengine/mainapp.h"
#include "coreengine/settings.h"
#include "coreengine/audiothread.h"
#include "coreengine/globalutils.h"
#include "resource_management/fontmanager.h"

#include "menue/gamemenue.h"

#include "network/NetworkInterface.h"

#include "spritingsupport/spritecreator.h"

#include "oxygine/KeyEvent.h"

// values which differ from release to debug build
#ifdef GAMEDEBUG
Console::eLogLevels Console::LogLevel = Console::eDEBUG;
#else
Console::eLogLevels Console::LogLevel = Console::eINFO;
#endif

bool Console::show = false;
bool Console::toggled = false;
QList<QString> Console::output;
Console* Console::m_pConsole = nullptr;
QString Console::curmsg = nullptr;
qint32 Console::curmsgpos = 0;
QElapsedTimer Console::toggle;
qint32 Console::curlastmsgpos = 0;
QList<QString> Console::lastmsgs;
qint32 Console::outputSize = 100;
QMutex Console::datalocker;

// Console Libary
const QString Console::functions[] =
{
    QString("createfunnymessage"),
    QString("setVolume"),
    QString("setLogLevel"),
    QString("connectToServer"),
    QString("getServerAdresse"),
    QString("createSprites"),
    QString("createColorTableSprites"),
    QString("applyImagesTable"),
    QString("updateMaskImages"),
    QString("inversImagesFrames"),
    QString("extendMaskImages"),
    QString("help"),
    QString("logActions"),
    QString("")
};

Console::Console()
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    // move console to top
    oxygine::Actor::setPriority(static_cast<qint32>(Mainapp::ZOrder::Console));
    m_pBackgroundsprite = new oxygine::ColorRectSprite();
    m_pBackgroundsprite->setPosition(0, 0);
    m_pBackgroundsprite->setSize(Settings::getWidth(), Settings::getHeight());
    m_pBackgroundsprite->attachTo(this);
    m_pBackgroundsprite->setColor(QColor(0,0,0, 180));

    m_text = new oxygine::TextField();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont16()).withColor(QColor(255,127,39)).alignLeft();
    m_text->setStyle(style);

    m_text->attachTo(this);
    // we're hidden to begin with
    oxygine::Actor::setVisible(false);
}

Console* Console::getInstance()
{
    if (m_pConsole == nullptr)
    {
        m_pConsole = new Console();
    }
    return m_pConsole;
}

void Console::init()
{
    toggle.start();

    Mainapp* pMainapp = Mainapp::getInstance();
    connect(pMainapp, &Mainapp::sigConsoleKeyDown, m_pConsole, &Console::KeyInput, Qt::QueuedConnection);
    // Print some Info

    Console::print("Enter \"help()\" for console info.", Console::eLogLevels::eINFO);
    Console::print("Starting Game...", Console::eLogLevels::eINFO);
    Console::print("Prepare to Fight...", Console::eLogLevels::eINFO);
    Console::print("Read this message while waiting...", Console::eLogLevels::eINFO);
    Console::print("", Console::eLogLevels::eINFO);
    Console::createfunnymessage();
    Console::print("", Console::eLogLevels::eINFO);
}

void Console::createSprites(QString input, QString colorTable, QString maskTable)
{
    SpriteCreator::createSprites(input, colorTable, maskTable);
}

void Console::createColorTableSprites(QString folder, QString filter, qint32 startIndex, qint32 maxColors)
{
    SpriteCreator::createColorTableSprites(folder, filter, startIndex, maxColors);
}

void Console::updateMaskImages(QString folder, QString filter, qint32 min)
{
    SpriteCreator::updateMaskImages(folder, filter, min);
}

void Console::inversImagesFrames(QString folder, QString filter, qint32 frames)
{
    SpriteCreator::inversImagesFrames(folder, filter, frames);
}

void Console::applyImagesTable(QString input, QString inTable, QString outTable, QColor eraseColor)
{
    SpriteCreator::applyImagesTable(input, inTable, outTable, eraseColor);
}

void Console::extendMaskImages(QString folder, QString filter)
{
    SpriteCreator::extendMaskImages(folder, filter);

}

void Console::dotask(QString message)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    print(message, Console::eINFO);
    QString order = "GameConsole." + message;
    // ignore console argument and evaluate the String on the Top-Level
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (message.startsWith("game:") &&
        pGameMenue.get() != nullptr &&
        !pGameMenue->isNetworkGame())
    {
        order = order.replace("GameConsole.game:", "");
        pInterpreter->doString(order);
    }
    else
    {
        for (const auto& function : functions)
        {
            if (message.startsWith(function + "("))
            {
                pInterpreter->doString(order);
                break;
            }
        }
    }
}

void Console::print(QString message, qint8 LogLevel)
{
    print(message, static_cast<eLogLevels>(LogLevel));}

void Console::print(QString message, eLogLevels MsgLogLevel)
{
    QMutexLocker locker(&datalocker);

    if (MsgLogLevel >= Console::LogLevel)
    {
        QString msg = tr(message.toStdString().c_str());
        QString prefix = "";
        switch (MsgLogLevel)
        {
            case eDEBUG:
            {
                qDebug(msg.toStdString().c_str());
                prefix = "DEBUG: ";
                break;
            }
            case eINFO:
            {
                qInfo(msg.toStdString().c_str());
                prefix = "INFO: ";
                break;
            }
            case eWARNING:
            {
                qWarning(msg.toStdString().c_str());
                prefix = "WARNING: ";
                break;
            }
            case eERROR:
            {
                qCritical(msg.toStdString().c_str());
                prefix = "CRITICAL: ";
                break;
            }
            case eFATAL:
            {
                prefix = "FATAL: ";
                qFatal(msg.toStdString().c_str());
                break;
            }
            default:
                break;
        }
        output.append(prefix + msg);
        while (output.size() > outputSize)
        {
            output.removeFirst();
        }
    }
}

void Console::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    if(show)
    {

        QMutexLocker locker(&datalocker);
        qint32 screenheight = Settings::getHeight();
        qint32 h = FontManager::getMainFont16()->getSize();
        // pre calc message start
        qint32 num = screenheight / h - 1;
        outputSize = num + 30;
        qint32 i = 0;
        qint32 start = output.size() - num;
        if (start < 0)
        {
            start = 0;
        }
        // create output text
        QString drawText;
        for(i = start; i < output.size();i++)
        {
            if(i >= 0)
            {
                drawText += "> " + output[i] + "\n";
            }
        }
        // create blinking cursor position
        QString curprintmsg = curmsg;
        if (toggle.elapsed() < BLINKFREQG)
        {
            curprintmsg.insert(curmsgpos,"|");
        }else{
            curprintmsg.insert(curmsgpos," ");
        }
        if (toggle.elapsed() > BLINKFREQG * 2)
        {
            toggle.start();
        }
        drawText += "> " + curprintmsg;
        m_text->setHtmlText(drawText);
    }
    oxygine::Actor::update(us);
}

void Console::toggleView()
{
    show = !show;
    oxygine::Actor::setVisible(show);
    if (show)
    {
        m_pBackgroundsprite->setSize(Settings::getWidth(), Settings::getHeight());
    }
    toggled = true;
}

Console::eLogLevels Console::getLogLevel()
{
    return LogLevel;
}

void Console::setVolume(qint32 volume)
{
    Mainapp::getInstance()->getAudioThread()->setVolume(volume);
}

void Console::setLogLevel(eLogLevels newLogLevel)
{
    Console::LogLevel = newLogLevel;
}

void Console::help(qint32 start, qint32 end)
{

    qint32 index = 0;
    while (functions[index] != "" && ((end >= 0 && index <= end) ||  end < 0))
    {
        if (index >= start && (index <= end || end < 0))
        {
            Console::print(functions[index], Console::eINFO);
        }
        index++;
    }
}

void Console::logActions(bool log)
{
    Settings::setLogActions(log);
}

void Console::createfunnymessage(qint32 message){
    if (message < 0)
    {
        message = GlobalUtils::randIntBase(0,327);
    }
    QString printmessage;
    switch(message)
    {
        case 0:
            printmessage = "No funny games found :( -> Installing some funny Viruses!";
            break;
        case 1:
            printmessage = "Remember Bug 1#: The calculation is correct, but why the hell is the output wrong? Maybe the Input is wrong... Maybe";
            break;
        case 2:
            printmessage = "What? I should work faster? Look at your PC even a snake is faster than your CPU";
            break;
        case 3:
            printmessage = "I know this Quest. Nothing can happen! Sorry we couldn't find their corps!";
            break;
        case 4:
            printmessage = "1+1=10 ... What you don't understand Binary Stuff -> I can't work with such stupid Users.";
            break;
        case 5:
            printmessage = "No good music found. No good videos found. There's nothing funny i can do here.";
            break;
        case 6:
            printmessage = "Even if you're eaten by a dragon, you still have two ways out.";
            break;
        case 7:
            printmessage = "I know what you did last summer...";
            break;
        case 8:
            printmessage = "Positioning GPS satellites... please stand by...";
            break;
        case 9:
            printmessage = "The volume goes to eleven!";
            break;
        case 10:
            printmessage = "The game will start when it is ready and not before!";
            break;
        case 11:
            printmessage = "You are the weakest link, goodbye!";
            break;
        case 12:
            printmessage = "/* You are not expected to understand this */";
            break;
        case 13:
            printmessage = "Computer load this, computer load that, never a thank you... Rude gamers!";
            break;
        case 14:
            printmessage = "Now that's what I call a dead parrot";
            break;
        case 15:
            printmessage = "Loading errors, please wait...";
            break;
        case 16:
            printmessage = "Your soundcard sucks, disabling sound forever...";
            break;
        case 17:
            printmessage = "Relax, it's only ones and zeros!";
            break;
        case 18:
            printmessage = "No, this is not a silly message.";
            break;
        case 19:
            printmessage = "I still know what you did last summer...";
            break;
        case 20:
            printmessage = "Please stand by for an important announcement.";
            break;
        case 21:
            printmessage = "Your computer is running... You better go chase it.";
            break;
        case 22:
            printmessage = "Drawing beards onto your photographs...";
            break;
        case 23:
            printmessage = "Did you know that by reading this message you have wasted 5 seconds of your life?";
            break;
        case 24:
            printmessage = "Press any key... No! Not that one!";
            break;
        case 25:
            printmessage = "PI is exactly 3! Because I want to find out how world looks like where PI is exactly 3!";
            break;
        case 26:
            printmessage = "Freedom for wolves, ban the shepherds!";
            break;
        case 27:
            printmessage = "Your computer is now an official part of my world domination plan. Thank you!";
            break;
        case 28:
            printmessage = "There's nothing to see here, go ahead...";
            break;
        case 29:
            printmessage = "I don't care what you did last summer!";
            break;
        case 30:
            printmessage = "Ohh... I am loading too slowly? Ok, next time you'll load it yourself!";
            break;
        case 31:
            printmessage = "Searching for available information on dragons - Connecting to Wikipedia...";
            break;
        case 32:
            printmessage = "Warning: Don't try this at home! Why do you ignore this warning?";
            break;
        case 33:
            printmessage = "Are we there yet? Are we there yet? Are we there yet? ";
            break;
        case 34:
            printmessage = "Oh this game sucks. It really game sucks! Did you know this game sucks!";
            break;
        case 35:
            printmessage = "Infiltrated your computer continuing to send your special videos to the CIA! Please Wait!";
            break;
        case 36:
            printmessage = "Press Any Key. I can't find the Any Key.";
            break;
        case 37:
            printmessage = "Why is it forbidden to use Emergency Exits in Germany? I mean they are called ""Not Exit""";
            break;
        case 38:
            printmessage = "Let's play tic-tac-toe instead, I'll be X...";
            break;
        case 39:
            printmessage = "Coffee missing. Insert cup and press any key.";
            break;
        case 40:
            printmessage = "Clicking the mouse will not make the game load faster!";
            break;
        case 41:
            printmessage = "Knowledge is a weapon... I intend to be formidably armed.";
            break;
        case 42:
            printmessage = "<singing> I can't get no... Bug infection...";
            break;
        case 43:
            printmessage = "Press Crtl + Alt + Del now for an IQ test. Sorry IQ is to low to press those Keys.";
            break;
        case 44:
            printmessage = "Press any key to continue or other key to quit...";
            break;
        case 45:
            printmessage = "You did not call in sick just to play this game, now did you?";
            break;
        case 46:
            printmessage = "Backup not found: (A)bort, (R)etry, (G)et a beer. ";
            break;
        case 47:
            printmessage = "Nuclear Power Plants are save as long as you don't try to use them to produce energy!";
            break;
        case 48:
            printmessage = "Your computer is your friend....Trust the Computer...";
            break;
        case 49:
            printmessage = "Counting to Infinity......Please wait...";
            break;
        case 50:
            printmessage = "This is not a drill. This is the Apocalypse. Please stay calm and exit the building.";
            break;
        case 51:
            printmessage = "Does a Table disappear always you leave a room. Because the modern physic says: it's possible!";
            break;
        case 52:
            printmessage = "Its not a bug but a feature.";
            break;
        case 53:
            printmessage = "Life? Don't talk to me about life!";
            break;
        case 54:
            printmessage = "Does a Table disappear always you leave a room. Because the modern physic says: it's possible!";
            break;
        case 55:
            printmessage = "Its not a bug but a feature.";
            break;
        case 56:
            printmessage = "Life? Don't talk to me about life!";
            break;
        case 57:
            printmessage = "Stop staring at this message or I will stop loading!";
            break;
        case 58:
            printmessage = "Loading 3D buffer... Not that one!";
            break;
        case 59:
            printmessage = "I've calculated your chance of survival, but I don't think you'll like it.";
            break;
        case 60:
            printmessage = "No more heroes available. Do you want to play as a monster? (Y/N)";
            break;
        case 61:
            printmessage = "Is their any sense in killing all those monsters? I mean the time you are back. They're back, too";
            break;
        case 62:
            printmessage = "Do you really want to start the game? Really? Are you really sure?";
            break;
        case 63:
            printmessage = "Gaming is about killing time, not killing people...";
            break;
        case 64:
            printmessage = "Ordering pizza online... I hope you like anchovies with pineapple!";
            break;
        case 65:
            printmessage = "Insert Disc 5... Oh, you don't have a Disc 5? Ok, filling harddisc with garbage...";
            break;
        case 66:
            printmessage = "Wasting energy, please wait...";
            break;
        case 67:
            printmessage = "Overclocking CPU: 3,0GHz...3,2GHz...3,6GHz... OOPS, I broke your CPU!";
            break;
        case 68:
            printmessage = "There is a light at the end of the tunnel, just pray it's not a train.";
            break;
        case 69:
            printmessage = "Please do not feed the dragons!";
            break;
        case 70:
            printmessage = "Not now! Can't you see I'm busy? Play something else.";
            break;
        case 71:
            printmessage = "Would you like to play a game of... world domination?";
            break;
        case 72:
            printmessage = "Insert coin to continue...";
            break;
        case 73:
            printmessage = "How can this task be delayed when i haven't even started working on it?";
            break;
        case 74:
            printmessage = "If the game does not start in five minutes... wait longer!";
            break;
        case 75:
            printmessage = "Randomizing constants...";
            break;
        case 76:
            printmessage = "Scroll down the list and choose your favourite silly message. I'm too lazy.";
            break;
        case 77:
            printmessage = "There's something written about a chance but I don't trust this game...";
            break;
        case 78:
            printmessage = "Checking mouse driver... Checking mouse's driving license...";
            break;
        case 79:
            printmessage = "Generating dead undead... Undead dead... uh...";
            break;
        case 80:
            printmessage = "Your skill in reading is increased by one point!";
            break;
        case 81:
            printmessage = "You'll get an adventure poqint32 for reading so much stupid messages. Use this poqint32 for improving your Stupidness!";
            break;
        case 82:
            printmessage = "Save the Earth... its the only planet with chocolate!";
            break;
        case 83:
            printmessage = "Why should we save the world? We didn't find out if we can survive without the world!";
            break;
        case 84:
            printmessage = "Operation failed (A)bort, (R)etry, (I)nfluence with large hammer?";
            break;
        case 85:
            printmessage = "The past temps us, the present confuses us, and the future frightens us.";
            break;
        case 86:
            printmessage = "Checking co2 emissions of your computer... Sorry, it's to high turning off your PC.";
            break;
        case 87:
            printmessage = "Are you still in the loading screen? You need a new computer!";
            break;
        case 88:
            printmessage = "Deleting all other games on the computer... You'll not need them in the next 5 seconds.";
            break;
        case 89:
            printmessage = "How long can you withstand dragon breath again?";
            break;
        case 90:
            printmessage = "Do you want to continue? (Y)es, (N)o, (M)aybe";
            break;
        case 91:
            printmessage = "Incredible... it's even worse than i thought it would be.";
            break;
        case 92:
            printmessage = "It seems that your mouse pointer is trying to move, (C)ancel or (A)llow?";
            break;
        case 93:
            printmessage = "Mommy, there's a monster in front of my screen!";
            break;
        case 94:
            printmessage = "Load it yourself, you slave driver!";
            break;
        case 95:
            printmessage = "You look rather silly staring at this screen...";
            break;
        case 96:
            printmessage = "Call 911 for support... ";
            break;
        case 97:
            printmessage = "****, can't find this silly message file...";
            break;
        case 98:
            printmessage = "Are you sure you can still see the real world as real world?";
            break;
        case 99:
            printmessage = "Warning! your computer will self destruct in 5... 4... 3... 2... 1... <kaboom>";
            break;
        case 100:
            printmessage = "Don't you have better things to do then reading silly messages?";
            break;
        case 101:
            printmessage = "Who wants to live forever? Well... I wouldn't mind giving it a try!";
            break;
        case 102:
            printmessage = "Understanding is a three edged sword... our side, their side and the truth!";
            break;
        case 103:
            printmessage = "You're part of the game now...";
            break;
        case 104:
            printmessage = "There's a fine line between genius and insanity. I have erased this line.";
            break;
        case 105:
            printmessage = "ealth warning: you have been playing for 12 seconds...";
            break;
        case 106:
            printmessage = "Keep clicking until i say stop!";
            break;
        case 107:
            printmessage = "Two beer or not two beer...";
            break;
        case 108:
            printmessage = "If you have a slow pc, you are able to learn every silly message by heart...";
            break;
        case 109:
            printmessage = "Failed to load quests, I will now try to create some on my own...";
            break;
        case 110:
            printmessage = "Starting your mp3s... Argh, i can't listen to this anymore, go get some new songs!";
            break;
        case 111:
            printmessage = "Press y+m+q+enter+space with one hand to continue.";
            break;
        case 112:
            printmessage = "We don't have bugs, we have dragons!";
            break;
        case 114:
            printmessage = "This message is intended not to be funny.";
            break;
        case 113:
            printmessage = "I don't know what you're talking about, there is no game here...";
            break;
        case 115:
            printmessage = "I invented the 'hello world' program!";
            break;
        case 116:
            printmessage = "CPU says you should not start those boring office programs anymore.";
            break;
        case 117:
            printmessage = "What do you think I am doing? Loading the game?";
            break;
        case 118:
            printmessage = "Let's ride some trojan horses... I've found two in the left corner of your harddisc.";
            break;
        case 119:
            printmessage = "Killing some bugs, please wait...";
            break;
        case 120:
            printmessage = "Loading loadingscreen...";
            break;
        case 121:
            printmessage = "Hit any user to continue.";
            break;
        case 122:
            printmessage = "This game never has bugs. It just develops random features!";
            break;
        case 123:
            printmessage = "You just can't trust developers these days - they got even the silly messages bugged!";
            break;
        case 124:
            printmessage = "I don't have a solution, but I admire the problem!";
            break;
        case 125:
            printmessage = "There cannot be a crisis today, my schedule is already full.";
            break;
        case 126:
            printmessage = "I have a fix opinion, don't confuse me with facts!";
            break;
        case 127:
            printmessage = "The best way to create panic is to say: There is no reason to react panically.";
            break;
        case 128:
            printmessage = "If you want to continue roll a dice with excatly 21 numbers.";
            break;
        case 129:
            printmessage = "Did you feed your Mouse? Because i think it's dead!";
            break;
        case 130:
            printmessage = "If a chance to survive is excactly 1:1000000. Than you'll survive to 100%. Tell me if i'm right by finding it out!";
            break;
        case 131:
            printmessage = "I prefer a Sword instead of the well known Weapon Knowledge. In a RPG without Magic.";
            break;
        case 132:
            printmessage = "I believe i can fly. The Resulst can be seen, now.";
            break;
        case 133:
            printmessage = "I always await an Ambush, even when i'm asleep.";
            break;
        case 134:
            printmessage = "I'm not always right! But i'm never wrong!";
            break;
        case 135:
            printmessage = "It's so funny to kill them! It should be forbidden! IT'S FORBIDDEN!!!";
            break;
        case 136:
            printmessage = "Cheat-Mode detected! Installing Virus on Hard, Middle and Soft Disc. Please wait!";
            break;
        case 137:
            printmessage = "Braindetector activated, calibrating, now searching.........still searching......get a good grip of your mobile....still searching.......no brains found.";
            break;
        case 138:
            printmessage = "Did I not see you yesterday at the mall, with a grey jacket? No? O, than it was a rubbish bag after all!";
            break;
        case 139:
            printmessage = "What he want, I do not want ... What I want, he does not want ... What we want, is not allowed!";
            break;
        case 140:
            printmessage = "An Error has appeared while creating an Error Message.";
            break;
        case 141:
            printmessage = "\"I hate when friends come over and ask, \"Do you have a bathroom?\" - \"No, we shit outside.\"";
            break;
        case 142:
            printmessage = "A PC is only as clever as his User.";
            break;
        case 143:
            printmessage = "Access denied, please enter your ID again...";
            break;
        case 144:
            printmessage = "As a Computer, I find your faith in technology... amusing!";
            break;
        case 145:
            printmessage = "Eek! A mouse! Plug it out! Eek! It's full of bytes!";
            break;
        case 146:
            printmessage = "Funny Bugreport #22650: No sound while PC is burning!";
            break;
        case 147:
            printmessage = "Loading is booooring...";
            break;
        case 148:
            printmessage = "Loading MP3 playlist...";
            break;
        case 149:
            printmessage = "Loading silly messages...";
            break;
        case 150:
            printmessage = "Loading Sprite #559486753...Please wait!";
            break;
        case 151:
            printmessage = "Loading will continue after a short message from our Sponsor.";
            break;
        case 152:
            printmessage = "Monday morning Quest: Find my Coffee Mug in the Office!";
            break;
        case 153:
            printmessage = "Out of Money error ... Insert new projekt fundings!";
            break;
        case 154:
            printmessage = "Please turn off your monitor to continue.";
            break;
        case 155:
            printmessage = "Scanning Mail Folder... emty... not even Spam Mail!";
            break;
        case 156:
            printmessage = "Searching for Adult Filter...";
            break;
        case 157:
            printmessage = "Seaching for saved game... not found. Restarting game...";
            break;
        case 158:
            printmessage = "Spiderpig, Spiderpig ...";
            break;
        case 159:
            printmessage = "Stop staring at this message or I will stop loading.";
            break;
        case 160:
            printmessage = "The... no, A..., no, wait, sorry, that one sounded better in my head.";
            break;
        case 161:
            printmessage = "There's something written about a chance but I don´t trust this game...";
            break;
        case 162:
            printmessage = "There is a hole... in your mind.";
            break;
        case 163:
            printmessage = "There is too much CPU usage in zlib.dll!";
            break;
        case 164:
            printmessage = "Unloading silly messages, not silly enough!";
            break;
        case 165:
            printmessage = "Updating funny messages...";
            break;
        case 166:
            printmessage = "Uplink to ISS etablished, downloading weather forecast...";
            break;
        case 167:
            printmessage = "Uploading user fingerprints to NSA computer...";
            break;
        case 168:
            printmessage = "Use the mouse, Luke!";
            break;
        case 169:
            printmessage = "Video ram too small, switching to grid display...";
            break;
        case 170:
            printmessage = "Waiting some time...";
            break;
        case 171:
            printmessage = "We're working on the documentation ...";
            break;
        case 172:
            printmessage = "Warning! CPU temperature reaching critical... Shutting system down...";
            break;
        case 173:
            printmessage = "We apologize for the fault in loading screen. The responsible parties have been sacked.";
            break;
        case 174:
            printmessage = "We'll be right back after this commercial break.";
            break;
        case 175:
            printmessage = "What's that behind you?";
            break;
        case 176:
            printmessage = "What are you doing? Stop that!";
            break;
        case 177:
            printmessage = "When it's done! Got it?";
            break;
        case 178:
            printmessage = "When you are leaving, please close the door behind you.";
            break;
        case 179:
            printmessage = "Would you like to play a game of...World Domination?";
            break;
        case 180:
            printmessage = "Wrong CD Key... this Game will now be locked on this Computer for ever.";
            break;
        case 181:
            printmessage = "Yeah but no but yeah but no but yeah but no.";
            break;
        case 182:
            printmessage = "You are here: X";
            break;
        case 183:
            printmessage = "You are the 1.000.000th visitor! Congratulations! You won a box of tissues - not.";
            break;
        case 184:
            printmessage = "You won´t get this package if you can not ID yourself!";
            break;
        case 185:
            printmessage = "You really have to find a girlfriend whose name doesn´t end with .jpg!";
            break;
        case 186:
            printmessage = "Mysterious Strangers visits: 0";
            break;
        case 187:
            printmessage = "I am a cute computer... really!";
            break;
        case 188:
            printmessage = "Never seen such a big... never mind...";
            break;
        case 189:
            printmessage = "Deleting all Celine Dion MP3s ...";
            break;
        case 190:
            printmessage = "Destroying brain cells... 15 percent... 50 percent... Duuh...";
            break;
        case 191:
            printmessage = "Cannot translate silly message into english.";
            break;
        case 192:
            printmessage = "Be sure to visit the gift shop on your way out.";
            break;
        case 193:
            printmessage = "Checking for 3d buffer ... Wait, not that one!";
            break;
        case 194:
            printmessage = "Activating coffee-machine...";
            break;
        case 195:
            printmessage = "Hehehe! You'll never guess what awaits you here!";
            break;
        case 196:
            printmessage = "Bacon is my favourite vegetable...";
            break;
        case 197:
            printmessage = "Do not include <STDHEADER.H>, use <STDHEADER> instead!";
            break;
        case 198:
            printmessage = "Cannot find Reality.sys… Universe halted.";
            break;
        case 199:
            printmessage = "Good. Out of the door. Line on the left. One cross each. Next. Crucifixion?";
            break;
        case 200:
            printmessage = "Here I am, brain the size of a planet, and they ask me to load files.";
            break;
        case 201:
            printmessage = "Man, what a mess on your harddisc...";
            break;
        case 202:
            printmessage = "Hidden DOS secret: Add bugs=off to your config.sys!";
            break;
        case 203:
            printmessage = "At some point, we'll have our own options.txt, son. One day...";
            break;
        case 204:
            printmessage = "Attention! This game can change your personality into evil...";
            break;
        case 205:
            printmessage = "I have the power of... of.. What's my line again?";
            break;
        case 206:
            printmessage = "Idea for a messagebox: 'This frame was properly rendered!'";
            break;
        case 207:
            printmessage = "Loading Bluescreen...";
            break;
        case 208:
            printmessage = "Sponsored by the Umbrella Corporation.";
            break;
        case 209:
            printmessage = "Don´t wake me up, I´m working...";
            break;
        case 210:
            printmessage = "It´s you again! Don´t you ever sleep?";
            break;
        case 211:
            printmessage = "No, this is not a silly message";
            break;
        case 212:
            printmessage = "Can I interest anyone in Fruit or Dessert?";
            break;
        case 213:
            printmessage = "our Oma is now Pwned by Studio II!";
            break;
        case 214:
            printmessage = "Detected System: Windows 3.11 - Switching Screen Resolution to 320 x 480 Pixels.";
            break;
        case 215:
            printmessage = "Hey Buddy! Need a cheap WiFi Cable? Only 100$!";
            break;
        case 216:
            printmessage = "Do you like Scary Movies?";
            break;
        case 217:
            printmessage = "My missus says that a spline is the mathmatical backbone of the game.";
            break;
        case 218:
            printmessage = "Two wrongs don't make a right, but three rights make a left!";
            break;
        case 219:
            printmessage = "I'm sensing a soul in search of answers...";
            break;
        case 220:
            printmessage = "Only silly coders need to optimize their code!";
            break;
        case 221:
            printmessage = "Uplink to ISS etablished, downloading weather forecast...";
            break;
        case 222:
            printmessage = "Good luck everyone - hope to see you again in the future!";
            break;
        case 223:
            printmessage = "A book can also be, a hat...";
            break;
        case 224:
            printmessage = "West of house, there is a small mailbox here. Exits are east and south.";
            break;
        case 225:
            printmessage = "Don't create multiple postings, use the edit button!";
            break;
        case 226:
            printmessage = "Do not disturb, system is disturbed already!";
            break;
        case 227:
            printmessage = "Don't push me. I'm doing my best, but your machine... tztztz...";
            break;
        case 228:
            printmessage = "... go to spriegel the schmitzel wertzen or something to that effect!";
            break;
        case 229:
            printmessage = "...Boldly going, where no one has ever gone before. At Least not recently.";
            break;
        case 230:
            printmessage = "Help! Your Keyboard is stuckkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk...";
            break;
        case 231:
            printmessage = "General Failure's fault. Not yours.";
            break;
        case 232:
            printmessage = "Deleting previously saved games...";
            break;
        case 233:
            printmessage = "earching for illegal Software ... just kidding!";
            break;
        case 234:
            printmessage = "Dude, where's my car?";
            break;
        case 235:
            printmessage = "Do you really want to start the game? Really?";
            break;
        case 236:
            printmessage = "Due to lack of system resources, entering slide-show mode...";
            break;
        case 237:
            printmessage = "Cheater detected. Loading bunnies...";
            break;
        case 238:
            printmessage = "Even if you die healthy, your dead.";
            break;
        case 239:
            printmessage = "Real Messages: Debugger doesn't work properly! Do you wish do Debug the Debugger with the Debugger?";
            break;
        case 240:
            printmessage = "Real Messages: Error Do you want to save your stuff or terminate without it? (Y)es ; (N)o.";
            break;
        case 241:
            printmessage = "e're going to be killed! Fascinating.";
            break;
        case 242:
            printmessage = "HOW MAY I SERVE YOU, INFERIOR BEING?";
            break;
        case 243:
            printmessage = "Didn't find a Dice for GETRANDOMNUMBER(). Please insert one into your CD Player.";
            break;
        case 244:
            printmessage = "Always doing the same is boring! Let's find out how a burning PC looks like!";
            break;
        case 245:
            printmessage = "The World is a plate and is carried by a Turtle on which stands three elephants. Everything else is wrong.";
            break;
        case 246:
            printmessage = "You don't have facebook??? Now, when you're officialy dead-> Dead people can't play! Stop loading...";
            break;
        case 247:
            printmessage = "Does a falling tree makes no Sound if there is nobody to here it fall?";
            break;
        case 248:
            printmessage = "Do you wish do spent all your money to the Project Owner? (Y)es ; No (disabled).";
            break;
        case 249:
            printmessage = "It should be fair. God go away with your human feelings. It's of course unfair.";
            break;
        case 250:
            printmessage = "\"Ha finally i've got the last word.\"......\"Nearly the last.\"";
            break;
        case 251:
            printmessage = "The part where you all shut up happens now.";
            break;
        case 252:
            printmessage = "My motto is that there are far too many women in the world to waste time with men.";
            break;
        case 253:
            printmessage = "You think i'm not funny? I can be not funny...";
            break;
        case 254:
            printmessage = "The bigger they are, the harder they fall!";
            break;
        case 255:
            printmessage = "Computers are useless.  They can only give you answers.";
            break;
        case 256:
            printmessage = "Computers are like bikinis. They save people a lot of guesswork.";
            break;
        case 257:
            printmessage = "We have computers, and we may have other weapons of mass destruction.";
            break;
        case 258:
            printmessage = "That's what's cool about working with computers.  They don't argue, they remember everything, and they don't drink all your beer.";
            break;
        case 259:
            printmessage = "If the automobile had followed the same development cycle as the computer, a Rolls-Royce would today cost $100, get a million miles per gallon, and explode once a year, killing everyone inside.";
            break;
        case 260:
            printmessage = "I've noticed lately that the paranoid fear of computers becoming intelligent and taking over the world has almost entirely disappeared from the common culture.  Near as I can tell, this coincides with the release of MS-DOS.";
            break;
        case 261:
            printmessage = "The question of whether computers can think is like the question of whether submarines can swim.";
            break;
        case 262:
            printmessage = "It's ridiculous to live 100 years and only be able to remember 30 million bytes.  You know, less than a compact disc.  The human condition is really becoming more obsolete every minute.";
            break;
        case 263:
            printmessage = "Never trust a computer you can't throw out a window.";
            break;
        case 264:
            printmessage = "Hardware: The parts of a computer system that can be kicked.";
            break;
        case 265:
            printmessage = "Most software today is very much like an Egyptian pyramid with millions of bricks piled on top of each other, with no structural integrity, but just done by brute force and thousands of slaves.";
            break;
        case 266:
            printmessage = "The Internet?  Is that thing still around?";
            break;
        case 267:
            printmessage = "Any fool can use a computer.  Many do.";
            break;
        case 268:
            printmessage = "There are only two industries that refer to their customers as ‘users'.";
            break;
        case 269:
            printmessage = "Programmers are in a race with the Universe to create bigger and better idiot-proof programs, while the Universe is trying to create bigger and better idiots.  So far the Universe is winning.";
            break;
        case 270:
            printmessage = "That's the thing about people who think they hate computers.  What they really hate is lousy programmers.";
            break;
        case 271:
            printmessage = "Don't worry if it doesn't work right.  If everything did, you'd be out of a job.";
            break;
        case 272:
            printmessage = "Measuring programming progress by lines of code is like measuring aircraft building progress by weight.";
            break;
        case 273:
            printmessage = "Writing code has a place in the human hierarchy worth somewhere above grave robbing and beneath managing.";
            break;
        case 274:
            printmessage = "First learn computer science and all the theory.  Next develop a programming style.  Then forget all that and just hack.";
            break;
        case 275:
            printmessage = "First, solve the problem. Then, write the code.";
            break;
        case 276:
            printmessage = "The best thing about a boolean is even if you are wrong, you are only off by a bit.";
            break;
        case 277:
            printmessage = "Should array indices start at 0 or 1?  My compromise of 0.5 was rejected without, I thought, proper consideration.";
            break;
        case 278:
            printmessage = "There are only two kinds of programming languages: those people always bitch about and those nobody uses.";
            break;
        case 279:
            printmessage = "I think Microsoft named .Net so it wouldn't show up in a Unix directory listing.";
            break;
        case 280:
            printmessage = "There is no programming language–no matter how structured–that will prevent programmers from making bad programs.";
            break;
        case 281:
            printmessage = "Fifty years of programming language research, and we end up with C++?";
            break;
        case 282:
            printmessage = "In C++ it's harder to shoot yourself in the foot, but when you do, you blow off your whole leg.";
            break;
        case 283:
            printmessage = "One of the main causes of the fall of the Roman Empire was that–lacking zero–they had no way to indicate successful termination of their C programs.";
            break;
        case 284:
            printmessage = "Fine, Java MIGHT be a good example of what a programming language should be like.  But Java applications are good examples of what applications SHOULDN'T be like.";
            break;
        case 285:
            printmessage = "If Java had true garbage collection, most programs would delete themselves upon execution.";
            break;
        case 286:
            printmessage = "Any code of your own that you haven't looked at for six or more months might as well have been written by someone else.";
            break;
        case 287:
            printmessage = "The first 90% of the code accounts for the first 10% of the development time.  The remaining 10% of the code accounts for the other 90% of the development time.";
            break;
        case 288:
            printmessage = "If debugging is the process of removing bugs, then programming must be the process of putting them in.";
            break;
        case 289:
            printmessage = "I don't care if it works on your machine!  We are not shipping your machine!";
            break;
        case 290:
            printmessage = "There are two ways to write error-free programs; only the third one works.";
            break;
        case 291:
            printmessage = "You can either have software quality or you can have pointer arithmetic, but you cannot have both at the same time.";
            break;
        case 292:
            printmessage = "Always code as if the guy who ends up maintaining your code will be a violent psychopath who knows where you live.";
            break;
        case 293:
            printmessage = "Windows NT addresses 2 Gigabytes of RAM, which is more than any application will ever need.";
            break;
        case 294:
            printmessage = "Keyboard not found. Press < F1 > to RESUME.";
            break;
        case 295:
            printmessage = "Any sufficiently advanced bug is indistinguishable from a feature.";
            break;
        case 296:
            printmessage = "Beware of bugs in the above code; I have only proved it correct, not tried it.";
            break;
        case 297:
            printmessage = "bug, n: An elusive creature living in a program that makes it incorrect. The activity of ""debugging"", or removing bugs from a program, ends when people get tired of doing it, not when the bugs are removed.";
            break;
        case 298:
            printmessage = "Don't get suckered in by the comments— they can be terribly misleading. Debug only code";
            break;
        case 299:
            printmessage = "If the code and the comments disagree, then both are probably wrong.";
            break;
        case 300:
            printmessage = "Once you're done writing the code, never open it again unless you want to see how uncomprehensible and utterly ridiculous it really is.";
            break;
        case 301:
            printmessage = "The generation of random numbers is too important to be left to chance.";
            break;
        case 302:
            printmessage = "The most likely way for the world to be destroyed, most experts agree, is by accident. That's where we come in; we're computer professionals. We cause accidents.";
            break;
        case 303:
            printmessage = "Sometimes I believe compiler ignores all my comments";
            break;
        case 304:
            printmessage = "<!-- Here be dragons  -->";
            break;
        case 305:
            printmessage = "You don't need luck. You need enough firepower.";
            break;
        case 306:
            printmessage = "This Game isn't about peace so \"make war\"";
            break;
        case 307:
            printmessage = "I have always wished for my computer to be as easy to use as my telephone; my wish has come true because I can no longer figure out how to use my telephone.";
            break;
        case 308:
            printmessage = "The gap between theory and practice is not as wide in theory as it is in practice.";
            break;
        case 309:
            printmessage = "There are two major products that come out of Berkeley: LSD and UNIX. We don't believe this to be a coincidence.";
            break;
        case 310:
            printmessage = "Debugging is twice as hard as writing the code in the first place.  Therefore, if you write the code as cleverly as possible, you are–by definition–not smart enough to debug it.";
            break;
        case 311:
            printmessage = "If at first you don't succeed, call it version 1.0";
            break;
        case 312:
            printmessage = "Rules of Optimization: Rule(1) : Don() 't do it.    Rule 2 (for experts only): Don't do it yet.";
            break;
        case 313:
            printmessage = "Programming can be fun, so can cryptography; however they should not be combined.";
            break;
        case 314:
            printmessage = "Computers are good at following instructions, but not at reading your mind.";
            break;
        case 315:
            printmessage = "I just saw my life flash before my eyes and all I could see was a close tag…";
            break;
        case 316:
            printmessage = "Java: write once, debug everywhere.";
            break;
        case 317:
            printmessage = "All methodologies are based on fear.";
            break;
        case 318:
            printmessage = "Real programmers don't document. If it was hard to write, it should be hard to understand.";
            break;
        case 319:
            printmessage = "If we knew what we were doing, it wouldn't be called research, would it?";
            break;
        case 320:
            printmessage = "There are two kind of sysadmins: Paranoids and Losers.";
            break;
        case 321:
            printmessage = "We all know Linux is great... it does infinite loops in 5 seconds.";
            break;
        case 322:
            printmessage = "Comments lie. Code doesn't.";
            break;
        case 323:
            printmessage = "Exceed Funny Message File. Creating some new one's.";
            break;
        case 324:
            printmessage = "The Code is the Comment or believe in god.";
            break;
        case 325:
            printmessage = "This message was created by slavery and kids.";
            break;
        case 326:
            printmessage = "Error 404 no game found. Do your homework instead.";
            break;
        case 327:
            printmessage = "It isn't over until it's over!";
            break;
        default:
            printmessage = "No more funny Messages found. Delete your Harddisk instead";
            break;
    }
    print(printmessage, Console::eINFO);
}

void Console::KeyInput(oxygine::KeyEvent event)
{
    // for debugging
    Qt::Key cur = event.getKey();
    if (cur == Settings::getKeyConsole())
    {
        Console::toggleView();
    }
    else if (show)
    {
        if ((event.getModifiers() & Qt::KeyboardModifier::ControlModifier) > 0)
        {
            switch(cur)
            {
                case Qt::Key_V:
                {
                    QString text = QGuiApplication::clipboard()->text();
                    curmsg = curmsg.insert(curmsgpos, text);
                    curmsgpos += text.size();
                    break;
                }
                case Qt::Key_C:
                {
                    QGuiApplication::clipboard()->setText(curmsg);
                    curmsgpos += curmsg.size();
                    break;
                }
                case Qt::Key_X:
                {
                    QGuiApplication::clipboard()->setText(curmsg);
                    curmsg = "";
                    curmsgpos = 0;
                    break;
                }
                default:
                {
                    // nothing
                    break;
                }
            }
        }
        else
        {
            //Handle Key Input for the console
            switch(cur)
            {
                case Qt::Key_Home:
                {
                    curmsgpos = 0;
                    break;
                }
                case Qt::Key_Up:
                {
                    curlastmsgpos--;
                    if(curlastmsgpos < 0)
                    {
                        curlastmsgpos = lastmsgs.size() - 1;
                    }
                    if(curlastmsgpos < lastmsgs.size() && curlastmsgpos >= 0)
                    {
                        curmsg = lastmsgs[curlastmsgpos];
                        curmsgpos = curmsg.size();
                    }
                    break;
                }
                case Qt::Key_Left:
                {
                    curmsgpos--;
                    if(curmsgpos < 0)
                    {
                        curmsgpos = 0;
                    }
                    break;
                }
                case Qt::Key_Down:
                {
                    curlastmsgpos++;
                    if(curlastmsgpos >= lastmsgs.size())
                    {
                        curlastmsgpos = 0;
                    }
                    if(curlastmsgpos < lastmsgs.size())
                    {
                        curmsg = lastmsgs[curlastmsgpos];
                        curmsgpos = curmsg.size();
                    }
                    break;
                }
                case Qt::Key_Right:
                {
                    curmsgpos++;
                    if(curmsgpos > curmsg.size())
                    {
                        curmsgpos = curmsg.size();
                    }
                    break;
                }
                case Qt::Key_Enter:
                case Qt::Key_Return:
                {
                    dotask(curmsg);
                    lastmsgs.append(curmsg);
                    while (lastmsgs.size() > lastMsgSize)
                    {
                        lastmsgs.removeFirst();
                    }
                    curlastmsgpos = lastmsgs.size();
                    curmsg = "";
                    curmsgpos = 0;
                    break;
                }
                case Qt::Key_Backspace:
                {
                    if(curmsgpos > 0)
                    {
                        curmsg.remove(curmsgpos - 1,1);
                        curmsgpos--;
                    }
                    break;
                }
                case Qt::Key_Delete:
                {
                    if (curmsgpos < curmsg.size())
                    {
                        curmsg.remove(curmsgpos, 1);
                    }
                    break;
                }
                case Qt::Key_End:
                {
                    curmsgpos = curmsg.size();
                    break;
                }
                default:
                {
                    // for the start we don't check for upper or lower key input
                    QString msg = event.getText();
                    curmsg.insert(curmsgpos,msg);
                    curmsgpos += msg.size();
                }
            }
        }
    }

}

void Console::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QFile file("console.log");
    if (!file.isOpen())
    {
        QStringList args = QCoreApplication::arguments();
        if (args.contains("-slave"))
        {
            QString slaveName = "slave";
            if (args.contains("-slaveServer"))
            {
                slaveName = args[args.indexOf("-slaveServer") + 1];
            }
            file.setFileName(slaveName + ".log");
        }
        file.open(QIODevice::WriteOnly);
    }
    static QTextStream stream(&file);
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
        case QtDebugMsg:
            if (Console::LogLevel <= Console::eLogLevels::eDEBUG)
            {
                stream << "Debug: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                stream.flush();
                fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            }
            break;
        case QtInfoMsg:
            if (Console::LogLevel <= Console::eLogLevels::eINFO)
            {
                stream << "Info: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                stream.flush();
                fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            }
            break;
        case QtWarningMsg:
            if (Console::LogLevel <= Console::eLogLevels::eWARNING)
            {
                stream << "Warning: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                stream.flush();
                fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            }
            break;
        case QtCriticalMsg:
            if (Console::LogLevel <= Console::eLogLevels::eERROR)
            {
                stream << "Critical: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                stream.flush();
                fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            }
            break;
        case QtFatalMsg:
            if (Console::LogLevel <= Console::eLogLevels::eFATAL)
            {
                stream << "Fatal: " << msg << " File: " << context.file << " Line: " << context.line << " Function: " << context.function << "\n";
                stream.flush();
                fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            }
            break;
    }
}
