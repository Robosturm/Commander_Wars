var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.ogg", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.ogg", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/ozzy.ogg", 877, 60878);
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
                animation.setSound("power2_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power2_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
        return 2;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };

    this.superPowerOffCounterBonus = 100;
    this.superPowerDefBonus = 200;
    this.powerOffBonus = 10;
    this.powerAtkDefBonus = 50;
    this.powerDefBonus = 100;

    this.d2dDefBonus = 15;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 50;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (isDefender)
                {
                    return CO_OZZY.superPowerOffCounterBonus;
                }
                return CO_OZZY.powerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_OZZY.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_OZZY.d2dCoZoneOffBonus;
                }
                break;
            }
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_OZZY.superPowerDefBonus;
            case GameEnums.PowerMode_Power:
                if (isAttacker)
                {
                    return CO_OZZY.powerDefBonus + CO_OZZY.powerAtkDefBonus;
                }
                else
                {
                    return CO_OZZY.powerDefBonus;
                }
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_OZZY.d2dCoZoneDefBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_OZZY.d2dDefBonus;
                }
            }
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "FACTORY" ||
                    buildingId === "TOWN" ||
                    BUILDING.isHq(building))
            {
                return ["ZCOUNIT_IRON_SHIELD_GENERATOR"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A psychoanalyst who specializes in defensive tactics when he's not playing his guitar. A habitual womanizer.");
    };
    this.getHits = function(co)
    {
        return qsTr("Mind games");
    };
    this.getMiss = function(co)
    {
        return qsTr("Waiting");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Ozzy's defensive tactics lowers the amount of damage his units take.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_OZZY.d2dDefBonus];
        }
        var text = qsTr("\nSpecial Unit:\nIron Shield Generator\n") +
            qsTr("\nGlobal Effect: \nOzzy's units have +%0% defence.") +
            qsTr("\n\nCO Zone Effect: \nOzzy's units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [values[0], CO_OZZY.d2dCoZoneOffBonus, CO_OZZY.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Ozzy's units gain +%0% firepower and +%1% defence. While attacking, they gain an additional +%2% defence from counterattacks.");
        text = replaceTextArgs(text, [CO_OZZY.powerOffBonus, CO_OZZY.powerDefBonus, CO_OZZY.powerAtkDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Hammer On");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Ozzy's units gain +%0% firepower and +%1% defence. Their counterattacks gain a total of +%2% firepower.");
        text = replaceTextArgs(text, [CO_OZZY.powerOffBonus, CO_OZZY.superPowerDefBonus, CO_OZZY.superPowerOffCounterBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Grand Finale");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("IT'S BEEN A LONG TIME SINCE I ROCK 'N ROLLED, MMM-HMM!"),
                qsTr("So... when're you gunna start fighting?"),
                qsTr("Time to unleash the MAGIC... the SPANISH CASTLE magic..."),
                qsTr("Jet Black, Johnny B... Killer Queen... Dizzy Lizzy, 'n Major Tom..."),
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
