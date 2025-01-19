var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(6);
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
                audio.addMusic("resources/music/cos/von_bolt.ogg", 47693, 113984);
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_VON_BOLT.throwLaserray(co, CO_VON_BOLT.powerDamage, CO_VON_BOLT.powerRange, powerNameAnimation, map);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_VON_BOLT.throwLaserray(co, CO_VON_BOLT.superPowerDamage, CO_VON_BOLT.superPowerRange, powerNameAnimation, map);
    };

    this.throwLaserray = function(co, damage, range, powerNameAnimation, map)
    {
        var meteorTarget = co.getOwner().getRockettarget(range, damage);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);

        animation2.setStartOfAnimationCall("CO_VON_BOLT", "preAnimationLaserray");
        animation2.setEndOfAnimationCall("CO_VON_BOLT", "postAnimationLaserray");
        CO_VON_BOLT.postAnimationLaserrayTarget = meteorTarget;
        CO_VON_BOLT.postAnimationLaserrayDamage = damage;
        CO_VON_BOLT.postAnimationLaserrayRange = range;
    };

    this.preAnimationLaserray = function(animation, map)
    {
        map.centerMap(CO_VON_BOLT.postAnimationLaserrayTarget.x, CO_VON_BOLT.postAnimationLaserrayTarget.y);
    };
    this.postAnimationLaserrayTarget = null;
    this.postAnimationLaserrayDamage = 0;
    this.postAnimationLaserrayRange = 0;
    this.postAnimationLaserray = function(animation, map)
    {
        var laserTarget = CO_VON_BOLT.postAnimationLaserrayTarget;
        var damage = CO_VON_BOLT.postAnimationLaserrayDamage;
        var range = CO_VON_BOLT.postAnimationLaserrayRange;
        var fields = globals.getCircle(0, range);
        // check all target fields
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var x = fields.at(i).x + laserTarget.x;
            var y = fields.at(i).y + laserTarget.y;
            // check with which weapon we can attack and if we could deal damage with this weapon
            if (map.onMap(x, y))
            {
                var unit = map.getTerrain(x, y).getUnit();
                if (unit !== null)
                {
                    unit.setHasMoved(true);
                    var hp = unit.getHpRounded();
                    if (hp - damage <= 0.1)
                    {
                        // set hp to very very low
                        unit.setHp(0.1);
                    }
                    else
                    {
                        unit.setHp(hp - damage);
                    }
                }
            }
        }
        CO_VON_BOLT.postAnimationLaserrayTarget = null;
        CO_VON_BOLT.postAnimationLaserrayDamage = 0;
        CO_VON_BOLT.postAnimationLaserrayRange = 0;
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BG";
    };

    this.superPowerDamage = 3;
    this.superPowerRange = 3;

    this.powerDamage = 1;
    this.powerRange = 2;
    this.powerOffBonus = 40;
    this.powerDefBonus = 40;

    this.d2dCoZoneOffBonus = 40;
    this.d2dCoZoneDefBonus = 40;

    this.d2dOffBonus = 10;
    this.d2dDefBonus = 10;

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
                return CO_VON_BOLT.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_VON_BOLT.d2dCoZoneOffBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_VON_BOLT.d2dOffBonus;
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
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return CO_VON_BOLT.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_VON_BOLT.d2dCoZoneDefBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_VON_BOLT.d2dDefBonus;
                }
            }
        }
        return 0;
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
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Former commander-in-chief of the Bolt Guard forces. A mysterious old man who has been alive for a very, very long time. Mastermind of the Omega war.");
    };
    this.getHits = function(co)
    {
        return qsTr("Long life");
    };
    this.getMiss = function(co)
    {
        return qsTr("Young 'uns!");
    };
    this.getCODescription = function(co)
    {
        return qsTr("His CO-Zone is more potent than most.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_VON_BOLT.d2dOffBonus, CO_VON_BOLT.d2dDefBonus];
        }
        var text = qsTr("\nSpecial Unit:\nCrystal Tanks\n\nGlobal Effect:\nVon Bolt's units have +%0% firepower and +%1% defence.") +
               qsTr("\n\nCO Zone Effect: \nVon Bolt's units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [values[0], values[1], CO_VON_BOLT.d2dCoZoneOffBonus, CO_VON_BOLT.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Fires a small shock wave, targeting the largest accumulation of enemy funds in a %0-space radius, dealing -%1 HP of damage to all affected units. Affected units are paralyzed and cannot move on their next turn. Von Bolt's units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [CO_VON_BOLT.powerRange, CO_VON_BOLT.powerDamage, CO_VON_BOLT.powerOffBonus, CO_VON_BOLT.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Shockwave");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Fires a massive laser array, targeting the largest accumulation of enemy funds in a %0-space radius, dealing -%1 HP of damage to all affected units. Affected units are paralyzed and cannot move on their next turn. Von Bolt's units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [CO_VON_BOLT.superPowerRange, CO_VON_BOLT.superPowerDamage, CO_VON_BOLT.powerOffBonus, CO_VON_BOLT.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Deus Ex Machina");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("No matter how powerful...hhh...you are, you're...hhh...still a worm on a hook..."),
                qsTr("Well...hhh...let me have a taste...hhh...of your soul."),
                qsTr("Heh heh heh... Stop panicking... it's...hhh...almost over..."),
                qsTr("Heh heh heh... The darkness rises."),
                qsTr("The sweet scent of destruction...hhh...it makes me...hhh...feel young!"),
                qsTr("Hold still...hhh...I'm going to...hhh...suck the marrow from your bones.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Predator...prey...hhh...I hunt them all..."),
                qsTr("Heh heh heh... Pathetic."),
                qsTr("Even death...hhh...fears me...")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("What? Why are you...hhh...youth so strong?"),
                qsTr("Nooo...hhh...I want to...hhh...live forever...")];
    };
    this.getName = function()
    {
        return qsTr("Von Bolt");
    };
}

Constructor.prototype = CO;
var CO_VON_BOLT = new Constructor();
