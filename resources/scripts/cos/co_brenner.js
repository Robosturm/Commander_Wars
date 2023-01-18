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
            animation.writeDataInt32(3);
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
            animation.writeDataInt32(5);
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
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_awdc.mp3", 0, 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_awdc.mp3", 0, 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/brenner.mp3", 631, 59458);
                break;
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
    this.coZoneBonus = 30;
    this.powerBonus = 30;
    this.superPowerBonus = 50;
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_BRENNER.superPowerBonus;
            case GameEnums.PowerMode_Power:
                return CO_BRENNER.powerBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_BRENNER.coZoneBonus;
                }
                break;
        }
        return 0;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_REPAIR_TANK"];
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
        return qsTr("He has faith in the goodness of humanity. Feels strong duty to help anyone who is in trouble.");
    };
    this.getHits = function(co)
    {
        return qsTr("Helping Others");
    };
    this.getMiss = function(co)
    {
        return qsTr("Abandoning Others");
    };
    this.getCODescription = function(co)
    {
        return qsTr("He specializes in defensive bonuses for his units.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nRepair Tanks\n\nGlobal Effect: \nNone") +
                   qsTr("\n\nCO Zone Effect: \nUnits gain %0% defense.");
        text = replaceTextArgs(text, [CO_BRENNER.coZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Heals 3 hp to all of his units and increases their defence.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Reinforce");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Heals 5 hp to all of his units and increases their defence by %0%.");
        text = replaceTextArgs(text, [CO_BRENNER.superPowerBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("First Aid");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I want everyone to get out here alive! Move!"),
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
