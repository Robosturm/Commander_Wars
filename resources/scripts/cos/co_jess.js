var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
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
                audio.addMusic("resources/music/cos/jess.mp3", 44504, 109979)
                break;
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
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                    (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                    (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
                {
                    return 70;
                }
                else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval))
                {
                    return 0;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                    (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                    (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
                {
                    return 50;
                }
                else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval))
                {
                    return 0;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                        (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                        (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
                    {

                        return 40;
                    }
                    else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                             (attacker.getUnitType() === GameEnums.UnitType_Naval))
                    {
                        return 0;
                    }
                    return 10;
                }
                break;
        }
        if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
            (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
            (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
        {
            return 10;
        }
        else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
            (attacker.getUnitType() === GameEnums.UnitType_Naval))
        {
            return -10;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if ((unit.getUnitType() !== GameEnums.UnitType_Air) &&
                (unit.getUnitType() !== GameEnums.UnitType_Naval) &&
                (unit.getUnitType() !== GameEnums.UnitType_Infantry))
            {
                return 1;
            }
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                 co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if ((unit.getUnitType() !== GameEnums.UnitType_Air) &&
                (unit.getUnitType() !== GameEnums.UnitType_Naval) &&
                (unit.getUnitType() !== GameEnums.UnitType_Infantry))
            {
                return 2;
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
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_TANK_HUNTER"];
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
        return qsTr("\nSpecial Unit:\nTank Hunter\n") +
               qsTr("\nGlobal Effect: \nAir and Sea Units are weaker and Ground Units have increased firepower.") +
               qsTr("\n\nCO Zone Effect: \nGround Units have increased firepower.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Movement range of vehicles increases by one space and their firepower increases. All units' fuel and ammunition supplies are replenished.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Turbo Charge");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Movement range of vehicles increases by two spaces and their firepower greatly increases. All units' fuel and ammunition supplies are replenished.");
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
