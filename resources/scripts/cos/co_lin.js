var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(2);
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
            animation.setSound("power1.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, delay);
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
                audio.addMusic("resources/music/cos/lin.mp3", 3991, 61044);
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 1;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };

    this.superPowerVisionRange = 2;
    this.superPowerOffBonus = 50;
    this.superPowerBaseOffBonus = 10;
    this.superPowerDefBonus = 50;
    this.superPowerBaseDefBonus = 10;

    this.powerVisionRange = 1;
    this.powerOffBonus = 30;
    this.powerBaseOffBonus = 10;
    this.powerDefBonus = 30;
    this.powerBaseDefBonus = 10;

    this.d2dOffBonus = 0;
    this.d2dBaseOffBonus = 0;
    this.d2dDefBonus = 0;
    this.d2dBaseDefBonus = 0;

    this.d2dCoZoneOffBonus = 30;
    this.d2dCoZoneBaseOffBonus = 10;
    this.d2dCoZoneDefBonus = 30;
    this.d2dCoZoneBaseDefBonus = 10;

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
                if (!seaAirUnit)
                {
                    return CO_LIN.superPowerOffBonus;
                }
                return CO_LIN.superPowerBaseOffBonus;
            case GameEnums.PowerMode_Power:
                if (!seaAirUnit)
                {
                    return CO_LIN.powerOffBonus;
                }
                return CO_LIN.powerBaseOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (!seaAirUnit)
                    {
                        return CO_LIN.d2dCoZoneOffBonus;
                    }
                    return CO_LIN.d2dCoZoneBaseOffBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    if (!seaAirUnit)
                    {
                        return CO_LIN.d2dOffBonus;
                    }
                    return CO_LIN.d2dBaseOffBonus;
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
            var seaAirUnit = (defender.getUnitType() === GameEnums.UnitType_Air) ||
                    (defender.getUnitType() === GameEnums.UnitType_Naval);
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (!seaAirUnit)
                {
                    return CO_LIN.superPowerDefBonus;
                }
                return CO_LIN.superPowerBaseDefBonus;
            case GameEnums.PowerMode_Power:
                if (!seaAirUnit)
                {
                    return CO_LIN.powerDefBonus;
                }
                return CO_LIN.powerBaseDefBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), defender))
                {
                    if (!seaAirUnit)
                    {
                        return CO_LIN.d2dCoZoneDefBonus;
                    }
                    return CO_LIN.d2dCoZoneBaseDefBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    if (!seaAirUnit)
                    {
                        return CO_LIN.d2dDefBonus;
                    }
                    return CO_LIN.d2dBaseDefBonus;
                }
            }
        }
        return 0;
    };

    this.getVisionrangeModifier = function(co, unit, posX, posY, map)
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
                    return CO_LIN.superPowerVisionRange;
                }
                else
                {
                    return 0;
                }

            case GameEnums.PowerMode_Power:                
                if (!seaAirUnit)
                {
                    return CO_LIN.powerVisionRange;
                }
                else
                {
                    return 0;
                }
            default:
                return 0;
            }
        }
        return 0;
    };

    this.getPerfectVision = function(co, map)
    {
        if (CO.isActive(co))
        {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return true;
            default:
                return false;
        }
        }
        return false;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        var seaAirUnit = (unit.getUnitType() === GameEnums.UnitType_Air) ||
                         (unit.getUnitType() === GameEnums.UnitType_Naval);

        if (!seaAirUnit)
        {
            return 2;
        }
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
                return ["ZCOUNIT_INTEL_TRUCK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A calm commander of Amber Corona. Believes that in battle, the clearest thinker wins.");
    };
    this.getHits = function(co)
    {
        return qsTr("Teasing Will");
    };
    this.getMiss = function(co)
    {
        return qsTr("Flying");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Her ground units are strong. Other units have normal power.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_LIN.d2dOffBonus, CO_LIN.d2dDefBonus];
        }
        var text = qsTr("\nSpecial Unit:\nIntel truck\n") +
               qsTr("\nGlobal Effect: \nLin's ground units gain +%0% firepower and +%1% defence.") +
               qsTr("\n\nCO Zone Effect: \nLin's ground units gain +%2% firepower and +%3% defence. Her other units gain +%4% firepower and +%5% defence.");
        text = replaceTextArgs(text, [values[0], values[1],
                                      CO_LIN.d2dCoZoneOffBonus, CO_LIN.d2dCoZoneDefBonus, CO_LIN.d2dCoZoneBaseOffBonus, CO_LIN.d2dCoZoneBaseDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Lin's ground units gain +%1% firepower, +%2% defence, and +%0 vision. Her other units gain +%3% firepower and +%4% defence. All of her units can see into hidden locations.");
        text = replaceTextArgs(text, [CO_LIN.powerVisionRange,
                                      CO_LIN.powerOffBonus, CO_LIN.powerDefBonus, CO_LIN.powerBaseOffBonus, CO_LIN.powerBaseDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Preliminary Scout");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Lin's ground units gain +%1% firepower, +%2% defence, and +%0 vision. Her other units gain +%3% firepower and +%4% defence. All of her units can see into hidden locations.");
        text = replaceTextArgs(text, [CO_LIN.superPowerVisionRange,
                                      CO_LIN.superPowerOffBonus, CO_LIN.superPowerDefBonus, CO_LIN.superPowerBaseOffBonus, CO_LIN.superPowerBaseDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Night Vision");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Begin search and destroy operations. Bring the fight to them."),
                qsTr("It's not about firepower. The force with the most information wins."),
                qsTr("I see enemies in the vicinity."),
                qsTr("All units, move out!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Now we have some breathing space..."),
                qsTr("I predicted this outcome...")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("We didn't have enough intel."),
                qsTr("Next time, I will be better prepared.")];
    };
    this.getName = function()
    {
        return qsTr("Lin");
    };
}

Constructor.prototype = CO;
var CO_LIN = new Constructor();
