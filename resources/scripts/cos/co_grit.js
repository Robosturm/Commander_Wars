var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt", "+alt2", "+alt3"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAtUnitCount(co, powerSurplus, turnMode, indirectUnits);
    };

    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co)
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
                audio.addMusic("resources/music/cos/grit.mp3", 13156, 66022)
                break;
        }
    };

    this.activatePower = function(co)
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
            if (unit.getBaseMaxRange() > 1)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                if (globals.randInt(0, 1) === 0)
                {
                    animation.setSound("power9_1.wav");
                }
                else
                {
                    animation.setSound("power9_2.wav");
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        units.remove();
    };

    this.activateSuperpower = function(co, powerMode)
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
            if (unit.getBaseMaxRange() > 1)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                if (globals.randInt(0, 1) === 0)
                {
                    animation.setSound("power12_1.wav");
                }
                else
                {
                    animation.setSound("power12_2.wav");
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
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
        units.remove();
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getBaseMaxRange() > 1)
                {
                    return 70;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Ground)
                {
                    return -10;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (attacker.getBaseMaxRange() > 1)
                {
                    return 50;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Ground)
                {
                    return -10;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getBaseMaxRange() > 1)
                    {
                        return 30;
                    }
                    else if (attacker.getUnitType() === GameEnums.UnitType_Ground)
                    {
                        return -10;
                    }
                    return 10;
                }
                break;
        }
        if (attacker.getBaseMaxRange() === 1 &&
            attacker.getUnitType() !== GameEnums.UnitType_Infantry)
        {
            return -20;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (unit.getBaseMaxRange() > 1)
                {
                    return 3;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (unit.getBaseMaxRange() > 1)
                {
                    return 2;
                }
                break;
            default:
                if (unit.getBaseMaxRange() > 1)
                {
                    return 1;
                }
                break;
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit)
    {
        if (unit.getBaseMaxRange() > 1)
        {
            return 2;
        }
        else if (unit.getUnitType() === GameEnums.UnitType_Ground)
        {
            return -1;
        }
        return 0;
    };

    this.getCOArmy = function()
    {
        return "BM";
    };
    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_SIEGE_CANNON"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A laid-back style masks his dependability. A peerless marksman. Works well with Olaf.");
    };
    this.getHits = function(co)
    {
        return qsTr("Cats");
    };
    this.getMiss = function(co)
    {
        return qsTr("Rats");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Indirect-combat units cause more damage and have increased firerange. Weak in non-infantry direct combat.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nSiege Cannon\n") +
               qsTr("\nGlobal Effect: \nIndirect units have 1 increased firerange and non-infantry direct units have reduced firepower.") +
               qsTr("\n\nCO Zone Effect: \nIndirect units  have an offensive bonus.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Increases range of indirect units by one space. Firepower of these units also rise.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Snipe Attack");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Increases range of indirect units by two spaces. Firepower of these units greatly rise.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Super Snipe");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Once you're in my sights, there's no gettin' away!"),
                qsTr("Reckon it's time to take you down!"),
                qsTr("Where's the fool who wants to help me with target practice?"),
                qsTr("Y'all can run, but you can't hide!"),
                qsTr("Y'all gimme no choice... Time to bring in the big guns!"),
                qsTr("Aw, shucks. I was hopin' it wouldn't come to this.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("This ain't for show."),
                qsTr("Maybe now I can get some shut-eye."),
                qsTr("I hope this gets easier. That was harder'n college!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Aw, possum spit!"),
                qsTr("Just as I reckoned... This ain't gonna be no Sunday stroll.")];
    };
    this.getName = function()
    {
        return qsTr("Grit");
    };
}

Constructor.prototype = CO;
var CO_GRIT = new Constructor();
