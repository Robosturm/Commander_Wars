var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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
            if (unit.getUnitType() === GameEnums.UnitType_Air)
            {
                var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                var delay = globals.randInt(135, 265);
                if (animations.length < 5)
                {
                    delay *= i;
                }
                if (i % 2 === 0)
                {
                    animation.setSound("power2_1.wav", 1, delay);
                }
                else
                {
                    animation.setSound("power2_2.wav", 1, delay);
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
            if (unit.getUnitType() === GameEnums.UnitType_Air)
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

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_awdc.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_awdc.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/tasha.mp3", 1663, 66947);
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
        return "BD";
    };

    this.superPowerOffBonus = 70;
    this.superPowerDefBonus = 30;
    this.superPowerMovementBonus = 2;

    this.powerOffBonus = 50;
    this.powerBaseOffBonus = 10;
    this.powerDefBonus = 30;
    this.powerMovementBonus = 1;
    this.powerBaseDefBonus = 10;

    this.d2dOffBonus = 0;
    this.d2dDeffBonus = 0;

    this.d2dCoZoneOffBonus = 50;
    this.d2dCoZoneDeffBonus = 30;
    this.d2dCoZoneBaseOffBonus = 10;
    this.d2dCoZoneBaseDeffBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_TASHA.superPowerOffBonus;
                }
                else
                {
                    return CO_TASHA.powerBaseOffBonus;
                }
            case GameEnums.PowerMode_Power:
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_TASHA.powerOffBonus;
                }
                else
                {
                    return CO_TASHA.powerBaseOffBonus;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return CO_TASHA.d2dCoZoneOffBonus;
                    }
                    return CO_TASHA.d2dCoZoneBaseOffBonus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_TASHA.d2dOffBonus;
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
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_TASHA.superPowerDefBonus;
                }
                else
                {
                    return CO_TASHA.powerBaseDefBonus;
                }
            case GameEnums.PowerMode_Power:
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_TASHA.powerDefBonus;
                }
                else
                {
                    return CO_TASHA.powerBaseDefBonus;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), defender))
                {
                    if (defender.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return CO_TASHA.d2dCoZoneDeffBonus;
                    }
                    return CO_TASHA.d2dCoZoneBaseDeffBonus;
                }
                else if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_TASHA.d2dDeffBonus;
                }
                break;
            }
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                    co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return CO_TASHA.superPowerMovementBonus;
            }
            else if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                return CO_TASHA.powerMovementBonus;
            }
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Air)
        {
            return 6;
        }
        return 0;
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
        return qsTr("Brown Desert soldier. Burns with desire to avenge her late brother. Aims to make Teal Isles pay for what they did.");
    };
    this.getHits = function(co)
    {
        return qsTr("Her brother John, revenge");
    };
    this.getMiss = function(co)
    {
        return qsTr("Teal Isles");
    };
    this.getCODescription = function(co)
    {
        return qsTr("She possesses superior air units.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nKirov\n") +
               qsTr("\nGlobal Effect: \nTasha's air units have +%0% firepower and +%1% defence.") +
               qsTr("\n\nCO Zone Effect: \nTasha's air units gain +%2% firepower and +%3% defence. Her other units gain +%4% firepower and +%5% defence.");
        text = replaceTextArgs(text, [CO_TASHA.d2dOffBonus , CO_TASHA.d2dDeffBonus, CO_TASHA.d2dCoZoneOffBonus, CO_TASHA.d2dCoZoneDeffBonus, CO_TASHA.d2dCoZoneBaseOffBonus, CO_TASHA.d2dCoZoneBaseDeffBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Tasha's air units gain +%0% firepower and +%1% defence. Her other units gain +%2% firepower and +%3% defence. All of her units gain +%4 movement.");
        text = replaceTextArgs(text, [CO_TASHA.powerOffBonus , CO_TASHA.powerDefBonus, CO_TASHA.powerBaseOffBonus, CO_TASHA.powerBaseDefBonus, CO_TASHA.powerMovementBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Sonic Boom");
    };
    this.getSuperPowerDescription = function(co)
    {

        var text = qsTr("Tasha's air units gain +%0% firepower and +%1% defence. Her other units gain +%2% firepower and +%3% defence. All of her units gain +%4 movement.");
        text = replaceTextArgs(text, [CO_TASHA.superPowerOffBonus , CO_TASHA.superPowerDefBonus, CO_TASHA.powerBaseOffBonus, CO_TASHA.powerBaseDefBonus, CO_TASHA.superPowerMovementBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Fox One");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("All units! Follow my lead!"),
                qsTr("The time for revenge is at hand!"),
                qsTr("I will fight on, even if it costs me my life!"),
                qsTr("I will have my revenge!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("It's not about tactics - it's about passion!"),
                qsTr("Revenge will be mine!"),
                qsTr("I will have my revenge!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is not the end. I will fight for John!"),
                qsTr("Next time... Next time I will avenge you John...")];
    };
    this.getName = function()
    {
        return qsTr("Tasha");
    };
}

Constructor.prototype = CO;
var CO_TASHA = new Constructor();
