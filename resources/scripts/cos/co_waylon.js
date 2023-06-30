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
        var size = units.size();
        for (var i = 0; i < size; i++)
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
        var size = units.size();
        for (var i = 0; i < size; i++)
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
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/waylon.mp3", 58182, 112806);
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
        return "TI";
    };

    this.superPowerOffBonus = 60;
    this.superPowerDefBonus = 310;

    this.powerBaseOffBonus = 10;
    this.powerOffBonus = 30;
    this.powerDefBonus = 200;
    this.powerDefBaseBonus = 10;

    this.d2dOffBonus = 0;
    this.d2dDefBonus = 0;

    this.d2dCoZoneDefBonus = 40;
    this.d2dCoZoneDefBaseBonus = 10;
    this.d2dCoZoneOffBonus = 30;
    this.d2dCoZoneOffBaseBonus = 10;

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
                    return CO_WAYLON.superPowerOffBonus;
                }
                return CO_WAYLON.powerBaseOffBonus;
            case GameEnums.PowerMode_Power:
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_WAYLON.powerOffBonus;
                }
                return CO_WAYLON.powerBaseOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return CO_WAYLON.d2dCoZoneOffBonus;
                    }
                    return CO_WAYLON.d2dCoZoneOffBaseBonus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_WAYLON.d2dOffBonus;
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
                    return CO_WAYLON.superPowerDefBonus;
                }
                return CO_WAYLON.powerDefBaseBonus;
            case GameEnums.PowerMode_Power:
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_WAYLON.powerDefBonus;
                }
                return CO_WAYLON.powerDefBaseBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    if (defender.getUnitType() === GameEnums.UnitType_Air)
                    {
                        return CO_WAYLON.d2dCoZoneDefBonus;
                    }
                    return CO_WAYLON.d2dCoZoneDefBaseBonus;
                }
                else if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return CO_WAYLON.d2dDefBonus;
                }
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
            if (buildingId === "AIRPORT" ||
                    buildingId === "TEMPORARY_AIRPORT")
            {
                return ["ZCOUNIT_KIROV"];
            }
        }
        return [];
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Air)
        {
            return 6;
        }
        return 0;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A Teal Isle Commander. Destructive, egotistical, and extremely vain.");
    };
    this.getHits = function(co)
    {
        return qsTr("Living the good life");
    };
    this.getMiss = function(co)
    {
        return qsTr("Responsibility");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Air units have higher firepower and defense.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nKirov\n\nGlobal Effect: \nWaylon's air units have +%0% firepower and +%1% defence.") +
                   qsTr("\n\nCO Zone Effect: \nWaylon's air units gain +%2% firepower and +%3% defence. His other units gain +%4% firepower and +%5% defence.");
        text = replaceTextArgs(text, [CO_WAYLON.d2dOffBonus, CO_WAYLON.d2dDefBonus, CO_WAYLON.d2dCoZoneOffBonus, CO_WAYLON.d2dCoZoneDefBonus, CO_WAYLON.d2dCoZoneOffBaseBonus, CO_WAYLON.d2dCoZoneDefBaseBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Waylon's air units gain +%0% firepower and +%1% defence. His other units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [CO_WAYLON.powerOffBonus, CO_WAYLON.powerDefBonus, CO_WAYLON.powerBaseOffBonus, CO_WAYLON.powerDefBaseBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Bad Company");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Waylon's air units gain +%0% firepower and +%1% defence. His other units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [CO_WAYLON.superPowerOffBonus, CO_WAYLON.superPowerDefBonus, CO_WAYLON.powerBaseOffBonus, CO_WAYLON.powerDefBaseBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Wingman");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I'm over here! C'mon, give me your best shot."),
                qsTr("You think you can get the better of me? You've got a lot to learn."),
                qsTr("Woo-hoo!"),
                qsTr("All I want is total air supremacy! Then I'll pick off your forces.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("The end of the world? Fine by me..."),
                qsTr("Woo-hoo!"),
                qsTr("How'd you like that?")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is not the freedom I want!"),
                qsTr("The eagle has landed...")];
    };
    this.getName = function()
    {
        return qsTr("Waylon");
    };
}

Constructor.prototype = CO;
var CO_WAYLON = new Constructor();
