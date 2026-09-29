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
        ownUnitsBuilder.setSounds(["power7_1.wav", "power7_2.wav"]);
        ownUnitsBuilder.setSprite("power7");
        dialogBuilder.queueAnimationBuilder(ownUnitsBuilder);

        dialogBuilder.displayAnimation();
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogBuilder = new DIALOG_ANIMATION_BUILDER(co, powerMode, map);

        var ownUnitsBuilder = new OWN_UNITS_ANIMATION_BUILDER(co, map);
        ownUnitsBuilder.setSounds(["power12_1.wav", "power12_2.wav"]);
        ownUnitsBuilder.setSprite("power12");
        ownUnitsBuilder.setAnimationAmount(7);
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
                audio.addMusic("resources/music/cos/power.ogg", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.ogg", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/xavier.ogg", 270, 74593);
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
        return "GS";
    };
    this.superpowerBonus = 70;

    this.powerFirepowerBonus = 10;
    this.powerSupportedFirepowerBonus = 40;
    this.powerDefBonus = 10;

    this.d2dSupportedFirepowerBonus = 10;
    this.d2dCoZoneFirepowerBonus = 10;
    this.d2dCoZoneSupportedFirepowerBonus = 30;
    this.d2dCoZoneDefBonus = 10;

    this.d2dMinLuckHp = 3;
    this.d2dCoZoneMinLuckHp = 5;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (!CO.isActive(co))
        {
            return 0;
        }
        var supported = defender !== null && CO_XAVIER.hasAdjacentSupport(attacker, defPosX, defPosY, map);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (supported)
            {
                return CO_XAVIER.superpowerBonus;
            }
            return CO_XAVIER.powerFirepowerBonus;
        case GameEnums.PowerMode_Power:
            if (supported)
            {
                return CO_XAVIER.powerSupportedFirepowerBonus;
            }
            return CO_XAVIER.powerFirepowerBonus;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (supported)
                {
                    return CO_XAVIER.d2dCoZoneSupportedFirepowerBonus;
                }
                return CO_XAVIER.d2dCoZoneFirepowerBonus;
            }
            else if (supported && (map === null || map.getGameRules().getCoGlobalD2D()))
            {
                return CO_XAVIER.d2dSupportedFirepowerBonus;
            }
            return 0;
        }
    };

    this.hasAdjacentSupport = function(attacker, targetX, targetY, map)
    {
        if (map === null)
        {
            return false;
        }
        return CO_XAVIER.isAlliedUnit(attacker, targetX, targetY + 1, map) ||
               CO_XAVIER.isAlliedUnit(attacker, targetX, targetY - 1, map) ||
               CO_XAVIER.isAlliedUnit(attacker, targetX + 1, targetY, map) ||
               CO_XAVIER.isAlliedUnit(attacker, targetX - 1, targetY, map);
    };

    this.isAlliedUnit = function(attacker, x, y, map)
    {
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null &&
                attacker.getOwner() === unit.getOwner() &&
                attacker !== unit)
            {
                return true;
            }
        }
        return false;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_XAVIER.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_XAVIER.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, action, luckmode, map, damageContext = null)
    {
        if (!CO.isActive(co) || damageContext === null || luckmode === GameEnums.LuckDamageMode_Off ||
            !(damageContext.maxPositiveLuckDamage > damageContext.positiveLuckDamage))
        {
            return 0;
        }
        if (co.getPowerMode() <= GameEnums.PowerMode_Off)
        {
            var maxHp = CO_XAVIER.d2dMinLuckHp;
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                maxHp = CO_XAVIER.d2dCoZoneMinLuckHp;
            }
            else if (map !== null && !map.getGameRules().getCoGlobalD2D())
            {
                return 0;
            }
            if (globals.roundUp(damageContext.baseHp) > maxHp)
            {
                return 0;
            }
        }
        if (!CO_XAVIER.hasAdjacentSupport(attacker, defPosX, defPosY, map))
        {
            return 0;
        }
        var luckDamage = damageContext.luckDamage - damageContext.positiveLuckDamage + damageContext.maxPositiveLuckDamage;
        var minimumDamage = ACTION_FIRE.calcDamageWithLuck(damageContext, luckDamage, map);
        return ACTION_FIRE.getLuckDamageBonus(damageContext, damage, minimumDamage);
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Fulfills his duties without second thought or consideration of the after-effects of his actions. Wears a pair of fake claws.");
    };
    this.getHits = function(co)
    {
        return qsTr("Uncertainty");
    };
    this.getMiss = function(co)
    {
        return qsTr("Definitives");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Xavier rewards coordinated attacks. Another owned unit directly beside the target provides support, boosting firepower and guaranteeing maximum positive luck for wounded units.");
    };
    this.getLongCODescription = function(co, map)
    {
        var text = qsTr("An attack or counterattack is supported when another owned unit is directly adjacent to its target. Diagonals do not count, and support does not stack. Negative luck still rolls normally.");
        if (map === null || map.getGameRules().getCoGlobalD2D())
        {
            text += qsTr("\n\nGlobal Effect: \nSupported attacks gain +%0% firepower and maximum positive luck at %1 displayed HP or less.");
        }
        else
        {
            text += qsTr("\n\nGlobal Effect: \nNo effect.");
        }
        text += qsTr("\n\nCO Zone Effect: \nSupported attacks gain +%2% firepower and maximum positive luck at %3 displayed HP or less. Unsupported attacks gain +%4% firepower. Units gain +%5% defence.");
        return replaceTextArgs(text, [CO_XAVIER.d2dSupportedFirepowerBonus, CO_XAVIER.d2dMinLuckHp,
                                     CO_XAVIER.d2dCoZoneSupportedFirepowerBonus, CO_XAVIER.d2dCoZoneMinLuckHp,
                                     CO_XAVIER.d2dCoZoneFirepowerBonus, CO_XAVIER.d2dCoZoneDefBonus]);
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Supported attacks and counterattacks gain +%0% firepower and maximum positive luck at any HP. Unsupported attacks gain +%1% firepower. Units gain +%2% defence. Negative luck still rolls normally.");
        return replaceTextArgs(text, [CO_XAVIER.powerSupportedFirepowerBonus, CO_XAVIER.powerFirepowerBonus, CO_XAVIER.powerDefBonus]);
    };
    this.getPowerName = function(co)
    {
        return qsTr("Phasing Charge");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Supported attacks and counterattacks gain +%0% firepower and maximum positive luck at any HP. Unsupported attacks gain +%1% firepower. Units gain +%2% defence. Negative luck still rolls normally.");
        return replaceTextArgs(text, [CO_XAVIER.superpowerBonus, CO_XAVIER.powerFirepowerBonus, CO_XAVIER.powerDefBonus]);
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Reality Minus");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("You know, the laws of physics are made to be broken. Observe."),
                qsTr("You might say I'm a rather...twisted individual."),
                qsTr("Reality is only trivial. Watch and learn."),
                qsTr("Allow me to reprimand, for your atrocious command."),
                qsTr("I'm sure you'll get quite a scare out of this!"),
                qsTr("Are you sure of the truth in what you see?")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I must return to my own work now."),
                qsTr("See, this is reality. You never stood a chance."),
                qsTr("You are intellectually inferior to me. Simple as that.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is not my reality."),
                qsTr("You were supposed to be intellectually inferior to me.")];
    };
    this.getName = function()
    {
        return qsTr("Xavier");
    };
}

Constructor.prototype = CO;
var CO_XAVIER = new Constructor();
