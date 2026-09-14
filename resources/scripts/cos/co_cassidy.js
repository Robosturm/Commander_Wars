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

    this.activatePower = function(co, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setSounds(["power8_1.wav", "power8_2.wav"]);
        ownUnitsBuilder.setSprite("power8");
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        CO_CASSIDY.cassidyDamage(co, CO_CASSIDY.superPowerDamage, dialogBuilder, map);

        dialogBuilder.displayAnimation();
    };

    this.cassidyDamage = function(co, value, parentBuilder, map)
    {
        var enemyUnitsBuilder = new ENEMY_UNITS_ANIMATION_BUILDER(co, map);
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
                audio.addMusic("resources/music/cos/cassidy.ogg", 76, 75279)
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "TI";
    };

    this.superPowerDamage = 1;
    this.superPowerBonus = 80;

    this.powerBonus = 60;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;

    this.d2dOffBonus = 30;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneBonus = 50;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var getsBonus = false;
            if (defender !== null)
            {
                getsBonus = attacker.getHp() >= defender.getHp();
            }
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (getsBonus)
                {
                    return CO_CASSIDY.superPowerBonus;
                }
                return CO_CASSIDY.powerOffBonus;
            case GameEnums.PowerMode_Power:
                if (getsBonus)
                {
                    return CO_CASSIDY.powerBonus;
                }
                return CO_CASSIDY.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (getsBonus)
                    {
                        return CO_CASSIDY.d2dCoZoneBonus;
                    }
                    return CO_CASSIDY.d2dCoZoneOffBonus;
                }
                else if (getsBonus)
                {
                    if (map === null ||
                        (map !== null && map.getGameRules().getCoGlobalD2D()))
                    {
                        return CO_CASSIDY.d2dOffBonus;
                    }
                }
                break;
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
                return CO_CASSIDY.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_CASSIDY.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("An extremely belligerent member of Teal Island that is notorious for her ruthlessness. She fights purely for the thrill of battle and is feared by both enemies and allies.");
    };
    this.getHits = function(co)
    {
        return qsTr("Roses");
    };
    this.getMiss = function(co)
    {
        return qsTr("Violets");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Cassidy's ruthlessness rubs off on her troops. Units show no mercy to enemy units equal to or below their own strength.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_CASSIDY.d2dOffBonus];
        }

        var text = qsTr("\nGlobal Effect: \nCassidy's units gain +%0% firepower when attacking units with equal or less health.") +
                qsTr("\n\nCO Zone Effect: \nCassidy's units gain +%1% firepower when attacking units with equal or less health and +%2% firepower otherwise. Her units gain +%3% defence.");
        text = replaceTextArgs(text, [values[0], CO_CASSIDY.d2dCoZoneBonus, CO_CASSIDY.d2dCoZoneOffBonus, CO_CASSIDY.d2dCoZoneDefBonus]);
        return text;

    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Cassidy's units gain +%0% firepower when attacking units with equal or less health and +%1% firepower otherwise. Her units gain +%2% defence.");
        text = replaceTextArgs(text, [CO_CASSIDY.powerBonus, CO_CASSIDY.powerOffBonus, CO_CASSIDY.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Rampage");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("All enemy units suffer -%0 HP of damage. Cassidy's units gain +%1% firepower when attacking units with equal or less health and +%2% firepower otherwise. Her units gain +%3% defence.");
        text = replaceTextArgs(text, [CO_CASSIDY.superPowerDamage, CO_CASSIDY.superPowerBonus, CO_CASSIDY.powerOffBonus, CO_CASSIDY.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Scorched Earth");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Take no prisoners!"),
                qsTr("Ahahaha! I'll destroy you all!"),
                qsTr("You won't like me when I am angry!"),
                qsTr("I swear to make you rue this day!"),
                qsTr("Burn everything! I don't want to see anything left untouched!"),
                qsTr("I have fury!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("You better hope we do not meet again."),
                qsTr("I love this job!"),
                qsTr("I wish the battle had lasted a little longer. Oh well.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("You will regret this..."),
                qsTr("Kill them... kill them all...")];
    };
    this.getName = function()
    {
        return qsTr("Cassidy");
    };
}

Constructor.prototype = CO;
var CO_CASSIDY = new Constructor();
