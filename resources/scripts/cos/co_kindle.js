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
                audio.addMusic("resources/music/cos/bh_power.ogg", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.ogg", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.ogg", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/kindle.ogg", 1995, 63956);
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setSounds(["power5_1.wav", "power5_2.wav"]);
        ownUnitsBuilder.setSprite("power5");
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        CO_KINDLE.kindleDamage(co, CO_KINDLE.powerDamage, ownUnitsBuilder, map);

        dialogBuilder.displayAnimation();
    };

    this.kindleDamage = function(co, value, parentBuilder, map)
    {
        var enemyUnitsBuilder = new ENEMY_UNITS_ANIMATION_BUILDER(co, map);
        enemyUnitsBuilder.setUnitFilter((unit, map) => 
            unit.getTerrain().getBuilding() !== null
        );
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

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setSounds(["power12_1.wav", "power12_2.wav"]);
        ownUnitsBuilder.setSprite("power12");
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BG";
    };

    this.superPowerOffBonus = 130;
    this.superPowerOffMultiplier = 3;

    this.powerDamage = 3;
    this.powerOffBonus = 80;
    this.powerBaseOffBonus = 10;
    this.powerDefBonus = 10;

    this.d2dOffBonus = 40;

    this.d2dCoZoneOffBonus = 70;
    this.d2dCoZoneBaseOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (map !== null)
            {
                if (map.onMap(atkPosX, atkPosY))
                {
                    var building = map.getTerrain(atkPosX, atkPosY).getBuilding();
                    switch (co.getPowerMode())
                    {
                    case GameEnums.PowerMode_Tagpower:
                    case GameEnums.PowerMode_Superpower:
                        var ret = 0;
                        if (building !== null)
                        {
                            ret = CO_KINDLE.superPowerOffBonus;
                        }
                        ret += co.getOwner().getBuildingListCount(["TEMPORARY_AIRPORT", "TEMPORARY_HARBOUR"], false) * CO_KINDLE.superPowerOffMultiplier + CO_KINDLE.powerBaseOffBonus;
                        return ret;
                    case GameEnums.PowerMode_Power:
                        if (building !== null)
                        {
                            return CO_KINDLE.powerOffBonus;
                        }
                        return CO_KINDLE.powerBaseOffBonus;
                    default:
                        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                        {
                            if (building !== null)
                            {
                                return CO_KINDLE.d2dCoZoneOffBonus;
                            }
                            return CO_KINDLE.d2dCoZoneBaseOffBonus;
                        }
                        else if (building !== null)
                        {
                            if (map === null ||
                                (map !== null && map.getGameRules().getCoGlobalD2D()))
                            {
                                return CO_KINDLE.d2dOffBonus;
                            }
                        }
                        break;
                    }
                }
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_KINDLE.d2dCoZoneBaseOffBonus;
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
                return CO_KINDLE.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_KINDLE.d2dCoZoneDefBonus;
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
                return ["ZCOUNIT_PARTISAN"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Jugger and Koal's commanding officer. Has a blunt, queen-like personality.");
    };
    this.getHits = function(co)
    {
        return qsTr("Anything chic");
    };
    this.getMiss = function(co)
    {
        return qsTr("Anything passe");
    };
    this.getCODescription = function(co)
    {
        return qsTr("An upper-crust CO who excels at urban warfare. Firepower of all units is increased on buildings.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_KINDLE.d2dOffBonus];
        }
        var text = qsTr("\nSpecial Unit:\nPartisan\n") +
               qsTr("\nGlobal Effect: \nKindle's units on buildings gain +%0% firepower.") +
               qsTr("\n\nCO Zone Effect: \nKindle's units gain +%2% firepower and +%3% defence. Her units on buildings gain a total of +%1% firepower.");
        text = replaceTextArgs(text, [values[0], CO_KINDLE.d2dCoZoneOffBonus, CO_KINDLE.d2dCoZoneBaseOffBonus, CO_KINDLE.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Deals -%0 HP of damage to enemy units on buildings. Kindle's units gain +%2% firepower and +%3% defence. Her units on buildings gain a total of +%1% firepower.");
        text = replaceTextArgs(text, [CO_KINDLE.powerDamage, CO_KINDLE.powerOffBonus, CO_KINDLE.powerBaseOffBonus, CO_KINDLE.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Urban Blight");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Kindle's units gain a base of +%0% firepower and +%1% defence. Her units gain a bonus of +%2% firepower per property she owns and an additional +%3% firepower if they are on buildings.");
        text = replaceTextArgs(text, [CO_KINDLE.powerBaseOffBonus, CO_KINDLE.powerDefBonus, CO_KINDLE.superPowerOffMultiplier, CO_KINDLE.superPowerOffBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("High Society");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("So, you want to see me get serious? Very well! You asked for it!"),
                qsTr("Even the prettiest rose has thorns!"),
                qsTr("I am Kindle, gorgeous AND clever!"),
                qsTr("Let us play a more dangerous game, shall we? I grow weary of this."),
                qsTr("Aha ha ha! Beg for mercy, rat!"),
                qsTr("Still putting up a fight? Unbelievable!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Winning is almost as wonderful as I am. Aha ha ha!"),
                qsTr("I won again? This is getting so boring."),
                qsTr("Aha ha ha! Pathetic.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Losing to you corn-fed country folk is enough to give me frown lines."),
                qsTr("That was a disappointment.")];
    };
    this.getName = function()
    {
        return qsTr("Kindle");
    };
    this.getAiUsePower = function(co, powerSurplus, turnMode)
    {
        // cop spam
        if (co.canUseSuperpower())
        {
            return GameEnums.PowerMode_Superpower;
        }
        else if (co.canUsePower())
        {
            return GameEnums.PowerMode_Power;
        }
        return GameEnums.PowerMode_Off;
    };
}

Constructor.prototype = CO;
var CO_KINDLE = new Constructor();
