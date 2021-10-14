CO_SANJURO.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SANJURO.activateSuperpower = function(co, powerMode)
{
	CO_SANJURO.activatePower(co, powerMode);
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
                             defender, defPosX, defPosY, isAttacker, action)
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
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return 10;
                }
                break;
        }
    }
    return 0;
};

CO_SANJURO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
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
            return modifier + 10;
        default:
            if (modifier > 0)
            {
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return modifier + 10;
                }
                else
                {
                    return modifier / 2 + 10;
                }
            }
            else
            {
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return modifier + 10;
                }
                else
                {
                    return modifier;
                }
            }
    }
};

CO_SANJURO.getCostModifier = function(co, id, baseCost)
{
    var variables = co.getVariables();
    var costModVar = variables.createVariable("SANJURO_COST_MOD");
    var buildedVar = variables.createVariable("SANJURO_BUILDED_" + id);
    var builded = buildedVar.readDataBool();
    var costMod = costModVar.readDataFloat();
    return (baseCost * costMod);
};

CO_SANJURO.getMovementcostModifier = function(co, unit, posX, posY)
{
    return 0;
};

CO_SANJURO.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action)
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
};
