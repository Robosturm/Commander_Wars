var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(6);
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
                audio.addMusic("resources/music/cos/sami.ogg", 1934, 62918);
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
            if (unit.getUnitType() === GameEnums.UnitType_Infantry)
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
                    animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unit.getUnitType() === GameEnums.UnitType_Infantry)
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

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };

    this.superPowerOffBonus = 80;
    this.superpowerMovementPoints = 2;
    this.superpowerCaptureMultiplier = 20;

    this.powerOffBonus = 50;
    this.powerDefBonus = 10;
    this.powerBaseOffBonus = 10;
    this.powerMovementPoints = 1;
    this.powerMovementCostModifier = 0;

    this.d2dCoZoneOffBonus = 50;
    this.d2dCoZoneBaseOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.d2dInfOffBonus = 20;
    this.d2dInfDefBonus = 0;
    this.d2dDirectOffBonus = -10;
    this.d2dCaptureMultiplier = 0.5;
    this.d2dTransporterMovementPoints = 1;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    return CO_SAMI.superPowerOffBonus;
                }
                else if (attacker.getBaseMaxRange() === 1)
                {
                    return CO_SAMI.powerBaseOffBonus + CO_SAMI.d2dDirectOffBonus;
                }
                return CO_SAMI.powerBaseOffBonus;
            case GameEnums.PowerMode_Power:
                if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    return CO_SAMI.powerOffBonus;
                }
                else if (attacker.getBaseMaxRange() === 1)
                {
                    return CO_SAMI.powerBaseOffBonus + CO_SAMI.d2dDirectOffBonus;
                }
                return CO_SAMI.powerBaseOffBonus;
            default:                
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                    {
                        return CO_SAMI.d2dCoZoneOffBonus;
                    }
                    else if (attacker.getBaseMaxRange() === 1)
                    {
                        return CO_SAMI.d2dCoZoneBaseOffBonus + CO_SAMI.d2dDirectOffBonus;
                    }
                    return CO_SAMI.d2dCoZoneBaseOffBonus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        return CO_SAMI.d2dInfOffBonus;
                    }
                }
                break;
            }
            if (attacker.getBaseMaxRange() === 1)
            {
                if (map === null ||
                    (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_SAMI.d2dDirectOffBonus;
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
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
				if (defender.getUnitType() === GameEnums.UnitType_Infantry)
				{
					return CO_SAMI.powerDefBonus + CO_SAMI.d2dInfDefBonus;
				}
				return CO_SAMI.powerDefBonus;
            }            
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_SAMI.d2dCoZoneDefBonus;
            }
            else if (defender.getUnitType() === GameEnums.UnitType_Infantry)
            {
                if (map === null ||
                    (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_SAMI.d2dInfDefBonus;
                }
            }
        }
        return 0;
    };
    this.getCaptureBonus = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            var hp = unit.getHpRounded();
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                    co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return hp * CO_SAMI.superpowerCaptureMultiplier;
            }
            else if (map === null ||
                     (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return hp * CO_SAMI.d2dCaptureMultiplier;
            }
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.isTransporter())
            {
                return CO_SAMI.d2dTransporterMovementPoints;
            }
            if (unit.getUnitType() === GameEnums.UnitType_Infantry)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return CO_SAMI.superpowerMovementPoints;
                case GameEnums.PowerMode_Power:
                    return CO_SAMI.powerMovementPoints;
                default:
                    return 0;
                }
            }
        }
        return 0;
    };
    this.getMovementcostModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.getOwner() === co.getOwner() &&
                unit.getUnitType() === GameEnums.UnitType_Infantry)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                case GameEnums.PowerMode_Power:
                    return CO_SAMI.powerMovementCostModifier;
                default:
                    return 0;
                }
            }
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getUnitID() !== "INFANTRY")
        {
            if (unit.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 8;
            }
            else if (unit.getBaseMaxRange() === 1)
            {
                return -1;
            }
        }
        return 2;
    };
    this.getAiCoBuildRatioModifier = function(co, map)
    {
        return 0.5;
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
                return ["ZCOUNIT_COMMANDO"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A strong-willed Orange Star special forces captain who loves long hair, despite having short hair for some reason.");
    };
    this.getHits = function(co)
    {
        return qsTr("Chocolate");
    };
    this.getMiss = function(co)
    {
        return qsTr("Cowards");
    };
    this.getCODescription = function(co)
    {
        return qsTr("As an infantry specialist, her footsoldiers do more damage and capture faster. Her other direct-combat units have weaker firepower.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0, 0, 0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_SAMI.d2dTransporterMovementPoints, CO_SAMI.d2dCaptureMultiplier * 100, CO_SAMI.d2dInfOffBonus, CO_SAMI.d2dInfDefBonus, CO_SAMI.d2dDirectOffBonus];
        }
        var text = qsTr("\nSpecial Unit:\nCommando\n") +
               qsTr("\nGlobal Effect: \nSami's footsoldiers gain +%2% firepower, +%3% defence, and a +%1% capture bonus. Her non-combat transporter units have +%0 movement. Her other direct-combat units have %4% firepower.") +
               qsTr("\n\nCO Zone Effect: \nSami's footsoldiers gain +%5% firepower. Her other direct units have -%6% firepower. Her indirect units gain +%7% firepower. All of her units gain +%8% defence.");
        text = replaceTextArgs(text, [values[0], values[1], values[2], values[3], values[4], CO_SAMI.d2dCoZoneOffBonus, CO_SAMI.d2dCoZoneBaseOffBonus+CO_SAMI.d2dDirectOffBonus, CO_SAMI.d2dCoZoneBaseOffBonus, CO_SAMI.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Sami's footsoldiers gain +%0 movement and +%1% firepower. Her other direct units have -%2% firepower. Her indirect units gain +%3% firepower. All of her units gain +%4% defence.");
        text = replaceTextArgs(text, [CO_SAMI.powerMovementPoints, CO_SAMI.powerOffBonus, CO_SAMI.powerBaseOffBonus+CO_SAMI.d2dDirectOffBonus, CO_SAMI.powerBaseOffBonus, CO_SAMI.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Double Time");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Sami's footsoldiers gain +%1 movement, +%2% firepower, and an outrageous +%0% capture bonus. Her other direct units have -%3% firepower. Her indirect units gain +%4% firepower. All of her units gain +%5% defence.");
        text = replaceTextArgs(text, [CO_SAMI.superpowerCaptureMultiplier * 100, CO_SAMI.superpowerMovementPoints, CO_SAMI.superPowerOffBonus, CO_SAMI.powerBaseOffBonus+CO_SAMI.d2dDirectOffBonus, CO_SAMI.powerBaseOffBonus, CO_SAMI.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Victory March");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("You're not bad!  Now it's my turn!"),
                qsTr("All right!  Time to end this!"),
                qsTr("Infantry... Assault!"),
                qsTr("Ready or not, here I come!"),
                qsTr("All right, it's make-or-break time!"),
                qsTr("Move out, grunts!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Mission accomplished! Awaiting further orders!"),
                qsTr("Commandos always complete their mission."),
                qsTr("Score one for the grunts!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Things would be easier if we had more infantry units..."),
                qsTr("Next time's for real. I won't lose focus.")];
    };
    this.getName = function()
    {
        return qsTr("Sami");
    };
}

Constructor.prototype = CO;
var CO_SAMI = new Constructor();
