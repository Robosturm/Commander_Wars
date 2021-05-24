var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(5);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
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
            animation.setSound("power1.wav");
            if (animations.length < 5)
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
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
            audio.addMusic("resources/music/cos/adam.mp3", 100, 58286);
            break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BD";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
    {
        var ret = 0;
        var variables = co.getVariables();
        var destroyed = variables.createVariable("DESTROYED");
        var value = destroyed.readDataBool();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            ret += 20;
            var unitId = attacker.getUniqueID();
            var bonusUnitVariable = variables.createVariable("BONUSUNITS");
            var bonusUnits = bonusUnitVariable.readDataListInt32();
            var offBonusVariable = variables.createVariable("OFFBONUS");
            var offbonusArray = offBonusVariable.readDataListInt32();
            var index = bonusUnits.indexOf(unitId);
            if (index >= 0)
            {
                ret += offbonusArray[index];
            }
            else if (value)
            {
                ret += 15;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (isDefender === false)
            {
                ret += 30;
                if (value)
                {
                    ret += 10;
                }
            }
            break;
        default:
            if (isDefender === false)
            {
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    ret += 10;
                }
            }
            break;
        }
        ret -= (10 - attacker.getHpRounded()) * 2;
        return ret;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
        var ret = 0;
        var unitId = defender.getUniqueID();
        var variables = co.getVariables();
        var bonusUnitVariable = variables.createVariable("BONUSUNITS");
        var bonusUnits = bonusUnitVariable.readDataListInt32();
        var defBonusVariable = variables.createVariable("DEFBONUS");
        var deffbonusArray = defBonusVariable.readDataListInt32();
        var index = bonusUnits.indexOf(unitId);
        if (index >= 0)
        {
            ret += deffbonusArray[index];
        }
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            ret += 10;
        }
        else
        {
            ret /= 2;
        }
        return ret;
    };

    this.startOfTurn = function(co)
    {
        // reset variables stop
        var variables = co.getVariables();
        var bonusUnitVariable = variables.createVariable("BONUSUNITS");
        var empty = [];
        bonusUnitVariable.writeDataListInt32(empty);
        var offBonusVariable = variables.createVariable("OFFBONUS");
        offBonusVariable.writeDataListInt32(empty);
        var defBonusVariable = variables.createVariable("DEFBONUS");
        defBonusVariable.writeDataListInt32(empty);
        var destroyed = variables.createVariable("DESTROYED");
        destroyed.writeDataBool(false);
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        var variables = co.getVariables();
        var bonusUnitVariable = variables.createVariable("BONUSUNITS");
        var bonusUnits = bonusUnitVariable.readDataListInt32();
        if (gotAttacked === false && attacker.getOwner() === co.getOwner())
        {
            var destroyed = variables.createVariable("DESTROYED");
            var offBonusVariable = variables.createVariable("OFFBONUS");
            var unitId = attacker.getUniqueID();
            var offbonusArray = offBonusVariable.readDataListInt32();
            var index = -1;
            index = bonusUnits.indexOf(unitId);
            if ((defender.getHp() <= 0.0) &&
                    (co.inCORange(Qt.point(attacker.getX(), attacker.getY()), attacker) ||
                     co.getPowerMode() > GameEnums.PowerMode_Off))
            {
                var powerMode = co.getPowerMode();
                if (powerMode === GameEnums.PowerMode_Tagpower ||
                        powerMode === GameEnums.PowerMode_Superpower)
                {
                    attacker.setHasMoved(false);

                    if (index >= 0)
                    {
                        offbonusArray[index] -= 45;
                    }
                    else
                    {
                        offbonusArray.push(-45);
                        bonusUnits.push(unitId);
                    }
                    offBonusVariable.writeDataListInt32(offbonusArray);
                }
                var defBonusVariable = variables.createVariable("DEFBONUS");
                var defbonusArray = defBonusVariable.readDataListInt32();
                index = bonusUnits.indexOf(unitId);
                if (index >= 0)
                {
                    defbonusArray[index] += 10;
                }
                else
                {
                    defbonusArray.push(15);
                    bonusUnits.push(unitId);
                }
                defBonusVariable.writeDataListInt32(defbonusArray);
                destroyed.writeDataBool(true);
            }
            else if (index < 0)
            {
                destroyed.writeDataBool(false);
            }
        }
        bonusUnitVariable.writeDataListInt32(bonusUnits);
    };


    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
        return 0;
    };

    this.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, action)
    {
        // check for luck finish if  the attacker is in co range or a power mode is active
        if (defender !== null && attacker !== null &&
                ((co.inCORange(Qt.point(atkPosX, atkPosY), attacker) && !isDefender) ||
                 (co.inCORange(Qt.point(defPosX, defPosY), defender) && isDefender) ||
                 co.getPowerMode() > GameEnums.PowerMode_Off))
        {
            // check for finishing blow return absurd amount of true damage if luck is enough
            if (isDefender)
            {
                if (defender.getHp() - damage / 10.0 - attackerBaseHp / 20.0 <= 0)
                {
                    return 100;
                }
            }
            else
            {
                if (defender.getHp() - damage / 10.0  - attacker.getHpRounded() / 20.0 <= 0)
                {
                    return 100;
                }
            }
        }
        // 0
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };


    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A researcher-turned commander of Brown Desert, Adam Deckster is a reclusive geek who nevertheless is slowly finding a place among the other commanders.");
    };
    this.getHits = function(co)
    {
        return qsTr("Vectors");
    };
    this.getMiss = function(co)
    {
        return qsTr("Percent Error");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Adam's units finish off units that are within basic luck range. Whenever he destroys an enemy unit, the attacking unit gains a defense boost. However, Adam's units lose firepower as they lose HP.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nUnits have increased firepower but lose additional firepower for lost HP.") +
                qsTr("\n\nCO Zone Effect: \nUnits gain additional firepower and finish off units when they are within basic luck range. Whenever a unit destroy an enemy unit it gets a defense boost for one turn.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("All of Adam's units gains a firepower boost and movement range. Whenever Adam destroys a unit, the next unit gains bonus firepower.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Decimation");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Each time Adam destroys an enemy unit, the attacking unit loses firepower and can move again, while the next unit he didn't used yet gets an powerup.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Cartesian Cleanup");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Get a hobby? This is my hobby!"),
                qsTr("Woo! Watch out for shrapnel!"),
                qsTr("Precision pays off!"),
                qsTr("As my troops approach their limit...you become insignificant!"),
                qsTr("Prepare to have your strategy debunked!"),
                qsTr("I'll blind you with science!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Call it bad luck, if you must."),
                qsTr("Who said math had no applications in real life?"),
                qsTr("Ha! The revenge of the nerds!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Hssss! Today was... unlucky. A bad day. Nothing more."),
                qsTr("It's the blasted weather! That was the problem! Hssss!")];
    };
    this.getName = function()
    {
        return qsTr("Adam");
    };
}
Constructor.prototype = CO;
var CO_ADAM = new Constructor();
