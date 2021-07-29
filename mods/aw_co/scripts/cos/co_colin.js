CO_COLIN.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_COLIN.activateSuperpower = function(co, powerMode)
{
    CO_COLIN.activatePower(co, powerMode);
};
CO_COLIN.getSuperPowerDescription = function()
{
    return CO_COLIN.getPowerDescription();
};
CO_COLIN.getSuperPowerName = function()
{
    return CO_COLIN.getPowerName();
};
CO_COLIN.globalBoost = 10;
CO_COLIN.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        return -CO_COLIN.globalBoost;
    }
    return 0;
};

CO_COLIN.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action)
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
this.getCostModifier = function(co, id, baseCost)
{
    if (co.getIsCO0() === true)
    {
        return -baseCost * CO_COLIN.costModifier / 100;
    }
    return 0;
};
