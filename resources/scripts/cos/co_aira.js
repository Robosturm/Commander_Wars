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

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAlways(co, powerSurplus, turnMode);
    };

    this.activatePower = function(co, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        var flashBuilder = new FLASH_ANIMATION_BUILDER(map);
        dialogBuilder.queueAnimationBuilder(flashBuilder);

        map.getGameRules().changeWeather("WEATHER_1SUN", map.getPlayerCount() * 1);

        var enemyUnitsBuilder = new ENEMY_UNITS_ANIMATION_BUILDER(co, map);
        enemyUnitsBuilder.setSounds(["power10_1.wav", "power10_2.wav"]);
        enemyUnitsBuilder.setSprite("power14");
        flashBuilder.queueAnimationBuilder(enemyUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        var flashBuilder = new FLASH_ANIMATION_BUILDER(map);
        dialogBuilder.queueAnimationBuilder(flashBuilder);

        map.getGameRules().changeWeather("WEATHER_1SUN", map.getPlayerCount() * 1);

        CO_AIRA.airaDamage(co, CO_AIRA.superPowerDamage, flashBuilder, map);

        dialogBuilder.displayAnimation();
    };

    this.airaDamage = function(co, value, parentBuilder, map)
    {
        var player = co.getOwner();
        var playerCounter = map.getPlayerCount();
        var cheapUnits = map.getUnits(null);
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {
                var units = enemyPlayer.getUnits();
                units.sortExpensive();
                var size = units.size();
                for (i = 0; i < size; i++)
                {
                    var unit = units.at(i);
                    if (i >= size / 2 || unit.useTerrainDefense() === false)
                    {
                        cheapUnits.append(unit);
                    }
                }
            }
        }
        var cheapUnitsBuilder = new UNITS_ANIMATION_BUILDER(cheapUnits, map);
        cheapUnitsBuilder.setSound("power4.wav");
        cheapUnitsBuilder.setSprite("power4");
        cheapUnitsBuilder.setPerAnimationFunction((unit, animation, map) =>
            {
                animation.writeDataInt32(unit.getX());
                animation.writeDataInt32(unit.getY());
                animation.writeDataInt32(value);
                animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
            }
        );
        parentBuilder.queueAnimationBuilder(cheapUnitsBuilder);
    };

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
                audio.addMusic("resources/music/cos/aira.ogg", 75, 58517);
                break;
            }
        }
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "PF";
    };
    this.superPowerDamage = 3;
    this.superPowerMovementCostIncrease = 1;

    this.powerFuelIncrease = 5;
    this.powerOffBonus = 20;
    this.powerDefBonus = 20;

    this.d2dCoZoneBonus = 20;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_AIRA.powerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_AIRA.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_AIRA.d2dCoZoneBonus;
                }
                break;
            }
        }
        return 0;
    };

    this.getMovementFuelCostModifier = function(co, unit, fuelCost, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power &&
                co.getOwner().isEnemyUnit(unit) === true)
            {
                return fuelCost * CO_AIRA.powerFuelIncrease;
            }
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_AIRA.d2dCoZoneBonus;
            }
            else if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_AIRA.powerDefBonus;
            }
        }
        return 0;
    };

    this.getMovementcostModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if ((co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                 co.getPowerMode() === GameEnums.PowerMode_Tagpower) &&
                    co.getOwner().isEnemyUnit(unit) === true)
            {
                return 1;
            }
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A reserved commander who is often found daydreaming. Likes to walk in the wind.");
    };
    this.getHits = function(co)
    {
        return qsTr("Cool breezes");
    };
    this.getMiss = function(co)
    {
        return qsTr("Surprises");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Aira never felt a need to specialize, so all of her units are average.");
    };
    this.getLongCODescription = function(co, map)
    {
        var text = qsTr("\nGlobal Effect: \nNone.") +
                   qsTr("\n\nCO Zone Effect: \nAira's units gain +%0% firepower and +%0% defence.");
        text = replaceTextArgs(text, [CO_AIRA.d2dCoZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Changes the weather to Sun. All enemies spend an additional %0 times more fuel when they move for the next turn. Aira's units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [CO_AIRA.powerFuelIncrease, CO_AIRA.powerOffBonus, CO_AIRA.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Gust Storm");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Changes the weather to Sun. All enemy air units and the cheapest 50% of all enemy units suffer -%0 HP of damage. All terrain costs are increased by %1 for all enemies. Aira's units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [CO_AIRA.superPowerDamage, CO_AIRA.superPowerMovementCostIncrease, CO_AIRA.powerOffBonus, CO_AIRA.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Hurricannon");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Amazing what a little gust can do..."),
                qsTr("Huh? Oh, wind's about to change."),
                qsTr("The only surprises I like are the ones that help me win."),
                qsTr("The winds will help me blow through this battle."),
                qsTr("Wind is coming in from the southeast. And the north. And the west."),
                qsTr("You're about to see why my troops don't wear skirts.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("...Huh? Oh, right. I won. Woohoo?"),
                qsTr("I tore through you like a tornado."),
                qsTr("You could say... you were swept under the wind.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("You caught me by surprise. I hate that."),
                qsTr("The winds weren't with me today.")];
    };
    this.getName = function()
    {
        return qsTr("Aira");
    };
}

Constructor.prototype = CO;
var CO_AIRA = new Constructor();