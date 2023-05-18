var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style

        return ["+alt"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co, map)
    {
        // put the co music in here.
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

    this.coSuperpowerBonus = 90;
    this.coPowerBonus = 70;
    this.coZoneBonus = 70;
    this.coPlainsBonus = 10;
    this.coDefenseBonus = 10;
    this.coIndirectBonus = 1;
    this.coMoveBonus = 2;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
     defender, defPosX, defPosY, isDefender, action, luckmode, map)
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
                        return CO_JAKE.coSuperpowerBonus;
                    }
                    return CO_JAKE.coPlainsBonus;
                case GameEnums.PowerMode_Power:
                    if (terrainID === "PLAINS")
                    {
                        return CO_JAKE.coPowerBonus;
                    }
                    return CO_JAKE.coPlainsBonus;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        if (terrainID === "PLAINS")
                        {
                            return CO_JAKE.coZoneBonus;
                        }
                        return CO_JAKE.coPlainsBonus;
                    }
                    break;
                }
            }
        }
        else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return CO_JAKE.coPlainsBonus;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
            co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return coDefenseBonus;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1 &&
                unit.getUnitType() === GameEnums.UnitType_Ground)
            {
                return CO_JAKE.coIndirectBonus;
            }
            break;
        default:
            return 0;
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if (unit.getUnitType() === GameEnums.UnitType_Ground &&
                unit.getBaseMaxRange() === 1)
            {
                return CO_JAKE.coMoveBonus;
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
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ" ||
            buildingId === "FORTHQ")
        {
            return ["ZCOUNIT_TANK_HUNTER"];
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
        return qsTr("Ground units have increased firepower on plains.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("<r>\n\nSpecial Unit:\nTank Hunter\n</r>") +
        qsTr("<r>\n\nActive CO Day-to-day: \nJakes's units (including air) gain </r><div c='#55ff00'>+%0%</div><r> firepower on plains terrain.</r>") +
        qsTr("<r>\n\nCO Zone Effect: \nJakes's units (including air) gain </r><div c='#55ff00'>+%1%</div><r> firepower on plains terrain and gain </r><div c='#55ff00'>+%2%</div><r> firepower and deffense overall.</r>");
        text = replaceTextArgs(text, [CO_JAKE.coPlainsBonus, CO_JAKE.coZoneBonus, CO_JAKE.coDefenseBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
     var text = qsTr("<r>Jake's plains terrain firepower bonus becomes </r><div c='#55ff00'>+%0%</div><r> and deffense becomes </r><div c='#55ff00'>+%1%</div><r>. His ground indirect-combat units gain </r><div c='#55ff00'>+%2</div><r> range.</r>");
     text = replaceTextArgs(text, [CO_JAKE.coPowerBonus, CO_JAKE.coDefenseBonus, CO_JAKE.coIndirectBonus]);
     return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Beat Down");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("<r>Jake's plains terrain firepower bonus becomes </r><div c='#55ff00'>+%0%</div><r> and deffense becomes </r><div c='#55ff00'>+%1%</div><r>. His ground indirect-combat units gain </r><div c='#55ff00'>+%2</div><r> range and all his ground vehicle units gain </r><div c='#55ff00'>+%3 movement</div><r>.</r>");
        text = replaceTextArgs(text, [CO_JAKE.coSuperpowerBonus, CO_JAKE.coDefenseBonus, CO_JAKE.coIndirectBonus CO_JAKE.coMoveBonus]);
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
