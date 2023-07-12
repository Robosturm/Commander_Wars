var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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
        var size = units.size();
        for (var i = 0; i < size; i++)
        {
            var unit = units.at(i);
            if (unit.getBaseMaxRange() === 1)
            {
                var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                var delay = globals.randInt(135, 265);
                if (animations.length < 5)
                {
                    delay *= i;
                }
                if (i % 2 === 0)
                {
                    animation.setSound("power10_1.wav", 1, delay);
                }
                else
                {
                    animation.setSound("power10_2.wav", 1, delay);
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    animations[counter].queueAnimation(animation);
                    animations[counter] = animation;
                    counter++;
                    if (counter >= animations.length)
                    {
                        counter = 0;
                    }
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
        var size = units.size();
        for (var i = 0; i < size; i++)
        {
            var unit = units.at(i);
            if (unit.getBaseMaxRange() === 1)
            {
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
        }
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
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
                audio.addMusic("resources/music/cos/will.mp3", 36421, 91424);
                break;
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

    this.superPowerOffBonus = 50;
    this.superPowerMovementBonus = 2;

    this.powerOffBaseBonus = 10;
    this.powerOffBonus = 30;
    this.powerDefBonus = 10;
    this.powerMovementBonus = 1;

    this.d2dOffBonus = 0;

    this.d2dCoZoneOffBonus = 30;
    this.d2dCoZoneBaseBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var seaAirUnit = (attacker.getUnitType() === GameEnums.UnitType_Air) ||
                    (attacker.getUnitType() === GameEnums.UnitType_Naval);
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
                {
                    return CO_WILL.superPowerOffBonus;
                }
                return CO_WILL.powerOffBaseBonus;
            case GameEnums.PowerMode_Power:
                if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
                {
                    return CO_WILL.powerOffBonus;
                }
                return CO_WILL.powerOffBaseBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
                    {
                        return CO_WILL.d2dCoZoneOffBonus;
                    }
                    return CO_WILL.d2dCoZoneBaseBonus;
                }
                else if (attacker.getBaseMaxRange() === 1 && !seaAirUnit)
                {
                    return CO_WILL.d2dOffBonus;
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
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_WILL.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_WILL.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            var seaAirUnit = (unit.getUnitType() === GameEnums.UnitType_Air) ||
                             (unit.getUnitType() === GameEnums.UnitType_Naval);
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (!seaAirUnit)
                {
                    return CO_WILL.superPowerMovementBonus;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                if (!seaAirUnit)
                {
                    return CO_WILL.powerMovementBonus;
                }
                return 0;
            default:
                return 0;
            }
        }
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        var seaAirUnit = (unit.getUnitType() === GameEnums.UnitType_Air) ||
                         (unit.getUnitType() === GameEnums.UnitType_Naval);
        if (unit.getBaseMaxRange() === 1 && !seaAirUnit)
        {
            return 2;
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
                return ["ZCOUNIT_HOT_TANK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Former Brown Desert cadet. Joined Amber Corona after being rescued by Brenner.");
    };
    this.getHits = function(co)
    {
        return qsTr("Brenner, Isabella");
    };
    this.getMiss = function(co)
    {
        return qsTr("The Beast, Greyfield, Caulder");
    };
    this.getCODescription = function(co)
    {
        return qsTr("His direct ground units are stronger.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nHot Tank\n") +
               qsTr("\nGlobal Effect:\nWill's direct ground units have +%0% firepower.") +
               qsTr("\n\nCO Zone Effect: \nWill's direct ground units gain +%1% firepower. His other units gain +%2% firepower. All of his units gain +%3% defence.");
        text = replaceTextArgs(text, [CO_WILL.d2dOffBonus, CO_WILL.d2dCoZoneOffBonus, CO_WILL.d2dCoZoneBaseBonus, CO_WILL.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Will's ground units gain +%0 movement. His ground direct units gain +%1% firepower. His other units gain +%2% firepower. All of his units gain +%3% defence.");
        text = replaceTextArgs(text, [CO_WILL.powerMovementBonus, CO_WILL.powerOffBonus, CO_WILL.powerOffBaseBonus, CO_WILL.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Rally Cry");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Will's ground units gain +%0 movement. His ground direct units gain +%1% firepower. His other units gain +%2% firepower. All of his units gain +%3% defence.");
        text = replaceTextArgs(text, [CO_WILL.superPowerMovementBonus, CO_WILL.superPowerOffBonus, CO_WILL.powerOffBaseBonus, CO_WILL.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("New Era");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("This is our chance to win. Prepare to move out."),
                qsTr("The captain saved my life. I will not waste it."),
                qsTr("I'll never give up..."),
                qsTr("There's no time. It's now or never...")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I did it!"),
                qsTr("I'll never give up!"),
                qsTr("Is everyone okay?")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Never give up hope..."),
                qsTr("I can't remember anything...")];
    };
    this.getName = function()
    {
        return qsTr("Will");
    };
}

Constructor.prototype = CO;
var CO_WILL = new Constructor();
