var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3", "+alt4"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAlways(co, powerSurplus);
    };

    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_DRAKE.drakeDamage(co, CO_DRAKE.powerDamage, powerNameAnimation, map);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation = GameAnimationFactory.createAnimation(map, 0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation);

        map.getGameRules().changeWeather("WEATHER_RAIN", map.getPlayerCount() * 1);
        CO_DRAKE.drakeDamage(co, CO_DRAKE.superPowerDamage, animation, map);
    };

    this.drakeDamage = function(co, value, animation2, map)
    {
        var player = co.getOwner();
        var counter = 0;
        var playerCounter = map.getPlayerCount();
        var animation = null;
        var animations = [];
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                var units = enemyPlayer.getUnits();
                units.randomize();
                for (i = 0; i < units.size(); i++)
                {
                    var unit = units.at(i);

                    animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    var delay = globals.randInt(135, 265);
                    if (animations.length < 5)
                    {
                        delay *= i;
                    }
                    animation.setSound("power4.wav", 1, delay);
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        if (animation2 !== null)
                        {
                            animation2.queueAnimation(animation);
                        }
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        animations[counter].queueAnimation(animation);
                        animations[counter] = animation;
                        counter++;
                        if (counter >= animations.length)
                        {
                            counter = 0;
                        }
                    }
                    animation.writeDataInt32(unit.getX());
                    animation.writeDataInt32(unit.getY());
                    animation.writeDataInt32(value);
                    animation.setEndOfAnimationCall("CO_DRAKE", "postAnimationDamage");
                }
            }
        }
    };

    this.postAnimationDamage = function(postAnimation, map)
    {
        postAnimation.seekBuffer();
        var x = postAnimation.readDataInt32();
        var y = postAnimation.readDataInt32();
        var damage = postAnimation.readDataInt32();
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null)
            {
                var hp = unit.getHpRounded();
                if (hp - damage <= 0.1)
                {
                    // set hp to very very low
                    unit.setHp(0.1);
                }
                else
                {
                    unit.setHp(hp - damage);
                }
                // reduce fuel
                unit.setFuel(unit.getFuel() / 2);
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
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/drake.mp3", 28, 60371)
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
        return "GE";
    };

    this.superPowerDamage = 2;
    this.superPowerNavalOffBonus = 40;
    this.superPowerAirOffBonus = 0;
    this.superPowerOtherOffBonus = 0;

    this.powerDamage = 1;
    this.powerNavalDefBonus = 10;
    this.powerDefBonus = 10;
    this.powerNavalOffBonus = 40;
    this.powerAirOffBonus = 0;
    this.powerOtherOffBonus = 0;

    this.d2dNavalDefBonus = 0;
    this.d2dDefBonus = 0;
    this.d2dNavalMovementPoints = 1;
    this.d2dNavalOffBonus = 10;
    this.d2dAirOffBonus = -10;
    this.d2dOtherOffBonus = 0;

    this.d2dCoZoneNavalDefBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneNavalOffBonus = 40;
    this.d2dCoZoneAirOffBonus = 0;
    this.d2dCoZoneOtherOffBonus = 0;

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
                if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_DRAKE.superPowerNavalOffBonus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_DRAKE.superPowerAirOffBonus;
                }
                else
                {
                    return CO_DRAKE.superPowerOtherOffBonus;
                }
            }
            case GameEnums.PowerMode_Power:
            {
                if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_DRAKE.powerNavalOffBonus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_DRAKE.powerAirOffBonus;
                }
                else
                {
                    return CO_DRAKE.powerOtherOffBonus;
                }
            }
            default:
            {
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return CO_DRAKE.d2dCoZoneNavalOffBonus;
                    }
                    else if (attacker.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return CO_DRAKE.d2dCoZoneAirOffBonus;
                    }
                    else
                    {
                        return CO_DRAKE.d2dCoZoneOtherOffBonus;
                    }
                }
                else
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return CO_DRAKE.d2dNavalOffBonus;
                    }
                    else if (attacker.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return CO_DRAKE.d2dAirOffBonus;
                    }
                    else
                    {
                        return CO_DRAKE.d2dOtherOffBonus;
                    }
                }
            }
            }
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.getUnitType() === GameEnums.UnitType_Naval)
            {
                return CO_DRAKE.d2dNavalMovementPoints;
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
                if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_DRAKE.powerNavalDefBonus;
                }
                else
                {
                    return CO_DRAKE.powerDefBonus;
                }
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_DRAKE.d2dCoZoneNavalDefBonus;
                }
                else
                {
                    return CO_DRAKE.d2dCoZoneDefBonus;
                }
            }
            else
            {
                if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_DRAKE.d2dDefBonus;
                }
                else
                {
                    return CO_DRAKE.d2dNavalDefBonus;
                }
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
        if (unit.getUnitType() === GameEnums.UnitType_Naval)
        {
            return 6;
        }
        else if (unit.getUnitType() === GameEnums.UnitType_Air)
        {
            return -2;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A big hearted former pirate who hates fighting. Also a great surfer.");
    };
    this.getHits = function(co)
    {
        return qsTr("The sea");
    };
    this.getMiss = function(co)
    {
        return qsTr("High places");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Naval units have superior firepower, but air units have reduced firepower.");
    };
    this.getLongCODescription = function()
    {
        let text = qsTr("\nSpecial Unit:\nMissile Submarine\n\n" +
                    "Global Effect: \nNaval units have %0 more movement point and have %1% firepower and %2% defence. Air units suffer from %3% reduced firepower." +
                    "\n\nCO Zone Effect: \nNaval units have increased %4% firepower and %5% defence.");
        text = replaceTextArgs(text, [CO_DRAKE.d2dNavalMovementPoints, CO_DRAKE.d2dNavalOffBonus, CO_DRAKE.d2dNavalDefBonus, CO_DRAKE.d2dAirOffBonus,
                                      CO_DRAKE.d2dCoZoneNavalOffBonus, CO_DRAKE.d2dCoZoneNavalDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        let text = qsTr("Causes a tidal wave that does %0 HP of damage to all enemy units and halves their fuel.");
        text = replaceTextArgs(text, [CO_DRAKE.powerDamage]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Tsunami");
    };
    this.getSuperPowerDescription = function(co)
    {
        let text = qsTr("Causes a giant tidal wave that does %0 HP of damage to all enemy units and halves their fuel and weather changes to rain.");
        text = replaceTextArgs(text, [CO_DRAKE.superPowerDamage]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Typhoon");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Panic is for landlubbers!"),
                qsTr("Prepare to be washed away!"),
                qsTr("Time to show you how we do things at sea!"),
                qsTr("Wave good-bye to your troops!"),
                qsTr("Can't you just feel the riptide washing you out to open sea?"),
                qsTr("Drake is taking the helm!")];
    };
    this.getVictorySentences = function(co)
        {
            return [qsTr("I'm just glad I survived that..."),
                    qsTr("That was some rough sailing!")];
        };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("A job well done! You've beaten me."),
                qsTr("Curses! I've been defeated. Time to hoist sail and flee!")];
    };
    this.getName = function()
    {
        return qsTr("Drake");
    };
}

Constructor.prototype = CO;
var CO_DRAKE = new Constructor();
