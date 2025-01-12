var Constructor = function()
{
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
                audio.addMusic("resources/music/cos/power_awdc.ogg", 0, 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_awdc.ogg", 0, 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/gage.ogg",9315 ,70409)
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
            if (unit.getBaseMaxRange() > 1 || unit.getUnitType() === GameEnums.UnitType_Naval)
            {
                var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                var delay = globals.randInt(135, 265);
                if (animations.length < 5)
                {
                    delay *= i;
                }
                if (i % 2 === 0)
                {
                    animation.setSound("power9_1.wav", 1, delay);
                }
                else
                {
                    animation.setSound("power9_2.wav", 1, delay);
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
            if (unit.getBaseMaxRange() > 1 || unit.getUnitType() === GameEnums.UnitType_Naval)
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
        return 2;
    };

    this.superPowerFirerangeBonus = 3;
    this.superPowerDefBonus = 30;
    this.superPowerOffBonus = 50;

    this.powerFirerangeBonus = 2;
    this.powerDefBonus = 20;
    this.powerBaseDefBonus = 10;
    this.powerOffBonus = 30;
    this.powerBaseOffBonus = 10;

    this.d2dOffBonus = 0;
    this.d2dDefBonus = 0;

    this.d2dCoZoneDefBonus = 20;
    this.d2dCoZoneBaseDefBonus = 10;
    this.d2dCoZoneOffBonus = 30;
    this.d2dCoZoneBaseOffBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getBaseMaxRange() > 1 ||
                    attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_GAGE.superPowerOffBonus;
                }
                return CO_GAGE.powerBaseOffBonus;
            case GameEnums.PowerMode_Power:
                if (attacker.getBaseMaxRange() > 1 ||
                    attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_GAGE.powerOffBonus;
                }
                return CO_GAGE.powerBaseOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getBaseMaxRange() > 1 ||
                        attacker.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return CO_GAGE.d2dCoZoneOffBonus;
                    }
                    return CO_GAGE.d2dCoZoneBaseOffBonus;
                }
                else if (attacker.getBaseMaxRange() > 1 ||
                         attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        return CO_GAGE.d2dOffBonus;
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
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (defender.getBaseMaxRange() > 1 ||
                    defender.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_GAGE.superPowerDefBonus;
                }
                return CO_GAGE.powerBaseDefBonus;
            case GameEnums.PowerMode_Power:
                if (defender.getBaseMaxRange() > 1 ||
                    defender.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return CO_GAGE.powerDefBonus;
                }
                return CO_GAGE.powerBaseDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    if (defender.getBaseMaxRange() > 1 ||
                        defender.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return CO_GAGE.d2dCoZoneDefBonus;
                    }
                    return CO_GAGE.d2dCoZoneBaseDefBonus;
                }
                else if (defender.getBaseMaxRange() > 1 ||
                         defender.getUnitType() === GameEnums.UnitType_Naval)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        return CO_GAGE.d2dDefBonus;
                    }
                }
                break;
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
                    return CO_GAGE.superPowerFirerangeBonus;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (unit.getBaseMaxRange() > 1)
                {
                    return CO_GAGE.powerFirerangeBonus;
                }
                break;
            default:
                break;
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getBaseMaxRange() > 1 ||
            unit.getUnitType() === GameEnums.UnitType_Naval)
        {
            return 6;
        }
        return 1;
    };

    this.getAiCoBuildRatioModifier = function(co, map)
    {
        return 0.4;
    };

    this.getCOArmy = function()
    {
        return "BD";
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
        return qsTr("Brown Desert Army soldier. A man of few words, but a consummate professional.");
    };
    this.getHits = function(co)
    {
        return qsTr("Getting things done");
    };
    this.getMiss = function(co)
    {
        return qsTr("Talking");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Indirect-combat units and naval units have increased firepower.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_GAGE.d2dOffBonus, CO_GAGE.d2dDefBonus];
        }
        var text = qsTr("\nSpecial Unit:\nSiege Cannon\n") +
               qsTr("\nGlobal Effect: \nGage's indirect and naval units have +%0% firepower and +%1% defence.") +
               qsTr("\n\nCO Zone Effect: \nGage's indirect and naval units have +%2% firepower and +%3% defence. His other units have +%4% firepower and +%5% defence.");
        text = replaceTextArgs(text, [values[0], values[1],
                                      CO_GAGE.d2dCoZoneOffBonus, CO_GAGE.d2dCoZoneDefBonus, CO_GAGE.d2dCoZoneBaseOffBonus, CO_GAGE.d2dCoZoneBaseDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Gage's indirect units gain +%0 range. His indirect and naval units have +%1% firepower and +%2% defence. His other units have +%3% firepower and +%4% defence.");
        text = replaceTextArgs(text, [CO_GAGE.powerFirerangeBonus, CO_GAGE.powerOffBonus, CO_GAGE.powerDefBonus, CO_GAGE.powerBaseOffBonus, CO_GAGE.powerBaseDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Longshot");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Gage's indirect units gain +%0 range. His indirect and naval units have +%1% firepower and +%2% defence. His other units have +%3% firepower and +%4% defence.");
        text = replaceTextArgs(text, [CO_GAGE.superPowerFirerangeBonus, CO_GAGE.superPowerOffBonus, CO_GAGE.superPowerDefBonus, CO_GAGE.powerBaseOffBonus, CO_GAGE.powerBaseDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Sniper's Shot");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Our business is war. Emotions will not distract us from our task."),
                qsTr("At this distance it should be within firing range..."),
                qsTr("Prepare to open fire!"),
                qsTr("My job is to fight, not think.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Now, let us prepare for the next battle."),
                qsTr("I have carried out my duties.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I missed the shot!"),
                qsTr("What? I didn't hit you?!")];
    };
    this.getName = function()
    {
        return qsTr("Gage");
    };
}

Constructor.prototype = CO;
var CO_GAGE = new Constructor();
