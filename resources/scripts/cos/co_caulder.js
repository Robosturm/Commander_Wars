var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(7);
        co.setSuperpowerStars(5);
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        if (turnMode === GameEnums.AiTurnMode_StartOfDay)
        {
            if (co.canUseSuperpower())
            {
                return GameEnums.PowerMode_Superpower;
            }
            else if (powerSurplus <= 0.5 &&
                     co.canUsePower())
            {
                return CO.getAiUsePowerAtUnitCount(co, powerSurplus, turnMode, repairUnits);
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setSounds(["power9_1.wav", "power9_2.wav"]);
        ownUnitsBuilder.setSprite("power9");
        ownUnitsBuilder.setPerAnimationFunction((unit, animation, map) => 
            {
                animation.writeDataInt32(unit.getX());
                animation.writeDataInt32(unit.getY());
                animation.writeDataInt32(CO_CAULDER.powerHeal);
                animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
            }
        );
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setSound("power11.wav");
        ownUnitsBuilder.setSprite("power11");
        ownUnitsBuilder.setPerAnimationFunction((unit, animation, map) => 
            {
                animation.writeDataInt32(unit.getX());
                animation.writeDataInt32(unit.getY());
                animation.writeDataInt32(CO_CAULDER.superPowerHeal);
                animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
            }
        );
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_ids_dc.ogg", 0, 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_ids_dc.ogg", 0, 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/caulder.ogg", 6755, 60471)
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
        return "DM";
    };

    this.superPowerHeal = 10;
    this.superPowerBonus = 60;

    this.powerHeal = 5;
    this.powerBonus = 60;

    this.d2dHealing = 0;
    this.d2dOffBonus = 0;
    this.d2dDefBonus = 0;

    this.d2dCoZoneOffBonus = 60;
    this.d2dCoZoneDefBonus = 60;
    this.d2dCoZoneHealing = 5;

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_CAULDER.superPowerBonus;
            case GameEnums.PowerMode_Power:
                return CO_CAULDER.powerBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_CAULDER.d2dCoZoneDefBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_CAULDER.d2dDefBonus;
                }
            }
        }
        return 0;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_CAULDER.superPowerBonus;
            case GameEnums.PowerMode_Power:
                return CO_CAULDER.powerBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_CAULDER.d2dCoZoneOffBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_CAULDER.d2dOffBonus;
                }
            }
        }
    };

    this.startOfTurn = function(co, map)
    {
        if (CO.isActive(co))
        {
            var player = co.getOwner();
            if (!player.getIsDefeated())
            {
                var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
                ownUnitsBuilder.setUnitFilter((unit, map) => 
                    {
                        if (co.inCORange(unit.getPosition(), unit))
                        {
                            return CO_CAULDER.d2dCoZoneHealing > 0;
                        }
                        else
                        {
                            return ((map === null || (map !== null && map.getGameRules().getCoGlobalD2D())) &&
                                CO_CAULDER.d2dHealing);
                        }
                    }
                );
                ownUnitsBuilder.setSound("power0.wav");
                ownUnitsBuilder.setSprite("power0");
                ownUnitsBuilder.setPerAnimationFunction((unit, animation, map) => 
                    {
                        var healing = CO_CAULDER.d2dHealing;
                        if (co.inCORange(unit.getPosition(), unit)){
                            healing = CO_CAULDER.d2dCoZoneHealing;
                        }
                        UNIT.repairUnit(unit, healing, map);
                        if (!map.getCurrentViewPlayer().getFieldVisible(unit.getX(), unit.getY()))
                        {
                            animation.setVisible(false);
                        }
                    }
                );

                ownUnitsBuilder.displayAnimation();
            }
        }
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
                return ["ZCOUNIT_CRYSTAL_TANK"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Head of IDS - the research department of Dark Matter. Conducts in inhuman experiments. All he wants is to be free to satisfy his curiosity.");
    };
    this.getHits = function(co)
    {
        return qsTr("Unrestricted experiments");
    };
    this.getMiss = function(co)
    {
        return qsTr("Ethics");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Caulder can reinforce his army while he is on the field.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_CAULDER.d2dOffBonus, CO_CAULDER.d2dDefBonus, CO_CAULDER.d2dHealing];
        }
        var text = qsTr("\nSpecial Unit:\nCrystal Tanks\n") +
                   qsTr("\nGlobal Effect: \nCaulder's units gain +%0% firepower and +%1% defense. They also repair +%2 HP at the start of his turn.") +
                   qsTr("\n\nCO Zone Effect: \nCaulder's units gain +%3% firepower and +%4% defense. They also repair +%5 HP at the start of his turn.");
        text = replaceTextArgs(text, [values[0], values[1], values[2],
                                      CO_CAULDER.d2dCoZoneOffBonus, CO_CAULDER.d2dCoZoneDefBonus, CO_CAULDER.d2dCoZoneHealing]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("All of Caulder's units restore +%0 HP and gain +%1% firepower and +%1% defense.");
        text = replaceTextArgs(text, [CO_CAULDER.powerHeal, CO_CAULDER.powerBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Mass Regeneration");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("All of Caulder's units restore +%0 HP and gain +%1% firepower and +%1% defense.");
        text = replaceTextArgs(text, [CO_CAULDER.superPowerHeal, CO_CAULDER.superPowerBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Perfect Healing");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Your silly moral values have no place on the battlefield."),
                qsTr("You are a worthy specimen for further research."),
                qsTr("Your ethics make you weak. Allow me to demonstrate."),
                qsTr("I am curious to see if you can survive this.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Interesting. Very interesting."),
                qsTr("Quite satisfactory."),
                qsTr("Fascinating...")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I may have to use the Great Owl after all."),
                qsTr("What useless clones. I will have to produce better ones.")];
    };
    this.getName = function()
    {
        return qsTr("Caulder");
    };
}

Constructor.prototype = CO;
var CO_CAULDER = new Constructor();
