var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.isBoostUnit = function(unit)
    {
        var unitId = unit.getUnitID();
        var idList = ["K_HELI", "T_HELI", "WATERPLANE"];
        if (unit.getUnitType() === GameEnums.UnitType_Naval ||
            idList.indexOf(unitId) >= 0)
        {
            return true;
        }
        return false;
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
            if (i % 2 === 0)
            {
                animation.setSound("power8_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power8_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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

    this.loadCOMusic = function(co, map)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/greyfield.mp3", 3229, 64409);
                break;
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "TI";
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        var boostUnit = CO_GREYFIELD.isBoostUnit(attacker);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (boostUnit)
                {
                    return 70;
                }
                else
                {
                    return 10;
                }
            case GameEnums.PowerMode_Power:
                if (boostUnit)
                {
                    return 20;
                }
                else
                {
                    return 10;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (boostUnit)
                    {
                        return 20;
                    }
                    return 10;
                }
                break;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        var boostUnit = CO_GREYFIELD.isBoostUnit(defender);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (boostUnit)
                {
                    return 70;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (boostUnit)
                {
                    return 50;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    if (boostUnit)
                    {
                        return 50;
                    }
                    return 10;
                }
                break;
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        var boostUnit = CO_GREYFIELD.isBoostUnit(unit);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (boostUnit)
                {
                    return 1;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                break;
        }
        return 0;
    };

    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "HARBOUR" ||
            buildingId === "TEMPORARY_HARBOUR")
        {
            return ["ZCOUNIT_MISSILE_SUB"];
        }
        return [];
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (CO_GREYFIELD.isBoostUnit(unit))
        {
            return 6;
        }
        return 0;
    };
    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "HARBOUR" ||
            buildingId === "TEMPORARY_HARBOUR")
        {
            return ["ZCOUNIT_MISSILE_SUB"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Wants to rule as the supreme global leader. Ambitious and power-hungry.");
    };
    this.getHits = function(co)
    {
        return qsTr("Power");
    };
    this.getMiss = function(co)
    {
        return qsTr("Insubordinates");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Copter, seaplanes and sea units have higher firepower and defense.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nMissile Submarine\n") +
               qsTr("\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \n Sea Units have 20% offensive and 50% defensive bonus.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("All units get resupplied and his copter, seaplanes and sea units have higher firepower and defense.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Supply Chain");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("All units get resupplied and his copter, seaplanes and sea units have higher firepower and defense and increase movement range by 1.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Naval War");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I will brook no opposition. The enemy will be eliminated."),
                qsTr("Overwhelming forces need overwhelming supplies."),
                qsTr("Supplies are here!"),
                qsTr("Admiral Greyfield here. Attack and eliminate them.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Disloyalty will be punished with death."),
                qsTr("Anyone who opposes me will be crushed!"),
                qsTr("The Teal Isles Army will prevail.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Nooo... this is not the end of Teal Isles!"),
                qsTr("What? My army has been crushed.")];
    };
    this.getName = function()
    {
        return qsTr("Greyfield");
    };
}

Constructor.prototype = CO;
var CO_GREYFIELD = new Constructor();
