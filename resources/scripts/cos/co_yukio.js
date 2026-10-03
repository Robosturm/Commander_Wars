var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(8);
        co.setSuperpowerStars(7);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.activatePower = function(co, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        CO_YUKIO.spawnUnits(co, CO_YUKIO.powerSpawnProportion, CO_YUKIO.powerSpawnList, dialogBuilder, map);

        dialogBuilder.displayAnimation();
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        CO_YUKIO.spawnUnits(co, CO_YUKIO.superPowerSpawnProportion, CO_YUKIO.superPowerSpawnList, dialogBuilder, map);

        CO_YUKIO.yukioDamage(co, CO_YUKIO.superPowerBombDamage, CO_YUKIO.superPowerBombProportion, dialogBuilder, map);

        dialogBuilder.displayAnimation();
    };

    this.yukioDamage = function(co, value, proportion, parentBuilder, map)
    {
        var player = co.getOwner();
        var playerCounter = map.getPlayerCount();
        var enemyUnits = map.getUnits(null);
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                var units = enemyPlayer.getUnits();
                units.randomize();
                var size = units.size() * proportion;
                for (i = 0; i < size; i++)
                {
                    var unit = units.at(i);
                    enemyUnits.append(unit);
                }
            }
        }

        var enemyUnitsBuilder = new UNITS_ANIMATION_BUILDER(enemyUnits, map);
        enemyUnitsBuilder.setSound("power4.wav");
        enemyUnitsBuilder.setSprite("power4");
        enemyUnitsBuilder.setPerAnimationFunction((unit, animation, map) => 
            {
                animation.writeDataInt32(unit.getX());
                animation.writeDataInt32(unit.getY());
                animation.writeDataInt32(value);
                animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
            }
        );
        parentBuilder.queueAnimationBuilder(enemyUnitsBuilder);
    };

    this.spawnUnits = function(co, count, invasion, parentBuilder, map)
    {
        var buildings = co.getOwner().getBuildings("TOWN");
        buildings.randomize();
        var targetSize = buildings.size() * count;
        while (buildings.size() > targetSize)
        {
            buildings.removeAt(buildings.size() - 1);
        }

        var ownBuildingsBuilder = new BUILDINGS_ANIMATION_BUILDER(buildings, map);
        ownBuildingsBuilder.setBuildingFilter((building, map) => 
            map.getTerrain(building.getX(), building.getY()).getUnit() === null
        );
        ownBuildingsBuilder.setSounds(["power8_1.wav", "power8_2.wav"]);
        ownBuildingsBuilder.setSprite("power8");
        var spawnIndex = 0;
        ownBuildingsBuilder.setPerAnimationFunction((building, animation, map) =>
            {
                animation.writeDataInt32(building.getX());
                animation.writeDataInt32(building.getY());
                animation.writeDataString(invasion[spawnIndex % invasion.length]);
                spawnIndex++;
                animation.writeDataInt32(co.getOwner().getPlayerID());
                animation.writeDataInt32(10);
                animation.setStartOfAnimationCall("ANIMATION", "postAnimationSpawnUnit");
            }
        );
        parentBuilder.queueAnimationBuilder(ownBuildingsBuilder);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.ogg", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.ogg", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.ogg", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/yukio.ogg", 100, 58286);
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 4;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };

    this.superPowerSpawnList = ["HEAVY_TANK", "FLAK", "LIGHT_TANK", "ARTILLERY", "LIGHT_TANK", "K_HELI", "K_HELI"];
    this.superPowerSpawnProportion = 0.7;
    this.superPowerBombDamage = 4;
    this.superPowerBombProportion = 0.5;

    this.powerSpawnList = ["ARTILLERY", "FLAK", "LIGHT_TANK", "FLAK", "LIGHT_TANK"];
    this.powerSpawnProportion = 0.4;
    this.powerTrueDamageBonus = 40;
    this.powerTrueDefenseBonus = 40;
    this.powerMinTrueDamage = 10
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneTrueDamageBonus = 30;
    this.d2dCoZoneTrueDefenseBonus = 30;
    this.d2dCoZoneMinTrueDamage = 10;

    this.d2dOffBonus = 0;
    this.d2dDefBonus = 0;
    this.d2dTrueDamageBonus = 15;
    this.d2dTrueDefenseBonus = 15;
    this.d2dMinTrueDamage = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return CO_YUKIO.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_YUKIO.d2dCoZoneOffBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_YUKIO.d2dOffBonus;
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
                return CO_YUKIO.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_YUKIO.d2dCoZoneDefBonus;
            }
            else if (map === null ||
                     (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return CO_YUKIO.d2dDefBonus
            }
        }
        return 0;
    };
    this.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (damage >= CO_YUKIO.powerMinTrueDamage)
                {
                    return CO_YUKIO.powerTrueDamageBonus;
                }
                break;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (damage >= CO_YUKIO.d2dCoZoneMinTrueDamage)
                    {
                        return CO_YUKIO.d2dCoZoneTrueDamageBonus;
                    }
                }
                else if (damage >= CO_YUKIO.d2dMinTrueDamage)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        return CO_YUKIO.d2dTrueDamageBonus;
                    }
                }
                break;
            }
        }
        return 0;
    };

    this.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                       defender, defPosX, defPosY, isDefender, luckMode, action, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return CO_YUKIO.powerTrueDefenseBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_YUKIO.d2dCoZoneTrueDefenseBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_YUKIO.d2dTrueDefenseBonus;
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
                    BUILDING.isHq(building))
            {
                return ["ZCOUNIT_LOGIC_TRUCK"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Founder of the Dark Matter Cooperation. He's the primary reason for the invasions of Sturm and Von Bolt. He uses war as a way of making money and doesn't care if people die.");
    };
    this.getHits = function(co)
    {
        return qsTr("Astronomic gains");
    };
    this.getMiss = function(co)
    {
        return qsTr("Any losses");
    };
    this.getCODescription = function(co)
    {
        return qsTr("As the leader of Dark Matter, his troops have special armor, as well as armor-piercing weapons.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_YUKIO.d2dTrueDefenseBonus, CO_YUKIO.d2dTrueDamageBonus / 10, CO_YUKIO.d2dMinTrueDamage / 10];
        }
        var text = qsTr("\nSpecial Unit:\nLogistic Truck\n") +
                   qsTr("\nGlobal Effect: \nEnemy attacks deal -%0% damage to Yukio's units. If an attack from Yukio's units would deal at least %2 HP damage, they deal an additional defense-ignoring -%1 HP of damage.") +
                   qsTr("\n\nCO Zone Effect: \nEnemy attacks deal -%3% damage to Yukio's units. If an attack from Yukio's units would deal at least %5 HP damage, they deal an additional defense-ignoring -%4 HP of damage. Yukio's units also gain +%6% firepower and +%7% defense.");
        text = replaceTextArgs(text, [values[0], values[1], values[2],
                                      CO_YUKIO.d2dCoZoneTrueDefenseBonus, CO_YUKIO.d2dCoZoneTrueDamageBonus/10, CO_YUKIO.d2dCoZoneMinTrueDamage/10, CO_YUKIO.d2dCoZoneOffBonus, CO_YUKIO.d2dCoZoneDefBonus]);
        return text;
    };

    this.createUnitNamesString = function(ids, singleStart, start, sep, beforeEnd)
    {
        var uniqueIDs = [];
        var unitNames = [];
        for (var i = 0; i < ids.length; i++)
        {
            if (!uniqueIDs.includes(ids[i]))
            {
                uniqueIDs.push(ids[i]);
                unitNames.push(Global[ids[i]].getName());
            }
        }
        var formatString = "";
        if (unitNames.length == 0)
        {
            formatString = singleStart + "%0"
        }
        else
        {
            formatString = start;
            for (var i = 0; i < unitNames.length; i++)
            {
                if (i < unitNames.length - 1)
                {
                    formatString += "%" + i + sep;
                }
                else
                {
                    formatString += beforeEnd + "%" + i;
                }
            }
        }
        formatString = qsTr(formatString);
        return replaceTextArgs(formatString, unitNames);
    };

    this.getPowerDescription = function(co)
    {
        var unitNameText = CO_YUKIO.createUnitNamesString(CO_YUKIO.powerSpawnList, qsTr("deploy a "), qsTr("randomly deploy either a "), qsTr(", "), qsTr("or "));
        var text = qsTr("A small army deploys to fight for Yukio. A random %0% of his unoccupied cities will %1, ready to move.") + 
                    qsTr("\nEnemy attacks deal -%2% damage to Yukio's units. If an attack from Yukio's units would deal at least %3 HP damage, they deal an additional defense-ignoring -%4 HP of damage. Yukio's units also gain +%5% firepower and +%6% defense.");
		text = replaceTextArgs(text, [CO_YUKIO.powerSpawnProportion * 100, unitNameText,
                                      CO_YUKIO.powerTrueDefenseBonus, CO_YUKIO.powerMinTrueDamage/10, CO_YUKIO.powerTrueDamageBonus/10,
                                      CO_YUKIO.powerOffBonus, CO_YUKIO.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Dark Assault");
    };
    this.getSuperPowerDescription = function(co)
    {
        var unitNameText = CO_YUKIO.createUnitNamesString(CO_YUKIO.superPowerSpawnList, qsTr("deploy a "), qsTr("randomly deploy either a "), qsTr(", "), qsTr("or "));
        var text = qsTr("A large army deploys to fight for Yukio. A random %0% of his unoccupied cities will %1, ready to move.") + 
                    qsTr(" In order to support the invasion, a bombardment is launched dealing -%2 HP of damage to a random %3% of all enemies.") + 
                    qsTr("\nEnemy attacks deal -%4% damage to Yukio's units. If an attack from Yukio's units would deal at least %5 HP damage, they deal an additional defense-ignoring -%6 HP of damage. Yukio's units also gain +%7% firepower and +%8% defense.");
        text = replaceTextArgs(text, [CO_YUKIO.superPowerSpawnProportion * 100, unitNameText,
                                      CO_YUKIO.superPowerBombDamage, CO_YUKIO.superPowerBombProportion * 100,
                                      CO_YUKIO.powerTrueDefenseBonus, CO_YUKIO.powerMinTrueDamage/10, CO_YUKIO.powerTrueDamageBonus/10,
                                      CO_YUKIO.powerOffBonus, CO_YUKIO.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Total Invasion");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I'll show you what true power means."),
                qsTr("Me? I'm the evil that destroys you."),
                qsTr("Your weapons are the rubbish my factories dispose of."),
                qsTr("Victory is mine! Alea iacta est!"),
                qsTr("I'll become the ruler of this world!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Investment... Profits... Perfect. Continue."),
                qsTr("War is just another tool to make profit."),
                qsTr("Give me all your money and your lives.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Even with this loss, I made profit..."),
                qsTr("I lost, but with all of this money I just will build a better army!")];
    };
    this.getName = function()
    {
        return qsTr("Yukio");
    };
}

Constructor.prototype = CO;
var CO_YUKIO = new Constructor();
