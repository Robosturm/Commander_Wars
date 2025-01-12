var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAtEnd(co, powerSurplus, turnMode);
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(6);
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
            if (unit.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                if (CO_EAGLE.powerRefresh)
                {
                    unit.setHasMoved(false);
                }
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
            if (unit.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                unit.setHasMoved(false);
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
                audio.addMusic("resources/music/cos/power.ogg", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.ogg", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/eagle.ogg", 27, 67923)
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
        return "GE";
    };

    this.getFuelCostModifier = function(co, unit, x, y, costs, map)
    {
        if (CO.isActive(co))
        {
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                if (unit.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_EAGLE.d2dFuelModifier;
                }
            }
        }
        return 0;
    };

    this.superPowerAirOffBonus = 30;
    this.superPowerNavalOffBonus = 0;
    this.superPowerOtherOffBonus = 0;
    this.superPowerAirDefBonus = 20;
    this.superPowerDefBonus = 10;

    this.powerAirOffBonus = -30;
    this.powerNavalOffBonus = -40;
    this.powerInfOffBonus = 0;
    this.powerOtherOffBonus = -45;
    this.powerDefBonus = 10;
    this.powerAirDefBonus = 20;
    this.powerRefresh = true;

    this.d2dCoZoneAirDefBonus = 20;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneAirOffBonus = 30;
    this.d2dCoZoneNavalOffBonus = 0;
    this.d2dCoZoneOtherOffBonus = 10;

    this.d2dFuelModifier = -2;
    this.d2dAirDefBonus = 10;
    this.d2dAirOffBonus = 10;
    this.d2dNavalOffBonus = -10;
    this.d2dOtherOffBonus = 0;

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
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_EAGLE.superPowerAirOffBonus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_EAGLE.superPowerNavalOffBonus;
                }
                else
                {
                    return CO_EAGLE.superPowerOtherOffBonus;
                }
            }
            case GameEnums.PowerMode_Power:
            {
				if (CO_EAGLE.powerRefresh)
				{
					if (attacker.getUnitType() === GameEnums.UnitType_Air)
					{
						return CO_EAGLE.powerAirOffBonus;
					}
					else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
					{
						return CO_EAGLE.powerNavalOffBonus;
					}
					else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
					{
						return CO_EAGLE.powerInfOffBonus;
					}
					else
					{
						return CO_EAGLE.powerOtherOffBonus;
					}
				}
				else
				{
					if (attacker.getUnitType() === GameEnums.UnitType_Air)
					{
						return 0-CO_EAGLE.powerAirOffBonus; //variable is negative and the power should make it positive
					}
					else
					{
						return 0;
					}
				}
            }
            default:
            {
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return CO_EAGLE.d2dCoZoneAirOffBonus;
                    }
                    else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return CO_EAGLE.d2dCoZoneNavalOffBonus;
                    }
                    else
                    {
                        return CO_EAGLE.d2dCoZoneOtherOffBonus;
                    }
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return CO_EAGLE.d2dAirOffBonus;
                    }
                    else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return CO_EAGLE.d2dNavalOffBonus;
                    }
                    else
                    {
                        return CO_EAGLE.d2dOtherOffBonus;
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
            if (co.getPowerMode() > GameEnums.PowerMode_Superpower)
            {
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_EAGLE.superPowerAirDefBonus;
                }
                else
                {
                    return CO_EAGLE.superPowerDefBonus;
                }
            }
            else if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_EAGLE.powerAirDefBonus;
                }
                else
                {
                    return CO_EAGLE.powerDefBonus;
                }
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_EAGLE.d2dCoZoneAirDefBonus;
                }
                else
                {
                    return CO_EAGLE.d2dCoZoneDefBonus;
                }
            }
            else if (map === null ||
                     (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_EAGLE.d2dAirDefBonus;
                }
            }
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Naval)
        {
            return -2;
        }
        else if (unit.getUnitType() === GameEnums.UnitType_Air)
        {
            return 6;
        }
        return 1;
    };
    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "AIRPORT" ||
                    buildingId === "TEMPORARY_AIRPORT")
            {
                return ["ZCOUNIT_KIROV"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Green Earth's daring pilot hero. Joined the air force to honor his father's legacy.");
    };
    this.getHits = function(co)
    {
        return qsTr("His lucky goggles");
    };
    this.getMiss = function(co)
    {
        return qsTr("Swimming");
    };
    this.getCODescription = function(co)
    {
        return qsTr("He has superior air units that also use less fuel, however, his naval units are weaker.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0, 0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_EAGLE.d2dAirOffBonus, CO_EAGLE.d2dAirDefBonus, CO_EAGLE.d2dFuelModifier, CO_EAGLE.d2dNavalOffBonus];
        }
        var text = qsTr("\nSpecial Unit:\nKirov\n") +
                qsTr("\nGlobal Effect: \nEagle's air units gain +%0% firepower, +%1% defence, and use %2 less fuel per day, but his naval units have %3% firepower.") +
                qsTr("\n\nCO Zone Effect: \nEagle's air units gain +%4% firepower and +%5% defence. His naval units have -%6% firepower and +%7% defence. All of his other units gain +%8% firepower and +%7% defence.");
        text = replaceTextArgs(text, [values[0], values[1], values[2], values[3],
                                      CO_EAGLE.d2dCoZoneAirOffBonus, CO_EAGLE.d2dCoZoneDefBonus, CO_EAGLE.d2dCoZoneNavalOffBonus, CO_EAGLE.d2dCoZoneDefBonus, CO_EAGLE.d2dCoZoneOtherOffBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        if (CO_EAGLE.powerRefresh)
        {
            var text = qsTr("All of Eagle's non-footsoldier units that have already carried out orders this turn may move again. His air units have %0% firepower and gain +%1% defence. His naval units have %2% firepower and gain +%4% defence. All of his other units have %3% firepower and gain +%4% defence.");
            text = replaceTextArgs(text, [CO_EAGLE.powerAirOffBonus, CO_EAGLE.powerAirDefBonus, CO_EAGLE.powerNavalOffBonus, CO_EAGLE.powerOtherOffBonus, CO_EAGLE.powerDefBonus]);
            return text;
        }
        else
        {
            var text = qsTr("Eagle's air units gain +%0% firepower and +%1% defence. His other units gain +%2% defence.");
            text = replaceTextArgs(text, [(0-CO_EAGLE.powerAirOffBonus), CO_EAGLE.powerAirDefBonus, CO_EAGLE.powerDefBonus]);
            return text;
        }
    };
    this.getPowerName = function(co)
    {
        return qsTr("Lightning Drive");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("All of Eagle's non-footsoldier units that have already carried out orders this turn may move again. His air units gain +%0% firepower and +%1% defence. His naval units have -%2% firepower and gain +%4% defence. All of his other units gain +%3% firepower and +%4% defence.");
        text = replaceTextArgs(text, [CO_EAGLE.superPowerAirOffBonus, CO_EAGLE.superPowerAirDefBonus, CO_EAGLE.superPowerNavalOffBonus, CO_EAGLE.superPowerOtherOffBonus, CO_EAGLE.superPowerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Lightning Strike");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Checkmate!"),
                qsTr("All units, ready for attack! Don't even give them time for regret!"),
                qsTr("Do you think you can keep up?"),
                qsTr("The Eagle soars above you!"),
                qsTr("I have no interest in underlings. Begone!"),
                qsTr("Aha! I've caught you with your guard down!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Hmph! As expected!"),
                qsTr("Where's the challenge in that?"),
                qsTr("Another victory! Was there ever any doubt?")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is what happens when you lose focus!"),
                qsTr("Impossible! I'm getting angrier by the moment!")];
    };
    this.getName = function()
    {
        return qsTr("Eagle");
    };
}

Constructor.prototype = CO;
var CO_EAGLE = new Constructor();
