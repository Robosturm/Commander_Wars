CO_NAPOLEON.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
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
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        return 20;
    default:
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return 20;
        }
        break;
    }
    return 0;
};

CO_NAPOLEON.getAttackHpBonus = function(co, unit, posX, posY, map)
{
    return 0;
};
CO_NAPOLEON.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                          defender, defPosX, defPosY, isDefender, luckMode, map)
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
    return 0;
};

CO_NAPOLEON.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
        {
            return 45;
        }
        else
        {
            return 25;
        }
    default:
        if (co.inCORange(Qt.point(defPosX, defPosY), defender))
        {
            if (Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 60;
            }
            else
            {
                return 40;
            }
        }
        break;
    }
};
