var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAtUnitCount(co, powerSurplus, turnMode, repairUnits);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/melanthe.mp3", 2716, 75957);
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_MELANTHE.melantheDamage(co, CO_MELANTHE.powerHeal, CO_MELANTHE.powerDamage, powerNameAnimation, map);
    };

    this.melantheDamage = function(co, heal, damage, powerNameAnimation, map)
    {

        var player = co.getOwner();
        var units = player.getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (CO_MELANTHE.isNature(unit.getX(), unit.getY(), map) === true)
            {
                var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());

                animation.writeDataInt32(unit.getX());
                animation.writeDataInt32(unit.getY());
                animation.writeDataInt32(heal);
                animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
                var delay = globals.randInt(135, 265);
                if (animations.length < 5)
                {
                    delay *= i;
                }
                animation.setSound("power3.wav", 1, delay);
                if (animations.length < 5)
                {
                    animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
        if (damage > 0)
        {
            var playerCounter = map.getPlayerCount();
            for (var i2 = 0; i2 < playerCounter; i2++)
            {
                var enemyPlayer = map.getPlayer(i2);
                if ((enemyPlayer !== player) &&
                        (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
                {

                    units = enemyPlayer.getUnits();
                    units.randomize();
                    for (i = 0; i < units.size(); i++)
                    {
                        unit = units.at(i);

                        if (CO_MELANTHE.isNature(unit.getX(), unit.getY(), map) === true)
                        {
                            animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                            animation.writeDataInt32(unit.getX());
                            animation.writeDataInt32(unit.getY());
                            animation.writeDataInt32(damage);
                            animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
                            var delay = globals.randInt(135, 265);
                            if (animations.length < 5)
                            {
                                delay *= i;
                            }
                            animation.setSound("power4.wav", 1, delay);
                            if (animations.length < 5)
                            {
                                animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                                powerNameAnimation.queueAnimation(animation);
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
                        }
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

        CO_MELANTHE.melantheDamage(co, CO_MELANTHE.superPowerHeal, CO_MELANTHE.superPowerDamage, powerNameAnimation, map);
    };

    this.isNature = function(posX, posY, map)
    {
        var terrain = map.getTerrain(posX, posY);
        var building = terrain.getBuilding();
        if (building !== null)
        {
            return false;
        }
        var terrainId = terrain.getID();
        if (terrainId === "STREET" ||
            terrainId === "SNOW_STREET" ||
            terrainId === "STREET1" ||
            terrainId === "BRIDGE" ||
            terrainId === "BRIDGE1" ||
            terrainId === "BRIDGE2" ||
            terrainId === "RUIN" )
        {
            return false;
        }
        return true;
    };

    this.superPowerHeal = 4;
    this.superPowerDamage = 2;

    this.powerOffBonus = 50;
    this.powerBaseOffBonus = 10;
    this.powerDefBonus = 10;
    this.powerTerrainBonus = 2;
    this.powerHeal = 3;
    this.powerDamage = 0;

    this.d2dOffBonus = 0;
    this.d2dTerrainBonus = 0;
    this.d2dRepairBonus = -1;

    this.d2dCoZoneOffBonus = 40;
    this.d2dCoZoneTerrainBonus = 2;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneBaseOffBonus = 10;

    this.getTerrainDefenseModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (CO_MELANTHE.isNature(posX, posY, map) === true)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                case GameEnums.PowerMode_Power:
                    return CO_MELANTHE.powerTerrainBonus;
                default:
                    if (co.inCORange(Qt.point(posX, posY), unit))
                    {
                        return CO_MELANTHE.d2dCoZoneTerrainBonus;
                    }
                    return CO_MELANTHE.d2dTerrainBonus;
                }
            }
        }
        return 0;
    };
    this.getRepairBonus = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            return CO_MELANTHE.d2dRepairBonus;
        }
        return 0;
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (map !== null)
            {
                if (map.onMap(atkPosX, atkPosY))
                {
                    switch (co.getPowerMode())
                    {
                    case GameEnums.PowerMode_Tagpower:
                    case GameEnums.PowerMode_Superpower:
                    case GameEnums.PowerMode_Power:
                        if (CO_MELANTHE.isNature(atkPosX, atkPosY, map) === true)
                        {
                            return CO_MELANTHE.powerOffBonus;
                        }
                        return CO_MELANTHE.powerBaseOffBonus;
                    default:
                        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                        {
                            if (CO_MELANTHE.isNature(atkPosX, atkPosY, map) === true)
                            {
                                return CO_MELANTHE.d2dCoZoneOffBonus;
                            }
                            return CO_MELANTHE.d2dCoZoneBaseOffBonus;
                        }
                        if (CO_MELANTHE.isNature(atkPosX, atkPosY, map) === true)
                        {
                            return CO_MELANTHE.d2dOffBonus;
                        }
                    }
                    return 0;
                }
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_MELANTHE.d2dCoZoneOffBonus;
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
                return CO_MELANTHE.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_MELANTHE.d2dCoZoneDefBonus;
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
                BUILDING.isHq(building))
            {
                return ["ZCOUNIT_NEOSPIDER_TANK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A genius scientist that harbors a great hatred for mankind. Believes plants to be a superior life forms.");
    };
    this.getHits = function(co)
    {
        return qsTr("Botany");
    };
    this.getMiss = function(co)
    {
        return qsTr("Bovines");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Effects of natural terrain are increased by one star and firepower is increased on natural terrain. Units are repaired by 1 HP less on Buildings.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nNeo Spider Tank\n") +
               qsTr("\nGlobal Effect: \nUnits gain an additional %0% firepower on natural terrain and increased terrain defence by %1 stars and repairs are reduced by %2.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain an additional %3% firepower on natural terrain and increased terrain defence by %4 stars.");
        text = replaceTextArgs(text, [CO_MELANTHE.d2dOffBonus, CO_MELANTHE.d2dTerrainBonus, CO_MELANTHE.d2dRepairBonus,
                                      CO_MELANTHE.d2dCoZoneOffBonus, CO_MELANTHE.d2dCoZoneTerrainBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Units on natural terrain restore %0 HP of health. Units gain an additional %1% firepower on natural terrain and increased terrain defence by %2 stars.");
        text = replaceTextArgs(text, [CO_MELANTHE.powerHeal, CO_MELANTHE.powerOffBonus, CO_MELANTHE.powerTerrainBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Synthetic Seedlings");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Enemies on natural terrain suffer %0 HP of damage. In addition, units on natural terrain restore %1 HP of health. Units gain an additional %2% firepower on natural terrain and increased terrain defence by %3 stars.");
        text = replaceTextArgs(text, [CO_MELANTHE.superPowerDamage, CO_MELANTHE.superPowerHeal, CO_MELANTHE.powerOffBonus, CO_MELANTHE.powerTerrainBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Agent Dreadroot");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Biological warfare is overrated; botanical warfare is supreme."),
				qsTr("The power of nature is on my side!"),
                qsTr("'Blades of grass' has a new meaning now, doesn't it?"),
				qsTr("Don't breathe in... heh heh heh."),
				qsTr("Ground up the enemy into fertilizer!"),
				qsTr("Don't have allergies? You will now.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("The seeds of success have been sown."),
				qsTr("The enemy was a lawn, and I its mower."),
				qsTr("Now that... that was fun.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("What? Humans have defeated the almighty Nature."),
				qsTr("Nobody destroys the Nature. Next time...")];
    };
    this.getName = function()
    {
        return qsTr("Melanthe");
    };
}

Constructor.prototype = CO;
var CO_MELANTHE = new Constructor();
