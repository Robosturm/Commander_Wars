var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.activatePower = function(co, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        var ownBuildingsBuilder = new OWN_BUILDINGS_ANIMATION_BUILDER(co, map);
        // ownBuildingsBuilder.setMinDelay(210);
        // ownBuildingsBuilder.setMaxDelay(340);
        ownBuildingsBuilder.setAnimationAmount(3);
        // ownBuildingsBuilder.setSound(null);
        // ownBuildingsBuilder.setSprite("white_pixel");
        // ownBuildingsBuilder.setScale(0);
        ownBuildingsBuilder.setSounds(["power9_1.wav", "power9_2.wav"]);
        ownBuildingsBuilder.setSprite("power9");
        ownBuildingsBuilder.setPerAnimationFunction(
            CO_ALEXIS.alexisBuildingFunctionFactory(co, false, true, CO_ALEXIS.powerHeal, CO_ALEXIS.powerRadius, ["power0.wav"], "power0")
        );
        dialogBuilder.queueAnimationBuilder(ownBuildingsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        var ownBuildingsBuilder = new OWN_BUILDINGS_ANIMATION_BUILDER(co, map);
        ownBuildingsBuilder.setAnimationAmount(3);
        ownBuildingsBuilder.setSounds(["power9_1.wav", "power9_2.wav"]);
        ownBuildingsBuilder.setSprite("power9");
        ownBuildingsBuilder.setPerAnimationFunction(
            CO_ALEXIS.alexisBuildingFunctionFactory(co, false, true, CO_ALEXIS.superPowerHeal, CO_ALEXIS.superPowerHealRadius, ["power0.wav"], "power0")
        );
        dialogBuilder.queueAnimationBuilder(ownBuildingsBuilder);

        var enemyBuildingsBuilder = new ENEMY_BUILDINGS_ANIMATION_BUILDER(co, map);
        enemyBuildingsBuilder.setBuildingFilter((building, map) =>
            building.getOwner() !== null
        );
        enemyBuildingsBuilder.setAnimationAmount(3);
        enemyBuildingsBuilder.setSounds(["power5_1.wav", "power5_2.wav"]);
        enemyBuildingsBuilder.setSprite("power5");
        enemyBuildingsBuilder.setPerAnimationFunction(
            CO_ALEXIS.alexisBuildingFunctionFactory(co, false, false, CO_ALEXIS.superPowerDamage, CO_ALEXIS.superPowerDamageRadius, ["power4.wav"], "power4")
        );
        ownBuildingsBuilder.queueAnimationBuilder(enemyBuildingsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.unitsNearBuilding = function(building, minRadius, radius, map)
    {
        var fields = globals.getCircle(minRadius, radius);
        var units = map.getUnits(null);
        var x = building.getX();
        var y = building.getY();
        for (i = 0; i < fields.size(); i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var unitX = x + point.x;
                var unitY = y + point.y;
                var unit = map.getTerrain(unitX, unitY).getUnit();
                if ((unit !== null))
                {
                    units.append(unit);
                }
            }
        }
        return units;
    }

    this.alexisBuildingFunctionFactory = function(co, isRepair, isHealing, value, radius, sounds, sprite)
    {
        return (building, animation, map) =>
        {
            var minRadius = 0;
            if (isRepair)
            {
                minRadius = 1;
            }
            var units = CO_ALEXIS.unitsNearBuilding(building, minRadius, radius, map);

            var unitsBuilder = new UNITS_ANIMATION_BUILDER(units, map);
            unitsBuilder.setUnitFilter((unit, map) =>
                {
                    if (isHealing)
                    {
                        return unit.getOwner() === co.getOwner();
                    }
                    else
                    {
                        return unit.getOwner() !== co.getOwner();
                    }
                }
            );
            unitsBuilder.setRandomize(false);
            unitsBuilder.setMinDelay(0);
            unitsBuilder.setMaxDelay(0);
            unitsBuilder.setAnimationAmount(units.size());
            unitsBuilder.setSounds(sounds);
            unitsBuilder.setSprite(sprite);
            unitsBuilder.setPerAnimationFunction((unit, animation, map) => 
                {
                    if (isRepair)
                    {
                        UNIT.repairUnit(unit, CO_ALEXIS.d2dHealBonus, map);
                        if (!map.getCurrentViewPlayer().getFieldVisible(unit.getX(), unit.getY()))
                        {
                            animation.setVisible(false);
                        }
                    }
                    else
                    {
                        animation.writeDataInt32(unit.getX());
                        animation.writeDataInt32(unit.getY());
                        animation.writeDataInt32(value);
                        if (isHealing)
                        {
                            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
                        }
                        else
                        {
                            animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
                        }
                    }
                }
            );

            unitsBuilder.displayAnimation(animation);
        }
    }

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.ogg", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.ogg", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/alexis.ogg", 51, 56938);
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "PF";
    };
    this.superPowerOffBonus = 50;
    this.superPowerOffBaseBonus = 15;
    this.superPowerHeal = 3;
    this.superPowerHealRadius = 2;
    this.superPowerDamage = 3;
    this.superPowerDamageRadius = 1;

    this.powerOffBonus = 30;
    this.powerOffBaseBonus = 15;
    this.powerDefBonus = 15;
    this.powerHeal = 3;
    this.powerRadius = 2;

    this.d2dCoZoneBonus = 15;

    this.d2dHealBonus = 1;
    this.d2dHealRadius = 1;
    this.d2dHealMalus = 1;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var nearBuildings = false;
            var fields = globals.getCircle(0, 2);
            if (map !== null)
            {
                for (var i = 0; i < fields.size(); i++)
                {
                    var x = fields.at(i).x + atkPosX;
                    var y = fields.at(i).y + atkPosY;
                    if (map.onMap(x, y))
                    {
                        var building = map.getTerrain(x, y).getBuilding();
                        if (building !== null && building.getOwner() === co.getOwner())
                        {
                            nearBuildings = true;
                            break;
                        }
                    }
                }
            }
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (nearBuildings)
                {
                    return CO_ALEXIS.superPowerOffBonus;
                }
                return CO_ALEXIS.superPowerOffBaseBonus;
            case GameEnums.PowerMode_Power:
                if (nearBuildings)
                {
                    return CO_ALEXIS.powerOffBonus;
                }
                return CO_ALEXIS.powerOffBaseBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_ALEXIS.d2dCoZoneBonus;
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
                return CO_ALEXIS.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_ALEXIS.d2dCoZoneBonus;
            }
        }
        return 0;
    };

    this.startOfTurn = function(co, map)
    {
        if (CO.isActive(co))
        {
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                var player = co.getOwner();
                if (!player.getIsDefeated())
                {
                    var ownBuildingsBuilder = new OWN_BUILDINGS_ANIMATION_BUILDER(co, map);
                    ownBuildingsBuilder.setAnimationAmount(3);
                    ownBuildingsBuilder.setSounds(["power9_1.wav", "power9_2.wav"]);
                    ownBuildingsBuilder.setSprite("power9");
                    ownBuildingsBuilder.setPerAnimationFunction(
                        CO_ALEXIS.alexisBuildingFunctionFactory(co, true, true, CO_ALEXIS.d2dHealBonus, CO_ALEXIS.d2dHealRadius, ["power0.wav"], "power0")
                    );
                    ownBuildingsBuilder.displayAnimation();
                }
            }
        }
    };

    this.getRepairBonus = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return -CO_ALEXIS.d2dHealMalus;
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
                return ["ZCOUNIT_REPAIR_TANK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A crystal-obsessed person. Blindly follows orders.");
    };
    this.getHits = function(co)
    {
        return qsTr("Crystals");
    };
    this.getMiss = function(co)
    {
        return qsTr("Experiments");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Units heal at half the normal rate on owned properties, but are able to heal while adjacent to them.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_ALEXIS.d2dHealMalus, CO_ALEXIS.d2dHealRadius,  CO_ALEXIS.d2dHealBonus];
        }
        var text = qsTr("\nSpecial Unit:\nRepair Tank\n") +
                   qsTr("\nGlobal Effect: \nAlexis' units heal only +%0 HP while on an owned property, however, her units will heal from any owned property for +%2 HP if they're with within %1 space. This effect stacks with each additional nearby property.") +
                   qsTr("\n\nCO Zone Effect: \nAlexis' units gain +%3% firepower and +%3% defence.");
        text = replaceTextArgs(text, [values[0], values[1], values[2], CO_ALEXIS.d2dCoZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text =  qsTr("Alexis' units restore +%2 HP for each of her nearby properties within %0 spaces. Her units receive a +%1% firepower bonus while near one of her properties and +%3% firepower otherwise. All of her units gain +%4% defence.");
        text = replaceTextArgs(text, [CO_ALEXIS.powerRadius, CO_ALEXIS.powerOffBonus,  CO_ALEXIS.powerHeal, CO_ALEXIS.powerOffBaseBonus, CO_ALEXIS.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Crystal Brace");
    };
    this.getSuperPowerDescription = function(co)
    {
		var text =  qsTr("Enemy units suffer -%4 HP of damage for each of their owned properties within %3 space. Alexis' units restore +%2 HP for each of her nearby properties within %0 spaces. Her units receive a +%1% firepower bonus while near one of her properties and +%5% firepower otherwise. All of her units gain +%6% defence.");
        text = replaceTextArgs(text, [CO_ALEXIS.superPowerHealRadius, CO_ALEXIS.superPowerOffBonus,  CO_ALEXIS.superPowerHeal,  CO_ALEXIS.superPowerDamageRadius,  CO_ALEXIS.superPowerDamage, CO_ALEXIS.powerOffBaseBonus, CO_ALEXIS.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Crystal Edge");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("You'll regret angering me!"),
                qsTr("It's time you learned the real power of the crystals!"),
                qsTr("You were probably enjoying this battle... until now!"),
                qsTr("Witness the power of the black crystal!"),
                qsTr("I hope you're ready for what's coming next!"),
                qsTr("Now my army will really shine... like diamonds!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I'm the brightest crystal of all!"),
                qsTr("Haha, you were shattered!"),
                qsTr("I'll leave you to clean this up, 'kay?")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("My crystals failed me."),
                qsTr("My diamonds... shattered?")];
    };
    this.getName = function()
    {
        return qsTr("Alexis");
    };
};

Constructor.prototype = CO;
var CO_ALEXIS = new Constructor();
