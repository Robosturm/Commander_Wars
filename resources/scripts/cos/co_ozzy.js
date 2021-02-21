var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
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
                audio.addMusic("resources/music/cos/ozzy.mp3", 877, 60878);
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
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

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
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (isDefender)
                {
                    return 100;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 200;
            case GameEnums.PowerMode_Power:
                if (isDefender)
                {
                    return 60;
                }
                else
                {
                    return 100;
                }
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return 50;
                }
                break;
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A psychoanalyst who specializes in defensive tactics when he's not playing his guitar. A habitual womanizer.");
    };
    this.getHits = function(co)
    {
        return qsTr("Mind Games");
    };
    this.getMiss = function(co)
    {
        return qsTr("Waiting");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Ozzy's defensive tactics lowers the amount of damage his units take.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nNo Effect.") +
               qsTr("\n\nCO Zone Effect: \nDefense is increased.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Units gain a modest defense boost. Enemy counterattacks are less effective.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Hammer On");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Defense rises dramatically. Counterattack strength is doubled.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Grand Finale");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("IT'S BEEN A LONG TIME SINCE I ROCK 'N ROLLED, MMM-HMM!"),
                qsTr("So when're you gunna start fighting?"),
                qsTr("Time to unleash the MAGIC...the SPANISH CASTLE magic..."),
                qsTr("Jet Black, Johnny B...Killer Queen...Dizzy Lizzy, 'n Major Tom..."),
                qsTr("Let's crank up the gain a little bit, shall we?"),
                qsTr("You're still there? Guess I'd better start trying.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Finally. Can I get back to my guitar now?"),
                qsTr("So nobody ever told you how it was gunna be, eh?"),
                qsTr("I could say something cliche here, but I think I've done enough already.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("That doesn't rock."),
                qsTr("That wasn't one of my best concerts.")];
    };
    this.getName = function()
    {
        return qsTr("Ozzy");
    };
}

Constructor.prototype = CO;
var CO_OZZY = new Constructor();
