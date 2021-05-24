CO_SANJURO.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
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
                    return 30;
                }
                else
                {
                    return 10;
                }
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                break;
            }
        }
    }
    return 0;
};

CO_SANJURO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
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
                return modifier + 50;
            }
            else
            {
                return modifier + 10;
            }
        case GameEnums.PowerMode_Power:
            return modifier + 10;
        default:
            return modifier;
        }
    }
    return 0;
};

CO_SANJURO.getCostModifier = function(co, id, baseCost)
{
    if (co.getIsCO0() === true)
    {
        var variables = co.getVariables();
        var costModVar = variables.createVariable("SANJURO_COST_MOD");
        var buildedVar = variables.createVariable("SANJURO_BUILDED_" + id);
        var builded = buildedVar.readDataBool();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (builded === true)
            {
                // reduce cost of following units of the same type
                return -(baseCost * costModVar.readDataFloat() + baseCost) * 0.5;
            }
            break;
        case GameEnums.PowerMode_Power:
            break;
        default:
            break;
        }
        return baseCost * costModVar.readDataFloat();
    }
    return 0;
};

CO_SANJURO.getMovementcostModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
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
    }
    return 0;
};

CO_SANJURO.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
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
                    co.getOwner().addFunds(atkDamage / 10.0 * defender.getUnitCosts() * 0.5);
                }
                break;
            default:
                break;
            }
        }
    }
};
