CO_NAPOLEON.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_NAPOLEON.activateSuperpower = function(co, powerMode, map)
{
    CO_NAPOLEON.activatePower(co, map);
};
CO_NAPOLEON.getSuperPowerDescription = function()
{
    return CO_NAPOLEON.getPowerDescription();
};
CO_NAPOLEON.getSuperPowerName = function()
{
    return CO_NAPOLEON.getPowerName();
};
CO_NAPOLEON.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    return 0;
};

CO_NAPOLEON.getAttackHpBonus = function(co, unit, posX, posY, map)
{
    return 0;
};

CO_NAPOLEON.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                          defender, defPosX, defPosY, isDefender, luckMode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            var defHp = defender.getHp() * 10;
            if (damage  > defHp / 2)
            {
                return damage - defHp / 2;
            }
            return 0;
        default:
            break;
        }
    }
    return 0;
};

CO_NAPOLEON.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 30
            }
            else
            {
                return 20;
            }
        default:
            if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 30
            }
            else
            {
                return 15;
            }
        }
    }
    return 0;
};
