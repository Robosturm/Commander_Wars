var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3", "+alt4", "+alt5", "+alt6"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAtUnitCount(co, powerSurplus, turnMode, indirectUnits);
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
                audio.addMusic("resources/music/cos/grit.mp3", 13156, 66022)
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
            if (unit.getBaseMaxRange() > 1)
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
            if (unit.getBaseMaxRange() > 1)
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
    this.superPowerOffBonus = 50;
    this.superPowerDirectMalus = -10;
    this.superPowerInfMalus = 10;

    this.powerFirerangeBonus = 2;
    this.powerOffBonus = 50;
    this.powerDirectMalus = -10;
    this.powerInfMalus = 10;
    this.powerDefBonus = 10;
    this.powerOtherOffBonus = 10;

    this.d2dFirerangeBonus = 1;
    this.d2dOffBonus = 0;
    this.d2dDirectMalus = -20;
    this.d2dInfMalus = 0;

    this.d2dCoZoneOffBonus = 30;
    this.d2dCoZoneDirectMalus = -10;
    this.d2dCoZoneInfMalus = 10;
    this.d2dCoZoneOtherOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

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
                if (attacker.getBaseMaxRange() > 1)
                {
                    return CO_GRIT.superPowerOffBonus;
                }
                else if (attacker.getBaseMaxRange() === 1 &&
                         attacker.getUnitType() !== GameEnums.UnitType_Infantry)
                {
                    return CO_GRIT.superPowerDirectMalus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    return CO_GRIT.superPowerInfMalus;
                }
                return CO_GRIT.powerOtherOffBonus;
            }
            case GameEnums.PowerMode_Power:
            {
                if (attacker.getBaseMaxRange() > 1)
                {
                    return CO_GRIT.powerOffBonus;
                }
                else if (attacker.getBaseMaxRange() === 1 &&
                         attacker.getUnitType() !== GameEnums.UnitType_Infantry)
                {
                    return CO_GRIT.powerDirectMalus;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    return CO_GRIT.powerInfMalus;
                }
                return CO_GRIT.powerOtherOffBonus;
            }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getBaseMaxRange() > 1)
                    {
                        return CO_GRIT.d2dCoZoneOffBonus;
                    }
                    else if (attacker.getBaseMaxRange() === 1 &&
                             attacker.getUnitType() !== GameEnums.UnitType_Infantry)
                    {
                        return CO_GRIT.d2dCoZoneDirectMalus;
                    }
                    else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                    {
                        return CO_GRIT.d2dCoZoneInfMalus;
                    }
                    return CO_GRIT.d2dCoZoneOtherOffBonus;
                }
                else
                {
                    if (attacker.getBaseMaxRange() > 1)
                    {
                        return CO_GRIT.d2dOffBonus;
                    }
                    else if (attacker.getBaseMaxRange() === 1 &&
                             attacker.getUnitType() !== GameEnums.UnitType_Infantry)
                    {
                        return CO_GRIT.d2dDirectMalus;
                    }
                    else if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                    {
                        return CO_GRIT.d2dInfMalus;
                    }
                    return 0;
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
                return CO_GRIT.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_GRIT.d2dCoZoneDefBonus;
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
                    return CO_GRIT.superPowerFirerangeBonus;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (unit.getBaseMaxRange() > 1)
                {
                    return CO_GRIT.powerFirerangeBonus;
                }
                break;
            default:
                if (unit.getBaseMaxRange() > 1)
                {
                    return CO_GRIT.d2dFirerangeBonus;
                }
                break;
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getBaseMaxRange() > 1)
        {
            return 6;
        }
        else if (unit.getUnitType() === GameEnums.UnitType_Ground)
        {
            return -1;
        }
        return 0;
    };

    this.getAiCoBuildRatioModifier = function(co, map)
    {
        return 0.1;
    };

    this.getCOArmy = function()
    {
        return "BM";
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
                return ["ZCOUNIT_SIEGE_CANNON"];
            }
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
        return qsTr("Indirect-combat units have more firepower and have increased range. Weak in non-footsoldier direct combat.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nSiege Cannon\n") +
               qsTr("\nGlobal Effect: \nGrit's indirect units gain +%0 range and +%1% firepower. His non-footsoldier direct units have %2% firepower. His footsoldier units have -%3% firepower.") +
               qsTr("\n\nCO Zone Effect: \nGrit's indirect units gain +%4% firepower. His non-footsoldier direct units have %5% firepower. His footsoldier units gain +%6% firepower. All of his units gain +%7% defence.");
        text = replaceTextArgs(text, [CO_GRIT.d2dFirerangeBonus, CO_GRIT.d2dOffBonus, CO_GRIT.d2dDirectMalus, CO_GRIT.d2dInfMalus, CO_GRIT.d2dCoZoneOffBonus, CO_GRIT.d2dCoZoneDirectMalus, CO_GRIT.d2dCoZoneInfMalus, CO_GRIT.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Grit's indirect units gain +%0 range and +%1% firepower. His non-footsoldier direct units have %2% firepower. His footsoldier units gain +%3% firepower. All of his units gain +%4% defence.");
        text = replaceTextArgs(text, [CO_GRIT.powerFirerangeBonus, CO_GRIT.powerOffBonus, CO_GRIT.powerDirectMalus, CO_GRIT.powerInfMalus, CO_GRIT.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Snipe Attack");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Grit's indirect units gain +%0 range and +%1% firepower. His non-footsoldier direct units have %2% firepower. His footsoldier units gain +%3% firepower. All of his units gain +%4% defence.");
        text = replaceTextArgs(text, [CO_GRIT.superPowerFirerangeBonus, CO_GRIT.superPowerOffBonus, CO_GRIT.superPowerDirectMalus, CO_GRIT.superPowerInfMalus, CO_GRIT.powerDefBonus]);
        return text;
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
