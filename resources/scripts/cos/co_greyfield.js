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
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_ids_dc.ogg", 0 , 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_ids_dc.ogg", 0 , 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.ogg", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/greyfield.ogg", 3229, 64409);
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
        return "TI";
    };

    this.superPowerMovementBonus = 1;
    this.superPowerOffBonus = 70;
    this.superPowerDefBonus = 70;

    this.powerOffBonus = 20;
    this.powerDefBonus = 50;
    this.powerBaseOffBonus = 10;
    this.powerBaseDefBonus = 10;

    this.d2dOffBonus = 0;
    this.d2dDefBonus = 20;

    this.d2dCoZoneBaseOffBonus = 10;
    this.d2dCoZoneBaseDefBonus = 10;
    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneDefBonus = 50;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var boostUnit = CO_GREYFIELD.isBoostUnit(attacker);
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            {
                if (boostUnit)
                {
                    return CO_GREYFIELD.superPowerOffBonus;
                }
                else
                {
                    return CO_GREYFIELD.powerBaseOffBonus;
                }
            }
            case GameEnums.PowerMode_Power:
            {
                if (boostUnit)
                {
                    return CO_GREYFIELD.powerOffBonus;
                }
                else
                {
                    return CO_GREYFIELD.powerBaseOffBonus;
                }
            }
            default:
            {
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (boostUnit)
                    {
                        return CO_GREYFIELD.d2dCoZoneOffBonus;
                    }
                    return CO_GREYFIELD.d2dCoZoneBaseOffBonus;
                }
                else if (boostUnit)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        return CO_GREYFIELD.d2dOffBonus;
                    }
                }
                return 0;
            }
            }
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var boostUnit = CO_GREYFIELD.isBoostUnit(defender);
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (boostUnit)
                {
                    return CO_GREYFIELD.superPowerDefBonus;
                }
                return CO_GREYFIELD.powerBaseDefBonus;
            case GameEnums.PowerMode_Power:
                if (boostUnit)
                {
                    return CO_GREYFIELD.powerDefBonus;
                }
                return CO_GREYFIELD.powerBaseDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    if (boostUnit)
                    {
                        return CO_GREYFIELD.d2dCoZoneDefBonus;
                    }
                    return CO_GREYFIELD.d2dCoZoneBaseDefBonus;
                }
                else if (boostUnit)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        return CO_GREYFIELD.d2dDefBonus;
                    }
                }
                return 0;
            }
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            var boostUnit = CO_GREYFIELD.isBoostUnit(unit);
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (boostUnit)
                {
                    return CO_GREYFIELD.superPowerMovementBonus;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                break;
            }
        }
        return 0;
    };

    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "HARBOUR" ||
                    buildingId === "TEMPORARY_HARBOUR")
            {
                return ["ZCOUNIT_MISSILE_SUB"];
            }
        }
        return [];
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (CO_GREYFIELD.isBoostUnit(unit))
        {
            return 6;
        }
        return 1;
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
        return qsTr("Copters, seaplanes and sea units are superior.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_GREYFIELD.d2dOffBonus, CO_GREYFIELD.d2dDefBonus];
        }
        var text = qsTr("\nSpecial Unit:\nMissile Submarine\n") +
               qsTr("\nGlobal Effect: \nGreyfield's copters, seaplanes, and naval units gain +%0% firepower and +%1% defence.") +
               qsTr("\n\nCO Zone Effect: \nGreyfield's copters, seaplanes, and naval units gain +%2% firepower and +%3% defence. His other units gain +%4% firepower and +%5% defence.");
        text = replaceTextArgs(text, [values[0], values[1],
                                      CO_GREYFIELD.d2dCoZoneOffBonus, CO_GREYFIELD.d2dCoZoneDefBonus, CO_GREYFIELD.d2dCoZoneBaseOffBonus, CO_GREYFIELD.d2dCoZoneBaseDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Greyfield's copters, seaplanes, and naval units gain +%0% firepower and +%1% defence. His other units gain +%2% firepower and +%3% defence. All of his units replenish all fuel and ammunition.");
        text = replaceTextArgs(text, [CO_GREYFIELD.powerOffBonus, CO_GREYFIELD.powerDefBonus, CO_GREYFIELD.powerBaseOffBonus, CO_GREYFIELD.powerBaseDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Supply Chain");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Greyfield's copters, seaplanes, and naval units gain +%0% firepower, +%1% defence, and +%2 movement. His other units gain +%3% firepower and +%4% defence. All of his units replenish all fuel and ammunition.");
        text = replaceTextArgs(text, [CO_GREYFIELD.superPowerOffBonus, CO_GREYFIELD.superPowerDefBonus, CO_GREYFIELD.superPowerMovementBonus, CO_GREYFIELD.powerBaseOffBonus, CO_GREYFIELD.powerBaseDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("High Command");
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
        return [qsTr("Nooo... This is not the end of Teal Isles!"),
                qsTr("What? My army has been crushed...")];
    };
    this.getName = function()
    {
        return qsTr("Greyfield");
    };
}

Constructor.prototype = CO;
var CO_GREYFIELD = new Constructor();
