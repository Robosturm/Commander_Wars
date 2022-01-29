CO_YUKIO.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(8);
};
CO_YUKIO.activateSuperpower = function(co, powerMode, map)
{
    CO_YUKIO.activatePower(co, powerMode);
};
CO_YUKIO.getSuperPowerDescription = function()
{
    return CO_YUKIO.getPowerDescription();
};
CO_YUKIO.getSuperPowerName = function()
{
    return CO_YUKIO.getPowerName();
};
CO_YUKIO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
    {
        return 10;
    }
    return 0;
};
CO_YUKIO.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
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

CO_YUKIO.getTrueDamage = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                  defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
    {
        // reduce counter damage by a flat amount here
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (damage >= CO_YUKIO.mintrueDamage)
            {
                return 15
            }
            break;
        default:
            if (damage >= CO_YUKIO.mintrueDamage)
            {
                return 15
            }
            break;
        }
    }
    return 0;
};

CO_YUKIO.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                       defender, defPosX, defPosY, isDefender, luckMode, map)
{
    if (co.getIsCO0() === true)
    {
        // reduce counter damage by a flat amount here
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 15;
        default:
            return 10;
        }
    }
    return 0;
};
