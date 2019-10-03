var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/nell.mp3", 59, 61394);
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();

        audio.clearPlayList();
        CO_NELL.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();

        audio.clearPlayList();
        CO_NELL.loadCOMusic(co);
        audio.playRandom();
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getBonusLuck = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 100;
            case GameEnums.PowerMode_Power:
                return 60;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return 15;
                }
                break;
        }
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Rachel's older sister and supreme commander of the Orange Star army, Nell is an able commanding officer with a superb sense of fashion.");
    };
    this.getHits = function()
    {
        return qsTr("Willful students");
    };
    this.getMiss = function()
    {
        return qsTr("Downtime");
    };
    this.getCODescription = function()
    {
        return qsTr("Sometimes strikes with slightly more force than expected. She's the first to tell you she was born lucky.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect:\nNo Effects") +
               qsTr("\nCO Zone Effect:\nUnits have 15% Luck.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Has a chance to strike with more damage than expected. Lucky!");
    };
    this.getPowerName = function()
    {
        return qsTr("Lucky Star");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Improves her chance to strike with massive damage. Very lucky!");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Lady Luck");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Luck IS a skill!"),
                qsTr("Hmm... Time to get serious!"),
                qsTr("I'm just getting started!"),
                qsTr("Don't hate me just because I'm lucky!"),
                qsTr("Everything will work out!"),
                qsTr("I'm feelin' lucky!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Did I go too far?"),
                qsTr("Lady luck was with me!"),
                qsTr("...And that's how it's done.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Seems like I just wasn't lucky enough..."),
                qsTr("Congratulations! You've beaten me!")];
    };
    this.getName = function()
    {
        return qsTr("Nell");
    };
}

Constructor.prototype = CO;
var CO_NELL = new Constructor();
