CO_ADAM.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ADAM.activateSuperpower = function(co, powerMode, map)
{
	CO_ADAM.activatePower(co, powerMode, map);
};
CO_ADAM.getSuperPowerDescription = function()
{
    return CO_ADAM.getPowerDescription();
};
CO_ADAM.getSuperPowerName = function()
{
    return CO_ADAM.getPowerName();
};
CO_ADAM.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    var ret = 0;
    var variables = co.getVariables();
    var destroyed = variables.createVariable("DESTROYED");
    var value = destroyed.readDataBool();
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
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

CO_ADAM.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
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
