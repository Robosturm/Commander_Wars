CO_ROBOANDY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_ROBOANDY.activateSuperpower = function(co, powerMode, map)
{
    CO_ROBOANDY.activatePower(co, map);
};
CO_ROBOANDY.getSuperPowerDescription = function()
{
    return CO_ROBOANDY.getPowerDescription();
};
CO_ROBOANDY.getSuperPowerName = function()
{
    return CO_ROBOANDY.getPowerName();
};
CO_ROBOANDY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            return co.getPowerFilled() * 4;
        }
    }
    return 0;
};
CO_ROBOANDY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_ROBOANDY.getBonusMisfortune = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            return co.getPowerFilled() * 2;
        }
    }
    return 0;
};


CO_ROBOANDY.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (gotAttacked && defender.getOwner() === co.getOwner())
            {
                if (defender.getHp() <= 0)
                {
                    attacker.setHp(attacker.getHp() - 1);
                }
            }
            else if (attacker.getOwner() === co.getOwner() && attacker.getHp() > 0)
            {
                attacker.setHp(attacker.getHp() + 1);
            }
            break;
        default:
            break;
        }
    }
};
