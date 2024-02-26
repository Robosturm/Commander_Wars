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
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);
    };
    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        var size = units.size();
        for (var i = 0; i < size; i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 7)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power12_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power12_2.wav", 1, delay);
            }
            if (animations.length < 7)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        var variables = co.getVariables();
        var dmgModVar = variables.createVariable("CONRAD_DMG_MOD");
        dmgModVar.writeDataFloat(CO_CONRAD.superPowerBonusLuckDamage);
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
                audio.addMusic("resources/music/cos/conrad.mp3", 4027, 104641)
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
        return "TI";
    };

    this.startOfTurn = function(co, map)
    {
        var variables = co.getVariables();
        // clean up possible variables from scop
        variables.clear();
    }

    this.superPowerBonusLuckDamage = 50;

    this.powerVisionMultiplier = 5;
    this.powerBonusVisionRange = 1;
    this.powerDefBonus = 10;
    this.powerOffBonus = 10;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneVisionMultiplier = 5;

    this.d2dCounterDamageBonus = -20;
    this.d2dVisionMultiplier = 1;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var ret = 0;
            if (isDefender)
            {
                ret = CO_CONRAD.d2dCounterDamageBonus;
            }
            var units = co.getOwner().getUnits();
            var visionCount = 0;
            var size = units.size();
            for (var i = 0; i < size; i++)
            {
                var unit = units.at(i);
                var x = unit.getX();
                var y = unit.getY();
                var distance = Math.abs(x - defPosX) + Math.abs(y - defPosY);
                if (distance <= unit.getVision(Qt.point(x, y)))
                {
                    visionCount += 1;
                }
            }
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                ret += visionCount * CO_CONRAD.powerVisionMultiplier + CO_CONRAD.powerOffBonus;
                break;
            case GameEnums.PowerMode_Power:
                ret += visionCount * CO_CONRAD.powerVisionMultiplier + CO_CONRAD.powerOffBonus;
                break;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    ret += visionCount * CO_CONRAD.d2dCoZoneVisionMultiplier + CO_CONRAD.d2dCoZoneOffBonus;
                }
                else
                {
                    ret += visionCount * CO_CONRAD.d2dVisionMultiplier;
                }
            }
            return ret;
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
                return CO_CONRAD.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_CONRAD.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };

    this.getBonusLuck = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            return -ACTION_FIRE.getDefaultLuck(unit);
        }
        return 0;
    };

    this.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (isDefender === false)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    var variables = co.getVariables();
                    var dmgModVar = variables.createVariable("CONRAD_DMG_MOD");
                    var bonusDamage = dmgModVar.readDataFloat();
                    return bonusDamage + attackerBaseHp / 2;
                case GameEnums.PowerMode_Power:
                default:
                    break;
                }
            }
            return attackerBaseHp / 2;
        }
        return 0;
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co) && defender !== null)
        {
            if (gotAttacked === false && attacker.getOwner() === co.getOwner())
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    var variables = co.getVariables();
                    var dmgModVar = variables.createVariable("CONRAD_DMG_MOD");
                    var defenderDamage = defender.getHp() * 10;
                    // overkill case
                    if (defenderDamage < 0)
                    {
                        dmgModVar.writeDataFloat(-defenderDamage);
                    }
                    else
                    {
                        dmgModVar.writeDataFloat(0);
                    }
                    break;
                case GameEnums.PowerMode_Power:
                    break;
                default:
                    break;
                }
            }
        }
    };

    this.getVisionrangeModifier = function(co, unit, posX, posY, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 0;
            case GameEnums.PowerMode_Power:
                return CO_CONRAD.powerBonusVisionRange;
            default:
                return 0;
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
                return ["ZCOUNIT_INTEL_TRUCK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A damage analyst from Teal Isles who favors precise tactics. Very intelligent, but inexperienced.");
    };
    this.getHits = function(co)
    {
        return qsTr("Precise information");
    };
    this.getMiss = function(co)
    {
        return qsTr("Unknown variables");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Conrad is so focused on being precise that it leaves no room for lucky strikes and his units have weaker counter strength. However, units are good at assisting one another with information.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nIntel truck\n") +
               qsTr("\nGlobal Effect: \nConrad's units gain +%0% firepower when attacking an enemy unit for each of his own units that can see it, which includes enhanced vision from terrain. His units always deal average luck damage and their counterattacks are %1% weaker.") +
               qsTr("\n\nCO Zone Effect: \nConrad's units gain +%3% firepower and +%4% defence. They also gain an additional +%2% firepower when attacking an enemy unit for each of his own units that can see it, which includes enhanced vision from terrain.");
        text = replaceTextArgs(text, [CO_CONRAD.d2dVisionMultiplier, CO_CONRAD.d2dCounterDamageBonus, CO_CONRAD.d2dCoZoneVisionMultiplier, CO_CONRAD.d2dCoZoneOffBonus, CO_CONRAD.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("All of Conrad's units gain +%0 vision, +%2% firepower, and +%3% defence. They also gain an additional +%1% firepower when attacking an enemy unit for each of his own units that can see it, which includes enhanced vision from terrain.");
        text = replaceTextArgs(text, [CO_CONRAD.powerBonusVisionRange, CO_CONRAD.powerVisionMultiplier, CO_CONRAD.powerOffBonus, CO_CONRAD.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Calculated Strike");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Conrad's next attack this turn deals +%0% luck damage. For the rest of the turn, all combat damage that exceeds the enemy's current health is carried over to the next engagement. Conrad's units gain +%2% firepower and +%3% defence. They also gain an additional +%1% firepower when attacking an enemy unit for each of his own units that can see it, which includes enhanced vision from terrain.");
        text = replaceTextArgs(text, [CO_CONRAD.superPowerBonusLuckDamage, CO_CONRAD.powerVisionMultiplier, CO_CONRAD.powerOffBonus, CO_CONRAD.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Precision Assault");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("You might be more experienced, but I'll still teach you a thing or two!"),
                qsTr("I won't go down that easily! This battle isn't even close to being finished!"),
                qsTr("You have a 0.1% chance of surviving this battle with me."),
                qsTr("My attack plan is perfect! I don't need luck!"),
                qsTr("Your strategy doesn't quite add up to par with mine!"),
                qsTr("I have all the information I need to turn this battle around.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Solid tactics get solid results!"),
                qsTr("That was as easy as I expected it to be. Numbers don't lie."),
                qsTr("Either I'm getting better, or you guys are getting far worse!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I obviously have more to learn."),
                qsTr("Did I miscalculate...? Impossible!")];
    };
    this.getName = function()
    {
        return qsTr("Conrad");
    };
}

Constructor.prototype = CO;
var CO_CONRAD = new Constructor();
