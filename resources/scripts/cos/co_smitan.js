var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(5);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
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
                audio.addMusic("resources/music/cos/smitan.ogg", 578, 70397);
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setUnitFilter((unit, map) => 
            unit.getBaseMaxRange() > 1
        );
        ownUnitsBuilder.setSounds(["power9_1.wav", "power9_2.wav"]);
        ownUnitsBuilder.setSprite("power9");
        ownUnitsBuilder.setPerAnimationFunction((unit, animation, map) => 
            {
                var variables = unit.getVariables();
                var variable = variables.createVariable("SMITAN_ATTACK_COUNT");
                variable.writeDataInt32(1);
            }
        );
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setUnitFilter((unit, map) => 
            unit.getBaseMaxRange() > 1
        );
        ownUnitsBuilder.setSounds(["power12_1.wav", "power12_2.wav"]);
        ownUnitsBuilder.setSprite("power12");
        ownUnitsBuilder.setAnimationAmount(7);
        ownUnitsBuilder.setPerAnimationFunction((unit, animation, map) => 
            {
                var variables = unit.getVariables();
                var variable = variables.createVariable("SMITAN_ATTACK_COUNT");
                variable.writeDataInt32(2);
            }
        );
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co))
        {
            if (gotAttacked === false && attacker.getOwner() === co.getOwner())
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                case GameEnums.PowerMode_Power:
                    if (attacker.getBaseMaxRange() > 1)
                    {
                        var variables = attacker.getVariables();
                        var variable = variables.createVariable("SMITAN_ATTACK_COUNT");
                        var counter = variable.readDataInt32();
                        counter--;
                        variable.writeDataInt32(counter);
                        // enable unit for another move
                        attacker.setHasMoved(false);
                    }
                    break;
                default:
                    break;
                }
            }
        }
    };

    this.getActionModifierList = function(co, unit, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (unit.getBaseMaxRange() > 1)
                {
                    var variables = unit.getVariables();
                    var variable = variables.createVariable("SMITAN_ATTACK_COUNT");
                    var counter = variable.readDataInt32();
                    if (counter <= 0)
                    {
                        return ["-ACTION_FIRE"];
                    }
                }
                break;
            default:
                break;
            }
        }
        return [];
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };

    this.superPowerFirerangeModifier = 2;
    this.superPowerOffBonus = 20;

    this.powerOffBonus = 10;
    this.powerBaseOffBonus = 10;
    this.powerDefBonus = 10;
    this.powerFirerangeModifier = 1;

    this.d2dOffBonus = 5;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneBaseOffBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var inRangeCount = 0;
            if (attacker.getBaseMaxRange() === 1)
            {
                var units = co.getOwner().getUnits();
                var size = units.size();
                for (var i = 0; i < size; i++)
                {
                    var unit = units.at(i);
                    var x = unit.getX();
                    var y = unit.getY();
                    var distance = Math.abs(x - defPosX) + Math.abs(y - defPosY);
                    if (unit.getBaseMaxRange() > 1)
                    {
                        if (unit.getMinRange(Qt.point(x, y)) <= distance && distance <= unit.getMaxRange(unit.getPosition()))
                        {
                            inRangeCount += 1;
                        }
                    }
                }
            }

            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getBaseMaxRange() === 1)
                {
                    return inRangeCount * CO_SMITAN.superPowerOffBonus + CO_SMITAN.powerBaseOffBonus;
                }
                else
                {
                    return CO_SMITAN.powerBaseOffBonus;
                }
            case GameEnums.PowerMode_Power:
                if (attacker.getBaseMaxRange() === 1)
                {
                    return inRangeCount * CO_SMITAN.powerOffBonus + CO_SMITAN.powerBaseOffBonus;
                }
                else
                {
                    return CO_SMITAN.powerBaseOffBonus;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getBaseMaxRange() === 1)
                    {
                        return inRangeCount * CO_SMITAN.d2dCoZoneOffBonus + CO_SMITAN.d2dCoZoneBaseOffBonus;
                    }
                    return CO_SMITAN.d2dCoZoneBaseOffBonus;
                }
                else if (attacker.getBaseMaxRange() === 1)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        return inRangeCount * CO_SMITAN.d2dOffBonus;
                    }
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
                return CO_SMITAN.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_SMITAN.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (unit.getBaseMaxRange() > 1)
                {
                    return CO_SMITAN.superPowerFirerangeModifier;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (unit.getBaseMaxRange() > 1)
                {
                    return CO_SMITAN.powerFirerangeModifier;
                }
                break;
            default:
                break;
            }
        }
        return 0;
    };
    this.getCOArmy = function()
    {
        return "GE";
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getBaseMaxRange() > 1)
        {
            return 4;
        }
        return 1;
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
                return ["ZCOUNIT_SIEGE_CANNON"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A violent old commander of the Green Earth army who hates limitations.");
    };
    this.getHits = function(co)
    {
        return qsTr("Dominance, yelling");
    };
    this.getMiss = function(co)
    {
        return qsTr("(Victory by) Surrender");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Direct units gain additional firepower against enemy units which are in range of indirect units.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_SMITAN.d2dOffBonus];
        }
        var text = qsTr("\nSpecial Unit:\nSiege Cannon\n") +
               qsTr("\nGlobal Effect: \nWhen attacking an enemy unit, Smitan's direct units gain +%0% firepower for each indirect unit in range of the target.") +
               qsTr("\n\nCO Zone Effect: \nSmitan's units gain +%1% firepower and +%2% defence. When attacking an enemy unit, Smitan's direct units gain an additional +%3% firepower for each indirect unit in range of the target.");
        text = replaceTextArgs(text, [values[0], CO_SMITAN.d2dCoZoneBaseOffBonus, CO_SMITAN.d2dCoZoneDefBonus, CO_SMITAN.d2dCoZoneOffBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Smitan's units gain +%0% firepower and +%1% defence. Smitan's indirect units gain +%2 range and can move after firing. When attacking an enemy unit, Smitan's direct units gain an additional +%3% firepower for each indirect unit in range of the target.");
		text = replaceTextArgs(text, [CO_SMITAN.powerBaseOffBonus, CO_SMITAN.powerDefBonus, CO_SMITAN.powerFirerangeModifier, CO_SMITAN.powerOffBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Scramble Tactics");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Smitan's units gain +%0% firepower and +%1% defence. Smitan's indirect units gain +%2 range, can attack twice, and can move after firing. When attacking an enemy unit, Smitan's direct units gain an additional +%3% firepower for each indirect unit in range of the target.");
		text = replaceTextArgs(text, [CO_SMITAN.powerBaseOffBonus, CO_SMITAN.powerDefBonus, CO_SMITAN.superPowerFirerangeModifier, CO_SMITAN.superPowerOffBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Flare Drive");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Whites of their eyes? Pahaha!"),
                qsTr("It's over! Get used to it!"),
                qsTr("Fire, you idiots! FIRE!"),
                qsTr("What're you waiting for?! SHOOT them!"),
                qsTr("'Calm before the storm'? Nonsense."),
                qsTr("No more games! All units, full power!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Wasn't even any dust to settle."),
                qsTr("Grah hah hah. Ooh, that was FUN.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Grah hah hah! Ooh! ...What? What do y'mean, 'I lost'?"),
                qsTr("Take this! And that! And... What, I lost?")];
    };
    this.getName = function()
    {
        return qsTr("Smitan");
    };
}

Constructor.prototype = CO;
var CO_SMITAN = new Constructor();
