var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(5);
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
            animation.setSound("power1.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power1", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, delay);
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
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BD";
    };

    this.superPowerOffBaseBonus = 20;
    this.superPowerOffDestroyedBonus = 15;
    this.superPowerOffMalus = 45;

    this.powerBaseBonus = 30;
    this.powerDestroyedBonus = 10;
    this.powerMovementBonus = 1;
    this.powerDefBonus = 10;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.d2dPowerMalus = 2;
    this.d2dDefDestroyedBonus = 15;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        var ret = 0;
        if (CO.isActive(co))
        {
            var variables = co.getVariables();
            var destroyed = variables.createVariable("DESTROYED");
            var value = destroyed.readDataBool();
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                ret += CO_ADAM.superPowerOffBaseBonus;
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
                    ret += CO_ADAM.superPowerOffDestroyedBonus;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (isDefender === false)
                {
                    ret += CO_ADAM.powerBaseBonus;
                    if (value)
                    {
                        ret += CO_ADAM.powerDestroyedBonus;
                    }
                }
                break;
            default:
                if (isDefender === false)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        ret += CO_ADAM.d2dCoZoneOffBonus;
                    }
                }
                break;
            }
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()) ||
                 co.getPowerMode() > GameEnums.PowerMode_Off ||
                 co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                // calc malus based on missing hp
                ret -= (10 - attacker.getHpRounded()) * CO_ADAM.d2dPowerMalus;
            }
        }
        return ret;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        var ret = 0;
        if (CO.isActive(co))
        {
            var unitId = defender.getUniqueID();
            var variables = co.getVariables();
            var bonusUnitVariable = variables.createVariable("BONUSUNITS");
            var bonusUnits = bonusUnitVariable.readDataListInt32();
            var defBonusVariable = variables.createVariable("DEFBONUS");
            var deffbonusArray = defBonusVariable.readDataListInt32();
            var index = bonusUnits.indexOf(unitId);
            if (index >= 0)
            {
                if (map === null ||
                    (map !== null && map.getGameRules().getCoGlobalD2D()) ||
                     co.getPowerMode() > GameEnums.PowerMode_Off ||
                     co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    ret += deffbonusArray[index];
                }
            }            
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                ret += CO_ADAM.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                ret += CO_ADAM.d2dCoZoneDefBonus;
            }
        }
        return ret;
    };

    this.startOfTurn = function(co, map)
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

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co) && defender !== null)
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
                            offbonusArray[index] -= CO_ADAM.superPowerOffMalus;
                        }
                        else
                        {
                            offbonusArray.push(-CO_ADAM.superPowerOffMalus);
                            bonusUnits.push(unitId);
                        }
                        offBonusVariable.writeDataListInt32(offbonusArray);
                    }
                    var defBonusVariable = variables.createVariable("DEFBONUS");
                    var defbonusArray = defBonusVariable.readDataListInt32();
                    index = bonusUnits.indexOf(unitId);
                    if (index >= 0)
                    {
                        defbonusArray[index] += CO_ADAM.d2dDefDestroyedBonus;
                    }
                    else
                    {
                        defbonusArray.push(CO_ADAM.d2dDefDestroyedBonus);
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
        }
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                return CO_ADAM.powerMovementBonus;
            }
        }
        return 0;
    };

    this.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
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
         return qsTr("Previously a researcher, he was recruited as a commander for his high intelligence. Although a reclusive geek, he is slowly finding his place among the other commanders.");
     };
    this.getHits = function(co)
    {
        return qsTr("Vectors");
    };
    this.getMiss = function(co)
    {
        return qsTr("Percent error");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Adam can take advantage of max luck rolls, but only if the enemy unit could be killed by one. However, his units lose even more firepower the less HP they have.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_ADAM.d2dPowerMalus, CO_ADAM.d2dDefDestroyedBonus];
        }
        var text = qsTr("\nGlobal Effect: \nAdam's units lose %0% more firepower per lost HP. His units gain a one-turn %1% defence boost by killing an enemy unit.") +
                   qsTr("\n\nCO Zone Effect: \nAdam's attacking units instantly kill an enemy unit if a max luck roll would kill them. His units gain +%2% firepower and +%3% defence.");

        text = replaceTextArgs(text, [values[0], values[1], CO_ADAM.d2dCoZoneOffBonus, CO_ADAM.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("All of Adam's units gain +%1 movement, +%3% defence, and a base of +%0% firepower when attacking. Whenever Adam destroys a unit, the next unit gains an additional +%2% bonus firepower when attacking.");
        text = replaceTextArgs(text, [CO_ADAM.powerBaseBonus, CO_ADAM.powerMovementBonus, CO_ADAM.powerDestroyedBonus, CO_ADAM.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Decimation");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Adam's units gain a base of +%1% firepower and +%3% defence. Each time Adam attacks and destroys an enemy unit, the attacking unit can move again. However, each kill reduces that unit's firepower by +%0% until next turn. Conversely, after killing a unit, using a unit that has not been used for that turn will give it an additional %2% firepower.");
        text = replaceTextArgs(text, [CO_ADAM.superPowerOffMalus, CO_ADAM.superPowerOffBaseBonus, CO_ADAM.superPowerOffDestroyedBonus, CO_ADAM.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Cartesian Cleanup");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Get a hobby? This IS my hobby!"),
                qsTr("Woo! Watch out for shrapnel!"),
                qsTr("Research pays off!"),
                qsTr("As my troops approach their limit...you become insignificant!"),
                qsTr("Prepare to have your strategy debunked!"),
                qsTr("I'll blind you with science!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Call it bad luck, if you must."),
                qsTr("Who said math had no applications in real life?"),
                qsTr("Ha! And you thought nerds were weak!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("How were my calculations so off?"),
                qsTr("Your stupidity rubbed off on me! Ugh!!")];
    };
    this.getName = function()
    {
        return qsTr("Adam");
    };
}
Constructor.prototype = CO;
var CO_ADAM = new Constructor();
