var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
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
    };

    this.activatePower = function(co)
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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (globals.randInt(0, 1) === 0)
            {
                animation.setSound("power5_1.wav");
            }
            else
            {
                animation.setSound("power5_2.wav");
            }
            if (animations.length < 5)
            {
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power5", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();
    };

    this.activateSuperpower = function(co, powerMode)
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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (globals.randInt(0, 1) === 0)
            {
                animation.setSound("power12_1.wav");
            }
            else
            {
                animation.setSound("power12_2.wav");
            }
            if (animations.length < 5)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();
    };

    
    this.getCOUnitRange = function(co)
    {
        return 2;
    };

    this.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, luckMode)
    {
        // reduce counter damage by a flat amount here
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (isDefender === true)
                {
                    return damage * 0.2;
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
                        return damage * 0.2;
                    }
                }
                break;
        }
        return 0;
    };

    this.startOfTurn = function(co)
    {
        var units = co.getOwner().getUnits();
        for (var i = 0; i < units.size(); i++)
        {
            // creates the variable or returns the existing one
            var variable = units.at(i).getVariables().createVariable("SOPHIE_SCOP");
            // set attacked twice this turn to false
            variable.writeDataBool(false);
        }
        units.remove();
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
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
                            var damageResult = ACTION_FIRE.calcBattleDamage2(attacker, Qt.point(attacker.getX(), attacker.getY()),
                                                                             defender.getX(), defender.getY(), GameEnums.LuckDamageMode_On);
                            // do another attack
                            ACTION_FIRE.battle(attacker, damageResult.x, damageResult.y,
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
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
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
                        // reduce damage during the second attack
                        return -30;
                    }
                }
                return 10;
            case GameEnums.PowerMode_Power:
                 return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 20;
                }
                break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 20;
        }
        return 0;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_COMMANDO"];
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
        return qsTr("Mail Call");
    };
    this.getMiss = function(co)
    {
        return qsTr("Mess Hall");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Enemy counterattacks deal less damage to Sophie's units.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nCommando\n") +
               qsTr("\nGlobal Effect: \nNo Effects") +
               qsTr("\n\nCO Zone Effect: \nEnemy counterattacks deal less damage. Units gain firepower and defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Enemy counterattacks deal no damage to Sophie's units.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Sudden Strike");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Sophie's units strike twice when ordered to attack. However, firepower is reduced");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Total War");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Get your butts in gear troops! The bad guys are not going to wait all day for us!"),
                qsTr("Give'em lead men!"),
                qsTr("It's time to prove that we are made of the right stuff!"),
                qsTr("War ain't no picnic soldier! You have to put in 110% if you want to survive!"),
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
                qsTr("My Soldiers lost. I can't be proud of them.")];
    };
    this.getName = function()
    {
        return qsTr("Sophie");
    };
}

Constructor.prototype = CO;
var CO_SOPHIE = new Constructor();
