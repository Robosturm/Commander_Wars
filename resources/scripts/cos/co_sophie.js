var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/sophie.mp3", 27799, 69801);
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power5_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power5_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
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
        for (var i = 0; i < units.size(); i++)
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
    };

    
    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };

    this.superPowerSecondAttackOffMalus = 30;
    this.superPowerDefenderDamageReduction = 0.2;

    this.powerOffBonus = 20;
    this.powerDefBonus = 20;

    this.d2dDefenderDamageReduction = 0.0;

    this.d2dCoZoneDefenderDamageReduction = 0.2;
    this.d2dCoZoneDefBonus = 20;
    this.d2dCoZoneOffBonus = 20;

    this.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                       defender, defPosX, defPosY, isDefender, luckMode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (isDefender === true)
                {
                    return damage * CO_SOPHIE.superPowerDefenderDamageReduction;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (isDefender === true)
                {
                    return damage;
                }
                break;
            default:
                if (isDefender === true)
                {
                    if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                    {
                        return damage * CO_SOPHIE.d2dCoZoneDefenderDamageReduction;
                    }
                    else
                    {
                        return damage * CO_SOPHIE.d2dDefenderDamageReduction;
                    }
                }
                break;
            }
        }
        return 0;
    };

    this.startOfTurn = function(co, map)
    {
        var units = co.getOwner().getUnits();
        var size = units.size();
        for (var i = 0; i < size; i++)
        {
            // creates the variable or returns the existing one
            var variable = units.at(i).getVariables().createVariable("SOPHIE_SCOP");
            // set attacked twice this turn to false
            variable.writeDataBool(false);
        }
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co))
        {
            if (gotAttacked === false && attacker.getOwner() === co.getOwner())
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (!gotAttacked && defender.getHp() > 0 && attacker.getHp() > 0)
                    {
                        // we're the attacker
                        var variable = attacker.getVariables().createVariable("SOPHIE_SCOP");
                        if (variable.readDataBool() === false)
                        {
                            variable.writeDataBool(true);
                            var damageResult = ACTION_FIRE.calcBattleDamage2(map, action, attacker, Qt.point(attacker.getX(), attacker.getY()),
                                                                             defender.getX(), defender.getY(), GameEnums.LuckDamageMode_On);
                            // do another attack
                            ACTION_FIRE.battle(map, attacker, damageResult.x, damageResult.y,
                                               defender.getX(), defender.getY(), damageResult.width, damageResult.height,
                                               true);
                        }
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

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (isDefender === false)
                {
                    var variable = attacker.getVariables().createVariable("SOPHIE_SCOP");
                    if (variable.readDataBool() === true)
                    {
                        return -CO_SOPHIE.superPowerSecondAttackOffMalus;
                    }
                }
                return CO_SOPHIE.powerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_SOPHIE.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_SOPHIE.d2dCoZoneOffBonus;
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
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_SOPHIE.powerDefBonus;
            }
            else if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_SOPHIE.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getCOArmy = function()
    {
        return "AC";
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
                return ["ZCOUNIT_COMMANDO"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A skilled veteran that demands perfection from her soldiers. She is small but feisty.");
    };
    this.getHits = function(co)
    {
        return qsTr("Mail call");
    };
    this.getMiss = function(co)
    {
        return qsTr("The mess hall");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Enemy counterattacks deal less damage to Sophie's units.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nCommando\n") +
                qsTr("\nGlobal Effect: \nEnemy counterattacks deal -%0% damage to Sophie's units.") +
                qsTr("\n\nCO Zone Effect: \nSophie's units gain +%2% firepower and +%3% defence. Enemy counterattacks deal -%1% damage to her units.");
        text = replaceTextArgs(text, [CO_SOPHIE.d2dDefenderDamageReduction * 100, CO_SOPHIE.d2dCoZoneDefenderDamageReduction * 100, CO_SOPHIE.d2dCoZoneOffBonus, CO_SOPHIE.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Sophie's units gain +%0% firepower and +%1% defence. Enemy counterattacks deal no damage to her units.");
		text = replaceTextArgs(text, [CO_SOPHIE.powerOffBonus, CO_SOPHIE.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Sudden Strike");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Sophie's units gain +%0% firepower and +%1% defence. Her units fire twice when attacking, however, the firepower of the second attack is reduced to -%2%. Enemy counterattacks deal -%3% damage to her units.");
        text = replaceTextArgs(text, [CO_SOPHIE.powerOffBonus, CO_SOPHIE.powerDefBonus, CO_SOPHIE.superPowerSecondAttackOffMalus, CO_SOPHIE.superPowerDefenderDamageReduction*100]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Total War");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Get your butts in gear, troops! They're not going to wait all day for us!"),
                qsTr("Give'em lead, men!"),
                qsTr("It's time to prove that we are made of the right stuff!"),
                qsTr("War ain't no picnic, soldier! You have to put in 110% if you want to survive!"),
                qsTr("All right, move out and try not to get yourselves killed!"),
                qsTr("I expect nothing but the best!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Hoorah! The day is done and the battle is won!"),
                qsTr("That showed them!"),
                qsTr("We are the finest fighting force in the world!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I thought my troops would be tougher."),
                qsTr("My soldiers lost. I can't be proud of them.")];
    };
    this.getName = function()
    {
        return qsTr("Sophie");
    };
}

Constructor.prototype = CO;
var CO_SOPHIE = new Constructor();
