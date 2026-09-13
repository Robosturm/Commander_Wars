var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3"];
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
                audio.addMusic("resources/music/cos/power.ogg", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.ogg", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/max.ogg", 57, 70080)
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setUnitFilter((unit, map) => 
            unit.getBaseMaxRange() === 1 && unit.getUnitType() !== GameEnums.UnitType_Infantry && unit.hasWeapons()
        );
        ownUnitsBuilder.setSound("power6.wav");
        ownUnitsBuilder.setSprite("power6");
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setUnitFilter((unit, map) => 
            unit.getBaseMaxRange() === 1 && unit.getUnitType() !== GameEnums.UnitType_Infantry && unit.hasWeapons()
        );
        ownUnitsBuilder.setSounds(["power12_1.wav", "power12_2.wav"]);
        ownUnitsBuilder.setSprite("power12");
        ownUnitsBuilder.setAnimationAmount(7);
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };

    this.superPowerOffBonus = 70;
    this.superpowerMovementBonus = 2;
    this.powerOffBonus = 45;
    this.powerMovementBonus = 1
    this.powerIndirectOffBonus = 0;
    this.powerOtherBonus = 10;
    this.powerDefBonus = 10;

    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneOffBonus = 45;
    this.d2dCoZoneIndirectOffBonus = 0;
    this.d2dCoZoneOtherOffBonus = 10;

    this.d2dIndirectFirerangeMalus = 1;
    this.d2dOffBonus = 20;
    this.d2dIndirectOffBonus = -10;
    this.d2dOtherOffBonus = 0;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var isDirect = (attacker.getBaseMaxRange() === 1 && attacker.getUnitType() !== GameEnums.UnitType_Infantry && attacker.hasWeapons());
            var isIndirect = (attacker.getBaseMaxRange() > 1);
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                {
                    if (isDirect)
                    {
                        return CO_MAX.superPowerOffBonus;
                    }
                    else if (isIndirect)
                    {
                        return CO_MAX.powerIndirectOffBonus;
                    }
                    return CO_MAX.powerOtherBonus;
                }
                case GameEnums.PowerMode_Power:
                {
                    if (isDirect)
                    {
                        return CO_MAX.powerOffBonus;
                    }
                    else if (isIndirect)
                    {
                        return CO_MAX.powerIndirectOffBonus;
                    }
                    return CO_MAX.powerOtherBonus;
                }
                default:
                {
                    var inCoZone = co.inCORange(Qt.point(atkPosX, atkPosY), attacker);
                    if (inCoZone)
                    {
                        if (isDirect)
                        {
                            return CO_MAX.d2dCoZoneOffBonus;
                        }
                        else if (isIndirect)
                        {
                            return CO_MAX.d2dCoZoneIndirectOffBonus;
                        }
                        else
                        {
                            return CO_MAX.d2dCoZoneOtherOffBonus;
                        }
                    }
                    else if (map === null ||
                             (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        if (isDirect)
                        {
                            return CO_MAX.d2dOffBonus;
                        }
                        else if (isIndirect)
                        {
                            return CO_MAX.d2dIndirectOffBonus;
                        }
                        else
                        {
                            return CO_MAX.d2dOtherOffBonus;
                        }
                    }
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
                return CO_MAX.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_MAX.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                if (unit.getBaseMaxRange() > 1 && unit.hasWeapons())
                {
                    return -CO_MAX.d2dIndirectFirerangeMalus;
                }
            }
        }
        return 0;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                if (unit.getBaseMaxRange() === 1 &&
                    unit.getUnitType() !== GameEnums.UnitType_Infantry &&
                    unit.hasWeapons())
                {
                    return CO_MAX.powerMovementBonus;
                }
            }
            else if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                     co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                if (unit.getBaseMaxRange() === 1 &&
                    unit.getUnitType() !== GameEnums.UnitType_Infantry &&
                    unit.hasWeapons())
                {
                    return CO_MAX.superpowerMovementBonus;
                }
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getBaseMaxRange() === 1 &&
            unit.getUnitType() !== GameEnums.UnitType_Infantry)
        {
            return 3;
        }
        else if (unit.getBaseMaxRange() > 1)
        {
            return -3;
        }
        return 1;
    };

    this.getAiCoBuildRatioModifier = function(co, map)
    {
        return 10;
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
                return ["ZCOUNIT_TANK_HUNTER"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A brave and loyal friend, not to mention a strong fighter. Max hates any kind of treachery, preferring a good, old-fashioned brawl.");
    };
    this.getHits = function(co)
    {
        return qsTr("Weight training");
    };
    this.getMiss = function(co)
    {
        return qsTr("Studying");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Non-footsoldier direct combat units are tops.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_MAX.d2dOffBonus, CO_MAX.d2dIndirectFirerangeMalus, CO_MAX.d2dIndirectOffBonus];
        }
        var text = qsTr("\nSpecial Unit:\nTank Hunter\n") +
            qsTr("\nGlobal Effect: \nMax's non-footsoldier direct combat units gain +%0% firepower. His indirect combat units lose -%1 range and have %2% firepower.") +
            qsTr("\n\nCO Zone Effect: \nMax's non-footsoldier direct combat units gain +%3% firepower. His indirect combat units have -%5% firepower. His footsoldiers gain +%4% firepower. All of his units gain +%6% defence.");
        text = replaceTextArgs(text, [values[0], values[1], values[2],
                                      CO_MAX.d2dCoZoneOffBonus, CO_MAX.d2dCoZoneOtherOffBonus, CO_MAX.d2dCoZoneIndirectOffBonus, CO_MAX.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text =  qsTr("Max's non-footsoldier direct combat units gain +%1 movement and +%0% firepower. His indirect combat units have -%4% firepower. His footsoldiers gain +%2% firepower. All of his units gain +%3% defence.");
        text = replaceTextArgs(text, [CO_MAX.powerOffBonus, CO_MAX.powerMovementBonus, CO_MAX.powerOtherBonus, CO_MAX.powerDefBonus, CO_MAX.powerIndirectOffBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Max Force");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Max's non-footsoldier direct combat units gain +%1 movement and +%0% firepower. His indirect combat units have -%4% firepower. His footsoldiers gain +%3% firepower. All of his units gain +%2% defence.");
        text = replaceTextArgs(text, [CO_MAX.superPowerOffBonus, CO_MAX.superpowerMovementBonus, CO_MAX.powerDefBonus, CO_MAX.powerOtherBonus, CO_MAX.powerIndirectOffBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Max Blast");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Roll, tanks, roll!"),
                qsTr("Now you're gonna get hurt!"),
                qsTr("Hey! Give up while you still can!"),
                qsTr("Wanna test might? I won't lose!"),
                qsTr("That's enough! Get outta the road!"),
                qsTr("Alright, the gloves are comin' off.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("That was a piece of cake!"),
                qsTr("Ha! It'll take more than that to beat me!"),
                qsTr("I'm on a roll!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Ouch... I let my guard down."),
                qsTr("Oh, man! Not good! What are we supposed to do now!?")];
    };
    this.getName = function()
    {
        return qsTr("Max");
    };
}

Constructor.prototype = CO;
var CO_MAX = new Constructor();
