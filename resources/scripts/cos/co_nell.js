var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3", "+alt4"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
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
        }
    };

    this.activatePower = function(co, map)
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
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power7_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power7_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.activateSuperpower = function(co, powerMode, map)
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
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 7)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power12_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power12_2.wav", 1, delay);
            }
            if (animations.length < 7)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };

    this.superPowerLuckBonus = 100;

    this.powerLuckBonus = 60;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;

    this.d2dCoZoneLuckBonus = 30;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.d2dLuckBonus = 10;

    this.getBonusLuck = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_NELL.superPowerLuckBonus;
            case GameEnums.PowerMode_Power:
                return CO_NELL.powerLuckBonus;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return CO_NELL.d2dCoZoneLuckBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_NELL.d2dLuckBonus;
                }
            }
        }
        return 0;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_NELL.powerOffBonus;
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_NELL.d2dCoZoneOffBonus;
            }
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_NELL.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_NELL.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Rachel's older sister and supreme commander of the Orange Star Army, Nell is an able commanding officer with a superb sense of fashion.");
    };
    this.getHits = function(co)
    {
        return qsTr("Willful students");
    };
    this.getMiss = function(co)
    {
        return qsTr("Downtime");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Can strike with more force than expected. She's the first to tell you she was born lucky.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_NELL.d2dLuckBonus];
        }
        var text = qsTr("\nGlobal Effect: \nNell's units have +%0 luck.") +
               qsTr("\n\nCO Zone Effect: \nNell's units have +%1 luck, +%2% firepower, and +%3% defence.");
        text = replaceTextArgs(text, [values[0], CO_NELL.d2dCoZoneLuckBonus, CO_NELL.d2dCoZoneOffBonus, CO_NELL.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Nell's units have +%0 luck, +%1% firepower, and +%2% defence. Lucky!");
        text = replaceTextArgs(text, [CO_NELL.powerLuckBonus, CO_NELL.powerOffBonus, CO_NELL.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Lucky Star");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Nell's units have +%0 luck, +%1% firepower, and +%2% defence. Very lucky!");
        text = replaceTextArgs(text, [CO_NELL.superPowerLuckBonus, CO_NELL.powerOffBonus, CO_NELL.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Lady Luck");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Luck IS a skill!"),
                qsTr("Hmm... Time to get serious!"),
                qsTr("I'm just getting started!"),
                qsTr("Don't hate me just because I'm lucky!"),
                qsTr("Everything will work out!"),
                qsTr("I'm feelin' lucky!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Did I go too far?"),
                qsTr("Lady Luck was with me!"),
                qsTr("...And that's how it's done.")];
    };
    this.getDefeatSentences = function(co)
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
