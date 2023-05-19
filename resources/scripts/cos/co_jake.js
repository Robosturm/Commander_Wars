var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt"];
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
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/jake.mp3", 5421, 96121);
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

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };

    this.superPowerMovementBonus = 2;
    this.superPowerPlainsBonus = 90;

    this.powerFirerangeBonus = 1;
    this.powerPlainsBonus = 70;
    this.powerDefBonus = 10;
    this.powerOffBonus = 10;

    this.d2dPlainsBonus = 0;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZonePlainsBonus = 70;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (map !== null)
            {
                if (map.onMap(atkPosX, atkPosY))
                {
                    var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
                    switch (co.getPowerMode())
                    {
                    case GameEnums.PowerMode_Tagpower:
                    case GameEnums.PowerMode_Superpower:
                        if (terrainID === "PLAINS")
                        {
                            return CO_JAKE.superPowerPlainsBonus;
                        }
                        return CO_JAKE.powerOffBonus;
                    case GameEnums.PowerMode_Power:
                        if (terrainID === "PLAINS")
                        {
                            return CO_JAKE.powerPlainsBonus;
                        }
                        return CO_JAKE.powerOffBonus;
                    default:
                        if (CO.getGlobalZone())
                        {
                            if (terrainID === "PLAINS")
                            {
                                return CO_JAKE.d2dPlainsBonus;
                            }
                        }
                        else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                        {
                            if (terrainID === "PLAINS")
                            {
                                return CO_JAKE.d2dCoZonePlainsBonus;
                            }
                            return CO_JAKE.d2dCoZoneOffBonus;
                        }
                        break;
                    }
                }
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_JAKE.d2dCoZoneOffBonus;
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
                return CO_JAKE.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_JAKE.d2dCoZoneDefBonus;
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
            case GameEnums.PowerMode_Power:
                if (unit.getBaseMaxRange() > 1 &&
                    unit.getUnitType() === GameEnums.UnitType_Ground)
                {
                    return CO_JAKE.powerFirerangeBonus;
                }
                break;
            default:
                return 0;
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
                if (unit.getUnitType() === GameEnums.UnitType_Ground &&
                    unit.getBaseMaxRange() === 1)
                {
                    return CO_JAKE.superPowerMovementBonus;
                }
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "FACTORY" ||
                    buildingId === "TOWN" ||
                    buildingId === "HQ" ||
                    buildingId === "FORTHQ")
            {
                return ["ZCOUNIT_TANK_HUNTER"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A young, energetic Orange Star CO who is a top-notch tank commander.");
    };
    this.getHits = function(co)
    {
        return qsTr("Clubbin'");
    };
    this.getMiss = function(co)
    {
        return qsTr("Easy listening");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Fights well in the open. Firepower of all units increased on plains.");
    };
    this.getLongCODescription = function()
    {
        let text = qsTr("\nSpecial Unit:\nTank Hunter\n") +
               qsTr("\nGlobal Effect: \nUnits have %0% higher firepower on plains.") +
               qsTr("\n\nCO Zone Effect: \nUnits have %0% higher firepower on plains.");
        text = replaceTextArgs(text, [CO_JAKE.d2dPlainsBonus, CO_JAKE.d2dCoZonePlainsBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        let text = qsTr("Firepower is increased by %0% on plains. Firing range of indirect-combat units is increased by %1.");
        text = replaceTextArgs(text, [CO_JAKE.powerPlainsBonus, CO_JAKE.powerFirerangeBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Beat Down");
    };
    this.getSuperPowerDescription = function(co)
    {
        let text = qsTr("Firepower is greatly increased by %0% on plains. Firing range of indirect units is increased by %1, and movement of direct units by %2");
        text = replaceTextArgs(text, [CO_JAKE.superPowerPlainsBonus, CO_JAKE.powerFirerangeBonus, CO_JAKE.superPowerMovementBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Block Rock");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Give it up, fool!"),
                qsTr("Prepare to get served."),
                qsTr("Prepare for a subpoena of pain! Yeah, that's lawyer style!"),
                qsTr("This is how I roll!"),
                qsTr("Wassup now?!"),
                qsTr("Here...have a taste!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Get the plates, 'cause you just got served!"),
                qsTr("Owned!"),
                qsTr("You got dropped like a phat beat!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I got spanked out there! This combat is the real deal..."),
                qsTr("Dude, we so don't have time for this.")];
    };
    this.getName = function()
    {
        return qsTr("Jake");
    };
}

Constructor.prototype = CO;
var CO_JAKE = new Constructor();
