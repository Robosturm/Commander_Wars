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
                animation.setSound("power9_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power9_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
                audio.addMusic("resources/music/cos/sanjuro.mp3", 100, 63433);
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
        return "GS";
    };

    this.superPowerDefBonus = 40;
    this.superPowerOffBonus = 50;
    this.superPowerCostReduction = 0.5;
    this.powerRefund = 0.5;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCostModifier = 0.01;
    this.d2dOffModifier = 1;
    this.d2dExceedBonus = 0.01;
    this.d2dMaxBonus = 20;

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (defender !== null)
            {
                var variables = co.getVariables();
                var buildedVar = variables.createVariable("SANJURO_BUILDED_" + defender.getUnitID());
                var builded = buildedVar.readDataBool();
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (builded === true)
                    {
                        return CO_SANJURO.superPowerDefBonus;
                    }
                    else
                    {
                        return CO_SANJURO.powerDefBonus;
                    }
                case GameEnums.PowerMode_Power:
                    return CO_SANJURO.powerDefBonus;
                default:
                    if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                    {
                        return CO_SANJURO.d2dCoZoneDefBonus;
                    }
                    break;
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
            var variables = co.getVariables();
            var dmgModVar = variables.createVariable("SANJURO_DMG_MOD");
            var builded = false
            if (attacker !== null)
            {
                var buildedVar = variables.createVariable("SANJURO_BUILDED_" + attacker.getUnitID());
                builded = buildedVar.readDataBool();
            }
            var modifier = dmgModVar.readDataFloat();
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (builded === true)
                {
                    return modifier + CO_SANJURO.superPowerOffBonus;
                }
                else
                {
                    return modifier + CO_SANJURO.powerOffBonus;
                }
            case GameEnums.PowerMode_Power:
                return modifier + CO_SANJURO.powerOffBonus;
            default:
                if (modifier > 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return modifier + CO_SANJURO.d2dCoZoneOffBonus;
                    }
                    else
                    {
                        return modifier;
                    }
                }
                else
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return modifier + CO_SANJURO.d2dCoZoneOffBonus;
                    }
                    else
                    {
                        return modifier;
                    }
                }
            }
        }
    };

    this.buildedUnit = function(co, unit, map)
    {
        var variables = co.getVariables();
        var buildedVar = variables.createVariable("SANJURO_BUILDED_" + unit.getUnitID());
        buildedVar.writeDataBool(true);
    };

    this.startOfTurn = function(co, map)
    {
        var player = co.getOwner();
        var funds = player.getFunds();
        var income = player.calcIncome();
        var costModifier = 0.0;
        var damageModifier = 0.0;
        var exceed = 0;
        var maxCounter = CO_SANJURO.d2dMaxBonus;
        var counter = 0;
        if (income < funds)
        {
            // we have more funds than income
            exceed = funds - income;
            // this means our troops get stronger and more expensive
            while (exceed >= 0 && counter < maxCounter)
            {
                damageModifier += CO_SANJURO.d2dOffModifier;
                costModifier += CO_SANJURO.d2dCostModifier;
                exceed -= income * CO_SANJURO.d2dExceedBonus;
                counter++;
            }
        }
        else
        {
            // we have less funds than income
            exceed = income - funds;
            // this means our troops get weaker and less expensive
            while (exceed >= 0 && counter < maxCounter)
            {
                damageModifier -= CO_SANJURO.d2dOffModifier;
                costModifier -= CO_SANJURO.d2dCostModifier;
                exceed -= income * CO_SANJURO.d2dExceedBonus;
                counter++;
            }
        }
        var variables = co.getVariables();
        // clean up possible variables from scop
        variables.clear();
        // create new cost and damage variables
        var dmgModVar = variables.createVariable("SANJURO_DMG_MOD");
        dmgModVar.writeDataFloat(damageModifier);
        var costModVar = variables.createVariable("SANJURO_COST_MOD");
        costModVar.writeDataFloat(costModifier);

    };

    this.getCostModifier = function(co, id, baseCost, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            var variables = co.getVariables();
            var costModVar = variables.createVariable("SANJURO_COST_MOD");
            var buildedVar = variables.createVariable("SANJURO_BUILDED_" + id);
            var builded = buildedVar.readDataBool();
            var costMod = costModVar.readDataFloat();
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (builded === true)
                {
                    // reduce cost of following units of the same type
                    return -(baseCost * costMod + baseCost) * CO_SANJURO.superPowerCostReduction;
                }
                break;
            case GameEnums.PowerMode_Power:
                break;
            default:
                break;
            }
            return (baseCost * costMod);
        }
        return 0;
    };

    this.getMovementcostModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (unit.getOwner() === co.getOwner())
                {
                    var variables = co.getVariables();
                    var buildedVar = variables.createVariable("SANJURO_BUILDED_" + unit.getUnitID());
                    var builded = buildedVar.readDataBool();
                    if (builded === true)
                    {
                        // movement boost for builded units
                        return -999;
                    }
                }
                break;
            case GameEnums.PowerMode_Power:
                break;
            default:
                break;
            }
        }
        return 0;
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co))
        {
            if (gotAttacked === true && defender.getOwner() === co.getOwner())
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:

                    break;
                case GameEnums.PowerMode_Power:
                    // get refunds
                    // damage can be negativ if we can't do a counter attack the damge is -1
                    // avoid loosing money cause of our passive or power
                    if (atkDamage > 0)
                    {
                        co.getOwner().addFunds(atkDamage / 10.0 * defender.getUnitCosts() * CO_SANJURO.powerRefund);
                    }
                    break;
                default:
                    break;
                }
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
                return ["ZCOUNIT_SMUGGLER"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A respected mercenary from Golden Sun. Once he accepts a job, he sees it through to the end no matter how difficult or dangerous it becomes.");
    };
    this.getHits = function(co)
    {
        return qsTr("Honest work");
    };
    this.getMiss = function(co)
    {
        return qsTr("Shady business");
    };
    this.getCODescription = function(co)
    {
        return qsTr("A keen mind for his finances, he spends more for better equipment when there's a surplus and focuses on getting the best deal when he's pressed for money.");
    };
    this.getLongCODescription = function()
    {
        var text =  qsTr("\nSpecial Unit:\nSmuggler\n" +
                         "\nGlobal Effect: \nFor each %0% difference between income and funds at the start of the turn. His troops gain %1% firepower per %0% percent and cost %2% more if the funds are greater than the current funds. " +
                         "Otherwise he looses %1% firepower per %0% percent and his troops cost %2% less. This effect stacks up to %5 times." +
                         "\n\nCO Zone Effect: \nGains %3% firepower and %4% defense.");
        text = replaceTextArgs(text, [CO_SANJURO.d2dExceedBonus * 100, CO_SANJURO.d2dOffModifier, CO_SANJURO.d2dCostModifier * 100, CO_SANJURO.d2dCoZoneOffBonus, CO_SANJURO.d2dCoZoneDefBonus, CO_SANJURO.d2dMaxBonus]);
        return text;
    };

    this.getPowerDescription = function(co)
    {
        var text = qsTr("When his units take combat damage, he receives %0% of the damage value in funds.");
        text = replaceTextArgs(text, [CO_SANJURO.powerRefund * 100]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Heart of Gold");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text =  qsTr("When a unit is built, units of the same type receive a %0% firepower and %1% defense boost are unhindered by terrain, and production costs for that unit type are reduced by %2%.");
        text = replaceTextArgs(text, [CO_SANJURO.superPowerOffBonus, CO_SANJURO.superPowerDefBonus, CO_SANJURO.superPowerCostReduction * 100]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Buying Power");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("My services are expensive, and worth every penny."),
                qsTr("Consider this a free sample!"),
                qsTr("You're finished! Surrender now!"),
                qsTr("War is my livelihood. Can you handle my level of experience?"),
                qsTr("I was born for battle. I have no limits!"),
                qsTr("If you think this is strictly business, think again. The battlefield calls to me!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I have a reputation to keep, you know."),
                qsTr("Another job well done...where's the next fight?"),
                qsTr("Did you expect anything less from a hardened mercenary?")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("For this I get too less money"),
                qsTr("Now, I know how the Romans felt like.")];
    };
    this.getName = function()
    {
        return qsTr("Sanjuro");
    };
}

Constructor.prototype = CO;
var CO_SANJURO = new Constructor();
