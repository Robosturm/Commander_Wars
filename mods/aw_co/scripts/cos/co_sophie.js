CO_SOPHIE.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_SOPHIE.activateSuperpower = function(co, powerMode)
{
    CO_SOPHIE.activatePower(co, powerMode);
};
CO_SOPHIE.getSuperPowerDescription = function()
{
    return CO_SOPHIE.getPowerDescription();
};
CO_SOPHIE.getSuperPowerName = function()
{
    return CO_SOPHIE.getPowerName();
};
CO_SOPHIE.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                        defender, defPosX, defPosY, isDefender, luckMode)
{
    if (co.getIsCO0() === true)
    {
        // reduce counter damage by a flat amount here
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (isDefender === true)
            {
                return damage;
            }
            break;
        default:
            if (isDefender === true)
            {
                return damage * 0.2;
            }
            break;
        }
    }
    return 0;
};

CO_SOPHIE.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
};

CO_SOPHIE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};

CO_SOPHIE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
