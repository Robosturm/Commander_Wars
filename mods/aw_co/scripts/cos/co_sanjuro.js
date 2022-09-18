CO_SANJURO.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_SANJURO.activateSuperpower = function(co, powerMode, map)
{
    CO_SANJURO.activatePower(co, powerMode, map);
};
CO_SANJURO.getSuperPowerDescription = function()
{
    return CO_SANJURO.getPowerDescription();
};
CO_SANJURO.getSuperPowerName = function()
{
    return CO_SANJURO.getPowerName();
};
CO_SANJURO.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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

CO_SANJURO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender, action, luckmode, map)
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
        case GameEnums.PowerMode_Power:
            return modifier;
        default:
            return modifier;
        }
    }
    return 0;
};

CO_SANJURO.getCostModifier = function(co, id, baseCost, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        var variables = co.getVariables();
        var costModVar = variables.createVariable("SANJURO_COST_MOD");
        var buildedVar = variables.createVariable("SANJURO_BUILDED_" + id);
        var builded = buildedVar.readDataBool();
        return baseCost * costModVar.readDataFloat();
    }
    return 0;
};

CO_SANJURO.getMovementcostModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                var variables = co.getVariables();
                var buildedVar = variables.createVariable("SANJURO_BUILDED_" + unit.getUnitID());
                var builded = buildedVar.readDataBool();
                if (builded === true)
                {
                    // movement boost for builded units
                    return -999;
                }
                break;
            case GameEnums.PowerMode_Power:
                break;
            default:
                break;
            }
        }
    }
    return 0;
};

CO_SANJURO.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
{
    if (co.getIsCO0() === true)
    {
        if (gotAttacked === true && defender.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
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
