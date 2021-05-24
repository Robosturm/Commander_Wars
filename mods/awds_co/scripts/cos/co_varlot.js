CO_VARLOT.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            var variables = co.getVariables();
            var bonusUnitVariable = variables.createVariable("BONUSUNITS");
            var bonusOffVariable = variables.createVariable("BONUSOFF");
            var bonusUnits = bonusUnitVariable.readDataListInt32();
            var bonusOffs = bonusOffVariable.readDataListInt32();
            var unitId = attacker.getUniqueID();
            var index = bonusUnits.indexOf(unitId);
            if (index >= 0)
            {
                return 10 + bonusOffs[index];
            }
            return 10;
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            break;
        }
    }
    return 0;
};
CO_VARLOT.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
    }
    return 0;
};

CO_VARLOT.getCaptureBonus = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 5;
        }
    }
    return 0;
};

CO_VARLOT.getIncomeReduction = function(co, building, income)
{
    if (co.getIsCO0() === true)
    {
        var unit = map.getTerrain(building.getX(), building.getY()).getUnit();
        // set income to 0 during scop
        if (unit !== null && unit.getOwner() === co.getOwner())
        {
            return income * unit.getCapturePoints() / 20;
        }
    }
    return 0;
};
