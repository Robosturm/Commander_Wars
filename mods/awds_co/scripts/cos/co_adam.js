CO_ADAM.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action)
{
    var ret = 0;
    if (co.getIsCO0() === true)
    {
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
            break;
        }
        ret -= (10 - attacker.getHpRounded()) * 2;
    }
    return ret;
};

CO_ADAM.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    var ret = 0;
    if (co.getIsCO0() === true)
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
            ret += deffbonusArray[index];
        }
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            ret +=  10;
        }
    }
    return ret;
};

CO_ADAM.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
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
    }
};


CO_ADAM.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
    }
    return 0;
};

CO_ADAM.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                 defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        // check for luck finish if  the attacker is in co range or a power mode is active
        if (defender !== null && attacker !== null)
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
