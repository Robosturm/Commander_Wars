var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAtUnitCount(co, powerSurplus, turnMode, directUnits);
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
                audio.addMusic("resources/music/cos/jess.mp3", 44504, 109979)
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
            unit.refill();

            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            animation.setSound("power0.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
            unit.refill();

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
        return "GE";
    };

    this.superPowerMovementBonus = 2;
    this.superPowerGroundBonus = 70;
    this.superPowerNavalAirBonus = 0;
    this.superPowerInfBonus = 10;

    this.powerMovementBonus = 1;
    this.powerDefBonus = 10;
    this.powerOffBonus = 10;
    this.powerGroundBonus = 50;
    this.powerNavalAirBonus = 0;
    this.powerInfBonus = 10;

    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneGroundBonus = 40;
    this.d2dCoZoneNavalAirBonus = 0;
    this.d2dCoZoneInfBonus = 10;

    this.d2dGroundBonus = 10;
    this.d2dNavalAirBonus = -10;
    this.d2dInfBonus = 0;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            {
                if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                    (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                    (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
                {
                    return CO_JESS.superPowerGroundBonus;
                }
                else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval))
                {
                    return CO_JESS.superPowerNavalAirBonus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    return CO_JESS.superPowerInfBonus;
                }
                return CO_JESS.powerOffBonus;
            }
            case GameEnums.PowerMode_Power:
            {
                if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                        (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                        (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
                {
                    return CO_JESS.powerGroundBonus;
                }
                else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval))
                {
                    return CO_JESS.powerNavalAirBonus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    return CO_JESS.powerInfBonus;
                }
                return CO_JESS.powerOffBonus;
            }
            default:
            {
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                        (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                        (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
                    {

                        return CO_JESS.d2dCoZoneGroundBonus;
                    }
                    else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                             (attacker.getUnitType() === GameEnums.UnitType_Naval))
                    {
                        return CO_JESS.d2dCoZoneNavalAirBonus;
                    }
                    else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                    {
                        return CO_JESS.d2dCoZoneInfBonus;
                    }
                    return CO_JESS.d2dCoZoneOffBonus;
                }
                break;
            }
            }
            if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
            {
                return CO_JESS.d2dGroundBonus;
            }
            else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                     (attacker.getUnitType() === GameEnums.UnitType_Naval))
            {
                return CO_JESS.d2dNavalAirBonus;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return CO_JESS.d2dInfBonus;
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
                return CO_JESS.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_JESS.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                if ((unit.getUnitType() !== GameEnums.UnitType_Air) &&
                        (unit.getUnitType() !== GameEnums.UnitType_Naval) &&
                        (unit.getUnitType() !== GameEnums.UnitType_Infantry))
                {
                    return CO_JESS.powerMovementBonus;
                }
            }
            else if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                     co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                if ((unit.getUnitType() !== GameEnums.UnitType_Air) &&
                        (unit.getUnitType() !== GameEnums.UnitType_Naval) &&
                        (unit.getUnitType() !== GameEnums.UnitType_Infantry))
                {
                    return CO_JESS.superPowerMovementBonus;
                }
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if ((unit.getUnitType() === GameEnums.UnitType_Air) ||
            (unit.getUnitType() === GameEnums.UnitType_Naval))
        {
            return -2;
        }
        else if (unit.getUnitType() !== GameEnums.UnitType_Infantry)
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
                    buildingId === "HQ" ||
                    buildingId === "FORTHQ")
            {
                return ["ZCOUNIT_TANK_HUNTER"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A gallant tank-driving commander who excels at analyzing information. Often argues with Eagle.");
    };
    this.getHits = function(co)
    {
        return qsTr("Dandelions");
    };
    this.getMiss = function(co)
    {
        return qsTr("Unfit COs");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Vehicular units have superior firepower. Air and naval units are comparatively weak.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nTank Hunter\n") +
               qsTr("\nGlobal Effect: \nInfantry units get %0% firepower. Air and Sea Units get %1% firepower and Ground Units have increased firepower by %2%.") +
               qsTr("\n\nCO Zone Effect: \nInfantry units get %3% firepower. Air and Sea Units get %4% firepower and Ground Units have increased firepower by %5%.");
        text = replaceTextArgs(text, [CO_JESS.d2dInfBonus, CO_JESS.d2dNavalAirBonus, CO_JESS.d2dGroundBonus,
                                      CO_JESS.d2dCoZoneInfBonus, CO_JESS.d2dCoZoneNavalAirBonus, CO_JESS.d2dCoZoneGroundBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Infantry units get %0% firepower. Air and Sea Units get %1% firepower and Ground Units have increased firepower by %2%. Movement range of vehicles increases by %3 space. All units' fuel and ammunition supplies are replenished.");
        text = replaceTextArgs(text, [CO_JESS.powerInfBonus, CO_JESS.powerNavalAirBonus, CO_JESS.powerGroundBonus, CO_JESS.powerMovementBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Turbo Charge");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Infantry units get %0% firepower. Air and Sea Units get %1% firepower and Ground Units have increased firepower by %2%. Movement range of vehicles increases by %3 spaces. All units' fuel and ammunition supplies are replenished.");
        text = replaceTextArgs(text, [CO_JESS.superPowerInfBonus, CO_JESS.superPowerNavalAirBonus, CO_JESS.superPowerGroundBonus, CO_JESS.superPowerMovementBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Overdrive");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Battle should never be taken lightly. I always give 100%! Always!!!"),
                qsTr("I won't let this chance pass me by!"),
                qsTr("Weakness has no place on the battlefield!"),
                qsTr("Here's my chance..."),
                qsTr("Time to turn the tables..."),
                qsTr("I've been waiting for this moment!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("This battle will advance my tactical research..."),
                qsTr("Looks like I survived another day."),
                qsTr("We made it... I'm glad that's done.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is looking grim..."),
                qsTr("It was inevitable under these conditions.")];
    };
    this.getName = function()
    {
        return qsTr("Jess");
    };
}

Constructor.prototype = CO;
var CO_JESS = new Constructor();
