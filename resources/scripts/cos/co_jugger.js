var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.ogg", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.ogg", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.ogg", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/jugger.ogg", 7861, 81685);
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
        return "BG";
    };

    this.superPowerBonusLuck = 95;
    this.superPowerBonusMissfortune = 45;

    this.powerBonusLuck = 55;
    this.powerBonusMissfortune = 25;

    this.powerOffBonus = 10;
    this.powerDefBonus = 10;

    this.d2dCoZoneBonusLuck = 55;
    this.d2dCoZoneBonusMissfortune = 25;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.d2dBonusLuck = 30;
    this.d2dBonusMissfortune = 15;

    this.getBonusLuck = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_JUGGER.superPowerBonusLuck;
            case GameEnums.PowerMode_Power:
                return CO_JUGGER.powerBonusLuck;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return CO_JUGGER.d2dCoZoneBonusLuck;
                }
                break;
            }
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return CO_JUGGER.d2dBonusLuck;
            }
        }
        return 0;
    };
	
    this.getBonusMisfortune = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_JUGGER.superPowerBonusMissfortune;
            case GameEnums.PowerMode_Power:
                return CO_JUGGER.powerBonusMissfortune;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return CO_JUGGER.d2dCoZoneBonusMissfortune;
                }
                break;
            }
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return CO_JUGGER.d2dBonusMissfortune;
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
                return CO_JUGGER.powerOffBonus;
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_JUGGER.d2dCoZoneOffBonus;
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
                return CO_JUGGER.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_JUGGER.d2dCoZoneDefBonus;
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
                return ["ZCOUNIT_AUTO_TANK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A robot-like commander in the Bolt Guard Army. No one knows his true identity!");
    };
    this.getHits = function(co)
    {
        return qsTr("Energy");
    };
    this.getMiss = function(co)
    {
        return qsTr("Static electricity");
    };
    this.getCODescription = function(co)
    {
        return qsTr("His units are wildly unpredictable. He has great luck, but also a lot of misfortune.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_JUGGER.d2dBonusLuck, CO_JUGGER.d2dBonusMissfortune]
        }
        var text = qsTr("\nSpecial Unit:\nAuto Tank\n") +
               qsTr("\nGlobal Effect: \nJugger's units gain +%0 luck and +%1 misfortune.") +
               qsTr("\n\nCO Zone Effect: \nJugger's units gain +%4% firepower, +%5% defence, +%2 luck, and +%3 misfortune.");
        text = replaceTextArgs(text, [values[0], values[1],
                                      CO_JUGGER.d2dCoZoneBonusLuck, CO_JUGGER.d2dCoZoneBonusMissfortune, CO_JUGGER.d2dCoZoneOffBonus, CO_JUGGER.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Jugger's units gain +%2% firepower, +%3% defence, +%0 luck, and +%1 misfortune.");
        text = replaceTextArgs(text, [CO_JUGGER.powerBonusLuck, CO_JUGGER.powerBonusMissfortune, CO_JUGGER.powerOffBonus, CO_JUGGER.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Overclock");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Jugger's units gain +%2% firepower, +%3% defence, +%0 luck, and +%1 misfortune.");
        text = replaceTextArgs(text, [CO_JUGGER.superPowerBonusLuck, CO_JUGGER.superPowerBonusMissfortune, CO_JUGGER.powerOffBonus, CO_JUGGER.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("System Crash");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Enemy: Prepare for mega hurtz."),
                qsTr("Memory: upgraded. Shell: shined. Ready to uh...roll."),
                qsTr("Enemy system purge initiated..."),
                qsTr("Blue screen of death!"),
                qsTr("Crushware loaded..."),
                qsTr("Approaching system meltdown.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Victory. Downloading party hat."),
                qsTr("Victory dance initiated."),
                qsTr("Jugger: Superior. Enemy: Lame.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Critical Error: Does not compute."),
                qsTr("Victory impossible! Units overwhelmed. Jugger must... Control-Alt-Delete.")];
    };
    this.getName = function()
    {
        return qsTr("Jugger");
    };
}

Constructor.prototype = CO;
var CO_JUGGER = new Constructor();
