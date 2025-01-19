var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(2);
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        if (turnMode === GameEnums.AiTurnMode_StartOfDay)
        {
            if (co.canUseSuperpower())
            {
                return GameEnums.PowerMode_Superpower;
            }
            else if (powerSurplus <= 0.5 &&
                     co.canUsePower())
            {
                return CO.getAiUsePowerAtUnitCount(co, powerSurplus, turnMode, repairUnits);
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
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(CO_BRENNER.powerHeal);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            animation.setSound("power11.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(CO_BRENNER.superPowerHeal);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
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

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_awdc.ogg", 0, 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_awdc.ogg", 0, 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/brenner.ogg", 631, 59458);
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };

    this.superPowerHeal = 5;
    this.superPowerDefBonus = 50;

    this.powerHeal = 3;
    this.powerDefBonus = 30;
    this.powerOffBonus = 10;

    this.d2dCoZoneDefBonus = 30;
    this.d2dCoZoneOffBonus = 10;

    this.d2dDefBonus = 0;

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_BRENNER.superPowerDefBonus;
            case GameEnums.PowerMode_Power:
                return CO_BRENNER.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_BRENNER.d2dCoZoneDefBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_BRENNER.d2dDefBonus;
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
                return CO_BRENNER.powerOffBonus;
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_BRENNER.d2dCoZoneOffBonus;
            }
        }
        return 0;
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
                return ["ZCOUNIT_REPAIR_TANK"];
            }
        }
        return [];
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("He has faith in the goodness of humanity and feels a strong duty to help anyone who is in trouble.");
    };
    this.getHits = function(co)
    {
        return qsTr("Helping others");
    };
    this.getMiss = function(co)
    {
        return qsTr("Abandoning others");
    };
    this.getCODescription = function(co)
    {
        return qsTr("He specializes in defensive bonuses for his units.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_BRENNER.d2dDefBonus];
        }
        var text = qsTr("\nSpecial Unit:\nRepair Tanks\n\nGlobal Effect: \nBrenner's units gain +%0% defence.") +
                   qsTr("\n\nCO Zone Effect: \nBrenner's units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [values[0], CO_BRENNER.d2dCoZoneOffBonus, CO_BRENNER.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Restores +%0 HP to all of Brenner's units. His units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [CO_BRENNER.powerHeal, CO_BRENNER.powerOffBonus, CO_BRENNER.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Reinforce");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Restores +%0 HP to all of Brenner's units. His units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [CO_BRENNER.superPowerHeal, CO_BRENNER.powerOffBonus, CO_BRENNER.superPowerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Lifeline");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I want everyone to get out of here alive! Move!"),
                qsTr("It's time to go on the offensive!"),
                qsTr("Never give up!"),
                qsTr("Where there is life, there is hope!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("When will this senseless war end?"),
                qsTr("Let's tend to the injured first.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("We barely escaped, but at least we are alive."),
                qsTr("At least we can recover from this...")];
    };
    this.getName = function()
    {
        return qsTr("Brenner");
    };
}

Constructor.prototype = CO;
var CO_BRENNER = new Constructor();
