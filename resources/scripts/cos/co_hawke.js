var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2"];
    };

    this.getAiUsePower = function(co, powerSurplus, turnMode)
    {
        if (co.canUseSuperpower())
        {
            return GameEnums.PowerMode_Superpower;
        }
        return GameEnums.PowerMode_Off;
    };

    this.init = function(co, map)
    {
        co.setPowerStars(5);
        co.setSuperpowerStars(4);
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
                audio.addMusic("resources/music/cos/hawke.ogg", 981, 63770);
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, GameEnums.PowerMode_Power, map);

        CO_HAWKE.hawkeDamage(co, CO_HAWKE.powerDamage, dialogBuilder, map);

        dialogBuilder.displayAnimation();
    };

    this.hawkeDamage = function(co, value, parentBuilder, map)
    {

        var player = co.getOwner();
        var units = player.getUnits();
        units.randomize();
        var enemyUnits = player.getEnemyUnits();
        enemyUnits.randomize();
        for (var i = 0; i < enemyUnits.size(); i++)
        {
            units.append(enemyUnits.at(i));
        }
        var unitsBuilder = new UNITS_ANIMATION_BUILDER(units, map);
        unitsBuilder.setRandomize(false);
        unitsBuilder.setSound("power4.wav");
        unitsBuilder.setSprite("power4");
        unitsBuilder.setPerAnimationFunction((unit, animation, map) => 
            {
                if (unit.getOwner() == co.getOwner())
                {
                    animation.writeDataInt32(unit.getX());
                    animation.writeDataInt32(unit.getY());
                    animation.writeDataInt32(value);
                    animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
                }
                else
                {
                    animation.writeDataInt32(unit.getX());
                    animation.writeDataInt32(unit.getY());
                    animation.writeDataInt32(value);
                    animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamage");
                }
            }
        );
        parentBuilder.queueAnimationBuilder(unitsBuilder);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        CO_HAWKE.hawkeDamage(co, CO_HAWKE.superPowerDamage, dialogBuilder, map);

        dialogBuilder.displayAnimation();
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BH";
    };

    this.superPowerDamage = 2;
    this.superPowerOffBonus = 40;

    this.powerDamage = 1;
    this.powerOffBonus = 40;
    this.powerDefBonus = 20;

    this.d2dOffBonus = 10;
    this.d2dDefBonus = 0;

    this.d2dCoZoneOffBonus = 40;
    this.d2dCoZoneDefBonus = 20;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_HAWKE.superPowerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_HAWKE.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_HAWKE.d2dCoZoneOffBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_HAWKE.d2dOffBonus
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
                return CO_HAWKE.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_HAWKE.d2dCoZoneDefBonus;
            }
            else if (map === null ||
                     (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return CO_HAWKE.d2dDefBonus;
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
        return qsTr("A commander of the Black Hole Army who will stop at nothing to achieve his goals.");
    };
    this.getHits = function(co)
    {
        return qsTr("Black coffee");
    };
    this.getMiss = function(co)
    {
        return qsTr("Incompetence");
    };
    this.getCODescription = function(co)
    {
        return qsTr("His CO-Zone provides a more potent firepower bonus. However, his CO power gauge is longer than average.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_HAWKE.d2dOffBonus, CO_HAWKE.d2dDefBonus];
        }
        var text = qsTr("\nGlobal Effect: \nHawke's units gain +%0% firepower and +%1% defence.") +
            qsTr("\n\nCO Zone Effect: \nHawke's units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [values[0], values[1],
                                      CO_HAWKE.d2dCoZoneOffBonus, CO_HAWKE.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("A wave of dark energy deals -%0 HP of damage to all enemy units and restores +%0 HP to all of Hawke's units. Hawke's units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [CO_HAWKE.powerDamage, CO_HAWKE.powerOffBonus, CO_HAWKE.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Black Wave");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("A storm of dark energy deals -%0 HP of damage to all enemy units and restores +%0 HP to all of Hawke's units. Hawke's units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [CO_HAWKE.superPowerDamage, CO_HAWKE.superPowerOffBonus, CO_HAWKE.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Black Storm");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("This is the end."),
                qsTr("Farewell."),
                qsTr("I give you credit for pushing me this far."),
                qsTr("You have inspired me to take action."),
                qsTr("To be defeated is to lose everything. Are you prepared for that outcome?"),
                qsTr("You leave me no choice.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Sad... You were no match for me."),
                qsTr("There is no chance.  My victory was assured.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("That's it then. We shall withdraw."),
                qsTr("...This is one situation I never expected myself to be in.")];
    };
    this.getName = function()
    {
        return qsTr("Hawke");
    };
}

Constructor.prototype = CO;
var CO_HAWKE = new Constructor();
