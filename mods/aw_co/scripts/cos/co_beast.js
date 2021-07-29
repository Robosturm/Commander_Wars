CO_BEAST.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_BEAST.activateSuperpower = function(co, powerMode)
{
    CO_BEAST.activatePower(co, powerMode);
};
CO_BEAST.getSuperPowerDescription = function()
{
    return CO_BEAST.getPowerDescription();
};
CO_BEAST.getSuperPowerName = function()
{
    return CO_BEAST.getPowerName();
};
CO_BEAST.coZoneBonus = 30;
CO_BEAST.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (!isDefender)
            {
                return CO_BEAST.coZoneBonus;
            }
            break;
        default:
            return CO_BEAST.coZoneBonus;
        }
    }
    return 0;
};
CO_BEAST.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_BEAST.getMovementpointModifier = function(co, unit, posX, posY)
{
    return 0;
};
CO_BEAST.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        if (attacker.getOwner() === co.getOwner() && attacker.getHp() > 0)
        {
            attacker.setHp(attacker.getHp() - 1);
        }
    }
};
